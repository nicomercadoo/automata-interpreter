#pragma once
#include "state.hpp"
#include "symbol.hpp"
#include <map>
#include <set>
#include <optional>
#include <regex>



class AutomataRep
{
private:
    State* start;           // estado inicial del automata
    std::set<Symbol<std::string>> alphabet; // alfabeto del automata
    std::regex valid_symbols; // expresión regular para validar los símbolos del alfabeto
    std::map<std::string,State> states; // acá se guardan todos los estados del automata (se puede ver como un conjunto de State)
public:
    std::string name; // nombre del automata
    AutomataRep();
    ~AutomataRep();
    State* add_state(State state);
    State* add_state(std::string id);
    State* add_state(std::string id, bool initial, bool final);
    void add_transition(std::string from_state, std::string symbol, std::string to_state);
    std::optional<State*> get_state(std::string state_id);
    void gen_aplhabet_valid_symbols_regex();
    std::vector<std::string> get_symbols_in_str(std::string input);
    void from_dot(std::string path);
    void to_dot(std::string path);
    bool accept(std::string input);
    bool accept(std::vector<Symbol<std::string>> input, State* current_state);
    void set_alphabet(std::set<Symbol<std::string>> alphabet);
    std::set<State*> lambda_closure(std::set<State *> states);
    std::set<State*> lambda_closure_from_start();
    std::set<State*> move(std::set<State *> states, Symbol<std::string> symbol);
    std::set<State*> move_from_start(std::string symbol);
    AutomataRep make_deterministic();
    // auto delta(State *state, Symbol<std::string> symbol);
};
