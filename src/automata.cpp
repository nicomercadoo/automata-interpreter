#include "../headers/automata.hpp"
#include "../headers/utils.hpp"
#include <iostream>

Automata::Automata(Nature nature) : nature(nature), rep(AutomataRep()) {}

Automata::Automata(AutomataRep rep, Nature nature) : nature(nature), rep(rep) {}

Automata::~Automata() {}

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
    if (RuntimeCfg::verbose)
        std::cout << "Reading automata..." << std::endl;
    this->rep.from_dot(path);
}

void Automata::to_dot(std::string path)
{
    if (RuntimeCfg::verbose)
        std::cout << "Writing automata..." << std::endl;
    this->rep.to_dot(path);
}

bool Automata::accept(std::string input)
{
    this->make_deterministic();

    if (RuntimeCfg::verbose)
        std::cout << "Running automata with: \"" << input << "\"..." << std::endl;
    return this->rep.accept(input);
}

bool Automata::deterministic_inv()
{
    return this->rep.deterministic_inv();
}

void Automata::make_deterministic()
{

    if (this->nature == Nature::AFD || RuntimeCfg::no_convert)
        return;

    if (RuntimeCfg::verbose)
        std::cout << "Converting to deterministic automata..." << std::endl;
    AutomataRep deterministic_rep = this->rep.make_deterministic();

    this->nature = Nature::AFD;
    this->rep = deterministic_rep;
}

Automata Automata::merge(Automata &other)
{
    if(RuntimeCfg::verbose)
    std::cout << "Merging automatas ..." << std::endl;

    return Automata(rep.merge(other.rep), Nature::AFND);
}

Automata Automata::concat(Automata &other)
{
    if(RuntimeCfg::verbose)
    std::cout << "Concating automatas ..." << std::endl;

    return Automata(rep.concat(other.rep), Nature::AFND);
}

Automata Automata::kleene()
{
    if(RuntimeCfg::verbose)
    std::cout << "Making Kleene closure to the automata ..." << std::endl;

    return Automata(rep.kleene_closure(), Nature::AFND);
}

Automata Automata::minimize()
{
    if(RuntimeCfg::verbose)
    std::cout << "Minimizing the automata ..." << std::endl;

    if (this->nature == Nature::AFND)
        this->rep = rep.make_deterministic();

    return Automata(rep.minimize(), Nature::AFD);
}
