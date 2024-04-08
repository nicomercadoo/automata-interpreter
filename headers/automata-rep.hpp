#pragma once
#include <state.hpp>
#include <symbol.hpp>
#include <set>
#include <string>


class AutomataRep
{
private:
    State *start;           // estado inicial del automata
    std::set<State> states; // acá se guardan todos los estados del automata
public:
    AutomataRep();
    ~AutomataRep();
    void add_state(std::string id, bool initial, bool final);
    void add_transition(string from_state, string symbol, string to_state);
    State *get_state(string state);
    void from_dot(string path);
    void to_dot(string path);
    // auto delta(State *state, Symbol<string> symbol);
};
