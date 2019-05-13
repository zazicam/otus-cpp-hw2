#include "ip_filter_lib.hpp"

std::vector<std::string> split(const std::string &str, char d)
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

Ip ip_from_str(const std::string &ip_str)
{
	std::vector<std::string> ip_parts = split(ip_str, '.');
	Ip ip;
	for(const auto& part : ip_parts)
		ip.push_back( std::stoi(part) );
	return ip;
}

bool greater(const Ip &a, const Ip &b)
{
	int size = std::min(a.size(), b.size());
	for(int i=0;i<size;++i)
	{
		if(a[i]>b[i]) return true;
		if(a[i]<b[i]) return false;
	}
	return false;
}

bool is_match(const Ip &ip, const std::vector<int> &mask)
{
	for(int i=0;i<mask.size(); ++i)
	{
		if(mask[i]!=-1 && ip[i] != mask[i])
			return false;
	}
	return true;
}

std::vector<Ip> filter(const std::vector<Ip> &ip_pool, const std::string &mask_str)
{
	std::vector<Ip> result;

	// mask string can contain many masks:  "1.2.3.*|1.2.*.4"
	// so first we are to split it to list of simple masks 
	std::vector<std::string> masks_str = split(mask_str, '|');

	// convert each mask to integer representation ("*.1.2.*" --> {-1, 1, 2, -1})
	// just to speed up a little bit (to exclude extra 'stoi' conversions)
	std::vector<std::vector<int>> masks;
	for(const std::string& m_str : masks_str)
	{
		std::vector<int> mask;
		std::vector<std::string> v = split(m_str, '.');
		for(auto& part : v)
		{
			if(part=="*")
				mask.push_back( -1 );
			else
				mask.push_back( std::stoi(part) );
		}
		masks.push_back(mask);
	}

	// finaly search matching ip adrresses and append to res
	for(const auto& ip: ip_pool)
	{
		for(const auto& mask: masks)
		{
			if(is_match(ip, mask)) 
			{
				result.push_back(ip);
				break;
			}
		}
	}
	return result;
}
