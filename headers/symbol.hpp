#pragma once
#include <string>

using namespace std;

template <typename T>
class Symbol
{
private:
    T symbol;
public:
    Symbol(T s) : symbol(s){};
    ~Symbol();
    T get_symbol();

    // Overload the == operator to make the class equatable
    bool operator==(const Symbol &other) const;
};

template <typename T>
struct std::hash<Symbol<T>>{
    std::size_t operator()(const Symbol<T> &s) const;
};


