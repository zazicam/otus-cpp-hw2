#include "gtest/gtest.h"

#include <fstream>
#include <algorithm>

#include "../ip_filter.hpp"

using VOS = vec_of_strings;

TEST(split_tests, test1)
{
	ASSERT_TRUE( split("", '.')      == VOS ({""}) );
	ASSERT_TRUE( split("11", '.')    == VOS ({"11"}) );
	ASSERT_TRUE( split("..", '.')    == VOS ({"", "", ""}) );
	ASSERT_TRUE( split("11.", '.')   == VOS ({"11", ""}) );
	ASSERT_TRUE( split(".11", '.')   == VOS ({"", "11"}) );
	ASSERT_TRUE( split("11.22", '.') == VOS ({"11", "22"}) );
}

TEST(greater_tests, test1)
{
	ASSERT_TRUE( greater(VOS({"0","0","0","2"}), VOS({"0","0","0","1"}) )); 
	ASSERT_TRUE( greater(VOS({"0","0","2","0"}), VOS({"0","0","1","0"}) )); 
	ASSERT_TRUE( greater(VOS({"0","2","0","1"}), VOS({"0","1","0","0"}) )); 
	ASSERT_TRUE( greater(VOS({"2","0","0","1"}), VOS({"1","0","0","0"}) )); 
	ASSERT_TRUE( greater(VOS({"1","2","3","4"}), VOS({"1","2","3","3"}) )); 
	ASSERT_TRUE( greater(VOS({"1","1","1","1"}), VOS({"0","0","0","2"}) )); 
	ASSERT_TRUE( greater(VOS({"4","3","2","1"}), VOS({"1","2","3","4"}) )); 
	ASSERT_TRUE( greater(VOS({"1","1","1","123"}), VOS({"1","1","1","122"}) )); 
}

TEST(greater_tests, test2)
{
	ASSERT_FALSE( greater(VOS({"0","0","0","0"}), VOS({"0","0","0","0"}) )); 
	ASSERT_FALSE( greater(VOS({"0","0","0","1"}), VOS({"0","0","1","0"}) )); 
	ASSERT_FALSE( greater(VOS({"0","0","0","1"}), VOS({"0","1","0","0"}) )); 
	ASSERT_FALSE( greater(VOS({"0","0","0","1"}), VOS({"1","0","0","0"}) )); 
	ASSERT_FALSE( greater(VOS({"1","2","3","4"}), VOS({"1","2","3","4"}) )); 
	ASSERT_FALSE( greater(VOS({"1","1","1","1"}), VOS({"2","2","2","2"}) )); 
	ASSERT_FALSE( greater(VOS({"1","1","1","01"}), VOS({"1","1","1","1"}) )); 
	ASSERT_FALSE( greater(VOS({"1","1","1","012"}), VOS({"1","1","1","12"}) )); 
}

TEST(is_match_tests, test1)
{
	ASSERT_TRUE( is_match(VOS({"1", "2", "3", "4"}), "*.*.*.*") );
	ASSERT_TRUE( is_match(VOS({"1", "2", "3", "4"}), "1.*.*.*") );
	ASSERT_TRUE( is_match(VOS({"1", "2", "3", "4"}), "1.2.*.*") );
	ASSERT_TRUE( is_match(VOS({"1", "2", "3", "4"}), "1.2.3.*") );
	ASSERT_TRUE( is_match(VOS({"1", "2", "3", "4"}), "1.2.3.4") );
	ASSERT_TRUE( is_match(VOS({"1", "2", "3", "4"}), "1.02.003.04") );
}

TEST(is_match_tests, test2)
{
	ASSERT_FALSE( is_match(VOS({"1", "2", "3", "4"}), "*.*.*.0") );
	ASSERT_FALSE( is_match(VOS({"1", "2", "3", "4"}), "0.*.*.*") );
	ASSERT_FALSE( is_match(VOS({"1", "2", "3", "4"}), "1.0.*.*") );
	ASSERT_FALSE( is_match(VOS({"1", "2", "3", "4"}), "1.2.0.*") );
	ASSERT_FALSE( is_match(VOS({"1", "2", "3", "4"}), "1.2.3.0") );
}

std::vector<VOS> load_input(std::string filename)
{
	std::ifstream fin(filename);
	std::vector<VOS> ip_pool;

	for(std::string line; std::getline(fin, line);)
	{
		vec_of_strings v = split(line, '\t');
		ip_pool.push_back(split(v.at(0), '.'));
	}

	fin.close();
	return ip_pool;
}

std::vector<VOS> load_result(std::string filename)
{
	std::ifstream fin(filename);
	std::vector<VOS> ip_pool;

	for(std::string line; std::getline(fin, line);)
	{
		ip_pool.push_back(split(line, '.'));
	}

	fin.close();
	return ip_pool;
}

TEST(filter_tests, test1)
{
	std::vector<VOS> ip_pool = load_input("ip_filter.tsv");
	std::vector<VOS> sorted = load_result("sort.out");
	std::sort(ip_pool.begin(), ip_pool.end(), greater);
	ASSERT_EQ(ip_pool, sorted);

	auto ip = filter(ip_pool, "1.*.*.*");
	std::vector<VOS> filtered = load_result("filter1.out");
	ASSERT_EQ(ip, filtered);
}

TEST(filter_tests, test2)
{
	std::vector<VOS> ip_pool = load_input("ip_filter.tsv");
	std::vector<VOS> sorted = load_result("sort.out");
	std::sort(ip_pool.begin(), ip_pool.end(), greater);
	ASSERT_EQ(ip_pool, sorted);

	auto ip = filter(ip_pool, "46.70.*.*");
	std::vector<VOS> filtered = load_result("filter2.out");
	ASSERT_EQ(ip, filtered);
}

TEST(filter_tests, test3)
{
	std::vector<VOS> ip_pool = load_input("ip_filter.tsv");
	std::vector<VOS> sorted = load_result("sort.out");
	std::sort(ip_pool.begin(), ip_pool.end(), greater);
	ASSERT_EQ(ip_pool, sorted);

	auto ip = filter(ip_pool, "46.*.*.*|*.46.*.*|*.*.46.*|*.*.*.46");
	std::vector<VOS> filtered = load_result("filter3.out");
	ASSERT_EQ(ip, filtered);
}
