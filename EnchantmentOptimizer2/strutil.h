#pragma once
#include <string>
#include <vector>

std::string Input(std::string_view msg);
std::string RemoveWhitespace(std::string_view str);
std::vector<std::string> Split(std::string_view str, std::string_view delim);