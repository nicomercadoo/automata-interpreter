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
    void add_state(State *state);
    void add_transition(State *from_state, Symbol<string> symbol, State *to_state);
    State *get_state(string state);
    void from_dot(string path);
    void to_dot(string path);
    // auto delta(State *state, Symbol<string> symbol);
};
