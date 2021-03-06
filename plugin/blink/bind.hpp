#pragma once

#include "chord_parameter.hpp"
#include "envelope_parameter.hpp"
#include "envelope_range.hpp"
#include "envelope_range_attribute.hpp"
#include "option_parameter.hpp"
#include "slider_parameter.hpp"
#include "slider_parameter_spec.hpp"
#include "toggle_parameter.hpp"
#include "group.hpp"

namespace blink {
namespace bind {

blink_Parameter parameter(const Parameter& parameter);

inline blink_IntRange range(const Range<int>& range)
{
	blink_IntRange out;

	out.min = range.min;
	out.max = range.max;

	return out;
}

inline blink_Slider slider(const Slider<float>& slider)
{
	blink_Slider out = { 0 };

	const auto& spec = slider.spec();

	out.default_value = spec.default_value;

	const auto no_control = !spec.constrain || !spec.decrement || !spec.to_string || !spec.drag || !spec.from_string || !spec.increment;

	if (!no_control)
	{
		out.proc_data = (void*)(&slider);

		out.constrain = [](void* proc_data, float value)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->tweaker().constrain(value);
		};

		out.increment = [](void* proc_data, float value, bool precise)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->tweaker().increment(value, precise);
		};

		out.decrement = [](void* proc_data, float value, bool precise)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->tweaker().decrement(value, precise);
		};

		out.drag = [](void* proc_data, float value, int amount, bool precise)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->tweaker().drag(value, amount, precise);
		};

		out.display_value = [](void* proc_data, float value)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->display_value(value);
		};

		out.from_string = [](void* proc_data, const char* str, float* value)
		{
			auto slider = (Slider<float>*)(proc_data);

			auto result = slider->tweaker().from_string(str);

			if (result)
			{
				*value = *result;
				return true;
			}

			return false;
		};

		out.stepify = [](void* proc_data, float value)
		{
			auto slider = (Slider<float>*)(proc_data);

			return slider->tweaker().stepify(value);
		};
	}

	return out;
}

inline blink_IntSlider slider(const Slider<int>& slider)
{
	blink_IntSlider out;

	out.proc_data = (void*)(&slider);
	out.default_value = slider.spec().default_value;

	out.constrain = [](void* proc_data, int value)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->tweaker().constrain(value);
	};

	out.increment = [](void* proc_data, int value, bool precise)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->tweaker().increment(value, precise);
	};

	out.decrement = [](void* proc_data, int value, bool precise)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->tweaker().decrement(value, precise);
	};

	out.drag = [](void* proc_data, int value, int amount, bool precise)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->tweaker().drag(value, amount, precise);
	};

	out.display_value = [](void* proc_data, int value)
	{
		auto slider = (Slider<int>*)(proc_data);

		return slider->display_value(value);
	};

	out.from_string = [](void* proc_data, const char* str, int* value)
	{
		auto slider = (Slider<int>*)(proc_data);

		auto result = slider->tweaker().from_string(str);

		if (result)
		{
			*value = *result;
			return true;
		}

		return false;
	};

	return out;
}

inline blink_EnvelopeSnapSettings envelope_snap_settings(const EnvelopeSnapSettings& snap_settings)
{
	blink_EnvelopeSnapSettings out;

	out.step_size = slider(snap_settings.step_size);
	out.default_snap_amount = snap_settings.default_snap_amount;

	return out;
}

inline blink_Group group(const Group& group)
{
	blink_Group out;

	out.name = group.name.c_str();

	return out;
}

inline blink_Chord chord(const ChordParameter& chord)
{
	blink_Chord out;

	out.parameter_type = blink_ParameterType_Chord;
	out.icon = chord.icon();
	out.flags = chord.flags();

	return out;
}

inline blink_Option option(const OptionParameter& option)
{
	blink_Option out;

	out.parameter_type = option.get_type();
	out.max_index = option.get_max_index();
	out.default_index = option.get_default_index();
	out.proc_data = (void*)(&option);

	out.get_text = [](void* proc_data, blink_Index index)
	{
		auto option = (const OptionParameter*)(proc_data);

		return option->get_text(index);
	};

	return out;
}

inline blink_Envelope envelope(const EnvelopeParameter& envelope)
{
	blink_Envelope out;

	out.parameter_type = blink_ParameterType_Envelope;
	out.default_value = envelope.get_default_value();
	out.flags = envelope.get_flags();
	out.proc_data = (void*)(&envelope);
	out.snap_settings = envelope_snap_settings(envelope.snap_settings());

	out.value_slider = slider(envelope.value_slider());
	out.min = slider(envelope.range().min());
	out.max = slider(envelope.range().max());
	out.options_count = envelope.get_options_count();
	out.sliders_count = envelope.get_sliders_count();

	out.get_option = [](void* proc_data, blink_Index index)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->get_option(index);
	};

	out.get_slider = [](void* proc_data, blink_Index index)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->get_slider(index);
	};

	out.get_gridline = [](void* proc_data, int index, float* out)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		auto result = envelope->get_gridline(index);

		if (result)
		{
			*out = *result;
			return true;
		}

		return false;
	};

	out.get_stepline = [](void* proc_data, int index, float step_size, float* out)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		auto result = envelope->get_stepline(index, step_size);

		if (result)
		{
			*out = *result;
			return true;
		}

		return false;
	};

	out.search = [](void* proc_data, const blink_EnvelopeData* data, float block_position)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->search(data, block_position);
	};

	out.display_value = [](void* proc_data, float value)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->display_value(value);
	};

	out.stepify = [](void* proc_data, float value)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->stepify(value);
	};

	out.snap_value = [](void* proc_data, float value, float step_size, float snap_amount)
	{
		auto envelope = (EnvelopeParameter*)(proc_data);

		return envelope->snap_value(value, step_size, snap_amount);
	};

	return out;
}

inline blink_SliderParameter slider_parameter(const SliderParameter<float>& slider_parameter)
{
	blink_SliderParameter out;

	out.parameter_type = blink_ParameterType_Slider;

	out.slider = slider(slider_parameter.slider());
	out.icon = slider_parameter.spec().icon;
	out.flags = slider_parameter.spec().flags;

	return out;
}

inline blink_IntSliderParameter slider_parameter(const SliderParameter<int>& slider_parameter)
{
	blink_IntSliderParameter out;

	out.parameter_type = blink_ParameterType_IntSlider;
	out.slider = slider(slider_parameter.slider());
	out.icon = slider_parameter.spec().icon;
	out.flags = slider_parameter.spec().flags;

	return out;
}

inline blink_Toggle toggle(const ToggleParameter& toggle)
{
	blink_Toggle out;

	out.parameter_type = blink_ParameterType_Toggle;
	out.default_value = toggle.get_default_value() ? BLINK_TRUE : BLINK_FALSE;
	out.icon = toggle.get_icon();
	out.flags = toggle.get_flags();

	return out;
}

inline blink_Parameter parameter(const Parameter& parameter)
{
	blink_Parameter out;

	out.uuid = parameter.get_uuid();
	out.group_index = parameter.get_group_index();
	out.name = parameter.get_name();
	out.long_desc = parameter.get_long_desc();

	const auto type = parameter.get_type();

	switch (type)
	{
		case blink_ParameterType_Chord:
		{
			out.parameter.chord = chord(*static_cast<const ChordParameter*>(&parameter));
			break;
		}

		case blink_ParameterType_Envelope:
		{
			out.parameter.envelope = envelope(*static_cast<const EnvelopeParameter*>(&parameter));
			break;
		}

		case blink_ParameterType_Option:
		{
			out.parameter.option = option(*static_cast<const OptionParameter*>(&parameter));
			break;
		}

		case blink_ParameterType_Slider:
		{
			out.parameter.slider = slider_parameter(*static_cast<const SliderParameter<float>*>(&parameter));
			break;
		}

		case blink_ParameterType_IntSlider:
		{
			out.parameter.int_slider = slider_parameter(*static_cast<const SliderParameter<int>*>(&parameter));
			break;
		}

		case blink_ParameterType_Toggle:
		{
			out.parameter.toggle = toggle(*static_cast<const ToggleParameter*>(&parameter));
			break;
		}
	}

	return out;
}

#ifdef BLINK_SAMPLER
inline blink_SamplerUnit sampler_unit(SamplerUnit* sampler)
{
	blink_SamplerUnit out;

	out.proc_data = sampler;

	out.process = [](void* proc_data, const blink_SamplerBuffer* buffer, float* out)
	{
		auto sampler = (SamplerUnit*)(proc_data);

		return sampler->sampler_process(buffer, out);
	};

	return out;
}

inline blink_SamplerInstance sampler_instance(SamplerInstance* instance)
{
	blink_SamplerInstance out;

	out.proc_data = instance;

	out.add_unit = [](void* proc_data)
	{
		auto instance = (SamplerInstance*)(proc_data);

		return sampler_unit(instance->add_unit());
	};

	return out;
}

#endif

#ifdef BLINK_EFFECT
inline blink_EffectUnit effect_unit(EffectUnit* unit)
{
	blink_EffectUnit out;

	out.proc_data = unit;

	out.process = [](void* proc_data, const blink_EffectBuffer* buffer, const float* in, float* out)
	{
		auto unit = (EffectUnit*)(proc_data);

		return unit->effect_process(buffer, in, out);
	};

	return out;
}

inline blink_EffectInstance effect_instance(EffectInstance* instance)
{
	blink_EffectInstance out;

	out.proc_data = instance;

	out.get_info = [](void* proc_data)
	{
		auto instance = (EffectInstance*)(proc_data);

		return instance->get_info();
	};

	out.add_unit = [](void* proc_data)
	{
		auto instance = (EffectInstance*)(proc_data);

		return effect_unit(instance->add_unit());
	};

	return out;
}
#endif

#ifdef BLINK_SYNTH
inline blink_SynthUnit synth_unit(SynthUnit* synth)
{
	blink_SynthUnit out;

	out.proc_data = synth;

	out.process = [](void* proc_data, const blink_SynthBuffer* buffer, float* out)
	{
		auto synth = (SynthUnit*)(proc_data);

		return synth->synth_process(buffer, out);
	};

	return out;
}

inline blink_SynthInstance synth_instance(SynthInstance* instance)
{
	blink_SynthInstance out;

	out.proc_data = instance;

	out.add_unit = [](void* proc_data)
	{
		auto instance = (SynthInstance*)(proc_data);

		return synth_unit(instance->add_unit());
	};

	return out;
}
#endif

}}
