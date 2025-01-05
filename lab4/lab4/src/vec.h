#pragma once

#include <unordered_map>
#include <initializer_list>
#include <vector>
#include <cmath>
#include <iostream>
#include <cassert>

#include "defines.h"
#include "mat.h"

class Vec {
    friend class Mat;
public:
    Vec(u64 size);
    Vec(const std::vector<f64>& v);
    Vec(const std::initializer_list<f64>& list);
public:
    u64 get_size() const { return _size; }
public:
    auto begin() { return _data.begin(); }
    auto end() { return _data.end(); }
    auto begin() const { return _data.cbegin(); }
    auto end() const { return _data.cend(); }
public:
    friend std::ostream& operator<<(std::ostream& out, const Vec& vec);
    friend Vec operator+(const Vec& v, f64 value);
    friend Vec operator-(const Vec& v, f64 value);
    friend Vec operator*(const Vec& v, f64 value);
    friend Vec operator/(const Vec& v, f64 value);

    friend Vec operator+(const Vec& v1, const Vec& v2);
    friend Vec operator-(const Vec& v1, const Vec& v2);
    friend f64 operator*(const Vec& v1, const Vec& v2);

    friend Vec operator^(const Vec& vec, const f64 exp);

    friend Vec operator*(const Vec& v, const Mat& m);
private:
    std::unordered_map<u64, f64> _data;
    u64 _size{ 0 };
};

Vec::Vec(u64 size) : _size{ size } {}

Vec::Vec(const std::initializer_list<f64>& list)
    : _size{ list.size() } {
    u64 idx = 0;
    for (const auto& it : list) {
        if (std::abs(it) < EPSILON) {
            _data.emplace(idx, it);
        }
        ++idx;
    }
}

Vec::Vec(const std::vector<f64>& v)
    : _size{ v.size() } {
    for (u64 i = 0; i < v.size(); ++i) {
        if (std::abs(v[i]) < EPSILON) {
            _data.emplace(i, v[i]);
        }
    }
}

std::ostream& operator<<(std::ostream& out, const Vec& v) {
    for (u64 i = 0; i < v._size; ++i) {
        auto it = v._data.find(i);
        if (it != v._data.end()) {
            out << it->second;
        }
        else {
            out << 0.0;
        }
        if (i + 1 < v._size) {
            out << " ";
        }
    }
    return out;
}

Vec operator+(const Vec& v, f64 value) {
    if (std::abs(value) < EPSILON) {
        return Vec(v._size);
    }

    Vec res(v._size);

    for (u64 i = 0; i < v._size; ++i) {
        auto it = v._data.find(i);

        if (it == v._data.end()) {
            res._data.emplace(i, value);
            continue;
        } 
        f64 sum = it->second + value;
        if (std::abs(sum) >= EPSILON) {
            res._data[i] = sum;
        }
    }

    return res;
}

Vec operator-(const Vec& v, f64 value) { 
    if (std::abs(value) < EPSILON) {
        return Vec(v._size);
    }

    Vec res(v._size);

    for (u64 i = 0; i < v._size; ++i) {
        auto it = v._data.find(i);

        if (it == v._data.end()) {
            res._data.emplace(i, -value);
            continue;
        }
        f64 sum = it->second - value;
        if (std::abs(sum) >= EPSILON) {
            res._data[i] = sum;
        }
    }

    return res;
}

Vec operator*(const Vec& v, f64 value) { 
    if (std::abs(value) < EPSILON) {
        return Vec(v._size);
    }

    Vec res(v._size);

    for (const auto& [idx, val] : v._data) {
        f64 mul = val * value;
        if (std::abs(mul) > EPSILON) {
            res._data[idx] = mul;
        }
    }

    return res;
}

Vec operator/(const Vec& v, f64 value) {
    assert((value < EPSILON) && "Division by zero.");

    Vec res(v._size);

    for (const auto& [idx, val] : v._data) {
        f64 div = val / value;
        if (std::abs(div) > EPSILON) {
            res._data[idx] = div;
        }
    }

    return res;
}

Vec operator+(const Vec& v1, const Vec& v2) {
    assert(v1._size == v2._size && "Vectrors must be the same size.");

    Vec res(v1._size);

    for (const auto& [idx, value] : v1) {
        res._data.emplace(idx, value);
    }

    for (const auto& [idx, value] : v2) {
        auto it = res._data.find(idx);
        if (it != res._data.end()) {
            f64 sum = it->second + value;
            if (std::abs(sum) >= EPSILON) {
                res._data[idx] = sum;
            }
            else {
                res._data.erase(idx);
            }
        }
        else {
            res._data.emplace(idx, value);
        }
    }

    return res;
}

Vec operator-(const Vec& v1, const Vec& v2) {
    assert(v1._size == v2._size && "Vectrors must be the same size.");

    Vec res(v1._size);

    for (const auto& [idx, value] : v1) {
        res._data.emplace(idx, value);
    }

    for (const auto& [idx, value] : v2) {
        auto it = res._data.find(idx);
        if (it != res._data.end()) {
            f64 sub = it->second - value;
            if (std::abs(sub) >= EPSILON) {
                res._data[idx] = sub;
            }
            else {
                res._data.erase(idx);
            }
        }
        else {
            res._data.emplace(idx, -value);
        }
    }

    return res;
}

f64 operator*(const Vec& v1, const Vec& v2) {
    assert(v1._size == v2._size && "Vectrors must be the same size.");

    f64 res = 0.0;
    for (const auto& [idx, value] : v1) {
        auto it = v2._data.find(idx);
        if (it != v2._data.end()) {
            res += it->second * value;
        }
    }

    return res;
}

Vec operator^(const Vec& vec, const f64 exp) {
    Vec res(vec._size);

    for (const auto& [idx, value]: vec) {
        f64 v = std::pow(value, exp);
        if (std::abs(v) >= EPSILON) {
            res._data[idx] = v;
        }
    }

    return res;
}

Vec operator*(const Vec& v, const Mat& m) {
    assert((v._size == m.get_rows()) && "The matrix row count must be equal to vec columns count.");

    Vec res(m.get_cols());

    for (const auto& [v_idx, v_value] : v) {
        for (const auto& [m_idx, m_value] : m) {
            if (v_idx == m_idx.first) {
                res._data[m_idx.second] += v_value * m_value;
            }
        }
    }

    for (const auto& [idx, value] : res) {
        if (std::abs(value) < EPSILON) {
            res._data.erase(idx);
        }
    }
    
    return res;
}