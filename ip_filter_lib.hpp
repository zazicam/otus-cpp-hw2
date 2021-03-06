#pragma once

#include <vector>
#include <string>

using Ip = std::vector<int>;

std::vector<std::string> split(const std::string &str, char d);

Ip ip_from_str(const std::string &ip_str);

bool greater(const Ip &a, const Ip &b);

bool is_match(const Ip &ip, const std::vector<int> &mask);

std::vector<Ip> filter(const std::vector<Ip> &ip_pool, const std::string &mask_str);
