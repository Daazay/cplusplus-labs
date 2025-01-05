#include <iostream>
#include <vector>
#include <list>

#include "number.h"

void print(const Number& number) {
    std::cout << "[" << number.get_i64() << ":\"" << number.get_str() << "\"]\n";
}

template <typename T>
concept iterable = requires (T list) {
    list.begin();
    list.end();
};

template<typename T>
requires iterable<T>
void print(const T& numbers) {
    for (const auto& num : numbers) {
        print(num);
    }
    std::cout << std::endl;
}

int main() {
    Number num1(10);
    print(num1);

    Number* num2 = new Number(20);
    print(*num2);

    std::vector<Number> numbers;
    numbers.push_back(num1);
    numbers.push_back(*num2);

    print(numbers);

    delete num2;

    std::list<Number> list;
    list.emplace_back(30);
    list.emplace_back(40);

    print(list);

    return 0;
}