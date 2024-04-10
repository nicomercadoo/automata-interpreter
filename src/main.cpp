#include "../headers/automata.hpp"
#include <iostream>
#include <regex>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    auto automata = new Automata(AFND);
    cout << "Hello, World!" << endl;

    // Print all command line arguments
    for(int i = 0; i < argc; i++) {
        cout << "Argument " << i << ": " << argv[i] << endl;
    }

    using std::regex_constants::grep;
    std::string reg = R"(^\s*([[:alnum:]]*->[[:alnum:]]+\[label="[[:alnum:]]+"\];)+)";
    cout << "a" << endl;
    const std::regex declaration_start(reg);
    cout << "b" << endl;
    const std::regex start_state("^\\s*inic->([[:alnum:]]+);");
    cout << "c" << endl;
    // const std::regex transition(R"(^\s*([[:alpha:]]\d->[[:alpha:]]\d \[label="\w+"\];)+)", egrep);
    // const std::regex final_state(R"(^\s*([[:alpha:]]\d[shape=doublecircle];)+)", egrep);
    // const std::regex declaration_end(R"(^})", egrep);
    // const std::regex irrelavant_dot_syntax(R"((\s*//.*|^rankdir = LR;|^\s*inic\[shape=point\];))");

    std::string t;
    std::smatch matches;

    cout << "Proof Regex: " << endl;
    cout << reg << endl << "text: ";
    cin  >> t;
    cout << std::boolalpha
         << std::regex_match(t, matches, declaration_start)
         << " matches: ";
    for_each(matches.begin(), matches.end(), [](auto m) { cout << m << endl; });

    return 0;
}
