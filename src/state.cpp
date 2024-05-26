#include "../headers/state.hpp"
#include <unordered_map>
#include <optional>
#include <algorithm>

State::State(std::string id, bool initial, bool final)
{
    this->id = id;
    this->initial = initial;
    this->final = final;
    this->transitions = std::unordered_multimap<Symbol<std::string>, StateID, Symbol<std::string>::hash>();
};

State::~State(){};

std::string State::get_id() const
{
    return this->id;
}

void State::add_transition(Symbol<std::string> symbol, StateID state)
{
    this->transitions.insert({symbol, state});
}

std::unordered_multimap<Symbol<std::string>, StateID, Symbol<std::string>::hash> State::get_transitions() const
{
    return this->transitions;
}

std::optional<std::vector<StateID>> State::get_transitions_by(Symbol<std::string> symbol) const
{
    std::vector<StateID> states;

    auto transitions_by_symbol = this->transitions.find(symbol);

    while (transitions_by_symbol != this->transitions.end() && transitions_by_symbol->first == symbol)
    {
        states.push_back(transitions_by_symbol->second);
        transitions_by_symbol++;
    }

    auto transitions_by_lambda = symbol != Symbol<std::string>("λ") ? this->transitions.find(Symbol<std::string>("λ")) : this->transitions.end();

    while (transitions_by_lambda != this->transitions.end() && transitions_by_lambda->first == Symbol<std::string>("λ"))
    {
        states.push_back(transitions_by_lambda->second);
        transitions_by_lambda++;
    }

    if (states.empty())
        return {};

    return states;
}

void State::set_initial(bool initial)
{
    this->initial = initial;
}

void State::set_final(bool final)
{
    this->final = final;
}

bool State::is_initial() const
{
    return this->initial;
}

bool State::is_final() const
{
    return this->final;
}

State State::make_initial()
{
    this->initial = true;
    this->final = false;
    return *this;
}

State State::make_final()
{
    this->initial = false;
    this->final = true;
    return *this;
}

State State::make_unique()
{
    this->initial = true;
    this->final = true;
    return *this;
}

State State::make_standard()
{
    this->initial = false;
    this->final = false;
    return *this;
}

bool State::operator==(const State &other) const
{
    return this->id == other.get_id();
}

bool State::operator<(const State &other) const
{
    return this->id < other.get_id();
}
