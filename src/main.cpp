#include "../headers/automata.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <regex>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Hello, World!" << endl;

    // Print all command line arguments
    for(int i = 0; i < argc; i++) {
        cout << "Argument " << i << ": " << argv[i] << endl;
    }

    // Open the file (arg 1) and print each line
    const std::regex declaration_start(R"(^digraph\s*\{(?:\s*//.*)?)");
    const std::regex start_state(R"(^\s*inic\s*->\s*(\w+);?(?:\s*//.*)?)");
    const std::regex transition(R"(^\s*(\w+)\s*->\s*(\w+)\s*\[label\s*=\s*\"([^"]+)\"\;?];?(?:\s*//.*)?)");
    const std::regex final_state(R"(^\s*(\w+)\s*\[shape\s*=\s*doublecircle;?\];?(?:\s*//.*)?)");
    const std::regex declaration_end(R"(^}(?:\s*//.*)?)");
    const std::regex irrelavant_dot_syntax(R"((^\s*$|^\s*//.*|^\s*rankdir\s*=\s*LR;?|^\s*inic\s*\[shape\s*=\s*point\;?];?))");


    if(argc > 1) {
    //     string line;
    //     ifstream myfile (argv[1]);
    //     if (myfile.is_open()) {
    //         int ln = 1;
    //         while ( getline (myfile,line) ) {
    //             cout << line << endl;
    //             if (std::regex_match(line, declaration_start)) {
    //                 cout << line << "\033[1;90m (declaration_start)\033[0m"     << "\n";
    //             } else if (std::regex_match(line, start_state)) {
    //                 cout << line << "\033[1;90m (start_state)\033[0m"           << "\n";
    //             } else if (std::regex_match(line, transition)) {
    //                 cout << line << "\033[1;90m (transition)\033[0m"            << "\n";
    //             } else if (std::regex_match(line, final_state)) {
    //                 cout << line << "\033[1;90m (final_state)\033[0m"           << "\n";
    //             } else if (std::regex_match(line, declaration_end)) {
    //                 cout << line << "\033[1;90m (declaration_end)\033[0m"       << "\n";
    //             } else if (std::regex_match(line, irrelavant_dot_syntax)) {
    //                 cout << line << "\033[1;90m (irrelavant_dot_syntax)\033[0m" << "\n";
    //             } else if (!std::regex_match(line, std::regex(R"(^\s*)"))) {
    //                 cout << "Invalid declaration in " << argv[1] << "\n" << std::right << std::setw(6) << ln << " |"<< line << "\n" << "\n";
    //             }

    //             ln++;
    //         }
    //         myfile.close();
    //     }
    //     else cout << "Unable to open file";
    }
    else {
        cout << "No file argument provided.";
    }

    auto automata = new Automata(AFND);
    automata->from_dot(argv[1]);
    automata->to_dot("output.dot");

    return 0;
}
