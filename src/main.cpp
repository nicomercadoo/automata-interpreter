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

    if (RuntimeCfg::input_paths.empty())
    {
        cerr << "Input file not provided" << endl;
        exit(EXIT_FAILURE);
    }

    if (RuntimeCfg::merge)
    {
        auto automata1 = Automata(AFND);
        auto automata2 = Automata(AFND);
        automata1.from_dot(RuntimeCfg::input_paths[0]);
        automata2.from_dot(RuntimeCfg::input_paths[1]);
        auto merged = automata1.merge(automata2);
        merged.to_dot(RuntimeCfg::output_path);
        exit(EXIT_SUCCESS);
    }

    if (RuntimeCfg::concat)
    {
        auto automata3 = Automata(AFND);
        auto automata4 = Automata(AFND);
        automata3.from_dot(RuntimeCfg::input_paths[0]);
        automata4.from_dot(RuntimeCfg::input_paths[1]);
        auto concated = automata3.concat(automata4);
        concated.to_dot(RuntimeCfg::output_path);
        exit(EXIT_SUCCESS);
    }

    if (RuntimeCfg::kleene)
    {
        auto automata5 = Automata(AFND);
        automata5.from_dot(RuntimeCfg::input_paths[0]);
        auto kleened = automata5.kleene();
        kleened.to_dot(RuntimeCfg::output_path);
        exit(EXIT_SUCCESS);
    }

    if (RuntimeCfg::minimize)
    {
        auto automata6 = Automata(AFD);
        automata6.from_dot(RuntimeCfg::input_paths[0]);
        auto minimized = automata6.minimize();
        minimized.to_dot(RuntimeCfg::output_path);
        exit(EXIT_SUCCESS);
    }

    auto automata = Automata(AFND);
    std::ifstream file(RuntimeCfg::input_string);
    automata.from_dot(RuntimeCfg::input_paths[0]);

    if (RuntimeCfg::check_determinism)
    {
        std::string is_det = automata.deterministic_inv() ? "deterministic" : "non-deterministic";
        cout << is_det << endl;
        exit(EXIT_SUCCESS);
    }

    if (RuntimeCfg::check_syntax)
    {
        std::string is_correct = check_syntax(RuntimeCfg::input_paths[0]) ? "\033[32mcorrect syntax\033[0m" : "\033[31mincorrect syntax\033[0m";
        cout << is_correct << endl;
        exit(EXIT_SUCCESS);
    }

    if (RuntimeCfg::show)
    {
        show(RuntimeCfg::input_paths[0]);
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
