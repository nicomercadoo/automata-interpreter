#include "../headers/symbol.hpp"

template <typename T>
Symbol<T>::Symbol(T s) : symbol(s){};

template <typename T>
Symbol<T>::~Symbol(){};

template <>
Symbol<std::string>::~Symbol() {}

template <typename T>
T Symbol<T>::get_symbol() const
{
  return this->symbol;
}

template <>
std::string Symbol<std::string>::get_symbol() const
{
  return this->symbol;
}

template <typename T>
bool Symbol<T>::operator==(const Symbol<T> &other) const
{
  return symbol == other.symbol;
}

template <typename T>
bool Symbol<T>::operator!=(const Symbol<T> &other) const
{
  return symbol != other.symbol;
}

template <typename T>
bool Symbol<T>::operator<(const Symbol<T> &other) const
{
  return symbol < other.symbol;
}

template <>
Symbol<std::string>::Symbol(std::string s) : symbol(s)
{
}

template <>
bool Symbol<std::string>::operator==(const Symbol &other) const
{
  return this->symbol == static_cast<const Symbol<std::string> &>(other).symbol;
}

template <>
bool Symbol<std::string>::operator!=(const Symbol &other) const
{
  return this->symbol != static_cast<const Symbol<std::string> &>(other).symbol;
}

template <>
bool Symbol<std::string>::operator<(const Symbol &other) const
{
  return this->symbol < static_cast<const Symbol<std::string> &>(other).symbol;
}
