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

#pragma once

#include <string>
#include "logger.hpp"


struct TimeResult {
    double value;
    std::string_view unit;
};


inline void print_header(std::string_view msg, bool printout = false) {
    printout ? (std::cout << msg << '\n') : (LOG_INFO("{}", msg), std::cout);
}

// scale and format the time measurement smoothly with zero heap allocations
inline TimeResult time_scaler(double time_res) {
    if (time_res <= 0.5 && time_res > 1e-3) {
        return { time_res * 1e3, " [ms]" };
    }
    if (time_res <= 1e-3 && time_res > 1e-6) {
        return { time_res * 1e6, " [us]" };
    }
    if (time_res <= 1e-6 && time_res > 1e-9) {
        return { time_res * 1e9, " [ns]" };
    }
    if (time_res <= 1e-9) {
        return { time_res * 1e12, " [ps]" };
    }
    return { time_res, " [s]" };
}

 
/* Measure time of a code block using a lambda or any callable
*  Example:
    // Define the lambda to wrap the function call
    auto func = [&]() {
        matTranspose(mat);
    };

    // Time the function using timeIt
    timeIt("matTranspose", func, 10);
*/
template <typename Func>
void timeIt(const std::string &func_name, Func &&func, uint16_t num_iters = 100, bool printout = false, uint16_t precision = 4) {
    
    auto start = std::chrono::steady_clock::now();
    for (uint16_t i = 0; i < num_iters; ++i) {
        // Enforce perfect forwarding on the callable to eliminate invocation/copy penalties
        std::forward<Func>(func)();  
    }
    auto end = std::chrono::steady_clock::now();
    
    std::chrono::duration<double> delta_time = end - start;
    auto [avg_value, time_unit] = time_scaler(delta_time.count() / num_iters);

    const std::string fmt_spec = std::format("'{{}}' took {{:.{}f}}{{}} in avg. over {{}} iterations.", precision);
    const std::string msg = std::vformat(fmt_spec, std::make_format_args(func_name, avg_value, time_unit, num_iters));

    printout ? (std::cout << msg << '\n') : (LOG_INFO("{}", msg), std::cout);
}

