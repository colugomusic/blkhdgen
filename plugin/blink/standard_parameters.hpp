#pragma once

#include <optional>
#include <regex>
#include <sstream>
#include "envelope_spec.hpp"
#include "slider_parameter_spec.hpp"

namespace blink {
namespace std_params {

template <class T> std::optional<T> find_number(const std::string& str);
template <class T> std::optional<T> find_positive_number(const std::string& str);

template <>
std::optional<float> find_number<float>(const std::string& str)
{
	std::regex r("(\\-?\\s*[\\.\\d]+)");
	std::smatch match;

	if (!std::regex_search(str, match, r)) return std::optional<float>();

	return std::stof(match[0].str());
}

template <>
std::optional<int> find_number<int>(const std::string& str)
{
	std::regex r("(\\-?\\s*[\\d]+)");
	std::smatch match;

	if (!std::regex_search(str, match, r)) return std::optional<int>();

	return std::stoi(match[0].str());
}

template <>
std::optional<float> find_positive_number<float>(const std::string& str)
{
	std::regex r("([\\.\\d]+)");
	std::smatch match;

	if (!std::regex_search(str, match, r)) return std::optional<float>();

	return std::stof(match[0].str());
}

template <>
std::optional<int> find_positive_number<int>(const std::string& str)
{
	std::regex r("([\\d]+)");
	std::smatch match;

	if (!std::regex_search(str, match, r)) return std::optional<int>();

	return std::stoi(match[0].str());
}

template <int Normal, int Precise>
inline float increment(float v, bool precise)
{
	return v + 1.0f / (precise ? Precise : Normal);
}

template <int Normal, int Precise>
inline float decrement(float v, bool precise)
{
	return v - 1.0f / (precise ? Precise : Normal);
}

template <int Normal>
inline float increment(float v)
{
	return v + 1.0f / (Normal);
}

template <int Normal>
inline float decrement(float v)
{
	return v - 1.0f / (Normal);
}

template <int Normal, int Precise>
inline float drag(float v, int amount, bool precise)
{
	return v + (float(amount) / (precise ? Precise : Normal));
}

inline float constrain(float v, float min, float max)
{
	if (v < min) return min;
	if (v > max) return max;

	return v;
}

template <class T>
inline auto display_number(T v)
{
	std::stringstream ss;

	ss << v;

	return ss.str();
};

inline auto stepify_0_1(float v) -> float
{
	return math::stepify(v, 0.1f);
}

inline auto stepify_0_01(float v) -> float
{
	return math::stepify(v, 0.01f);
}

inline auto stepify_0_001(float v) -> float
{
	return math::stepify(v, 0.001f);
}

inline auto snap_value(float v, float step_size, float snap_amount)
{
	if (snap_amount <= 0.0f) return v;

	if (snap_amount >= 1.0f)
	{
		v /= step_size;
		v = std::round(v);
		v *= step_size;

		return v;
	}

	const auto up = std::ceil((v / step_size) + 0.0001f) * step_size;
	const auto down = std::floor(v / step_size) * step_size;
	const auto x = math::inverse_lerp(down, up, v);
	const auto t = x * 2.0f;
	const auto i = 1.0f + (std::pow(snap_amount, 4.0f) * 99.0f);
	const auto curve =
		t < 1.0f
		? 1.0f - (0.5f * (std::pow(1.0f - t, 1.0f / i) + 1.0f))
		: 0.5f * (std::pow(t - 1.0f, 1.0f / i) + 1.0f);

	return math::lerp(down, up, curve);
}

namespace percentage {

inline auto stepify(float v) -> float
{
	return stepify_0_001(v);
}

inline auto snap_value(float v, float step_size, float snap_amount)
{
	return stepify(std_params::snap_value(v, step_size, snap_amount));
}

inline float constrain(float v)
{
	return std::clamp(v, 0.0f, 1.0f);
};

inline auto increment(float v, bool precise)
{
	return constrain(stepify(std_params::increment<100, 1000>(v, precise)));
};

inline auto decrement(float v, bool precise)
{
	return constrain(stepify(std_params::decrement<100, 1000>(v, precise)));
};

inline auto drag(float v, int amount, bool precise) -> float
{
	return constrain(stepify(std_params::drag<100, 1000>(v, amount / 5, precise)));
};

inline auto display(float v)
{
	std::stringstream ss;

	ss << stepify(v * 100.0f) << "%";

	return ss.str();
}

inline auto from_string(const std::string& str) -> std::optional<float>
{
	auto value = find_number<float>(str);

	if (!value) return std::optional<float>();

	return (*value / 100.0f);
};

}

namespace percentage_bipolar {

inline auto stepify(float v) -> float
{
	return math::stepify(v, 0.0005f);
}

inline auto display(float v)
{
	std::stringstream ss;

	ss << percentage::stepify((v - 0.5f) * 200.0f) << "%";

	return ss.str();
}

inline auto increment(float v, bool precise)
{
	return percentage::constrain(stepify(std_params::increment<200, 2000>(v, precise)));
};

inline auto decrement(float v, bool precise)
{
	return percentage::constrain(stepify(std_params::decrement<200, 2000>(v, precise)));
};

inline auto drag(float v, int amount, bool precise) -> float
{
	return percentage::constrain(stepify(std_params::drag<200, 2000>(v, amount / 5, precise)));
};

}

namespace filter_frequency {

inline auto linear2hz(float v)
{
	return std::pow(v, 2.0f) * 20000.0f;
}

inline float constrain(float v)
{
	return std::clamp(v, 0.0f, 1.0f);
};

inline auto stepify(float v) -> float
{
	return v;
}

inline auto display(float v)
{
	const auto hz = math::convert::linear_to_filter_hz(v);

	std::stringstream ss;

	if (hz >= 1000.0f)
	{
		ss << (v / 1000.0f) << " MHz";
	}
	else
	{
		ss << v << " Hz";
	}

	return ss.str();
}

inline auto from_string(const std::string& str) -> std::optional<float>
{
	auto value = find_number<float>(str);

	if (!value) return std::optional<float>();

	return math::convert::filter_hz_to_linear(*value);
};

inline auto increment(float v, bool precise)
{
	return constrain(stepify(std_params::increment<100, 1000>(v, precise)));
};

inline auto decrement(float v, bool precise)
{
	return constrain(stepify(std_params::decrement<100, 1000>(v, precise)));
};

inline auto drag(float v, int amount, bool precise) -> float
{
	return constrain(stepify(std_params::drag<100, 1000>(v, amount / 5, precise)));
};

}

namespace amp
{
	inline auto stepify(float v) -> float
	{
		if (v <= 0.00001f) return 0.0f;

		return math::convert::db_to_linear(math::stepify(math::convert::linear_to_db(v), 0.1f));
	}

	inline auto constrain(float v)
	{
		const auto db = math::convert::linear_to_db(v);

		if (db < -60.0f) return 0.0f;
		if (db > 12.0f) return math::convert::db_to_linear(12.0f);

		return v;
	};

	inline auto from_string(const std::string& str) -> std::optional<float>
	{
		auto db = find_number<float>(str);

		if (!db) return db;

		return math::convert::db_to_linear(*db);
	};

	inline auto display(float v)
	{
		std::stringstream ss;

		if (v <= 0.0f)
		{
			ss << "Silent";
		}
		else
		{
			ss << math::stepify(float(math::convert::linear_to_db(v)), 0.1f) << " dB";
		}

		return ss.str();
	}

	inline auto increment(float v, bool precise)
	{
		if (v <= 0.0f) return math::convert::db_to_linear(-60.0f);

		return constrain(stepify(math::convert::db_to_linear(std_params::increment<1, 10>(math::convert::linear_to_db(v), precise))));
	};

	inline auto decrement(float v, bool precise)
	{
		return constrain(stepify(math::convert::db_to_linear(std_params::decrement<1, 10>(math::convert::linear_to_db(v), precise))));
	};

	inline auto drag(float v, int amount, bool precise) -> float
	{
		if (v <= 0.0f) v = math::convert::db_to_linear(-61.0f);

		return constrain(stepify(math::convert::db_to_linear(std_params::drag<1, 10>(math::convert::linear_to_db(v), amount / 5, precise))));
	};
}

namespace pan
{
	inline auto stepify(float v) -> float
	{
		return stepify_0_01(v);
	}

	inline float constrain(float v)
	{
		if (v < -1.0f) return -1.0f;
		if (v > 1.0f) return 1.0f;

		return v;
	};

	inline std::string display(float v)
	{
		std::stringstream ss;

		if (v < 0.0f)
		{
			ss << stepify(std::abs(v * 100)) << "% L";
		}
		else if (v > 0.0f)
		{
			ss << stepify(v * 100) << "% R";
		}
		else
		{
			ss << "Center";
		}

		return ss.str();
	}
	
	inline auto increment(float v, bool precise)
	{
		return constrain(stepify(std_params::increment<100>(v)));
	};

	inline auto decrement(float v, bool precise)
	{
		return constrain(stepify(std_params::decrement<100>(v)));
	};

	inline auto drag(float v, int amount, bool precise) -> float
	{
		return constrain(stepify(std_params::drag<500, 5000>(v, amount, precise)));
	};

	inline auto from_string(const std::string& str) -> std::optional<float>
	{
		std::string uppercase = str;

		std::transform(str.begin(), str.end(), uppercase.begin(), ::toupper);

		if (uppercase.find("CENTER") != std::string::npos) return 0.0f;

		const auto negative = uppercase.find('L') != std::string::npos || uppercase.find('-') != std::string::npos;

		auto value = find_positive_number<int>(str);

		if (!value) return std::optional<float>();

		return (float(*value) / 100) * (negative ? -1 : 1);
	};
}

namespace pitch
{
	inline auto stepify(float v) -> float
	{
		return stepify_0_1(v);
	}

	inline auto snap_value(float v, float step_size, float snap_amount)
	{
		return stepify(std_params::snap_value(v, step_size, snap_amount));
	}

	inline float constrain(float v)
	{
		if (v < -60.0f) return -60.0f;
		if (v > 60.0f) return 60.0f;

		return v;
	};

	inline auto increment(float v, bool precise)
	{
		return constrain(stepify(std_params::increment<1, 10>(v, precise)));
	};

	inline auto decrement(float v, bool precise)
	{
		return constrain(stepify(std_params::decrement<1, 10>(v, precise)));
	};

	inline auto drag(float v, int amount, bool precise) -> float
	{
		return constrain(stepify(std_params::drag<1, 10>(v, amount / 5, precise)));
	};
}

namespace speed
{
	constexpr auto FREEZE = 0.0f;
	constexpr auto THIRTYSECOND = 0.03125f;
	constexpr auto SIXTEENTH = 0.0625f;
	constexpr auto EIGHTH = 0.125f;
	constexpr auto QUARTER = 0.25f;
	constexpr auto HALF = 0.5f;
	constexpr auto NORMAL = 1.0f;
	constexpr auto DOUBLE = 2.0f;
	constexpr auto TRIPLE = 3.0f;

	inline auto constrain(float v)
	{
		if (v < math::convert::linear_to_speed(-8.0f)) return FREEZE;
		if (v > 32.0f) return 32.0f;

		return v;
	};

	inline auto increment(float v, bool precise)
	{
		if (v <= FREEZE) return math::convert::linear_to_speed(-8.0f);

		return constrain(math::convert::linear_to_speed(std_params::increment<1, 10>(math::convert::speed_to_linear(v), precise)));
	};

	inline auto decrement(float v, bool precise)
	{
		return constrain(math::convert::linear_to_speed(std_params::decrement<1, 10>(math::convert::speed_to_linear(v), precise)));
	};

	inline auto drag(float v, int amount, bool precise) -> float
	{
		if (v <= FREEZE) v = math::convert::linear_to_speed(-8.0f);

		return constrain(math::convert::linear_to_speed(std_params::drag<1, 10>(math::convert::speed_to_linear(v), amount / 5, precise)));
	};

	inline auto from_string(const std::string& str) -> std::optional<float>
	{
		std::string uppercase = str;

		std::transform(str.begin(), str.end(), uppercase.begin(), ::toupper);

		if (uppercase.find("FREEZE") != std::string::npos) return FREEZE;
		if (uppercase.find("1/32") != std::string::npos) return THIRTYSECOND;
		if (uppercase.find("1/16") != std::string::npos) return SIXTEENTH;
		if (uppercase.find("1/8") != std::string::npos) return EIGHTH;
		if (uppercase.find("1/4") != std::string::npos) return QUARTER;
		if (uppercase.find("1/2") != std::string::npos) return HALF;
		if (uppercase.find("NORMAL") != std::string::npos) return NORMAL;
		if (uppercase.find("DOUBLE") != std::string::npos) return DOUBLE;
		if (uppercase.find("TRIPLE") != std::string::npos) return TRIPLE;

		auto ff = find_number<float>(str);

		if (!ff) return ff;

		return *ff;
	};

	inline auto display(float v)
	{
		const auto milestone_hit = [](float value, float milestone)
		{
			constexpr auto threshold = 0.001f;

			return value > milestone - threshold && value < milestone + threshold;
		};

		std::stringstream ss;

		if (v <= FREEZE)
		{
			ss << "Freeze";
		}
		else if (milestone_hit(v, THIRTYSECOND))
		{
			ss << "1/32";
		}
		else if (milestone_hit(v, SIXTEENTH))
		{
			ss << "1/16";
		}
		else if (milestone_hit(v, EIGHTH))
		{
			ss << "1/8";
		}
		else if (milestone_hit(v, QUARTER))
		{
			ss << "1/4";
		}
		else if (milestone_hit(v, HALF))
		{
			ss << "1/2";
		}
		else if (milestone_hit(v, NORMAL))
		{
			ss << "Normal";
		}
		else if (milestone_hit(v, DOUBLE))
		{
			ss << "Double";
		}
		else if (milestone_hit(v, TRIPLE))
		{
			ss << "Triple";
		}
		else
		{
			ss << "x" << v;
		}

		return ss.str();
	}
}

namespace sample_offset {

inline auto constrain(int v)
{
	return v;
};

inline auto from_string(const std::string& str) -> std::optional<float>
{
	auto db = find_number<float>(str);

	if (!db) return db;

	return math::convert::db_to_linear(*db);
};

inline auto increment(int v, bool precise)
{
	return v + 1;
};

inline auto decrement(int v, bool precise)
{
	return v - 1;
};

inline auto drag(int v, int amount, bool precise) -> int
{
	return v + (amount / (precise ? 50 : 1));
};

}

namespace sliders
{

inline SliderSpec<float> amp()
{
	SliderSpec<float> out;

	out.constrain = amp::constrain;
	out.increment = amp::increment;
	out.decrement = amp::decrement;
	out.drag = amp::drag;
	out.from_string = amp::from_string;
	out.display_value = amp::display;
	out.stepify = amp::stepify;
	out.default_value = 1.0f;

	return out;
}

inline SliderSpec<float> pan()
{
	SliderSpec<float> out;

	out.constrain = pan::constrain;
	out.increment = pan::increment;
	out.decrement = pan::decrement;
	out.drag = pan::drag;
	out.from_string = pan::from_string;
	out.display_value = pan::display;
	out.stepify = pan::stepify;
	out.default_value = 0.0f;

	return out;
}

inline SliderSpec<float> pitch()
{
	SliderSpec<float> out;

	out.constrain = pitch::constrain;
	out.increment = pitch::increment;
	out.decrement = pitch::decrement;
	out.drag = pitch::drag;
	out.display_value = display_number<float>;
	out.from_string = find_number<float>;
	out.stepify = pitch::stepify;
	out.default_value = 0.0f;

	return out;
}

inline SliderSpec<float> speed()
{
	SliderSpec<float> out;

	out.constrain = speed::constrain;
	out.increment = speed::increment;
	out.decrement = speed::decrement;
	out.drag = speed::drag;
	out.display_value = speed::display;
	out.from_string = speed::from_string;
	out.default_value = 1.0f;

	return out;
}

inline SliderSpec<int> sample_offset()
{
	SliderSpec<int> out;

	out.constrain = sample_offset::constrain;
	out.increment = sample_offset::increment;
	out.decrement = sample_offset::decrement;
	out.drag = sample_offset::drag;
	out.display_value = display_number<int>;
	out.from_string = find_number<int>;
	out.default_value = 0;

	return out;
}

inline SliderSpec<float> percentage()
{
	SliderSpec<float> out;

	out.constrain = percentage::constrain;
	out.increment = percentage::increment;
	out.decrement = percentage::decrement;
	out.drag = percentage::drag;
	out.display_value = percentage::display;
	out.from_string = percentage::from_string;
	out.default_value = 0;

	return out;
}

inline SliderSpec<float> percentage_bipolar()
{
	SliderSpec<float> out;

	out.constrain = percentage::constrain;
	out.increment = percentage_bipolar::increment;
	out.decrement = percentage_bipolar::decrement;
	out.drag = percentage_bipolar::drag;
	out.display_value = percentage_bipolar::display;
	out.from_string = percentage::from_string;
	out.default_value = 0;

	return out;
}

inline SliderSpec<float> filter_frequency()
{
	SliderSpec<float> out;

	out.constrain = filter_frequency::constrain;
	out.increment = filter_frequency::increment;
	out.decrement = filter_frequency::decrement;
	out.drag = filter_frequency::drag;
	out.display_value = filter_frequency::display;
	out.from_string = filter_frequency::from_string;
	out.default_value = 0;

	return out;
}

namespace parameters {

inline SliderParameterSpec<float> amp()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SLIDER_AMP;
	out.name = "Amp";

	out.slider = sliders::amp();
	out.icon = blink_StdIcon_Amp;
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

inline SliderParameterSpec<float> pan()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SLIDER_PAN;
	out.name = "Pan";

	out.slider = sliders::pan();
	out.icon = blink_StdIcon_Pan;

	return out;
}

inline SliderParameterSpec<float> pitch()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SLIDER_PITCH;
	out.name = "Pitch";

	out.slider = sliders::pitch();
	out.icon = blink_StdIcon_Pitch;
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

inline SliderParameterSpec<float> speed()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SLIDER_SPEED;
	out.name = "Speed";

	out.slider = sliders::speed();
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

inline SliderParameterSpec<int> sample_offset()
{
	SliderParameterSpec<int> out;

	out.uuid = BLINK_STD_UUID_SLIDER_SAMPLE_OFFSET;
	out.name = "Sample Offset";

	out.slider = sliders::sample_offset();

	out.icon = blink_StdIcon_SampleOffset;
	out.flags = blink_SliderFlags_MovesDisplay;

	return out;
}

SliderParameterSpec<float> noise_width()
{
	SliderParameterSpec<float> out;

	out.uuid = BLINK_STD_UUID_SLIDER_NOISE_WIDTH;
	out.name = "Noise Width";

	out.slider = std_params::sliders::percentage();

	return out;
}

}

} // sliders

namespace toggles {

inline ToggleSpec loop()
{
	ToggleSpec out;

	out.uuid = BLINK_STD_UUID_TOGGLE_LOOP;
	out.name = "Loop";
	out.flags = blink_ToggleFlags_ShowInContextMenu | blink_ToggleFlags_ShowButton | blink_ToggleFlags_MovesDisplay;
	out.default_value = false;

	return out;
}

inline ToggleSpec reverse()
{
	ToggleSpec out;

	out.uuid = BLINK_STD_UUID_TOGGLE_REVERSE;
	out.name = "Reverse";
	out.flags = blink_ToggleFlags_ShowInContextMenu | blink_ToggleFlags_ShowButton | blink_ToggleFlags_MovesDisplay;
	out.default_value = false;

	return out;
}

} // toggles

namespace options {

inline OptionSpec noise_mode()
{
	OptionSpec out;

	out.uuid = BLINK_STD_UUID_OPTION_NOISE_MODE;
	out.name = "Noise Mode";
	out.default_index = 0;
	out.options = {
		"Multiply",
		"Mix",
	};

	return out;
}

}

namespace envelopes {

// returns the y value at the given block position
// [search_beg_index] is the index of the point to begin searching from
// [left] returns the index of the point to the left of the block position,
//        or zero if there isn't one.
//        in some scenarios this can be passed as search_beg_index to
//        speed up the search in the next iteration
template <class SearchFunc>
inline float generic_search(const blink_EnvelopeData* data, float default_value, blink_Position block_position, int search_beg_index, int* left, SearchFunc search)
{
	*left = 0;

	const auto clamp = [data](float value)
	{
		return std::clamp(value, data->min, data->max);
	};

	if (data->points.count < 1) return clamp(default_value);
	if (data->points.count == 1) return clamp(data->points.points[0].position.y);

	auto search_beg = data->points.points + search_beg_index;
	auto search_end = data->points.points + data->points.count;
	const auto pos = search(search_beg, search_end);

	if (pos == search_beg)
	{
		// It's the first point
		return clamp(pos->position.y);
	}

	if (pos == search_end)
	{
		// No points to the right so we're at the end of the envelope
		*left = int(std::distance<const blink_EnvelopePoint*>(data->points.points, (pos - 2)));

		return clamp((pos - 1)->position.y);
	}

	// We're somewhere in between two envelope points. Linear interpolate
	// between them.
	const auto p0 = (pos - 1)->position;
	const auto p1 = pos->position;

	const auto segment_size = p1.x - p0.x;	// Should never be zero
	const auto r = (block_position - p0.x) / segment_size;

	*left = int(std::distance<const blink_EnvelopePoint*>(data->points.points, (pos - 1)));

	return math::lerp(clamp(p0.y), clamp(p1.y), float(r));
}

// Use a binary search to locate the envelope position
inline float generic_search_binary(const blink_EnvelopeData* data, float default_value, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_EnvelopePoint* beg, const blink_EnvelopePoint* end)
	{
		const auto less = [](blink_Position position, const blink_EnvelopePoint& point)
		{
			return position < point.position.x;
		};

		return std::upper_bound(beg, end, block_position, less);
	};

	return generic_search(data, default_value, block_position, search_beg_index, left, find);
}

// Use a forward search to locate the envelope position (can be
// faster when envelope is being traversed forwards)
inline float generic_search_forward(const blink_EnvelopeData* data, float default_value, blink_Position block_position, int search_beg_index, int* left)
{
	const auto find = [block_position](const blink_EnvelopePoint* beg, const blink_EnvelopePoint* end)
	{
		const auto greater = [block_position](const blink_EnvelopePoint& point)
		{
			return point.position.x > block_position;
		};

		return std::find_if(beg, end, greater);
	};

	return generic_search(data, default_value, block_position, search_beg_index, left, find);
}

inline EnvelopeSpec amp()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_AMP;
	out.name = "Amp";

	out.get_gridline = [](int index) -> float
	{
		return math::convert::linear_to_speed(float(index));
	};

	out.default_value = 1.0f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.display_value = amp::display;
	out.stepify = amp::stepify;

	out.value_slider = sliders::amp();

	out.range.min.default_value = 0.0f;
	out.range.min.display_value = amp::display;

	out.range.max.default_value = 1.0f;
	out.range.max.display_value = amp::display;
	out.range.max.from_string = amp::from_string;
	out.range.max.constrain = amp::constrain;
	out.range.max.increment = amp::increment;
	out.range.max.decrement = amp::decrement;
	out.range.max.drag = amp::drag;

	out.flags = blink_EnvelopeFlags_MovesDisplay;

	return out;
}

inline EnvelopeSpec pan()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_PAN;
	out.name = "Pan";

	out.default_value = 0.0f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = pan::stepify;

	out.display_value = pan::display;
	out.value_slider = sliders::pan();

	out.range.min.default_value = -1.0f;
	out.range.min.display_value = pan::display;

	out.range.max.default_value = 1.0f;
	out.range.max.display_value = pan::display;

	out.flags = blink_EnvelopeFlags_SnapToDefaultOnly | blink_EnvelopeFlags_NoGridLabels;

	return out;
}

inline EnvelopeSpec pitch()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_PITCH;
	out.name = "Pitch";

	out.default_value = 0.0f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = pitch::stepify;
	out.snap_value = pitch::snap_value;

	out.value_slider = sliders::pitch();

	out.display_value = display_number<float>;

	out.get_gridline = [](int index) -> float
	{
		return float(index * 12);
	};

	out.get_stepline = [](int index, float step_size) -> float
	{
		return step_size * index;
	};

	out.range.min.constrain = pitch::constrain;
	out.range.min.decrement = pitch::decrement;
	out.range.min.increment = pitch::increment;
	out.range.min.default_value = -24.0f;
	out.range.min.display_value = display_number<float>;
	out.range.min.drag = pitch::drag;
	out.range.min.from_string = find_number<float>;

	out.range.max.constrain = pitch::constrain;
	out.range.max.decrement = pitch::decrement;
	out.range.max.increment = pitch::increment;
	out.range.max.default_value = 24.0f;
	out.range.max.display_value = display_number<float>;
	out.range.max.drag = pitch::drag;
	out.range.max.from_string = find_number<float>;

	out.step_size.constrain = [](float v) { return constrain(v, 0.0f, 60.0f); };
	out.step_size.decrement = [out](float v, bool precise) { return out.step_size.constrain(decrement<1, 10>(v, precise)); };
	out.step_size.increment = [out](float v, bool precise) { return out.step_size.constrain(increment<1, 10>(v, precise)); };
	out.step_size.default_value = 1.0f;
	out.step_size.display_value = display_number<float>;
	out.step_size.drag = [out](float v, int amount, bool precise) { return out.step_size.constrain(pitch::drag(v, amount, precise)); };
	out.step_size.from_string = find_number<float>;

	out.default_snap_amount = 1.0f;

	out.flags = blink_EnvelopeFlags_MovesDisplay;

	return out;
}

inline EnvelopeSpec speed()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_SPEED;
	out.name = "Speed";

	out.default_value = speed::NORMAL;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.display_value = speed::display;

	out.value_slider = sliders::speed();

	out.get_gridline = [](int index) -> float
	{
		return math::convert::linear_to_speed(float(index));
	};

	out.range.min.constrain = speed::constrain;
	out.range.min.increment = speed::increment;
	out.range.min.decrement = speed::decrement;
	out.range.min.display_value = speed::display;
	out.range.min.drag = speed::drag;
	out.range.min.from_string = speed::from_string;
	out.range.min.default_value = speed::FREEZE;

	out.range.max.constrain = speed::constrain;
	out.range.max.increment = speed::increment;
	out.range.max.decrement = speed::decrement;
	out.range.max.display_value = speed::display;
	out.range.max.drag = speed::drag;
	out.range.max.from_string = speed::from_string;
	out.range.max.default_value = 2.0f;

	out.flags = blink_EnvelopeFlags_MovesDisplay;

	return out;
}

inline EnvelopeSpec formant()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_FORMANT;
	out.name = "Formant";

	// TODO:

	//out.display_value = [](float v)
	//{
	//	std::stringstream ss;

	//	ss << v;

	//	return ss.str();
	//};

	//out.range.min_range.range.min = -60.0f;
	//out.range.min_range.range.max = 0.0f;
	//out.range.min_range.value = -24.0f;
	//out.range.min_range.step_size = 1.0f;

	//out.range.max_range.range.min = -60.0f;
	//out.range.max_range.range.max = 60.0f;
	//out.range.max_range.value = 24.0f;
	//out.range.max_range.step_size = 1.0;

	//out.step_size.range.min = 0.0f;
	//out.step_size.range.max = 60.0f;
	//out.step_size.value = 1.0f;
	//out.step_size.step_size = 1.0f;

	//out.default_value = 0.0f;
	//out.default_snap_amount = 1.0f;

	return out;
}

inline EnvelopeSpec noise_amount()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_NOISE_AMOUNT;
	out.name = "Noise Amount";

	out.default_value = 0.0f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = percentage::stepify;

	out.value_slider = sliders::percentage();

	out.range.min.default_value = 0.0f;
	out.range.min.display_value = percentage::display;
	out.range.max.default_value = 1.0f;
	out.range.max.display_value = speed::display;
	out.display_value = percentage::display;

	return out;
}

inline EnvelopeSpec noise_color()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_NOISE_COLOR;
	out.name = "Noise Color";

	out.default_value = 0.5f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = percentage::stepify;

	out.value_slider = sliders::percentage_bipolar();

	out.range.min.default_value = 0.0f;
	out.range.min.display_value = percentage_bipolar::display;
	out.range.max.default_value = 1.0f;
	out.range.max.display_value = speed::display;
	out.display_value = percentage_bipolar::display;
	out.flags = blink_EnvelopeFlags_NoGridLabels;

	return out;
}

inline EnvelopeSpec filter_frequency()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_FILTER_FREQUENCY;
	out.name = "Frequency";

	out.default_value = 0.52833f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;

	out.value_slider = sliders::filter_frequency();

	out.range.min.default_value = 0.0f;
	out.range.min.display_value = filter_frequency::display;
	out.range.max.default_value = 1.0f;
	out.range.max.display_value = filter_frequency::display;
	out.display_value = filter_frequency::display;

	return out;
}

inline EnvelopeSpec resonance()
{
	EnvelopeSpec out;

	out.uuid = BLINK_STD_UUID_ENVELOPE_FILTER_RESONANCE;
	out.name = "Resonance";

	out.default_value = 0.0f;
	out.search_binary = generic_search_binary;
	out.search_forward = generic_search_forward;
	out.stepify = percentage::stepify;

	out.value_slider = sliders::percentage();

	out.range.min.default_value = 0.0f;
	out.range.min.display_value = percentage::display;
	out.range.max.default_value = 1.0f;
	out.range.max.display_value = speed::display;
	out.display_value = percentage::display;

	return out;
}

} // envelopes

}}