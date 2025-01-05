#pragma once
#include <string>
#include <iostream>
#include <iomanip>

#include "defines.h"

enum class VariableType : u32 {
    UNKNOWN = 0,
    I8 = 1,   // char
    U8 = 2,   // unsigned char
    I16 = 3,  // short
    U16 = 4,  // unsigned short
    I32 = 5,  // long
    U32 = 6,  // unsigned long
    I64 = 7,  // long long
    U64 = 8,  // unsigned long long
    F32 = 9,  // float
    F64 = 10, // double
    COUNT,
};

union VariableInitValue {
    i8 i8;
    u8 u8;
    i16 i16;
    u16 u16;
    i32 i32;
    u32 u32;
    i64 i64;
    u64 u64{ 0 };
    f32 f32;
    f64 f64;
};

struct Variable {
    VariableType type{ VariableType::UNKNOWN };
    VariableInitValue value;
};

std::string get_var_type_name(const VariableType  type) {
    switch (type) {
    case VariableType::I8:  return "char";
    case VariableType::U8:  return "unsigned char";
    case VariableType::I16: return "short";
    case VariableType::U16: return "unsigned short";
    case VariableType::I32: return "long";
    case VariableType::U32: return "unsigned long";
    case VariableType::I64: return "long long";
    case VariableType::U64: return "unsigned long long";
    case VariableType::F32: return "float";
    case VariableType::F64: return "double";
    case VariableType::UNKNOWN: return "";
    case VariableType::COUNT: return "";
    };
}

void print_var_types() {
    std::cout << "Available variables types:\n";
    for (u32 i = static_cast<u32>(VariableType::UNKNOWN) + 1; i != static_cast<u32>(VariableType::COUNT); ++i) {
        std::cout << "[" << std::setw(2) << i << "] " << get_var_type_name(static_cast<VariableType>(i)) << "\n";
    }
}