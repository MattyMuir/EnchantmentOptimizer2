#pragma once
#include <utility>

#include "Enchantments.h"
#include "Item.h"

static constexpr size_t MAX_ITEMSET_SIZE = 16;

struct CombineOrder
{
	std::pair<uint32_t, uint32_t>& operator[](size_t idx) { return steps[idx]; }

	std::pair<uint32_t, uint32_t> steps[MAX_ITEMSET_SIZE - 1];
};

class ItemSet
{
protected:
	struct Element
	{
		Item i;
		uint32_t next = 0, prev = 0;
	};

public:
	struct Location
	{
		uint32_t rawLoc;
	};

	ItemSet(const std::vector<Item>& items_);

	void CombineItems(ItemSet::Location i1, ItemSet::Location i2);

	Item& operator[](Location idx);
	const Item& operator[](Location idx) const;
	Location First() const { return { 0 }; }

	// Returns raw location of new iterator
	void Increment(Location& it) const;

	// Returns raw location of new iterator
	void Decrement(Location& it) const;

	size_t size;

protected:
	Element items[MAX_ITEMSET_SIZE];
};

class CombineOptimizer
{
public:
	CombineOptimizer(uint32_t numEnchants) : numSteps(numEnchants) {}

	std::pair<uint32_t, CombineOrder> CombineOptimally(const ItemSet& itemSet);

	void DisplayOrder(std::vector<DisplayItem> displayItems, ItemSet items, CombineOrder order);

protected:
	uint32_t numSteps;

	static uint32_t CombineCost(const Item& i1, const Item& i2);
	static uint32_t CombineLevelsCost(const Item& i1, const Item& i2);
};