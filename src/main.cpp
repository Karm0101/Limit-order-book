#include <list>
#include <map>
#include <functional>
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
		if (is_buy)
		{
			bids[limit_price].push_back(new_order);
			// Iterator of new bid is retrieved and stored for fast lookup
			auto it{ std::prev(bids[limit_price].end()) };

			index_map[new_id] = { 1, limit_price, it };
		}
		else
		{
			asks[limit_price].push_back(new_order);
			// Iterator of new ask is retrieved and stored for fast lookup
			auto it{ std::prev(asks[limit_price].end()) };

			index_map[new_id] = { 0, limit_price, it };
			++asks_depth;
					// Delete the bid order from index_map


		}
	}
	else
	{
		// Processing for market orders goes here...
	}
	++new_id;
}

// Calculates the difference between the highest bid and the lowest ask
float calculate_spread()
{
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
	std::cout << "Spread: " << calculate_spread() << '\n';
	std::cout << "Order depth: " << calculate_depth() << '\n';
}

int main()
{
	return 0;
}