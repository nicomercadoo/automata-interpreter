#include "../headers/automata.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    auto automata = new Automata(AFND);
    cout << "Hello, World!" << endl;

    // Print all command line arguments
    for(int i = 0; i < argc; i++) {
        cout << "Argument " << i << ": " << argv[i] << endl;
    }

    return 0;
}
