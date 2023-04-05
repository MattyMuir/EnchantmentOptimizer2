#include "strutil.h"

#include <iostream>
#include <ranges>

std::string Input(std::string_view msg)
{
	std::cout << msg;

	std::string line;
	std::getline(std::cin, line);

	return line;
}

std::string RemoveWhitespace(std::string_view str)
{
	std::string ret { str };
	ret.erase(std::remove_if(ret.begin(), ret.end(), std::isspace), ret.end());
	return ret;
}

std::vector<std::string> Split(std::string_view str, std::string_view delim)
{
	std::vector<std::string> parts;
	for (const auto& part : std::views::split(str, delim))
		parts.emplace_back(part.begin(), part.end());

	return parts;
}