#pragma once
#include <cstdint>
#include <vector>
#include <string>

struct Item
{
	uint32_t totalEnchantCost = 0;
	uint32_t priorPenalty = 0;
};

struct DisplayItem
{
	std::string ToString()
	{
		if (!isBook) return "Item";

		std::string ret { "(" };

		for (size_t i = 0; i < enchants.size(); i++)
		{
			if (i) ret += ", ";
			ret += enchants[i];
		}
		ret += ")";

		return ret;
	}

	std::vector<std::string> enchants;
	bool isBook = true;
};