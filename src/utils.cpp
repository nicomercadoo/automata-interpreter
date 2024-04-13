#include "../headers/utils.hpp"

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
