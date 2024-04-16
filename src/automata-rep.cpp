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
#include <algorithm>
#include <map>
#include <string>

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
    this->alphabet.insert(s);
    f->add_transition(s, t);
}

void AutomataRep::set_alphabet(std::set<Symbol<std::string>> alphabet)
{
    this->alphabet = alphabet;
    gen_aplhabet_valid_symbols_regex();
}

void AutomataRep::gen_aplhabet_valid_symbols_regex()
{
    std::string alphabet_reg;
    for (auto &symbol : this->alphabet)
        alphabet_reg |= symbol.get_symbol();

    this->valid_symbols = std::regex(alphabet_reg);
}

std::vector<std::string> AutomataRep::get_symbols_in_str(std::string input)
{
    std::vector<std::string> symbols;
    std::string acc;
    for (auto &s : input)
    {
        if (std::regex_match(std::string(1, s), this->valid_symbols))
        {
            if (!acc.empty())
                symbols.push_back(acc);
            symbols.push_back(std::string(1, s));
            continue;
        }
        else
        {
            acc.push_back(s);
            if (std::regex_match(acc, this->valid_symbols))
            {
                symbols.push_back(acc);
                acc.clear();
            }
        }
    }

    return symbols;
}

bool AutomataRep::accept(std::string input)
{
    // auto str_symbols = all_matches(std::regex(this->valid_symbols |= "(?:.*(?!" + wrap_into_group(this->valid_symbols) + ").*)*" ), input);
    auto str_symbols = get_symbols_in_str(input);
    std::cout << "Symbols: (" << str_symbols.size() << ")" << std::endl;
    std::for_each(str_symbols.begin(), str_symbols.end(), [](std::string &s)
                  { std::cout << s << std::endl; });
    // std::cout << std::endl;
    std::vector<Symbol<std::string>> input_symbols;
    for (auto &symbol : str_symbols)
        input_symbols.push_back(Symbol<std::string>(symbol));

    State *current_state = this->start;
    std::cout << "Start state: " << current_state->get_id() << std::endl;
    std::cout << "transitions: (" << current_state->get_transitions().size() << ")" << std::endl;

    current_state = &this->states.find(current_state->get_id())->second;
    std::cout << "Start state: " << current_state->get_id() << std::endl;
    std::cout << "transitions: (" << current_state->get_transitions().size() << ")" << std::endl;

    return this->accept(input_symbols, current_state);
}

// recursive (non deterministic compatible) automata accept function
bool AutomataRep::accept(std::vector<Symbol<std::string>> input, State *current_state)
{
    // State *current_state = this->start;
    std::cout << "state: " << current_state->get_id() << std::endl;
    std::cout << "transitions: (" << current_state->get_transitions().size() << ")" << std::endl;

    Symbol<std::string> current_symbol = input.at(0);
    auto transitions = current_state->get_transitions_by(current_symbol); // including labmbda transitions
    // auto lambda_transitions = current_state->get_transitions_by(Symbol<std::string>("λ"));
    if (!transitions.has_value())
        return false;

    std::vector<State*> all_transitions;
    all_transitions.insert(all_transitions.end(), transitions.value().begin(), transitions.value().end());
    bool accepted = false;
    for (auto it = all_transitions.begin(); it != all_transitions.end(); ++it)
    {
        auto& next_state = *it;
        if (input.size() == 1)
        {
            if (next_state->is_final())
                accepted = true;
        }
        else
        {
            std::vector<Symbol<std::string>> next_input(input.begin() + 1, input.end());
            accepted = accept(next_input, next_state);
        }
    }
    return accepted;
}

// returns a deterministic automata
AutomataRep AutomataRep::make_deterministic(){
    std::map<std::set<State*>,bool> T;
    std::map<std::set<State*>,std::string> N;

    int counter = 0;
    std::string name = "q";

    auto Q0 = this->lambda_closure_from_start();

    T.insert({Q0, false}); // insert Q0 in T not marked
    N.insert({Q0, name + std::to_string(counter++)});

    AutomataRep D = AutomataRep();

    D.add_state(N[Q0], true, false);

    auto det_alphabet = this->alphabet;
    det_alphabet.erase(Symbol<std::string>("λ"));
    D.set_alphabet(det_alphabet);

    D.name = this->name + "_D";


    std::cout << "Alphabet: (" << det_alphabet.size() << ")" << std::endl;
    for (auto &s : det_alphabet)
        std::cout << s.get_symbol() << std::endl;

    for(auto [S,m] : T){
        if (m) continue;
        std::cout << "State: " << N[S] << std::endl;
        for (auto &s : S)
        {
            std::cout << s->get_id() << std::endl;
        }

        T.insert({S, true}); // mark S
        for (auto a : det_alphabet){
            auto M = lambda_closure(move(S, a));
            if (T.find(M) == T.end()){ // M is not in T
                T.insert({M, false});  // insert M in T not marked
                N.insert({M, name + std::to_string(counter++)});
            }
            D.add_transition(N[S], a.get_symbol(), N[M]);
        }
    }

    // set final states
    for (auto [Q, n] : N){
        std::cout << "State: " << n << std::endl;
        for (auto &m : Q)
        {
            std::cout << m->get_id() << std::endl;
        }

        for (auto s : Q){
            if (s->is_final()){
                D.get_state(n).value()->make_final();
                break;
            }
        }
    }

    D.start = D.get_state(N[Q0]).value();

    return D;
}


std::set<State *> AutomataRep::move_from_start(std::string symbol)
{
    return move({this->start}, Symbol<std::string>(symbol));
}


std::set<State *> AutomataRep::move(std::set<State *> states, Symbol<std::string> symbol)
{
    std::cout << "move" << std::endl;
    std::set<State *> reachable_states;
    for (auto state : states)
    {
        auto transitions = state->get_transitions_by(symbol);
        if (transitions.has_value())
            for (auto next : transitions.value())
                reachable_states.insert(next);
    }
    return reachable_states;
}

std::set<State *> AutomataRep::lambda_closure_from_start()
{
    return lambda_closure({this->start});
}

std::set<State*> AutomataRep::lambda_closure(std::set<State *> states)
{
    std::cout << "lambda_closure" << std::endl;
    std::set<State *> closure;
    std::set<State *> visited;
    std::vector<State *> stack;
    for (auto state : states)
        stack.push_back(state);

    while (!stack.empty())
    {
        State *current = stack.back();
        stack.pop_back();
        if (visited.find(current) != visited.end())
            continue;
        visited.insert(current);
        closure.insert(current);
        auto transitions = current->get_transitions_by(Symbol<std::string>("λ"));
        if (transitions.has_value())
            for (auto &next : transitions.value())
                stack.push_back(next);
    }

    return closure;
}

void AutomataRep::from_dot(std::string path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path);

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
    gen_aplhabet_valid_symbols_regex();
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

    std::cout << "to_dot: start info:" << std::endl;
    std::cout << "start: " << this->start->get_id() << std::endl;
    std::cout << "transitions: (" << this->start->get_transitions().size() << ")" << std::endl;

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
