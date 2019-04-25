#ifndef IP_FILTER_HPP
#define IP_FILTER_HPP

#include <vector>
#include <string>

using vec_of_strings = std::vector<std::string>;

vec_of_strings split(const std::string &str, char d);

bool greater(const vec_of_strings &a, const vec_of_strings &b);

bool is_match(const vec_of_strings &ip, const std::string &mask_str);

std::vector<vec_of_strings> filter(const std::vector<vec_of_strings> &ip_pool, const std::string &mask_str);

#endif // IP_FILTER_HPP
