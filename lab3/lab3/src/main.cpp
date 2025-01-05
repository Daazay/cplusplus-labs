#include <iostream>
#include <vector>
#include <list>
#include <random>
#include <algorithm>
#include <numeric>
#include "number.h"

int main(int argc, char** argv) {
    std::mt19937 mt;

    u64 size = static_cast<u64>(mt()) % 501 + 500;
    std::vector<Number> v1;
    for (int i = 0; i < size; ++i) {
        v1.emplace_back(mt());
    }

    u64 b = std::max(static_cast<i64>(0), static_cast<i64>(size) - 200);
    u64 e = size;

    std::vector<Number> v2(v1.begin() + b, v1.begin() + e);

    u64 n = static_cast<u64>(mt()) % 31 + 20;

    std::vector<Number> sorted = v1;

    std::sort(sorted.begin(), sorted.end(), [](const Number& n1, const Number& n2) {
        return n1.get_i64() > n2.get_i64();
    });

    std::list<Number> list1(sorted.begin(), sorted.begin() + n);

    b = std::max(static_cast<i64>(0), static_cast<i64>(size) - static_cast<i64>(n));
    e = size;

    std::list<Number> list2(sorted.begin() + b, sorted.begin() + e);
    
    for (const auto& item : list1) {
        v1.erase(std::remove(v1.begin(), v1.end(), item), v1.end());
    }
    for (const auto& item : list2) {
        v2.erase(std::remove(v2.begin(), v2.end(), item), v2.end());
    }

    double mean_value = std::accumulate(list1.begin(), list1.end(), 0.0,
        [](double sum, const Number& elem) {
            return sum + elem.get_i64();
        }) / n;

    std::stable_partition(list1.begin(), list1.end(),
        [mean_value](const Number& elem) {
            return elem.get_i64() > mean_value;
        });

    list2.erase(std::remove_if(list2.begin(), list2.end(),
        [](const Number& elem) {
            return elem.get_i64() % 2 != 0;
        }), list2.end());

    std::vector<Number> v3;
    for (const auto& item : v1) {
        if (std::find(v2.begin(), v2.end(), item) != v2.end()) {
            v3.push_back(item);
        }
    }

    size_t min_size = std::min(list1.size(), list2.size());

    std::list<std::pair<Number, Number>> list3;

    auto it1 = list1.begin();
    auto it2 = list2.begin();

    for (u64 i = 0; i < min_size; ++i) {
        list3.emplace_back(*it1, *it2);
        ++it1;
        ++it2;
    }

    std::cout << "List 3 pairs:\n";
    for (const auto& pair : list3) {
        std::cout << "(" << pair.first.get_i64() << ", " << pair.second.get_i64() << ")\n";
    }

    return 0;
}