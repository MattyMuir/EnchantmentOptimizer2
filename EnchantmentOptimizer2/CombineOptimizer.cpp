#include "CombineOptimizer.h"

#include <cassert>
#include <iostream>
#include <format>

ItemSet::ItemSet(const std::vector<Item>& items_)
	: size(items_.size())
{
	assert(items_.size() <= MAX_ITEMSET_SIZE);

	for (uint32_t i = 0; i < items_.size(); i++)
		items[i] = { items_[i], i + 1, i - 1 };

#ifdef _DEBUG
	items[items_.size() - 1].next = UINT32_MAX;
#endif
}

void ItemSet::CombineItems(ItemSet::Location i1, ItemSet::Location i2)
{
	// References for readability
	Element& e2 = items[i2.rawLoc];
	Item& item1 = items[i1.rawLoc].i;
	Item& item2 = items[i2.rawLoc].i;

	item1.totalEnchantCost += item2.totalEnchantCost;
	item1.priorPenalty = std::max(item1.priorPenalty, item2.priorPenalty) + 1;

	// Remove i2 from set
	if (e2.prev != UINT32_MAX) items[e2.prev].next = e2.next; // Update pointer of previous item to jump over i2
	if (e2.next != UINT32_MAX) items[e2.next].prev = e2.prev; // Update pointer of next item to jump back over i2

	size--;
}

Item& ItemSet::operator[](Location idx)
{
	return items[idx.rawLoc].i;
}

const Item& ItemSet::operator[](Location idx) const
{
	return items[idx.rawLoc].i;
}

void ItemSet::Increment(Location& it) const
{
	it.rawLoc = items[it.rawLoc].next;
}

void ItemSet::Decrement(Location& it) const
{
	it.rawLoc = items[it.rawLoc].prev;
}

std::pair<uint32_t, CombineOrder> CombineOptimizer::CombineOptimally(const ItemSet& itemSet)
{
	if (itemSet.size == 2)
	{
		// Only target item and one book remain
		auto it = itemSet.First();
		const Item& i1 = itemSet[it];
		itemSet.Increment(it);
		const Item& i2 = itemSet[it];

		CombineOrder order;
		order[numSteps - 1] = { 0, it.rawLoc };

		return { CombineCost(i1, i2), order };
	}

	uint32_t bestTotalCost = UINT32_MAX;
	CombineOrder bestOrder;

	auto firstIt = itemSet.First();
	for (uint32_t firstIndex = 0; firstIndex < itemSet.size - 1; firstIndex++)
	{
		auto secondIt = firstIt;
		itemSet.Increment(secondIt);
		for (uint32_t secondIndex = firstIndex + 1; secondIndex < itemSet.size; secondIndex++)
		{
			ItemSet setCopy{ itemSet };
			std::pair<uint32_t, uint32_t> pair;
			uint32_t totalCost = 0;

			if (firstIndex != 0 && setCopy[firstIt].totalEnchantCost < setCopy[secondIt].totalEnchantCost)
			{
				totalCost = CombineCost(setCopy[secondIt], setCopy[firstIt]);
				setCopy.CombineItems(secondIt, firstIt);
				pair = { secondIt.rawLoc, firstIt.rawLoc };
			}
			else
			{
				totalCost = CombineCost(setCopy[firstIt], setCopy[secondIt]);
				setCopy.CombineItems(firstIt, secondIt);
				pair = { firstIt.rawLoc, secondIt.rawLoc };
			}

			auto [combineCost, combineOrder] = CombineOptimally(setCopy);
			totalCost += combineCost;

			if (totalCost < bestTotalCost)
			{
				// New best order found
				bestTotalCost = totalCost;
				bestOrder = combineOrder;
				bestOrder[numSteps + 1 - itemSet.size] = pair;
			}

			itemSet.Increment(secondIt);
		}
		itemSet.Increment(firstIt);
	}

	return { bestTotalCost, bestOrder };
}

void CombineOptimizer::DisplayOrder(std::vector<DisplayItem> displayItems, ItemSet items, CombineOrder order)
{
	std::vector<std::string> logLines;
	std::vector<uint32_t> levelCosts;

	for (uint32_t stepIndex = 0; stepIndex < numSteps; stepIndex++)
	{
		const std::pair<uint32_t, uint32_t>& step = order[stepIndex];
		Item& i1 = items[{ step.first }];
		Item& i2 = items[{ step.second }];
		DisplayItem& di1 = displayItems[step.first];
		DisplayItem& di2 = displayItems[step.second];

		// Combine items
		levelCosts.push_back(CombineLevelsCost(i1, i2));
		items.CombineItems({ step.first }, { step.second });

		logLines.push_back(di1.ToString() + " + " + di2.ToString());

		// Combine display items
		for (const std::string& enchant : di2.enchants)
			di1.enchants.push_back(enchant);
	}

	size_t longestLine = 0;
	for (const std::string& line : logLines)
		longestLine = std::max(longestLine, line.size());

	for (uint32_t stepIndex = 0; stepIndex < numSteps; stepIndex++)
	{
		std::cout << std::format("{:<3}: {:<{}} | {}\n", stepIndex + 1, logLines[stepIndex], longestLine, levelCosts[stepIndex]);
	}
}

uint32_t CombineOptimizer::CombineCost(const Item& i1, const Item& i2)
{
	uint32_t levels = CombineLevelsCost(i1, i2);

	if (levels < 17) [[likely]] return (levels + 6) * levels;
	else if (levels < 32) return (levels * 5 - 81) * levels / 2 + 360;
	else return (levels * 9 - 325) * levels / 2 + 2220;
}

uint32_t CombineOptimizer::CombineLevelsCost(const Item& i1, const Item& i2)
{
	return i2.totalEnchantCost + (1UL << i1.priorPenalty) + (1UL << i2.priorPenalty) - 2;
}