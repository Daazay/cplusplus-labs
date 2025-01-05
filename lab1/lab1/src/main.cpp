#include <iostream>
#include <fstream>
#include <assert.h>

#include "operation.h"
#include "variable.h"

constexpr const char* ORDIONAL_NOUNS[] = { "first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth", "ninth", "tenth" };

bool write_file_with_f_function(const std::string& filepath, Variable* variables, const u32 var_count, Operation* operations, const u32 ops_count) {
    std::ofstream out{ filepath };
    if (!out.is_open()) {
        std::cout << "Error: Failed to open the file.\n";
        return false;
    }

    out << ""
        "#include <iostream>\n"
        "\n"
        "void f() {\n";

    for (u32 i = 0; i < var_count; ++i) {
        const auto& var = variables[i];
        out << "    " << get_var_type_name(var.type) << " var" << i + 1 << "{ ";

        switch (var.type) {
        case VariableType::I8:  { out << var.value.i8; } break;
        case VariableType::U8:  { out << var.value.u8; } break;
        case VariableType::I16: { out << var.value.i16; } break;
        case VariableType::U16: { out << var.value.u16; } break;
        case VariableType::I32: { out << var.value.i32; } break;
        case VariableType::U32: { out << var.value.u32; } break;
        case VariableType::I64: { out << var.value.i64; } break;
        case VariableType::U64: { out << var.value.u64; } break;
        case VariableType::F32: { out << var.value.f32; } break;
        case VariableType::F64: { out << var.value.f64; } break;
        default: {
            UNREACHABLE();
        } break;
        };
        out << " };\n";
    }
    out << "\n";

    for (u32 i = 0; i < ops_count; ++i) {
        const auto& op = operations[i];

        out << "    std::cout << \"var" << op.operands[0] << "(\" << var" << op.operands[0] << " << \") " << get_op_type_value(op.type) << " var" << op.operands[1] << "(\" << var" << op.operands[1] << " << \") = \" << var" << op.operands[0] << " " << get_op_type_value(op.type) << " var" << op.operands[1] << " << \"\\n\";\n";
    }

    out << "}" << std::endl;

    return true;
}

int main(int argc, char** argv) {
    u32 var_count = 0;
    std::cout << "Enter variable count [2 or 3]:\n";

    while (var_count == 0) {
        std::cin >> var_count;

        if (var_count < 2 || 3 < var_count) {
            var_count = 0;
            std::cout << "Error: invalid var count. Try again..\n";
            continue;
        }
    }

    Variable vars[3]{ {VariableType::UNKNOWN, 0 } };

    u32 var_num = 0;
    while (var_num < var_count) {
        print_var_types();
        std::cout << "\nEnter " << ORDIONAL_NOUNS[var_num] << " type and default value:\n";
        u32 input{ 0 };
        std::cin >> input;

        if (input <= static_cast<u32>(VariableType::UNKNOWN) || static_cast<u32>(VariableType::COUNT) <= input) {
            std::cout << "Error: invalid type for variable. Try again..\n";
            continue;
        }

        vars[var_num].type = static_cast<VariableType>(input);

        switch (vars[var_num].type) {
        case VariableType::I8: { std::cin >> vars[var_num].value.i8; } break;
        case VariableType::U8: { std::cin >> vars[var_num].value.u8; } break;
        case VariableType::I16: { std::cin >> vars[var_num].value.i16; } break;
        case VariableType::U16: { std::cin >> vars[var_num].value.u16; } break;
        case VariableType::I32: { std::cin >> vars[var_num].value.i32; } break;
        case VariableType::U32: { std::cin >> vars[var_num].value.u32; } break;
        case VariableType::I64: { std::cin >> vars[var_num].value.i64; } break;
        case VariableType::U64: { std::cin >> vars[var_num].value.u64; } break;
        case VariableType::F32: { std::cin >> vars[var_num].value.f32; } break;
        case VariableType::F64: { std::cin >> vars[var_num].value.f64; } break;
        };

        ++var_num;
    }

    u32 ops_count = 0;
    std::cout << "Enter operations count [1..10]:\n";
    std::cin >> ops_count;

    u32 op_num = 0;
    Operation operations[10]{ 0 };

    while (op_num < ops_count) {
        std::cout << "\nEnter " << ORDIONAL_NOUNS[op_num] << " operation [(1st operand) operation (2nd operand)]:\n";
        u32 input{ 0 };

        std::cin >> input;
        if (input < 1 && var_count < input) {
            std::cout << "Error: ivalid 1st operand. Try again..\n";
            continue;
        }
        operations[op_num].operands[0] = input;

        print_operations();
        std::cin >> input;
        if (input <= static_cast<u32>(OperationType::UNKNOWN) || static_cast<u32>(OperationType::COUNT) <= input) {
            std::cout << "Error: invalid operation. Try again..\n";
            continue;
        }
        operations[op_num].type = static_cast<OperationType>(input);

        std::cin >> input;
        if (input < 1 || var_count < input) {
            std::cout << "Error: ivalid 2nd operand. Try again..\n";
            continue;
        }
        operations[op_num].operands[1] = input;

        op_num++;
    }

    write_file_with_f_function("D:/study/univer/ITMO/cplusplus/lab1/lab1_test/src/f.cpp", vars, var_count, operations, ops_count);

    return 0;
}

/*
3 
2 4
6 12
9 24.5
3
1 3 2
2 3 3
1 1 1

*/