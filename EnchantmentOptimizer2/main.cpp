#include <iostream>

#include "Enchantments.h"
#include "CombineOptimizer.h"

#include "strutil.h"
#include "Timer.h"

int main()
{
	Enchantments::LogAllEnchants();

	std::string enchantsListStr = Input("Enter the enchantments you would like to apply:\n");
	std::vector<std::string> idStrsVec = Split(RemoveWhitespace(enchantsListStr), ",");

	std::vector<Item> items { { 0, 0 } }; // Initialize 'items' with target item
	for (const std::string& idStr : idStrsVec)
		items.push_back(Enchantments::ConvertIdToItem(std::stoul(idStr)));

	// Determine optimal combination order
	CombineOptimizer combiner{ (uint32_t)idStrsVec.size() };

	TIMER(search);
	auto[cost, order] = combiner.CombineOptimally(items);
	STOP_LOG(search);

	// Log output
	std::vector<DisplayItem> displayItems { { { }, false } };
	for (const std::string& idStr : idStrsVec)
		displayItems.push_back(Enchantments::ConvertIdToDisplayItem(std::stoul(idStr)));

	combiner.DisplayOrder(displayItems, items, order);
}