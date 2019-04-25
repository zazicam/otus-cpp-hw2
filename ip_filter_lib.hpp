#ifndef IP_FILTER_HPP
#define IP_FILTER_HPP

#include <vector>
#include <string>

using Ip = std::vector<std::string>;

Ip split(const std::string &str, char d);

bool greater(const Ip &a, const Ip &b);

bool is_match(const Ip &ip, const std::string &mask_str);

std::vector<Ip> filter(const std::vector<Ip> &ip_pool, const std::string &mask_str);

#endif // IP_FILTER_HPP
