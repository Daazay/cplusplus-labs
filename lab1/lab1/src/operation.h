#pragma once
#include <string>
#include <iostream>
#include <iomanip>

#include "defines.h"

enum class OperationType : u32 {
    UNKNOWN = 0,
    SUM = 1, // '+'
    SUB = 2, // '-'
    MUL = 3, // '*'
    DIV = 4, // '/'
    REM = 5, // '%'
    COUNT,
};

std::string get_op_type_value(const OperationType op) {
    switch (op) {
    case OperationType::SUM:  return "+";
    case OperationType::SUB:  return "-";
    case OperationType::MUL:  return "*";
    case OperationType::DIV:  return "/";
    case OperationType::REM:  return "%";
    case OperationType::UNKNOWN: return "";
    case OperationType::COUNT: return "";
    };
}

struct Operation {
    u32 operands[2]{ 0 };
    OperationType type;
};

void print_operations() {
    std::cout << "Available operations:\n";
    for (u32 i = static_cast<u32>(OperationType::UNKNOWN) + 1; i != static_cast<u32>(OperationType::COUNT); ++i) {
        std::cout << "[" << std::setw(2) << i << "] '" << get_op_type_value(static_cast<OperationType>(i)) << "'\n";
    }
}