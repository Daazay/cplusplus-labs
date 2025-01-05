#include <vector>
#include <iostream>
#include <chrono>
#include <cassert>
#include <string>

#include "defines.h"
#include "vec.h"
#include "mat.h"

constexpr u64 VECTOR_SIZE = 10'000;
constexpr u64 MATRIX_SIZE = 100;
constexpr u64 DELTA_STEP = 25;

using SEC = std::chrono::seconds;
using MS = std::chrono::milliseconds;
using MCS = std::chrono::microseconds;
using NS = std::chrono::nanoseconds;

class Bench {
    using clock_t = std::chrono::steady_clock;
    using tp_t = std::chrono::steady_clock::time_point;
public:
    Bench(const std::string& name)
        : _name{name} { 
        std::cout << name << " - starting." << std::endl;
        _start = clock_t::now();
    }
    ~Bench() {
        tp_t end = clock_t::now();
        std::cout << _name << " - finished with time: ";
        time();
        std::cout << std::endl;
    }
public:
    void time() const {
        tp_t end = clock_t::now();
        std::cout << std::chrono::duration_cast<MCS>(end - _start);
    }
private:
    std::string _name;
    tp_t _start;
};

std::vector<f64> std_vec_sum(const std::vector<f64>& v1, const std::vector<f64>& v2);

std::vector<f64> std_vec_sub(const std::vector<f64>& v1, const std::vector<f64>& v2);

std::vector<f64> std_vec_mul(const std::vector<f64>& v, f64 value);

std::vector<f64> std_vec_div(const std::vector<f64>& v, f64 value);

f64 std_vec_mul(const std::vector<f64>& v1, const std::vector<f64>& v2);

std::vector<std::vector<f64>> std_mat_add(const std::vector<std::vector<f64>>& m1, const std::vector<std::vector<f64>>& m2);

void test_vectors();

void test_matrix();

//

f64 std_vec_mul(const std::vector<f64>& v1, const std::vector<f64>& v2) {
    assert((v1.size() == v2.size()) && "Invalid vec size");

    f64 res = 0.0;

    for (u64 i = 0; i < v1.size(); ++i) {
        res += v1[i] * v2[i];
    }

    return res;
}

std::vector<f64> std_vec_sum(const std::vector<f64>& v1, const std::vector<f64>& v2) {
    assert((v1.size() == v2.size()) && "Invalid vec size");
    
    std::vector<f64> res(v1.size());

    for (u64 i = 0; i < v1.size(); ++i) {
        res[i] = v1[i] + v2[i];
    }

    return res;
}

std::vector<f64> std_vec_sub(const std::vector<f64>& v1, const std::vector<f64>& v2) {
    assert((v1.size() == v2.size()) && "Invalid vec size");

    std::vector<f64> res(v1.size());

    for (u64 i = 0; i < v1.size(); ++i) {
        res[i] = v1[i] - v2[i];
    }

    return res;
}

std::vector<f64> std_vec_mul(const std::vector<f64>& v, f64 value) {
    std::vector<f64> res(v.size());

    for (u64 i = 0; i < v.size(); ++i) {
        res[i] = v[i] * value;
    }

    return res;
}

std::vector<f64> std_vec_div(const std::vector<f64>& v, f64 value) {
    assert((std::abs(value) < EPSILON) && "Division by zero.");

    std::vector<f64> res(v.size());

    for (u64 i = 0; i < v.size(); ++i) {
        res[i] = v[i] / value;
    }

    return res;
}

std::vector<std::vector<f64>> std_mat_add(const std::vector<std::vector<f64>>& m1, const std::vector<std::vector<f64>>& m2) {
    std::vector<std::vector<f64>> res(m1.size(), std::vector<f64>(m1[0].size()));

    for (int i = 0; i < m1.size(); ++i) {
        for (int j = 0; j < m1[0].size(); ++j) {
            res[i][j] = m1[i][j] + m2[i][j];
        }
    }
 
    return res;
}

void test_vectors() {
    std::vector<f64> v1(VECTOR_SIZE, 0.0);
    std::vector<f64> v2(VECTOR_SIZE, 0.0);

    for (u64 i = 0; i < VECTOR_SIZE; ++i) {
        v1[i] = v2[i] = 1.0;
    }

    Vec vec1 = v1;
    Vec vec2 = v2;

    {
        Bench bench("custom vec");

        Vec res1 = vec1 + vec2;     // vectors add
        
        std::cout << "add:";
        bench.time();
        std::cout << "\n";

        Vec res2 = vec1 * 2;         // vector value multiply

        std::cout << "mul val:";
        bench.time();
        std::cout << "\n";

        double res3 = vec1 * vec2;  // vectors multiply
     
        std::cout << "mul vec:";
        bench.time();
        std::cout << "\n";
    }
    {
        Bench bench("std vec");

        std::vector<f64> res1 = std_vec_sum(v1, v2);    // vectors add
        
        std::cout << "add:";
        bench.time();
        std::cout << "\n";

        std::vector<f64> res2 = std_vec_mul(v1, 2);     // vector value mult
        
        std::cout << "mul val:";
        bench.time();
        std::cout << "\n";

        f64 res3 = std_vec_mul(v1, v2);                 // vectors multiply
        
        std::cout << "mul vec:";
        bench.time();
        std::cout << "\n";
    }
}

void test_matrix() {
    std::vector<std::vector<double>> m1(MATRIX_SIZE, std::vector<double>(MATRIX_SIZE));
    std::vector<std::vector<double>> m2(MATRIX_SIZE, std::vector<double>(MATRIX_SIZE));

    for (int i = 0; i < MATRIX_SIZE; i += DELTA_STEP) {
        for (int j = 0; j < MATRIX_SIZE; j += DELTA_STEP) {
            m1[i][j] = m2[i][j] = 1.0;
        }
    }

    Mat mat1 = m1;
    Mat mat2 = m2;

    {
        Bench bench("custom mat");

        Mat res1 = mat1 + mat2; // matrices add
        
        std::cout << "add:";
        bench.time();
        std::cout << "\n";
    }
    {
        Bench bench("std mat");

        std::vector<std::vector<f64>> res1 = std_mat_add(m1, m2);   // matrices add
     
        std::cout << "add:";
        bench.time();
        std::cout << "\n";
    }
}

int main(int argc, char* argv) {
    test_vectors();
    test_matrix();

    //Mat a = { 
    //    {3, 2, 1}, 
    //    {0, 4, 6}, 
    //    {1, 1, 1} 
    //};
    //Mat b = { 
    //    {9, 2, 6}, 
    //    {6, -13, 5}, 
    //    {-1, 4, 3} 
    //};
    //std::cout << "A:\n" << a;
    //std::cout << "B:\n" << b;

    //std::cout << "sum:\n";
    //Mat sum = a + b;
    //std::cout << sum;

    //std::cout << "sub:\n";
    //Mat sub = a - b;
    //std::cout << sub;
    //
    //std::cout << "mul:\n";
    //Mat mul = a * b;
    //std::cout << mul;

    return 0;
}