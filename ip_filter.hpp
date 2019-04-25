#ifndef IP_FILTER_HPP
#define IP_FILTER_HPP

using vec_of_strings = std::vector<std::string>;

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

#endif // IP_FILTER_HPP
