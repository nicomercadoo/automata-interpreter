#pragma once
#include "symbol.hpp"
#include <unordered_map>


class State
{
private:
    std::string id;
    bool initial;
    bool final;
    std::unordered_multimap<Symbol<std::string>, State*, Symbol<std::string>::hash> transitions;

public:
    State(std::string id);
    State(std::string id, bool initial, bool final);
    ~State();

    // Add a transition to the state
    void add_transition(Symbol<std::string> symbol, State* state);

    // Get the state id
    std::string get_id() const;

    // Check if the state is initial
    bool is_initial() const;

    // Check if the state is final
    bool is_final() const;

    // Set the initial flag
    State* set_initial(bool initial);

    // Set the final flag
    State* set_final(bool final);

    // Make a State object with the final flag set to true
    State* make_initial();

    // Make a State object with the initial flag set to true
    State* make_final();

    // Make a State object with the initial and final flags set to true
    State* make_unique();

    // Make a State object with the initial and final flags set to false
    State* make_standard();

    // Overload the == operator to make the class equatable
    bool operator==(const State &other) const;

    // Overload the < operator to make the class comparable
    bool operator<(const State &other) const;
};
