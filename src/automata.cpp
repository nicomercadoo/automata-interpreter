#include <headers/automata.hpp>
// #include <headers/state.hpp>
#include <set>

using namespace std;

Automata::Automata(Nature nature) : nature(nature)
{
    this->rep = AutomataRep();
}

void Automata::add_state(string id, bool initial, bool final)
{
    auto state = new State(id, initial, final);
    this->rep.add_state(state);
}

void Automata::add_transition(string from_state, string symbol, string to_state)
{
    auto from = this->rep.get_state(from_state);
    auto to = this->rep.get_state(to_state);
    auto symbol = Symbol<string>(symbol);
    this->rep.add_transition(from, symbol, to);
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
