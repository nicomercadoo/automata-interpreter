#include <../headers/automata-rep.hpp>
#include <../headers/state.hpp>
#include <../headers/symbol.hpp>
#include <unordered_map>
#include <set>
#include <string>

AutomataRep::AutomataRep() : states() {
    this->start = nullptr;
}

AutomataRep::~AutomataRep() {};

State* AutomataRep::get_state(string state)
{
    State s = State(state, false, false);
    auto it = this->states.find(s);
    if (it != this->states.end())
    {
        return const_cast<State*>(&(*it));
    }
    return nullptr;
}

void AutomataRep::add_state(string id, bool initial, bool final)
{
    State *state = new State(id, initial, final);
    this->states.insert(*state);
    if (state->is_initial())
    {
        this->start = state;
    }
}

void AutomataRep::add_transition(string from_state, string symbol, string to_state)
{
    auto f = get_state(from_state);
    auto t = get_state(to_state);
    auto s = Symbol<string>(symbol);
    get_state(f->get_id())->add_transition(s, t);
}

void AutomataRep::from_dot(string path)
{
    // TODO
}

void AutomataRep::to_dot(string path)
{
    // TODO
}


