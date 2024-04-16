#pragma once
#include "symbol.hpp"
#include <unordered_map>
#include <vector>
#include <optional>


using StateID = std::string;

class State
{
private:
    StateID id;
    bool initial;
    bool final;
    std::unordered_multimap<Symbol<std::string>, StateID, Symbol<std::string>::hash> transitions;

public:
    State(std::string id, bool initial, bool final);
    ~State();

    // Add a transition to the state
    void add_transition(Symbol<std::string> symbol, StateID state);

    // Get the state id
    std::string get_id() const;

    // Check if the state is initial
    bool is_initial() const;

    // Check if the state is final
    bool is_final() const;

    // Set the initial flag
    void set_initial(bool initial);

    // Set the final flag
    void set_final(bool final);

    // Make a State object with the final flag set to true
    void make_initial();

    // Make a State object with the initial flag set to true
    void make_final();

    // Make a State object with the initial and final flags set to true
    void make_unique();

    // Make a State object with the initial and final flags set to false
    void make_standard();

    // Get the transitions of the state
    std::unordered_multimap<Symbol<std::string>, StateID, Symbol<std::string>::hash> get_transitions() const;

    // Get the transitions of the state by symbol
    std::optional<std::vector<StateID>> get_transitions_by(Symbol<std::string> symbol) const;

    // Overload the == operator to make the class equatable
    bool operator==(const State &other) const;

    // Overload the < operator to make the class comparable
    bool operator<(const State &other) const;
};
