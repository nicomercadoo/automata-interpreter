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
#include <numeric>

AutomataRep::AutomataRep() : states()
{
    this->start = "";
}

AutomataRep::~AutomataRep(){};

std::optional<State> AutomataRep::get_state(std::string state_id)
{
    // std::cout << "Finding state: " << state_id << std::endl;
    auto it = this->states.find(state_id);
    if (it != this->states.end())
        return it->second;
    return {};
}

State AutomataRep::add_state(State state)
{
    // std::cout << "Insert or Assing state (State): " << state.get_id() << "\n";
    this->states.insert_or_assign(state.get_id(), state).first->second;
    return state;
    // return &this->states[state.get_id()]; // ????
}

State AutomataRep::add_state(StateID id)
{
    // std::cout << "Adding state (string): " << id << "\n";
    return add_state(State(id, false, false));
}

State AutomataRep::add_state(std::string id, bool initial, bool final)
{
    // std::cout << "Adding state (string, bool, bool): " << id << "\n";
    return add_state(State(id, initial, final));
}

void AutomataRep::add_transition(StateID from_state, std::string symbol, StateID to_state)
{
    // std::cout << "Adding transition: " << from_state << " -> " << to_state << " [label=\"" << symbol << "\"]\n";

    auto tmp = this->states.find(from_state);
    if (tmp == this->states.end())
    {
        // std::cout << "State: " << from_state << " not found in automata: " << this->name << std::endl;
        // std::cout << "Adding state: " << from_state << std::endl;
        this->add_state(from_state);
    }
    State f = this->states.find(from_state)->second;

    tmp = this->states.find(to_state);
    if (tmp == this->states.end())
    {
        // std::cout << "State: " << to_state << " not found in automata: " << this->name << std::endl;
        // std::cout << "Adding state: " << to_state << std::endl;
        this->add_state(to_state);
    }

    State t = this->states.find(to_state)->second;

    // State f = get_state(from_state).value_or(add_state(from_state)); // if from_state does not exist, create it
    // State t = get_state(to_state).value_or(add_state(to_state));     // if to_state does not exist, create it
    // bool f_is_in_states = this->states.find(from_state) != this->states.end();
    // bool t_is_in_states = this->states.find(to_state) != this->states.end();
    // std::cout << "From state: " << f.get_id() << " is into states? = " << std::boolalpha << f_is_in_states << "\n";
    // std::cout << "To state: " << t.get_id() << " is into states? = " << std::boolalpha << f_is_in_states << "\n";
    auto s = Symbol<std::string>(symbol);
    this->alphabet.insert(s);
    f.add_transition(s, t.get_id());
    this->add_state(f); // update f
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
    // std::cout << "Automata: " << this->name << std::endl;
    // for (auto &[n, q] : this->states)
    // {
    //     std::cout << "state: " << q.get_id() << std::endl;
    //     std::cout << "transitions: (" << q.get_transitions().size() << ")" << std::endl;
    //     for (auto &[s, nq] : q.get_transitions())
    //     {
    //         std::cout << s.get_symbol() << " -> " << nq << std::endl;
    //     }
    // }
    // std::cout << "End Automata." << std::endl;

    auto str_symbols = get_symbols_in_str(input);

    // std::cout << "Symbols: (" << str_symbols.size() << ")" << std::endl;
    // std::for_each(str_symbols.begin(), str_symbols.end(), [](std::string &s)
    //               { std::cout << s << std::endl; });

    std::vector<Symbol<std::string>> input_symbols;
    for (auto &symbol : str_symbols)
        input_symbols.push_back(Symbol<std::string>(symbol));

    // std::cout << "Start state: " << current_state<< std::endl;
    // std::cout << "transitions: (" << current_state->get_transitions().size() << ")" << std::endl;

    // State current_state = this->states.find(this->start)->second;
    // std::cout << "Start state: " << current_state.get_id() << std::endl;
    // std::cout << "transitions: (" << current_state.get_transitions().size() << ")" << std::endl;

    // std::cout << "Accepting input: " << input << std::endl;
    return this->accept(input_symbols, this->start);
}

// recursive (non deterministic compatible) automata accept function
bool AutomataRep::accept(std::vector<Symbol<std::string>> input, StateID current)
{
    // StateID current_state = this->start;
    State current_state = this->states.find(current)->second;
    // std::cout << "in " << current_state.get_id() << std::endl;
    // std::cout << "symbols: " << input.size() << std::endl;
    // for (auto &s : input)
    //     std::cout << s.get_symbol() << " ";
    // std::cout << std::endl;
    // std::cout << "state: " << current_state.get_id() << std::endl;
    // std::cout << "transitions: (" << current_state.get_transitions().size() << ")" << std::endl;
    // for (auto &[s, q] : current_state.get_transitions())
    // {
    //     std::cout << s.get_symbol() << " -> " << q << std::endl;
    // }

    bool accepted = false;

    if (input.size() <= 0 && current_state.is_final())
    {
        // std::cout << "\033[33minput is empty and current state is final: true\033[0m" << std::endl;
        return true;
    }

    if (input.size() <= 0 && !current_state.is_final())
    {
        // std::cout << "\033[33minput is empty and current state is not final: false\033[0m" << std::endl;
        return false;
    }

    for (auto &[s, q] : current_state.get_transitions())
    {
        if (accepted)
            break;

        if (s.get_symbol() == "λ")
        {
            // std::cout << "lambda transition: " << s.get_symbol() << " -> " << q << std::endl;
            accepted = accept(input, q);
        }

        Symbol<std::string> current_symbol = input.at(0);
        if (s.get_symbol() == current_symbol.get_symbol())
        {
            // std::cout << "symbol transition: " << s.get_symbol() << " -> " << q << std::endl;

            std::vector<Symbol<std::string>> next_input(input.begin() + 1, input.end());
            // std::cout << "consuming: " << current_symbol.get_symbol() << " and moving to: " << q << std::endl;
            accepted = accept(next_input, q); // recursive call (consume symbol and move to next state)
        }
    }

    return accepted;
}

// returns a deterministic automata
AutomataRep AutomataRep::make_deterministic()
{
    // std::cout << "Deterministic Automata: " << this->name << std::endl;
    std::map<std::set<State>, bool> T;
    std::map<std::set<State>, StateID> N;

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


    // std::cout << "Alphabet: (" << det_alphabet.size() << ")" << std::endl;
    // for (auto &s : det_alphabet)
    //     std::cout << s.get_symbol() << std::endl;

    for (auto [S, m] : T)
    {
        if (m)
            continue;
        // std::cout << "State: " << N[S] << std::endl;
        // for (auto &s : S)
        // {
        //     std::cout << s.get_id() << std::endl;
        // }

        T.insert({S, true}); // mark S
        for (auto a : det_alphabet)
        {
            auto M = lambda_closure(move(S, a));
            if (T.find(M) == T.end())
            {                         // M is not in T
                T.insert({M, false}); // insert M in T not marked
                N.insert({M, name + std::to_string(counter++)});
            }
            D.add_transition(N[S], a.get_symbol(), N[M]);
        }
    }

    // set final states
    for (auto [Q, n] : N)
    {
        // std::cout << "State: " << n << std::endl;
        // for (auto &m : Q)
        // {
        //     std::cout << m.get_id() << std::endl;
        // }

        for (auto s : Q)
        {
            if (s.is_final())
            {
                State final = D.get_state(n).value();
                final.make_final();
                D.states.insert_or_assign(final.get_id(), final); // update final
                break;
            }
        }
    }

    D.start = D.get_state(N[Q0]).value().get_id();

    return D;
}

std::set<State> AutomataRep::move_from_start(std::string symbol)
{
    State start_state = this->states.find(this->start)->second;
    return move({start_state}, Symbol<std::string>(symbol));
}

std::set<State> AutomataRep::move(std::set<State> states, Symbol<std::string> symbol)
{
    // std::cout << "move" << std::endl;
    std::set<State> reachable_states;
    for (auto state : states)
    {
        auto transitions = state.get_transitions_by(symbol);
        if (transitions.has_value())
            for (auto next : transitions.value())
            {
                State next_state = this->states.find(next)->second;
                reachable_states.insert(next_state);
            }
    }
    return reachable_states;
}

std::set<State> AutomataRep::lambda_closure_from_start()
{
    State start_state = this->states.find(this->start)->second;
    return lambda_closure({start_state});
}

std::set<State> AutomataRep::lambda_closure(std::set<State> states)
{
    // std::cout << "lambda_closure" << std::endl;
    std::set<State> closure;
    std::set<State> visited;
    std::vector<State> stack;
    for (auto state : states)
        stack.push_back(state);

    while (!stack.empty())
    {
        State current_state = stack.back();
        stack.pop_back();
        if (visited.find(current_state) != visited.end())
            continue;
        visited.insert(current_state);
        closure.insert(current_state);
        auto transitions = current_state.get_transitions_by(Symbol<std::string>("λ"));
        if (transitions.has_value())
            for (auto &next : transitions.value())
            {
                State next_state = this->states.find(next)->second;
                stack.push_back(next_state);
            }
    }

    return closure;
}

void AutomataRep::from_dot(std::string path)
{
    // std::cout << "from_dot: " << path << std::endl;
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path);

    // regexes
    using namespace dot_syntax;

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
        // if (this->states.size() > 0)
        // {
        //     std::cout << "\033[31mAutomata: \033[0m" << std::endl;
        //     for (auto &[n, q] : this->states)
        //     {
        //         std::cout << "state: " << q.get_id() << std::endl;
        //         std::cout << "transitions: (" << q.get_transitions().size() << ")" << std::endl;
        //         for (auto &[s, nq] : q.get_transitions())
        //         {
        //             std::cout << s.get_symbol() << " -> " << nq << std::endl;
        //         }
        //     }
        //     std::cout << "End Automata." << std::endl;
        // }

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
            State f = get_state(state_id).value_or(add_state(state_id, true, false));
            this->start = f.get_id();
            f.make_initial();
            this->states.insert_or_assign(f.get_id(), f); // update f
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
            State final = get_state(state_id).value_or(add_state(state_id, false, true));
            final.make_final();
            this->states.insert_or_assign(final.get_id(), final); // update final
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
         << "    edge [arrowsize = 0.8, penwidth = 0.8];\n"
         << "    node [shape = circle];\n\n";

    // std::cout << "to_dot: start info:" << std::endl;
    // std::cout << "start: " << this->start << std::endl;
    // State start_state = this->states.find(this->start)->second;
    // std::cout << "transitions: (" << start_state.get_transitions().size() << ")" << std::endl;

    std::string init = "    // initial state\n";
    std::string transitions = "    // transitions\n";
    std::string finals = "    // final states\n";

    for (auto &[id, state] : this->states)
    {
        if (state.is_initial())
            init += "    inic -> " + id + ";\n";
        if (state.is_final())
            finals += "    " + id + " [shape = doublecircle];\n";

        std::map<StateID, std::vector<std::string>> transitions_for_each_next_state;
        for (const auto &[symbol, next_state] : state.get_transitions())
            transitions_for_each_next_state[next_state].push_back(symbol.get_symbol());

        for (const auto &[next_state, symbols] : transitions_for_each_next_state)
        {
            std::string symbols_str;
            for (auto it = symbols.rbegin(); it != symbols.rend(); ++it) {
                if (!symbols_str.empty()) {
                    symbols_str += ", ";
                }
                symbols_str += *it;
            }
            transitions += "    " + id + " -> " + next_state + " [label = \"" + symbols_str + "\"];\n";
        }
    }

    file << init << "\n" << transitions << "\n" << finals << std::endl;

    file << "}" << std::endl;
    file.close();
}
