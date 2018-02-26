#pragma once

#include <optional>
#include <string_view>
#include <string>
#include <list>
#include <algorithm>

// for the sake of simplicity
using Employee = std::string;

std::optional<std::string_view> FindAddr(const std::list<Employee>& l, std::string_view name)
{
	auto it = std::find_if(l.cbegin(), l.cend(), [&name](const Employee& e) { return e == name; });
	return it != l.cend() ? std::make_optional<std::string_view>(*it) : std::optional<std::string_view>();
}

