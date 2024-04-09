#include "../headers/automata.hpp"

Automata::Automata(Nature nature) : nature(nature), rep(AutomataRep()) {}

void Automata::add_state(std::string id, bool initial, bool final)
{
    this->rep.add_state(id, initial, final);
}

void Automata::add_transition(std::string from_state, std::string symbol, std::string to_state)
{
    this->rep.add_transition(from_state, symbol, to_state);
}

Nature Automata::get_nature()
{
    return this->nature;
}

void Automata::from_dot(std::string path)
{
    this->rep.from_dot(path);
}

void Automata::to_dot(std::string path)
{
    this->rep.to_dot(path);
}
