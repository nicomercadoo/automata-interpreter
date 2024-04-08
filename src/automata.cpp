#include <../headers/automata.hpp>
#include <set>

using namespace std;

Automata::Automata(Nature nature) : nature(nature)
{
    this->rep = AutomataRep();
}

void Automata::add_state(string id, bool initial, bool final)
{
    this->rep.add_state(id, initial, final);
}

void Automata::add_transition(string from_state, string symbol, string to_state)
{
    this->rep.add_transition(from_state, symbol, to_state);
}

Nature Automata::get_nature()
{
    return this->nature;
}

void Automata::from_dot(string path)
{
    this->rep.from_dot(path);
}

void Automata::to_dot(string path)
{
    this->rep.to_dot(path);
}
