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
	std::list<unsigned int>::iterator c_it{};
};

std::map<float, std::list<Order>, std::greater<float>> bids;
std::map<float, std::list<Order>> asks;
std::unordered_map<unsigned int, Order_location> index_map;

int main()
{
	return 0;
}