#pragma once
#include <string>

template <typename T>
class Symbol
{
private:
    T symbol;

public:
    Symbol(T s);
    ~Symbol();
    T get_symbol() const;

    // Overload the == operator to make the class equatable
    bool operator==(const Symbol &other) const;

    // Overload the < operator to make the class comparable
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
