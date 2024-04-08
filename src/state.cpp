#include <../headers/state.hpp>

State::State(string id, bool initial, bool final) : id(id),
                                                    initial(initial),
                                                    final(final)
{
    this->transitions = unordered_multimap<Symbol<string>, State *, Symbol<string>::hash>();
};

State::~State(){};

string State::get_id() const
{
    return this->id;
}

void State::add_transition(Symbol<string> symbol, State *state)
{
    this->transitions.insert({symbol, state});
}

bool State::is_initial()
{
    return this->initial;
}

bool State::is_final()
{
    return this->final;
}

bool State::operator==(const State &other) const
{
    return this->id == other.get_id();
}

bool State::operator<(const State &other) const {
    return this->id < other.get_id();
}
