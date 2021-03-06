#include "gtest/gtest.h"

#include <fstream>
#include <algorithm>

#include "../ip_filter_lib.hpp"

TEST(split_tests, test1)
{
	ASSERT_TRUE( split("", '.')      == std::vector<std::string> ({""}));
	ASSERT_TRUE( split("11", '.')    == std::vector<std::string> ({"11"}));
	ASSERT_TRUE( split("..", '.')    == std::vector<std::string> ({"", "", ""}));
	ASSERT_TRUE( split("11.", '.')   == std::vector<std::string> ({"11", ""}));
	ASSERT_TRUE( split(".11", '.')   == std::vector<std::string> ({"", "11"}));
	ASSERT_TRUE( split("11.22", '.') == std::vector<std::string> ({"11", "22"}));
}

TEST(greater_tests, test1)
{
	ASSERT_TRUE( greater(Ip({0,0,0,2}), Ip({0,0,0,1}) )); 
	ASSERT_TRUE( greater(Ip({0,0,2,0}), Ip({0,0,1,0}) )); 
	ASSERT_TRUE( greater(Ip({0,2,0,1}), Ip({0,1,0,0}) )); 
	ASSERT_TRUE( greater(Ip({2,0,0,1}), Ip({1,0,0,0}) )); 
	ASSERT_TRUE( greater(Ip({1,2,3,4}), Ip({1,2,3,3}) )); 
	ASSERT_TRUE( greater(Ip({1,1,1,1}), Ip({0,0,0,2}) )); 
	ASSERT_TRUE( greater(Ip({4,3,2,1}), Ip({1,2,3,4}) )); 
	ASSERT_TRUE( greater(Ip({1,1,1,123}), Ip({1,1,1,122}) )); 
}

TEST(greater_tests, test2)
{
	ASSERT_FALSE( greater(Ip({0,0,0,0}), Ip({0,0,0,0}) )); 
	ASSERT_FALSE( greater(Ip({0,0,0,1}), Ip({0,0,1,0}) )); 
	ASSERT_FALSE( greater(Ip({0,0,0,1}), Ip({0,1,0,0}) )); 
	ASSERT_FALSE( greater(Ip({0,0,0,1}), Ip({1,0,0,0}) )); 
	ASSERT_FALSE( greater(Ip({1,2,3,4}), Ip({1,2,3,4}) )); 
	ASSERT_FALSE( greater(Ip({1,1,1,1}), Ip({2,2,2,2}) )); 
	ASSERT_FALSE( greater(Ip({1,1,1,1}), Ip({1,1,1,1}) )); 
}

TEST(is_match_tests, test1)
{
	ASSERT_TRUE( is_match(Ip({1, 2, 3, 4}), std::vector<int> ({-1, -1, -1, -1}) ));
	ASSERT_TRUE( is_match(Ip({1, 2, 3, 4}), std::vector<int> ({1, -1, -1, -1}) ));
	ASSERT_TRUE( is_match(Ip({1, 2, 3, 4}), std::vector<int> ({1, 2, -1, -1}) ));
	ASSERT_TRUE( is_match(Ip({1, 2, 3, 4}), std::vector<int> ({1, 2, 3, -1}) ));
	ASSERT_TRUE( is_match(Ip({1, 2, 3, 4}), std::vector<int> ({1, 2, 3, 4}) ));
}

TEST(is_match_tests, test2)
{
	ASSERT_FALSE( is_match(Ip({1, 2, 3, 4}), std::vector<int> ({-1, -1, -1, 0}) ));
	ASSERT_FALSE( is_match(Ip({1, 2, 3, 4}), std::vector<int> ({0, -1, -1, -1}) ));
	ASSERT_FALSE( is_match(Ip({1, 2, 3, 4}), std::vector<int> ({1, 0, -1, -1}) ));
	ASSERT_FALSE( is_match(Ip({1, 2, 3, 4}), std::vector<int> ({1, 2, 0, -1}) ));
	ASSERT_FALSE( is_match(Ip({1, 2, 3, 4}), std::vector<int> ({1, 2, 3, 0}) ));
}

std::vector<Ip> load_input(std::string filename)
{
	std::ifstream fin(filename);
	std::vector<Ip> ip_pool;

	for(std::string line; std::getline(fin, line);)
	{
		std::vector<std::string> v = split(line, '\t');
		std::string ip_str = v.at(0);
		Ip ip = ip_from_str(ip_str);
		ip_pool.push_back(ip);
	}

	fin.close();
	return ip_pool;
}

std::vector<Ip> load_result(std::string filename)
{
	std::ifstream fin(filename);
	std::vector<Ip> ip_pool;

	for(std::string line; std::getline(fin, line);)
	{
		Ip ip = ip_from_str(line);
		ip_pool.push_back(ip);
	}

	fin.close();
	return ip_pool;
}

TEST(filter_tests, test1)
{
	std::vector<Ip> ip_pool = load_input("ip_filter.tsv");
	std::vector<Ip> sorted = load_result("sort.out");
	std::sort(ip_pool.begin(), ip_pool.end(), greater);
	ASSERT_EQ(ip_pool, sorted);

	auto ip = filter(ip_pool, "1.*.*.*");
	std::vector<Ip> filtered = load_result("filter1.out");
	ASSERT_EQ(ip, filtered);
}

TEST(filter_tests, test2)
{
	std::vector<Ip> ip_pool = load_input("ip_filter.tsv");
	std::vector<Ip> sorted = load_result("sort.out");
	std::sort(ip_pool.begin(), ip_pool.end(), greater);
	ASSERT_EQ(ip_pool, sorted);

	auto ip = filter(ip_pool, "46.70.*.*");
	std::vector<Ip> filtered = load_result("filter2.out");
	ASSERT_EQ(ip, filtered);
}

TEST(filter_tests, test3)
{
	std::vector<Ip> ip_pool = load_input("ip_filter.tsv");
	std::vector<Ip> sorted = load_result("sort.out");
	std::sort(ip_pool.begin(), ip_pool.end(), greater);
	ASSERT_EQ(ip_pool, sorted);

	auto ip = filter(ip_pool, "46.*.*.*|*.46.*.*|*.*.46.*|*.*.*.46");
	std::vector<Ip> filtered = load_result("filter3.out");
	ASSERT_EQ(ip, filtered);
}

