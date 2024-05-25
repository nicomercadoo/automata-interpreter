#pragma once
#include <regex>
#include <string>
#include <vector>
#include <string>
#include <regex>

template <typename T>
std::regex make_regex(std::string str);
std::string append(std::string dst, std::string src);
std::string& operator|=(std::string& dst, const std::string& src);
std::string wrap_into_group(std::string text);
std::vector<std::string> all_matches(std::regex re, std::string text);

void parse_args(int argc, char *argv[]);
void print_help(std::string program_name);
void show(std::string path);
bool check_syntax(std::string path);

class RuntimeCfg {
public:
    static bool help;
    static bool verbose;
    static bool just_make_deterministic;
    static bool no_convert;
    static bool no_output_file;
    static bool check_syntax;
    static bool check_determinism;
    static bool show;
    static bool merge;
    static bool concat;
    static bool kleene;
    static bool minimize;
    static std::string output_path;
    static std::vector<std::string> input_paths;
    static std::string input_string;
};

namespace dot_syntax
{
    const std::regex declaration_start(R"(^digraph\s*\{(?:\s*//.*)?)");
    const std::regex start_state(R"(^\s*inic\s*->\s*(\w+);?(?:\s*//.*)?)");
    const std::regex transition(R"(^\s*(\w+)\s*->\s*(\w+)\s*\[label\s*=\s*\"([^"]+)\"\;?];?(?:\s*//.*)?)");
    const std::regex final_state(R"(^\s*(\w+)\s*\[shape\s*=\s*doublecircle;?\];?(?:\s*//.*)?)");
    const std::regex declaration_end(R"(^}(?:\s*//.*)?)");
    const std::regex irrelavant_dot_syntax(R"((^\s*$|^\s*//.*|^\s*rankdir\s*=\s*LR;?|^\s*inic\s*\[[^\]]+\];?|^\s*node\s*\[[^\]]+\];?|^\s*edge\s*\[[^\]]+\];?))");
}
