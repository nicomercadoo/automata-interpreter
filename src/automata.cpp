#include "../headers/automata.hpp"
#include <iostream>

std::string Automata::rep_name()
{
    return this->rep.name;
}

Automata::Automata(Nature nature, std::string name) : nature(nature), rep(AutomataRep()) {
    this->rep.name = name;
}

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


bool Automata::accept(std::string input)
{
    if (this->nature == Nature::AFND)
        this->make_deterministic();
        // this->to_dot("deterministic.dot");
        // this->from_dot("deterministic.dot");
    return this->rep.accept(input);
}


void Automata::make_deterministic()
{
    if (this->nature == Nature::AFD)
        return;

    AutomataRep deterministic_rep = this->rep.make_deterministic();

    this->nature = Nature::AFD;
    this->rep = deterministic_rep;
}


