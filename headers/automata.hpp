#pragma once
#include "state.hpp"
#include "symbol.hpp"
#include "automata-rep.hpp"

// using namespace std;
enum Nature
{
    AFD,
    AFND
};

// no importa si el grafo es o no deterministico, la representacion es la misma
// solo cambia la forma en que se construye el grafo
// la diferencia se resuelve con la funcion delta

// se podria evaluar que el automata guarde un puntero a la representacion en lugar de tenerla como atributo
// esto permitiria cambiar la representacion del automata en tiempo de ejecucion

// Weighted Digraph based representation
class Automata
{
private:
    AutomataRep rep;
    Nature nature;

public:
    Automata(Nature nature, std::string name);
    ~Automata();
    std::string rep_name();
    bool deterministic_inv(); // determina si el automata es deterministico y esta correctamente implementado (ningun nodo tiene mas de una transicion con el mismo simbolo y no teine transiciones lambda)
    void from_dot(std::string path);
    void to_dot(std::string path);
    void add_transition(std::string from_state, std::string symbol, std::string to_state);
    void add_state(std::string id, bool initial, bool final);
    Nature get_nature();
    bool accept(std::string input);
    void make_deterministic();
    // auto delta(State *state, Symbol<std::string> symbol);
};

