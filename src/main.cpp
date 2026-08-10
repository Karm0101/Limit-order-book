#include <list>
#include <map>
#include <functional>
#include <cstdint>
#include <iostream>

struct Order
{
	unsigned int id{};
	bool is_buy{};
	unsigned short quantity{};
	bool is_limit{};
	float limit_price{};
	std::uint64_t timestamp{};
};

struct Order_location
{
	bool is_buy{};
	float limit_price{};
	std::list<Order>::iterator c_it{};
};

std::map<float, std::list<Order>, std::greater<float>> bids;
std::map<float, std::list<Order>> asks;
std::unordered_map<unsigned int, Order_location> index_map;

unsigned int new_id { 1 };
unsigned int bids_depth { 0 };
unsigned int asks_depth { 0 };

void accept_order(bool is_buy, unsigned short quantity, bool is_limit, float limit_price, std::uint64_t timestamp)
{
	Order new_order{ new_id, is_buy, quantity, is_limit, limit_price, timestamp };

	if (is_limit)
	{
		if (is_buy)
		{
			bids[limit_price].push_back(new_order);
			auto it{ std::prev(bids[limit_price].end()) };

			index_map[new_id] = { 1, limit_price, it };
			++bids_depth;
		}
		else
		{
			asks[limit_price].push_back(new_order);
			auto it{ std::prev(asks[limit_price].end()) };

			index_map[new_id] = { 0, limit_price, it };
			++asks_depth;

		}
	}
	else
	{
		// Processing for market orders goes here...
	}
	++new_id;
}

float calculate_spread()
{
	float highest_bid{ bids.begin()->first };
	float lowest_ask{ asks.begin()->first };

	float spread{ lowest_ask - highest_bid };

	return spread;
}

int calculate_depth()
{
	return bids_depth + asks_depth;
}

void book_info()
{
	std::cout << "Spread: " << calculate_spread() << '\n';
	std::cout << "Order depth: " << calculate_depth() << '\n';
}

int main()
{
	return 0;
}