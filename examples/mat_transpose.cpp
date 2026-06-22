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

#include <vector>
#include <cassert>
#include "../timer/timer.hpp"

using namespace std;

using Matrix = vector<vector<int>>;

inline void display_msg(std::string_view msg, bool printout = false)
{
    if (printout)
    {
        std::cout << msg << '\n';
    }
    else
    {
        LOG_INFO("{}", msg);
    }
}

inline void display_error(std::string_view msg, bool printout = false)
{
    if (printout)
    {
        std::cerr << msg << '\n';
    }
    else
    {
        LOG_ERROR("{}", msg);
    }
}

template <size_t Start, size_t End, typename F>
inline constexpr void compile_time_for(F &&f)
{
    if constexpr (Start < End)
    {
        f(integral_constant<size_t, Start>{});
        compile_time_for<Start + 1, End>(forward<F>(f));
    }
}

// Simple matrix transpose operation, juxtaposing naive approach vs optimized

// ---------------------------------------------------------
// 1. Naive Implementation
// ---------------------------------------------------------
// Std. traversal - suffers from frequent cache misses
// on the destination matrix for large sizes.
static void naive_transpose(const Matrix &src, Matrix &dst, size_t N)
{
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            dst[j][i] = src[i][j];
        }
    }
}

// ---------------------------------------------------------
// 2. Optimized Implementation (Cache-Aware)
// ---------------------------------------------------------
// Processes the matrix in sub-blocks to keep data in the L1/L2 cache,
// significantly minimizing page faults and cache thrashing.
template <size_t block_size = 64, size_t micro_kernel_size = 8>
static void blocked_transpose(const Matrix &src, Matrix &dst, size_t N)
{
    static_assert(block_size % micro_kernel_size == 0, "block_size must be a multiple of micro_kernel_size!");
    assert(N % block_size == 0 && "N must be a multiple of block_size!");

    for (size_t i = 0; i < N; i += block_size)
    {
        for (size_t j = 0; j < N; j += block_size)
        {
            // Transpose the current block using optimized register tiling
            for (size_t ii = i; ii < i + block_size; ii += micro_kernel_size)
            {
                for (size_t jj = j; jj < j + block_size; jj += micro_kernel_size)
                {
                    compile_time_for<0, micro_kernel_size>([&](auto r)
                                                           { compile_time_for<0, micro_kernel_size>([&](auto c)
                                                                                                    { dst[jj + c][ii + r] = src[ii + r][jj + c]; }); });
                }
            }
        }
    }
}

// 3. Even faster
static void blocked_transpose2(const Matrix &src, Matrix &dst, size_t N, size_t block_size = 64)
{
    size_t micro_kernel_size = 8;
    assert(block_size % micro_kernel_size == 0 && "block_size must be a multiple of micro_kernel_size!");
    assert(N % block_size == 0 && "N must be a multiple of block_size!");

    for (size_t i = 0; i < N; i += block_size)
    {
        for (size_t j = 0; j < N; j += block_size)
        {
            // Transpose the current block using optimized register tiling
            for (size_t ii = i; ii < i + block_size; ii += micro_kernel_size)
            {
                for (size_t jj = j; jj < j + block_size; jj += micro_kernel_size)
                {
                    for (size_t r = 0; r < 8; ++r)
                    {
                        dst[jj + 0][ii + r] = src[ii + r][jj + 0];
                        dst[jj + 1][ii + r] = src[ii + r][jj + 1];
                        dst[jj + 2][ii + r] = src[ii + r][jj + 2];
                        dst[jj + 3][ii + r] = src[ii + r][jj + 3];
                        dst[jj + 4][ii + r] = src[ii + r][jj + 4];
                        dst[jj + 5][ii + r] = src[ii + r][jj + 5];
                        dst[jj + 6][ii + r] = src[ii + r][jj + 6];
                        dst[jj + 7][ii + r] = src[ii + r][jj + 7];
                    }
                }
            }
        }
    }
}

static bool verify_matrices(const Matrix &mat_a, const Matrix &mat_b)
{
    return mat_a == mat_b;
}

// ---------------------------------------------------------
// Main Execution & Benchmarking
// ---------------------------------------------------------
int main()
{
    constexpr size_t N = 512 << 3;
    constexpr uint16_t iters = 3;

    constexpr bool printout = false;
    constexpr uint8_t prec = 5;

    Matrix src(N, vector<int>(N));
    Matrix dst_naive(N, vector<int>(N, 0));
    Matrix dst_blocked1(N, vector<int>(N, 0));
    Matrix dst_blocked2(N, vector<int>(N, 0));

    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            src[i][j] = static_cast<int>((i * N) + j);
        }
    }

    const string header_msg = format("Benchmarking Matrix Transposition ({} x {})...", N, N);
    display_msg(header_msg, printout);

    auto runNaive = [&]()
    {
        naive_transpose(src, dst_naive, N);
    };
    time_it("Naive Transpose", runNaive, iters, printout, prec);

    auto runBlocked1 = [&]()
    {
        blocked_transpose(src, dst_blocked1, N);
    };
    time_it("Blocked Transpose1", runBlocked1, iters, printout, prec);

    auto runBlocked2 = [&]()
    {
        blocked_transpose2(src, dst_blocked2, N);
    };
    time_it("Blocked Transpose2", runBlocked2, iters, printout, prec);

    if (verify_matrices(dst_naive, dst_blocked1) && verify_matrices(dst_naive, dst_blocked2))
    {
        display_msg("[SUCCESS] Matrices match perfectly!\n", printout);
    }
    else
    {
        display_error("[ERROR] Matrix mismatch detected!Optimization logic is flawed.\n", printout);
    }

    return 0;
}