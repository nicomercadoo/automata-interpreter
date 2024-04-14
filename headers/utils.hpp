#pragma once
#include <regex>
#include <string>
#include <vector>

template <typename T>
std::regex make_regex(T src);
std::string append(std::string dst, std::string src);
std::string& operator|=(std::string& dst, const std::string& src);
std::string wrap_into_group(std::string text);
std::vector<std::string> all_matches(std::regex re, std::string text);
