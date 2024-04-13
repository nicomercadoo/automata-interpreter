#include "../headers/automata-rep.hpp"
#include "../headers/state.hpp"
#include "../headers/symbol.hpp"
#include "../headers/utils.hpp"
#include <unordered_map>
#include <optional>
#include <fstream>
#include <regex>
#include <iostream>
#include <iomanip>

AutomataRep::AutomataRep() : states()
{
    this->start = nullptr;
}

AutomataRep::~AutomataRep(){};

std::optional<State *> AutomataRep::get_state(std::string state_id)
{
    auto it = this->states.find(state_id);
    if (it != this->states.end())
        return &it->second;
    return {};
}

State *AutomataRep::add_state(State state)
{
    // std::cout << "Adding state (State): " << state.get_id() << "\n";
    State *saved = &this->states.insert({state.get_id(), state}).first->second;
    return saved;
    // return &this->states[state.get_id()]; // ????
}

State *AutomataRep::add_state(std::string id)
{
    // std::cout << "Adding state (string): " << id << "\n";
    return add_state(State(id, false, false));
}

State *AutomataRep::add_state(std::string id, bool initial, bool final)
{
    // std::cout << "Adding state (string, bool, bool): " << id << "\n";
    return add_state(State(id, initial, final));
}

void AutomataRep::add_transition(std::string from_state, std::string symbol, std::string to_state)
{
    // std::cout << "Adding transition: " << from_state << " -> " << to_state << " [label=\"" << symbol << "\"]\n";
    auto f = get_state(from_state).value_or(add_state(from_state)); // if from_state does not exist, create it
    auto t = get_state(to_state).value_or(add_state(to_state));     // if to_state does not exist, create it
    bool f_is_in_states = this->states.find(from_state) != this->states.end();
    bool t_is_in_states = this->states.find(to_state) != this->states.end();
    // std::cout << "From state: " << f->get_id() << " is into states? = " << std::boolalpha << f_is_in_states << "\n";
    // std::cout << "To state: " << t->get_id() << " is into states? = " << std::boolalpha << f_is_in_states << "\n";
    auto s = Symbol<std::string>(symbol);
    f->add_transition(s, t);
}

void AutomataRep::from_dot(std::string path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + path);
    }

    // regexes
    const std::regex declaration_start(R"(^digraph\s*\{(?:\s*//.*)?)");
    const std::regex start_state(R"(^\s*inic\s*->\s*(\w+);?(?:\s*//.*)?)");
    const std::regex transition(R"(^\s*(\w+)\s*->\s*(\w+)\s*\[label\s*=\s*\"([^"]+)\"\;?];?(?:\s*//.*)?)");
    const std::regex final_state(R"(^\s*(\w+)\s*\[shape\s*=\s*doublecircle;?\];?(?:\s*//.*)?)");
    const std::regex declaration_end(R"(^}(?:\s*//.*)?)");
    const std::regex irrelavant_dot_syntax(R"((^\s*$|^\s*//.*|^\s*rankdir\s*=\s*LR;?|^\s*inic\s*\[shape\s*=\s*point\;?];?|^\s*node\s*\[shape\s*=\s*circle;?\];?))");

    // flags
    bool errors_existance = false;
    bool declaration_start_found = false;
    bool declaration_end_found = false;
    bool start_found = false;

    std::smatch matches;
    std::string line;
    int ln = 0;

    while (getline(file, line))
    {
        // std::cout << "states: (" << this->states.size() << ")" << std::endl;
        for (const auto &elem : this->states)
        {
            // std::cout << elem.first << " " << elem.second.get_id() << std::endl ;
        }

        ln++;

        // ignore empty lines, comments and other irrelevant dot syntax
        if (std::regex_match(line, irrelavant_dot_syntax))
            continue;

        // digraph {
        if (std::regex_match(line, declaration_start))
        {
            if (declaration_start_found)
            {
                std::cerr << "Invalid syntax in " << path << "\n"
                          << std::right << std::setw(6) << ln << " | " << line << "\n"
                          << "\n";
                errors_existance = true;
                continue;
            }
            declaration_start_found = true;
            continue;
        }

        // }
        if (std::regex_match(line, declaration_end))
        {
            if (declaration_end_found)
            {
                std::cerr << "Invalid syntax in " << path << "\n"
                          << std::right << std::setw(6) << ln << " | " << line << "\n"
                          << "\n";
                errors_existance = true;
                continue;
            }
            declaration_end_found = true;
            continue;
        }

        // inic -> q0;
        if (std::regex_match(line, matches, start_state))
        {
            if (start_found)
            {
                std::cerr << "Multiple start states found in " << path << "\n"
                          << std::right << std::setw(6) << ln << " | " << line << "\n"
                          << "\n";
                errors_existance = true;
            }
            // std::cout << "Start state found: " << matches[1] << "\n"; //dbg
            std::string state_id = matches[1];
            State *f = get_state(state_id).value_or(add_state(state_id, true, false));
            this->start = f;
            f->make_initial();
            start_found = true;
            // std::cout << "Start state: " << f->get_id() << "\n";
            continue;
        }

        // q0 -> q1 [label="a,bb,ccc"];
        if (std::regex_match(line, matches, transition))
        {
            // std::cout << "Transition found: " << matches[1] << " -> " << matches[2] << " [label=\"" << matches[3] << "\"]\n";
            std::string from_state = matches[1];
            std::string to_state = matches[2];
            auto symbols = all_matches(std::regex(R"(\w+|λ)"), matches[3]); // get transitions from "a,bb,ccc" to ["a", "bb", "ccc"]
            for (auto symbol : symbols)
                add_transition(from_state, symbol, to_state);
            continue;
        }

        // q1 [shape=doublecircle];
        if (std::regex_match(line, matches, final_state))
        {
            std::string state_id = matches[1];
            State *final = get_state(state_id).value_or(add_state(state_id, false, true));
            final->make_final();
            continue;
        }

        // if none of the above, then it is an syntax error
        std::cerr << "Invalid syntax in " << path << "\n"
                  << std::right << std::setw(6) << ln << " | " << line << "\n"
                  << "\n";
        errors_existance = true;
    }

    file.close();
    if (errors_existance)
        std::abort();
}

void AutomataRep::to_dot(std::string path)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + path);
    }

    file << "digraph {\n"
         << "    rankdir = LR;\n"
         << "    inic [shape = point];\n"
         << "    node [shape = circle];\n";
    for (auto &[id, state] : this->states)
    {
        if (state.is_initial())
            file << "    inic -> " << id << ";\n";
        if (state.is_final())
            file << "    " << id << " [shape = doublecircle];\n";
        for (auto &[symbol, next_state] : state.get_transitions())
        {
            file << "    " << id << " -> " << next_state->get_id() << " [label = \"" << symbol.get_symbol() << "\"];\n";
        }
    }
    file << "}" << std::endl;
    file.close();
}
