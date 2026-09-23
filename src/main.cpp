#include <list>
#include <map>
#include <functional>
#include <optional>
#include <cstdint>
#include <iostream>

// Represents each processed order
struct Order
{
	// Integer data members are unsigned to allow for a greater maximum value,
	// and follow the member's nature as they are always positive
	unsigned int id{};
	bool is_buy{};
	unsigned short quantity{};
	bool is_limit{};
	float limit_price{};
};

// Used to make finding each order more efficient, i.e. O(1) time complexity
struct Order_location
{
	bool is_buy{};
	float limit_price{};
	std::list<Order>::iterator c_it{};
};

std::map<float, std::list<Order>, std::greater<float>> bids;	// Stores bids in descending order of prices
std::map<float, std::list<Order>> asks;							// Stores asks in ascending order of prices
std::unordered_map<unsigned int, Order_location> index_map;		// Stores each order, used for fast lookup

// new_id is kept as a global variable to ensure incremental order IDs are used
// This prevents potential ID-sharing
unsigned int new_id { 1 };
// Depths are stored as global variables to minimise time spent on displaying them
unsigned int bids_depth { 0 };
unsigned int asks_depth { 0 };

// Accepts and stores orders in their correct map, depending on their side
void accept_order(bool is_buy, unsigned short quantity, bool is_limit, float limit_price)
{
	Order new_order { new_id, is_buy, quantity, is_limit, limit_price };

	if (is_limit)
	{
		// Flag used to decide whether to add new orders or not
		// Set to 1 by default as orders are not guaranteed to cross the spread
		bool to_add { 1 };

		if (is_buy)
		{
			// Order matching logic

			auto temp_quantity = quantity;										// Keeps track of current order's quantity without continuously updating
			// Ensures that as long as the spread is crossed and there are orders, they will be matched
			while (!asks.begin()->second.empty() && limit_price >= asks.begin()->first && to_add)
			{
				if (temp_quantity > asks.begin()->second.front().quantity)		// New order has a greater quantity than target order
				{
					temp_quantity -= asks.begin()->second.front().quantity;		// Subtract from the quantity of the current order as its quantity > 0
					asks.begin()->second.pop_front();							// Deletes order from its list

					--asks_depth;

					// Delete the ask order from index_map
					index_map.erase(new_id);

					// Check whether the key-value pair's list is empty
					if (asks.begin()->second.empty())
					{
						// If the list of orders is empty, delete the price associated with it
						asks.erase(asks.begin());
					}
				}
				else if (temp_quantity < asks.begin()->second.front().quantity)
				{
					to_add = { 0 };
					asks.begin()->second.front().quantity -= temp_quantity;		// New order has a lesser quantity than target order
				}
				else
				{
					to_add = { 0 };

					--asks_depth;

					// Delete the ask order from index_map
					index_map.erase(new_id);

					// Check whether the key-value pair's list is empty
					if (asks.begin()->second.empty())
					{
						// If the list of orders is empty, delete the price associated with it
						asks.erase(asks.begin());
					}
				}
			}
			if (to_add)
			{
				bids[limit_price].push_back(new_order);
				// Iterator of new bid is retrieved and stored for fast lookup
				auto it{ std::prev(bids[limit_price].end()) };

				index_map[new_id] = { 1, limit_price, it };
				++bids_depth;
			}
		}
		else
		{
			// Order matching logic

			auto temp_quantity = quantity;										// Keeps track of current order's quantity without continuously updating
			// Ensures that as long as the spread is crossed and there are orders, they will be matched
			while (!bids.begin()->second.empty() && limit_price >= bids.begin()->first && to_add)
			{
				if (temp_quantity > bids.begin()->second.front().quantity)		// New order has a greater quantity than target order
				{
					temp_quantity -= bids.begin()->second.front().quantity;		// Subtract from the quantity of the current order as its quantity > 0
					bids.begin()->second.pop_front();							// Deletes order from its list

					--bids_depth;

					// Delete the bid order from index_map
					index_map.erase(new_id);

					// Check whether the key-value pair's list is empty
					if (bids.begin()->second.empty())
					{
						// If the list of orders is empty, delete the price associated with it
						bids.erase(bids.begin());
					}
				}
				else if (temp_quantity < bids.begin()->second.front().quantity)
				{
					to_add = { 0 };
					bids.begin()->second.front().quantity -= temp_quantity;		// New order has a lesser quantity than target order
				}
				else
				{
					to_add = { 0 };

					--bids_depth;

					// Delete the bid order from index_map
					index_map.erase(new_id);

					// Check whether the key-value pair's list is empty
					if (bids.begin()->second.empty())
					{
						// If the list of orders is empty, delete the price associated with it
						bids.erase(bids.begin());
					}
				}
			}
			if (to_add)
			{
				asks[limit_price].push_back(new_order);
				// Iterator of new ask is retrieved and stored for fast lookup
				auto it{ std::prev(asks[limit_price].end()) };

				index_map[new_id] = { 0, limit_price, it };
				++asks_depth;
			}
		}
	}
	else
	{
		// Processing for market orders goes here...
	}
	++new_id;
}

// Calculates the difference between the highest bid and the lowest ask
std::optional<float> calculate_spread()
{
	// Check if either maps are empty
	if (bids.empty() || asks.empty())
	{
		// If one of the maps is empty, indicate no value is returned
		return std::nullopt;
	}
	// As bids and asks are stored in their respective maps, the highest bid is always
	// the first pair in its map, and vice-versa for the lowest ask
	float highest_bid{ bids.begin()->first };
	float lowest_ask{ asks.begin()->first };

	float spread{ lowest_ask - highest_bid };

	return spread;
}

// Calculates the total number of current orders in the book
int calculate_depth()
{
	return bids_depth + asks_depth; // As both variables are global, calculating depth is more time efficient
}

// Outputs the spread and order depth to the console
void book_info()
{
	std::optional<float> spread{ calculate_spread() };

	if (spread.has_value()) // Spread is only displayed if it exists
	{
		std::cout << "Spread: " << *spread << '\n';
	}
	std::cout << "Order depth: " << calculate_depth() << '\n';
}

int main()
{
	return 0;
}