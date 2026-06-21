/* MIT License
 *
 * Copyright (c) 2026 Yaron Dayan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include "timer.hpp" 
#include <vector>

using namespace std; 


using Matrix = vector<vector<int>>;

// Simple matrix transpose operation, juxtaposing naive approach vs optimized 

// ---------------------------------------------------------
// 1. Naive Implementation
// ---------------------------------------------------------
// Std. traversal - suffers from frequent cache misses 
// on the destination matrix for large sizes.
static void naive_transpose(const Matrix& src, Matrix& dst, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            dst[j][i] = src[i][j];
        }
    }
}

// ---------------------------------------------------------
// 2. Optimized Implementation (Cache-Aware)
// ---------------------------------------------------------
// Processes the matrix in sub-blocks to keep data in the L1/L2 cache,
// significantly minimizing page faults and cache thrashing.
static void blocked_transpose(const Matrix& src, Matrix& dst, size_t N, size_t block_size = 64) {
    for (size_t i = 0; i < N; i += block_size) {
        for (size_t j = 0; j < N; j += block_size) {
            // Transpose the current block, taking advantage of the fact that N % block_size == 0 
            for (size_t ii = i; ii < i + block_size; ++ii) {
                for (size_t jj = j; jj < j + block_size; ++jj) {
                    dst[jj][ii] = src[ii][jj];
                }
            }
        }
    }
}

// ---------------------------------------------------------
// Main Execution & Benchmarking
// ---------------------------------------------------------
int main() {
    constexpr size_t N = 1024 << 3; 
    constexpr uint16_t iters = 10; 

    constexpr bool printout = false; 
    constexpr uint16_t prec = 5; 

    Matrix src(N, vector<int>(N));
    Matrix dst_naive(N, vector<int>(N, 0));
    Matrix dst_blocked(N, vector<int>(N, 0));

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            src[i][j] = static_cast<int>(i * N + j);
        }
    }

    cout << "Benchmarking Matrix Transposition (" << N << " x " << N << ")...\n";

    auto runNaive = [&]() {
        naive_transpose(src, dst_naive, N);
        };
    timeIt("Naive Transpose", runNaive, iters, printout, prec);

    auto runBlocked = [&]() {
        blocked_transpose(src, dst_blocked, N);
        };
    timeIt("Blocked Transpose", runBlocked, iters, printout, prec);

    return 0;
}