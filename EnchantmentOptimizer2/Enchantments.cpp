#include "Enchantments.h"

#include <iostream>
#include <fstream>
#include <format>

#include "strutil.h"

// Statically load enchants from file
std::vector<Enchant> Enchantments::allEnchants = LoadEnchantments("enchants.csv");

void Enchantments::LogAllEnchants()
{
	for (uint32_t id = 0; id < allEnchants.size(); id++)
		std::cout << std::format("{:<3}: {}\n", id, allEnchants[id].name);
}

Item Enchantments::ConvertIdToItem(uint32_t id)
{
	const Enchant& enchant = allEnchants[id];
	return { enchant.max * enchant.mulBook, 0 };
}

DisplayItem Enchantments::ConvertIdToDisplayItem(uint32_t id)
{
	const Enchant& enchant = allEnchants[id];
	return { { enchant.name } };
}

std::vector<Enchant> Enchantments::LoadEnchantments(const std::string& filepath)
{
	// Open file
	std::ifstream file { filepath };

	std::string line; std::vector<Enchant> ret;
	while (std::getline(file, line))
	{
		if (line.starts_with("//")) continue; // Line is a comment, skip

		std::vector<std::string> splitLine = Split(RemoveWhitespace(line), ",");

		// Add enchant to vector (name, max, mulItem, mulBook)
		ret.push_back({ splitLine[0], std::stoul(splitLine[1]), std::stoul(splitLine[2]), std::stoul(splitLine[3]) });
	}

	return ret;
}