#pragma once
#include <iostream>
#include <unordered_map>
#include <initializer_list>
#include <utility>
#include <vector>
#include <cmath>
#include <cassert>
#include <iomanip>

#include "defines.h"
#include "vec.h"

template<>
struct std::hash<std::pair<u64, u64>> {
    u64 operator()(const std::pair<u64, u64>& values) const noexcept {
        return (std::hash<u64>()(values.first)
            ^ std::hash<u64>()(values.second));
    }
};
class Vec;

class Mat {
    friend class Vec;
public:
    Mat(u64 rows, u64 cols);
    Mat(const std::initializer_list<std::initializer_list<f64>>& mat);
    Mat(const std::vector<std::vector<f64>>& mat);
public:
    u64 get_rows() const { return _rows; }
    u64 get_cols() const { return _cols; }
public:
    auto begin() { return _data.begin(); }
    auto end() { return _data.end(); }
    auto begin() const { return _data.begin(); }
    auto end() const { return _data.end(); }
public:
    Mat transpose() const;
    Mat inverse() const;
    Mat power(u64 power) const;
public:
    friend std::ostream& operator<<(std::ostream& out, const Mat& mat);
    friend Mat operator+(const Mat& m, f64 value);
    friend Mat operator-(const Mat& m, f64 value);
    friend Mat operator*(const Mat& m, f64 value);
    friend Mat operator/(const Mat& m, f64 value);

    friend Mat operator+(const Mat& m1, const Mat& m2);
    friend Mat operator-(const Mat& m1, const Mat& m2);
    friend Mat operator*(const Mat& m1, const Mat& m2);

    friend Mat operator^(const Mat& m, u32 exp);
private:
    u64 _rows{ 0 };
    u64 _cols{ 0 };
    std::unordered_map<std::pair<u64, u64>, f64> _data;
};

Mat::Mat(u64 rows, u64 cols) 
    : _rows{ rows }, _cols{ cols } {}

Mat::Mat(const std::initializer_list<std::initializer_list<f64>>& mat) {
    _rows = mat.size();
    _cols = mat.begin()->size();

    u64 row = 0;
    for (const auto& r : mat) {
        assert((r.size() == _cols) && "Invalid column size.");
        u64 col = 0;
        for (const auto& v : r) {
            if (std::abs(v) > EPSILON) {
                _data.emplace(std::pair<u64, u64>{ row, col }, v);
            }
            ++col;
        }
        ++row;
    }
}

Mat::Mat(const std::vector<std::vector<f64>>& mat) {
    u64 row = 0;
    for (const auto& r : mat) {
        u64 col = 0;
        for (const auto& v : r) {
            if (std::abs(v) > EPSILON) {
                _data.emplace(std::pair<u64, u64>{ row, col }, v);
            }
            ++col;
        }
        ++row;
    }
}

Mat Mat::transpose() const {
    Mat res(_cols, _rows);

    for (const auto& [idx, value] : _data) {
        res._data.emplace(std::pair<u64, u64>{ idx.second, idx.first }, value);
    }

    return res;
}

Mat Mat::inverse() const {
    assert((_rows == _cols) && "The matrix must be of the square form.");

    Mat curr = *this;

    Mat res(_rows, _rows);

    for (u64 i = 0; i < _rows; ++i) {
        res._data.emplace(std::pair<u64, u64>{ i, i }, 1.0);
    }

    for (u64 i = 0; i < _rows; ++i) {
        f64 pivot = curr._data.at(std::pair<u64, u64>{ i, i });

        for (u64 j = 0; j < _rows; ++j) {
            if (curr._data.contains(std::pair<u64, u64>{ i, j })) {
                curr._data[std::pair<u64, u64>{ i, j }] /= pivot;
            }
            if (res._data.contains(std::pair<u64, u64>{ i, j })) {
                res._data[std::pair<u64, u64>{ i, j }] /= pivot;
            }
        }

        for (u64 k = 0; k < _rows; ++k) {
            if (k != i) {
                f64 factor = curr._data.at(std::pair<u64, u64>{ k, i });

                for (u64 j = 0; j < _rows; ++j) {
                    if (curr._data.contains(std::pair<u64, u64>{ i, j })) {
                        curr._data[std::pair<u64, u64>{ k, j }] -= factor * res._data.at(std::pair<u64, u64>{ i, j });
                    }
                    if (res._data.contains(std::pair<u64, u64>{ i, j })) {
                        res._data[std::pair<u64, u64>{ k, j }] -= factor * res._data.at(std::pair<u64, u64>{ i, j });
                    }
                }
            }
        }
    }
    return res;
}

Mat Mat::power(u64 power) const {
    Mat res(*this);

    for (int i = 2; i <= power; ++i) {
        res = (res * *this);
    }
    return res;
}

std::ostream& operator<<(std::ostream& out, const Mat& mat) {
    for (u64 i = 0; i < mat._rows; ++i) {
        for (u64 j = 0; j < mat._cols; ++j) {
            out << std::setw(5);
            if (mat._data.contains(std::pair<u64, u64>{ i, j })) {
                out << mat._data.at({ i, j });
            }
            else {
                out << 0.0;
            }
        }
        if (i + 1 < mat._cols) {
            std::cout << "\n";
        }
    }
    std::cout << std::endl;
    return out;
}

Mat operator+(const Mat& m, f64 value) {
    if (std::abs(value) < EPSILON) {
        return m;
    }

    Mat res(m._rows, m._cols);

    for (u64 i = 0; i < m._rows; ++i) {
        for (u64 j = 0; j < m._cols; ++j) {
            auto it = m._data.find({ i, j });
            if (it == m._data.end()) {
                res._data.emplace(std::pair<u64, u64>{ i, j }, value);
                continue;
            }

            f64 v = it->second + value;
            if (std::abs(v) > EPSILON) {
                res._data.emplace(std::pair<u64, u64>{ i, j }, v);
            }
            else {
                res._data.erase(std::pair<u64, u64>{ i, j });
            }
        }
    }

    return res;
}

Mat operator-(const Mat& m, f64 value) {
    if (std::abs(value) < EPSILON) {
        return m;
    }

    Mat res{ m._rows, m._cols };

    for (u64 i = 0; i < m._rows; ++i) {
        for (u64 j = 0; j < m._cols; ++j) {
            auto it = m._data.find({ i, j });

            if (it == m._data.end()) {
                res._data.emplace(std::pair<u64, u64>{ i, j }, -value);
                continue;
            }

            f64 v = it->second - value;
            if (std::abs(v) > EPSILON) {
                res._data.emplace(std::pair<u64, u64>{ i, j }, v);
            }
            else {
                res._data.erase(std::pair<u64, u64>{ i, j });
            }
        }
    }

    return res;
}

Mat operator*(const Mat& m, f64 value) {
    if (std::abs(value) < EPSILON) {
        return Mat{ m._rows, m._cols };
    }

    Mat res{ m._rows, m._cols };

    for (const auto& [idx, val] : m._data) {
        f64 v = val * value;
        if (std::abs(v) >= EPSILON) {
            res._data[idx] = v;
        }
        else {
            res._data.erase(idx);
        }
    }

    return res;
}

Mat operator/(const Mat& m, f64 value) {
    assert((std::abs(value) < EPSILON) && "Division by zero.");

    Mat res{ m._rows, m._cols };

    for (const auto& [idx, val] : m._data) {
        f64 v = val / value;
        if (std::abs(v) >= EPSILON) {
            res._data[idx] = v;
        }
        else {
            res._data.erase(idx);
        }
    }

    return res;
}

Mat operator+(const Mat& m1, const Mat& m2) {
    assert(m1._rows == m2._rows && m1._cols == m2._cols && "The matrices must be the same size.");

    Mat res(m1._rows, m1._cols);

    for (const auto& [idx1, value1] : m1) {
        res._data[idx1] = value1;
    }

    for (const auto& [idx2, value2] : m2) {
        auto it = res._data.find(idx2);
        if (it == res._data.end()) {
            res._data[idx2] = value2;
        }
        else {
            f64 sum = it->second + value2;
            if (std::abs(sum) > EPSILON) {
                res._data[idx2] = sum;
            }
            else {
                res._data.erase(idx2);
            }
        }
    }

    return res;
}

Mat operator-(const Mat& m1, const Mat& m2) {
    assert(m1._rows == m2._rows && m1._cols == m2._cols && "The matrices must be the same size.");

    Mat res(m1._rows, m1._cols);

    for (const auto& [idx1, value1] : m1) {
        res._data[idx1] = value1;
    }

    for (const auto& [idx2, value2] : m2) {
        auto it = res._data.find(idx2);
        if (it == res._data.end()) {
            res._data[idx2] = -value2;
        }
        else {
            f64 sum = it->second - value2;
            if (std::abs(sum) > EPSILON) {
                res._data[idx2] = sum;
            }
            else {
                res._data.erase(idx2);
            }
        }
    }

    return res;
}

Mat operator*(const Mat& m1, const Mat& m2) {
    assert(m1._cols == m2._rows && "Invalid matrices.");

    u64 rows = m1._rows;
    u64 cols = m2._cols;

    Mat res(rows, cols);

    for (const auto& [idx1, value1] : m1) {
        for (const auto& [idx2, value2] : m2) {
            if (idx1.second == idx2.first) {
                f64 value = value1 * value2;
                res._data[std::pair<u64, u64>(idx1.first, idx2.second)] += value;
            }
        }
    }

    std::vector<std::pair<u64, u64>> indices;
    indices.reserve(rows + cols);

    for (const auto& [idx, value] : res._data) {
        if (std::abs(value) < EPSILON) {
            indices.push_back(idx);
        }
    }

    for (const auto& idx : indices) {
        res._data.erase(idx);
    }

    return res;
}

Mat operator^(const Mat& m, u32 exp) {
    assert(m._rows != m._cols && "Matrix must be square for exponential.");
    
    Mat res = m;

    while (exp--) {
        res = res * m;
    }

    return res;
}