#pragma once

#include <string>
#include <list>

using Employee = std::string;

std::string FindAddr(std::list<Employee> l, std::string name)
{
	for (std::list<Employee>::iterator i = l.begin();
		i != l.end();
		i++)
	{
		if (*i == name)
		{
			return *i;
		}
	}
	return "";
}

