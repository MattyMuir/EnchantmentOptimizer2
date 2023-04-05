#pragma once
#include <string>
#include <vector>

#include "Item.h"

struct Enchant
{
	std::string name;
	uint32_t max, mulItem, mulBook;
};

class Enchantments
{
public:
	static void LogAllEnchants();
	static Item ConvertIdToItem(uint32_t id);
	static DisplayItem ConvertIdToDisplayItem(uint32_t id);

protected:
	static std::vector<Enchant> allEnchants;
	
	static std::vector<Enchant> LoadEnchantments(const std::string& filepath);
};