#include "../headers/automata-rep.hpp"
#include "../headers/state.hpp"
#include "../headers/symbol.hpp"
#include <unordered_map>
#include <optional>
#include <fstream>
#include <regex>

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
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    using std::regex_constants::egrep;

    std::smatch matches;

    const std::regex declaration_start(R"(^digraph\s{$)");
    const std::regex start_state(R"("^\s*inic->([[:alnum:]]+);")");
    const std::regex transition(R"(^\s*([[:alnum:]]*->[[:alnum:]]+ \[label="[[:alnum:]]+"\];)+)");
    const std::regex final_state(R"(^\s*([[:alnum:]]+[shape=doublecircle];)+)");
    const std::regex declaration_end(R"(^})");
    const std::regex irrelavant_dot_syntax(R"((\s*//.*|^rankdir = LR;|^\s*inic\[shape=point\];))");

    bool start_found = false;
    std::string line;
    int ln = 0;

    // digraph {
    getline(file, line); ln++;
    if (!std::regex_match(line, declaration_start))
        throw std::runtime_error("Invalid declaration start at line " + std::to_string(ln) + ": " + line);

    while (getline(file, line)) {
        ln++;
        if (std::regex_match(line, declaration_end)) break;
        if (std::regex_match(line, irrelavant_dot_syntax)) continue;

        // "^\s*inic->([[:alnum:]]+);"
        if (std::regex_match(line, matches, start_state) && !start_found) {
            std::string state_id = matches[1];
            State* f = get_state(state_id).value_or(add_state(state_id, true, false));
            this->start = f;
            continue;
        } else {
            throw std::runtime_error("Invalid declaration at line " + std::to_string(ln) + ": " + line);
        }


        if (std::regex_match(line, matches, transition)) {
            std::string from_state = matches[1];
            std::string to_state = matches[2];
            std::string symbol = matches[3];
            add_transition(from_state, symbol, to_state);
        } else if (std::regex_match(line, matches, final_state)) {
            std::string state_id = matches[1];
            add_state(state_id)->set_final(true);
        } else {
            throw std::runtime_error("Invalid declaration at line " + std::to_string(ln) + ": " + line);
        }
    }


    // }
    getline(file, line); ln++;
    if (!std::regex_match(line, declaration_end))
        throw std::runtime_error("Invalid declaration at line " + std::to_string(ln) + ": " + line);

    file.close();
}

void AutomataRep::to_dot(std::string path)
{
    // TODO
}
