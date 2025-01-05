#pragma once

#include <string>
#include <iostream>

#include "defines.h"

class Number {
public:
    explicit Number(i64 value) : _value{ value }, _str{ std::to_string(value) } {
        std::cout << "ktor (i64) called." << std::endl;
    }
    Number(const Number& other) : _value{ other._value}, _str{ other._str } {
        std::cout << "ktor (const Number&) called." << std::endl;
    }
    Number(Number&& other) noexcept : _value{ other._value }, _str{ std::move(other._str) } {
        std::cout << "ktor (Number&&) called." << std::endl;
    }
    ~Number() {
        std::cout << "dtor called." << std::endl;
    }
public:
    Number& operator=(const Number& other) {
        std::cout << "oper (const Number&) called." << std::endl;
        if (this != &other) {
            _value = other._value;
            _str = other._str;
        }
        return *this;
    }
    Number& operator=(Number&& other) {
        std::cout << "oper (Number&&) called." << std::endl;
        if (this != &other) {
            _value = other._value;
            _str = std::move(other._str);
        }
        return *this;
    }
public:
    i64 get_i64() const { return _value; }
    std::string get_str() const { return _str; }
private:
    i64 _value{ 0 };
    std::string _str;
};