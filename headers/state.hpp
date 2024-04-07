#pragma once
#include <string>
#include <unordered_map>
#include <set>
#include <symbol.hpp>

using namespace std;

class State
{
private:
    string id;
    bool initial;
    bool final;
    unordered_multimap<Symbol<string>, State*> transitions;

public:
    State(string id, bool initial, bool final);
    ~State();
    void add_transition(Symbol<string> symbol, State* state);
    string get_id() const;
    bool is_initial();
    bool is_final();

    // Overload the == operator to make the class equatable
    bool operator==(const State &other) const;
};
