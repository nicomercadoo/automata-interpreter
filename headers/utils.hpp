#pragma once
#include <regex>
#include <string>
#include <vector>
#include <string>
#include <regex>

template <typename T>
std::regex make_regex(T src);
std::string append(std::string dst, std::string src);
std::string& operator|=(std::string& dst, const std::string& src);
std::string wrap_into_group(std::string text);
std::vector<std::string> all_matches(std::regex re, std::string text);

void parse_args(int argc, char *argv[]);

class RuntimeCfg {
public:
    static bool verbose;
    static bool no_convertion;
    static bool no_output_file;
    static std::string output_path;
    static std::string input_path;
    static std::string input_string;
};

namespace dot_syntax
{
    const std::regex declaration_start(R"(^digraph\s*\{(?:\s*//.*)?)");
    const std::regex start_state(R"(^\s*inic\s*->\s*(\w+);?(?:\s*//.*)?)");
    const std::regex transition(R"(^\s*(\w+)\s*->\s*(\w+)\s*\[label\s*=\s*\"([^"]+)\"\;?];?(?:\s*//.*)?)");
    const std::regex final_state(R"(^\s*(\w+)\s*\[shape\s*=\s*doublecircle;?\];?(?:\s*//.*)?)");
    const std::regex declaration_end(R"(^}(?:\s*//.*)?)");
    const std::regex irrelavant_dot_syntax(R"((^\s*$|^\s*//.*|^\s*rankdir\s*=\s*LR;?|^\s*inic\s*\[shape\s*=\s*point\;?];?|^\s*node\s*\[shape\s*=\s*circle;?\];?|^\s*edge\s*\[arrowsize\s*=\s*0.8,\s*penwidth\s*=\s*0.8\;?];?))");

} // namespace dot_syntax

