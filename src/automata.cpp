#include "../headers/automata.hpp"
#include <iostream>

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


bool Automata::accept(std::string input)
{
    // this->make_deterministic();
    return this->rep.accept(input);
}


void Automata::make_deterministic()
{
    if (this->nature == Nature::AFD)
        return;
    AutomataRep deterministic_rep = AutomataRep();

    std::set<State*> start_lambda_closure = this->rep.lambda_closure_from_start();
    // print
    std::cout << "lambda_closure:" << std::endl;

    for (auto state : start_lambda_closure)
    {
        std::cout << state->get_id() << std::endl;
    }

    std::set<State*> move = this->rep.move_from_start("b");
    // print
    std::cout << "move_from_start:" << std::endl;
    for (auto state : move)
    {
        std::cout << state->get_id() << std::endl;
    }

    // doit det...

    this->nature = Nature::AFND;
    this->rep = deterministic_rep;
}


