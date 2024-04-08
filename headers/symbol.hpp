#pragma once
#include <string>

using namespace std;

template <typename T>
class Symbol
{
private:
    T symbol;

public:
    Symbol(T s);
    ~Symbol();
    T get_symbol();

    // Overload the == operator to make the class equatable
    bool operator==(const Symbol &other) const;

    bool operator<(const Symbol &other) const;

    // Custom hash function for Symbol
    struct hash
    {
        size_t operator()(const Symbol<T> &s) const
        {
            return std::hash<T>()(s.symbol); // Use std::hash for the internal symbol
        }
    };
};

// template <typename T>
// struct std::hash<Symbol<T>>
// {
//     std::size_t operator()(const Symbol<T> &s) const;
// };
