#pragma once

#include <string>

std::string ToUpperCase(std::string input)
{
	std::string output = "";

	std::string::iterator i = input.begin();

	for (i; i < input.end(); i++)
	{
		char c = *i;
		if (c >= 97 && c <= 122)
			c -= 32;
		output += c;
	}

	return output;
}