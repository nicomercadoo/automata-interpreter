#include <headers/automata-rep.hpp>
#include <headers/state.hpp>
#include <headers/symbol.hpp>
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

void AutomataRep::add_state(State *state)
{
    this->states.insert(*state);
    if (state->is_initial())
    {
        this->start = state;
    }
}

void AutomataRep::add_transition(State *from_state, Symbol<string> symbol, State *to_state)
{
    get_state(from_state->get_id())->add_transition(symbol, to_state);
}

void AutomataRep::from_dot(string path)
{
    // TODO
}

void AutomataRep::to_dot(string path)
{
    // TODO
}


