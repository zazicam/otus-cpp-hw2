#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using vec_of_strings = std::vector<std::string>;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
vec_of_strings split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void print(const std::vector<vec_of_strings> &ip_pool)
{
	for(auto ip: ip_pool)
	{
		for(auto ip_part = ip.cbegin(); ip_part!=ip.cend(); ++ip_part) 
		{
			if (ip_part != ip.cbegin())
				std::cout << ".";
			std::cout << *ip_part;
		}
		std::cout << std::endl;
	}
}

bool greater(const vec_of_strings &a, const vec_of_strings &b)
{
	int size = std::min(a.size(), b.size());
	for(int i=0;i<size;i++)
	{
		int x = std::stoi(a[i]);
		int y = std::stoi(b[i]);
		if(x>y) return true;
		if(x<y) return false;
	}
	return false;
}

bool is_match(const vec_of_strings &ip, const std::string &mask_str)
{
	vec_of_strings mask = split(mask_str, '.');
	for(int i=0;i<mask.size();i++)
	{
		if(mask[i]!="*" && std::stoi(ip[i]) != std::stoi(mask[i]))
			return false;
	}
	return true;
}

std::vector<vec_of_strings> filter(const std::vector<vec_of_strings> &ip_pool, const std::string &mask_str)
{
	std::vector<vec_of_strings> res;
	vec_of_strings masks = split(mask_str, '|');
	for(auto ip: ip_pool)
	{
		for(auto mask: masks)
		{
			if(is_match(ip, mask)) 
			{
				res.push_back(ip);
				break;
			}
		}
	}
	return res;
}

int main(int argc, char const *argv[])
{
    try
    {
        std::vector<vec_of_strings> ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            vec_of_strings v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        // TODO reverse lexicographically sort
		std::sort(ip_pool.begin(), ip_pool.end(), greater);
		print(ip_pool);

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)
		auto ip = filter(ip_pool, "1.*.*.*");
		print(ip);		

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
		ip = filter(ip_pool, "46.70.*.*");
		print(ip);		

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)
		ip = filter(ip_pool, "46.*.*.*|*.46.*.*|*.*.46.*|*.*.*.46");
		print(ip);

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
