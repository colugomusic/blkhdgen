#pragma once

#include <optional>
#include <string>
#include <blink.h>
#include "parameter_spec.hpp"

namespace blink {

class Parameter
{
public:

	Parameter(const ParameterSpec& spec)
		: uuid_(spec.uuid)
		, name_(spec.name)
	{
		if (spec.long_desc)
		{
			long_desc_ = spec.long_desc;
		}
	}

	void set_group_index(int group_index) { group_index_ = group_index; }

	blink_UUID get_uuid() const { return uuid_; }
	int get_group_index() const { return group_index_; }
	const char* get_name() const { return name_.c_str(); }
	const char* get_long_desc() const { return long_desc_ ? long_desc_->c_str() : nullptr; }

	virtual blink_ParameterType get_type() const = 0;

private:

	blink_UUID uuid_;
	int group_index_ = -1;
	std::string name_;
	std::optional<std::string> long_desc_;
};

}