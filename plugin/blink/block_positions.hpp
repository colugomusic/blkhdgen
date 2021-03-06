#pragma once

#include <limits>
#include <snd/transport/frame_position.hpp>

#pragma warning(push, 0)
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

namespace blink {

struct BlockPositions
{
	snd::transport::DSPVectorFramePosition positions;
	snd::transport::FramePosition prev_pos = std::numeric_limits<std::int32_t>::max();

	int count = kFloatsPerDSPVector;
	std::int64_t data_offset = 0;

	BlockPositions()
	{
		positions.set(count - 1, std::numeric_limits<std::int32_t>::max());
	}

	BlockPositions(const snd::transport::DSPVectorFramePosition& positions_, snd::transport::FramePosition prev_pos_ = std::numeric_limits<std::int32_t>::max())
		: positions(positions_)
		, prev_pos(prev_pos_)
	{
	}

	BlockPositions(const blink_Position* blink_positions, std::int64_t offset, int count_)
		: count(count_)
		, data_offset(offset)
	{
		for (int i = 0; i < count_; i++)
		{
			positions.set(i, blink_positions[i] - offset);
		}
	}

	void operator()(const blink_Position* blink_positions, std::int64_t offset, int count_)
	{
		prev_pos = positions[count - 1];

		for (int i = 0; i < count_; i++)
		{
			positions.set(i, blink_positions[i] - offset);
		}
		
		count = count_;
		data_offset = offset;
	}

	void operator()(const snd::transport::DSPVectorFramePosition& vec_positions, std::int64_t offset, int count_)
	{
		prev_pos = positions[count - 1];

		positions = vec_positions - std::int32_t(offset);

		count = count_;
		data_offset = offset;
	}
};

}