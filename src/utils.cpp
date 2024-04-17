#include "../headers/utils.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <regex>

using namespace dot_syntax;

std::regex make_regex(std::string str)
{
    return std::regex(str);
}

std::string append(std::string dst, std::string src)
{
    if (dst.empty())
        return src;
    return dst + "|" + src;
}

std::string &operator|=(std::string &dst, const std::string &src)
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

bool checks_dot_sintax(std::string path)
{
    bool correct_syntax = true;
    std::string line;
    std::ifstream myfile(path);
    if (myfile.is_open())
    {
        int ln = 1;
        while (getline(myfile, line))
        {
            std::cout << line << std::endl;
            ;
            if (std::regex_match(line, declaration_start))
            {
                std::cout << line << "\033[1;90m (declaration_start)\033[0m"
                          << "\n";
            }
            else if (std::regex_match(line, start_state))
            {
                std::cout << line << "\033[1;90m (start_state)\033[0m"
                          << "\n";
            }
            else if (std::regex_match(line, transition))
            {
                std::cout << line << "\033[1;90m (transition)\033[0m"
                          << "\n";
            }
            else if (std::regex_match(line, final_state))
            {
                std::cout << line << "\033[1;90m (final_state)\033[0m"
                          << "\n";
            }
            else if (std::regex_match(line, declaration_end))
            {
                std::cout << line << "\033[1;90m (declaration_end)\033[0m"
                          << "\n";
            }
            else if (std::regex_match(line, irrelavant_dot_syntax))
            {
                std::cout << line << "\033[1;90m (irrelavant_dot_syntax)\033[0m"
                          << "\n";
            }
            else if (!std::regex_match(line, std::regex(R"(^\s*)")))
            {
                std::cout << "Invalid declaration in " << path << "\n"
                          << std::right << std::setw(6) << ln << " |" << line << "\n"
                          << "\n";
                correct_syntax = false;
            }

            ln++;
        }
        myfile.close();
    }
    return correct_syntax;
}

void parse_args(int argc, char *argv[])
{
    bool output_path_set = false;
    bool input_path_set = false;
    bool input_string_set = false;

    for (int i = 1; i < argc; i++)
    {
        if (std::regex_match(argv[i], std::regex(R"(-h|--help)")))
        {
            std::cout << "Usage: " << argv[0] << " [OPTIONS] [INPUT_FILE] [INPUT_STRING]\n"
                      << "Options:\n"
                      << "  -h, --help\t\tShow this help message\n"
                      << "  -v, --verbose\t\tVerbose mode\n"
                      << "  --no-convert\tDo not convert the input automata to deterministic\n"
                      << "  --no-output-file\tDo not output the automata to a file\n"
                      << "  -o, --output [OUTPUT_FILE]\tOutput file path\n"
                      << "  -i, --input [INPUT_FILE]\tInput file path\n"
                      << "  -s, --string [INPUT_STRING]\tInput string\n";
            exit(EXIT_SUCCESS);
        }
        else if (std::regex_match(argv[i], std::regex(R"(-v|--verbose)")))
        {
            RuntimeCfg::verbose = true;
        }
        else if (std::string(argv[i]) == "--no-convert")
        {
            RuntimeCfg::no_convertion = true;
        }
        else if (std::string(argv[i]) == "--no-output-file")
        {
            RuntimeCfg::no_output_file = true;
        }
        else if (std::regex_match(argv[i], std::regex(R"(-o|--output)")))
        {
            RuntimeCfg::output_path = argv[i + 1];
        }
        else if (!input_string_set && std::regex_match(argv[i], std::regex(R"(-i|--input)")))
        {
            RuntimeCfg::input_path = argv[i + 1];
            input_path_set = true;
        }
        else if (!input_string_set && std::regex_match(argv[i], std::regex(R"(-s|--string)")))
        {
            RuntimeCfg::input_string = argv[i + 1];
            input_string_set = true;
        }
        else if (!input_path_set && std::regex_match(argv[i], std::regex(R"(.*\.dot$)")))
        {
            RuntimeCfg::input_path = argv[i];
            input_path_set = true;
        }
        else if (!input_string_set)
        {
            RuntimeCfg::input_string = argv[i];
            input_string_set = true;
        }
        else
        {
            if (argv[i] == RuntimeCfg::input_path || argv[i] == RuntimeCfg::input_string || argv[i] == RuntimeCfg::output_path)
                continue;
            std::cout << "Invalid argument: " << argv[i] << "\n";
            exit(EXIT_FAILURE);
        }
    }
}

bool RuntimeCfg::verbose = false;
bool RuntimeCfg::no_convertion = false;
bool RuntimeCfg::no_output_file = false;
std::string RuntimeCfg::output_path = "output.dot";
std::string RuntimeCfg::input_path = "";
std::string RuntimeCfg::input_string = "";
