#include "../headers/automata-rep.hpp"
#include "../headers/state.hpp"
#include "../headers/symbol.hpp"
#include <unordered_map>
#include <optional>

AutomataRep::AutomataRep() : states() {
    this->start = nullptr;
}

AutomataRep::~AutomataRep() {};

std::optional<State*> AutomataRep::get_state(std::string state_id)
{
    auto it = this->states.find(state_id);
    if (it != this->states.end())
        return &it->second;
    return {};
}

State* AutomataRep::add_state(State state)
{
    State* saved = &this->states.insert({state.get_id(), state}).first->second;
    return saved;
    // return &this->states[state.get_id()]; // ????
}

State* AutomataRep::add_state(std::string id)
{
    return add_state(State(id));
}

State* AutomataRep::add_state(std::string id, bool initial, bool final)
{
    return add_state(State(id, initial, final));
}

void AutomataRep::add_transition(std::string from_state, std::string symbol, std::string to_state)
{
    auto f = get_state(from_state).value_or(add_state(from_state)); // if from_state does not exist, create it
    auto t = get_state(to_state).value_or(add_state(to_state)); // if to_state does not exist, create it
    auto s = Symbol<std::string>(symbol);
    f->add_transition(s, t);
}

void AutomataRep::from_dot(std::string path)
{
    // TODO
}

void AutomataRep::to_dot(std::string path)
{
    // TODO
}
