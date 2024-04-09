#pragma once
#include "state.hpp"
#include "symbol.hpp"
#include <map>
#include <optional>



class AutomataRep
{
private:
    State* start;           // estado inicial del automata
    std::map<std::string,State> states; // acá se guardan todos los estados del automata (se puede ver como un conjunto de State)
public:
    AutomataRep();
    ~AutomataRep();
    State* add_state(State state);
    State* add_state(std::string id);
    State* add_state(std::string id, bool initial, bool final);
    void add_transition(std::string from_state, std::string symbol, std::string to_state);
    std::optional<State*> get_state(std::string state_id);
    void from_dot(std::string path);
    void to_dot(std::string path);
    // auto delta(State *state, Symbol<std::string> symbol);
};
