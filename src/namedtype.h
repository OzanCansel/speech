#ifndef NAMEDTYPE_H
#define NAMEDTYPE_H

#include <iostream>

template <typename T, typename Parameter>
class NamedType
{
public:
    explicit NamedType(T const& value) : value_(value) {}
    explicit NamedType(T&& value = T{}) : value_(std::move(value)) {}
    T& get() { return value_; }
    T const& get() const {return value_; }
private:
    T value_;
};

#endif // NAMEDTYPE_H
