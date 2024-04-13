#include "../headers/state.hpp"
#include <unordered_map>

State::State(std::string id, bool initial, bool final) //: id(id), initial(initial), final(final)
{
    this->id = id;
    this->initial = initial;
    this->final = final;
    this->transitions = std::unordered_multimap<Symbol<std::string>, State*, Symbol<std::string>::hash>();
};

State::~State(){};

std::string State::get_id() const
{
    return this->id;
}

void State::add_transition(Symbol<std::string> symbol, State* state)
{
    this->transitions.insert({symbol, state});
}

std::unordered_multimap<Symbol<std::string>, State*, Symbol<std::string>::hash> State::get_transitions() const
{
    return this->transitions;
}

State *State::set_initial(bool initial)
{
    this->initial = initial;
    return this;
}

State *State::set_final(bool final)
{
    this->final = final;
    return this;
}

bool State::is_initial() const
{
    return this->initial;
}

bool State::is_final() const
{
    return this->final;
}

State *State::make_initial()
{
    this->initial = true;
    this->final = false;
    return this;
}

State *State::make_final()
{
    this->initial = false;
    this->final = true;
    return this;
}

State *State::make_unique()
{
    this->initial = true;
    this->final = true;
    return this;
}

State *State::make_standard()
{
    this->initial = false;
    this->final = false;
    return this;
}

bool State::operator==(const State &other) const
{
    return this->id == other.get_id();
}

bool State::operator<(const State &other) const
{
    return this->id < other.get_id();
}
