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

std::map<float, std::list<Order>, std::greater<float>> bids;
std::map<float, std::list<Order>> asks;

int main()
{
	return 0;
}