/*MIT License

Copyright(c) 2026 Yaron Dayan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <iostream>
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;

// Function to scale and format the time measurement
inline pair<double, string> time_scaler(double time_res) {
    string time_unit = " [s]";
    if (time_res < 0.5 && time_res > 1e-3) {
        time_res *= 1e3;
        time_unit = " [ms]";
    }
    else if (time_res < 1e-3 && time_res > 1e-6) {
        time_res *= 1e6;
        time_unit = " [us]";
    }
    else if (time_res < 1e-6 && time_res > 1e-9) {
        time_res *= 1e9;
        time_unit = " [ns]";
    }
    return { time_res, time_unit };
}

 
/*Function to measure time of a code block using a lambda or any callable
*  Example:
    // Define the lambda to wrap the function call
    auto func = [&]() {
        matTranspose(mat);
    };

    // Time the function using timeIt
    timeIt("matTranspose", func, 10);
*/
template <typename Func>
void timeIt(const string &funcName, Func &&func, int numIters = 100) {
    
    auto start = steady_clock::now();
    for (int i = 0; i < numIters; i++) {
        func();  // Execute the code block (lambda or callable) 
    }
    auto end = steady_clock::now();
    auto delta_time = duration_cast<duration<double>>(end - start);
    pair<double, string> avgDuration = time_scaler(delta_time.count() / numIters);

    cout << "\n\nFunction or code section'" << funcName << "' took " << avgDuration.first << avgDuration.second << endl;
}
