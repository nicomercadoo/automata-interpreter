#include <../headers/symbol.hpp>

template <typename T>
Symbol<T>::Symbol(T s) : symbol(s){};

template <typename T>
Symbol<T>::~Symbol(){};

template <>
Symbol<std::string>::~Symbol() {
    // Destructor logic for Symbol<std::string>, if needed
}

template <typename T>
T Symbol<T>::get_symbol() {
    return this->symbol;
}

template <typename T>
bool Symbol<T>::operator==(const Symbol<T> &other) const{
    return symbol == other.symbol;
}

template <typename T>
bool Symbol<T>::operator<(const Symbol<T> &other) const {
    return symbol < other.symbol;
}

template <>
Symbol<std::string>::Symbol(std::string s) : symbol(s) {

}

template <>
bool Symbol<std::string>::operator==(const Symbol &other) const {
  return this->symbol == static_cast<const Symbol<std::string>&>(other).symbol;
}

// template <typename T>
// struct std::hash<Symbol<T>>
// {
//     std::size_t operator()(const Symbol<T> &s) const
//     {
//         std::hash<T> hashFunc;
//         return hashFunc(s.get_symbol());
//     }
// };
