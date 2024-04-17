#include "../headers/automata.hpp"
#include "../headers/utils.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <regex>
#include <string>

//TODO: modificar to_dot para cuando haya mas de un simbolo por transicion
//TODO: parsear argumentos
//TODO: implementar flags y runtimecfg
//TODO: implementar usage
//TODO: hacer el readme

using namespace std;

int main(int argc, char* argv[]) {
    // Print all command line arguments
    // for(int i = 0; i < argc; i++) {
    //     cout << "Argument " << i << ": " << argv[i] << endl;
    // }
    parse_args(argc, argv);

    auto automata = Automata(AFND);
    std::ifstream file(RuntimeCfg::input_string);

    automata.from_dot(RuntimeCfg::input_path);

    std::string acceptation = automata.accept(RuntimeCfg::input_string) ? "\033[32maccepted\033[0m" : "\033[31mrejected\033[0m";
    cout << acceptation << endl;

    automata.to_dot(RuntimeCfg::output_path);

    return 0;
}
