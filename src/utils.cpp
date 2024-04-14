#include "../headers/utils.hpp"

std::regex make_regex(std::string str){
    return std::regex(str);
}

std::string append(std::string dst, std::string src)
{
    if (dst.empty())
        return src;
    return dst + "|" + src;
}

std::string& operator|=(std::string& dst, const std::string& src)
{
    if (dst.empty())
        dst = src;
    else
        dst += "|" + src;
    return dst;
}

std::string wrap_into_group(std::string text)
{
    return "(" + text + ")";
}

std::vector<std::string> all_matches(std::regex re, std::string text)
{
    std::vector<std::string> matches;
    std::sregex_iterator it(text.begin(), text.end(), re);
    for (std::sregex_iterator i = it; i != std::sregex_iterator(); ++i)
    {
        std::smatch m = *i;
        matches.push_back(m.str());
    }
    return matches;
}
