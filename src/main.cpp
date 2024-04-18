#include "../headers/automata.hpp"
#include "../headers/utils.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <regex>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    if (RuntimeCfg::help)
    {
        print_help(argv[0]);
        exit(EXIT_SUCCESS);
    }

    if (RuntimeCfg::input_path.empty())
    {
        cerr << "Input file not provided" << endl;
        exit(EXIT_FAILURE);
    }

    auto automata = Automata(AFND);
    std::ifstream file(RuntimeCfg::input_string);
    automata.from_dot(RuntimeCfg::input_path);

    if (RuntimeCfg::check_determinism)
    {
        std::string is_det = automata.deterministic_inv() ? "deterministic" : "non-deterministic";
        cout << is_det << endl;
        exit(EXIT_SUCCESS);
    }

    if (RuntimeCfg::check_syntax)
    {
        std::string is_correct = check_syntax(RuntimeCfg::input_path) ? "\033[32mcorrect syntax\033[0m" : "\033[31mincorrect syntax\033[0m";
        cout << is_correct << endl;
        exit(EXIT_SUCCESS);
    }

    if (RuntimeCfg::show)
    {
        show(RuntimeCfg::input_path);
        exit(EXIT_SUCCESS);
    }

    if (RuntimeCfg::just_make_deterministic)
    {
        automata.make_deterministic();
        automata.to_dot(RuntimeCfg::output_path);
        exit(EXIT_SUCCESS);
    }

    std::string acceptation = automata.accept(RuntimeCfg::input_string) ? "\033[32maccepted\033[0m" : "\033[31mrejected\033[0m";
    cout << acceptation << endl;

    automata.to_dot(RuntimeCfg::output_path);

    return 0;
}
