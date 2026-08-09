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


void accept_order(unsigned int id, bool is_buy, unsigned short quantity, bool is_limit, float limit_price, std::uint64_t timestamp)
{
	Order new_order { id, is_buy, quantity, is_limit, limit_price, timestamp };

	if (is_limit)
	{
		if (is_buy)
		{
			bids[limit_price].push_back(new_order);
			auto it { std::prev(bids[limit_price].end())};

			index_map[id] = { 1, limit_price, it };
		}
		else
		{
			asks[limit_price].push_back(new_order);
			auto it{ std::prev(asks[limit_price].end()) };

			index_map[id] = { 0, limit_price, it };

		}
	}
	else
	{
		// Processing for market orders goes here...
	}

}

int main()
{
	return 0;
}