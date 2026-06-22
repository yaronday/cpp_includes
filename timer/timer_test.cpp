#include <cassert>
#include "timer.hpp"


namespace timer::unittests {

    // Helper to check if two floating-point values are approximately equal at compile time
    static constexpr bool compile_time_approx(double actual, double expected, double epsilon = 1e-6) {
        double diff = actual - expected;
        if (diff < 0.0) diff = -diff;
        return diff <= epsilon;
    }

    // Global compile-time validation suite
    static constexpr bool run_compile_time_tests() {

        // --- Test Case 1: Seconds Boundary (No scaling applied) ---
        {
            auto [value, unit] = time_scaler(1.5);
            if (!compile_time_approx(value, 1.5) || unit != " [s]") return false;
        }

        // --- Test Case 2: Milliseconds Range ---
        {
            auto [value, unit] = time_scaler(0.25); // <= 0.5 and > 1e-3
            if (!compile_time_approx(value, 250.0) || unit != " [ms]") return false;
        }

        // --- Test Case 3: Microseconds Range ---
        {
            auto [value, unit] = time_scaler(5e-4); // <= 1e-3 and > 1e-6
            if (!compile_time_approx(value, 500.0) || unit != " [us]") return false;
        }

        // --- Test Case 4: Nanoseconds Range ---
        {
            auto [value, unit] = time_scaler(7e-8); // <= 1e-6 and > 1e-9
            if (!compile_time_approx(value, 70.0) || unit != " [ns]") return false;
        }

        // --- Test Case 5: Picoseconds Range ---
        {
            auto [value, unit] = time_scaler(1e-11); // <= 1e-9
            if (!compile_time_approx(value, 10.0) || unit != " [ps]") return false;
        }

        // --- Test Case 6: Edge Case Boundaries ---
        {
            // Lower limit exact thresholds
            auto [val_ms, unit_ms] = time_scaler(1e-3); // Fits into [us] due to strict '<= 1e-3' branch layout
            if (!compile_time_approx(val_ms, 1000.0) || unit_ms != " [us]") return false;

            auto [val_zero, unit_zero] = time_scaler(0.0); // Exact zero defaults to picoseconds branch
            if (!compile_time_approx(val_zero, 0.0) || unit_zero != " [ps]") return false;
        }

        // Negative duration check
        auto [val_neg, unit_neg] = time_scaler(-0.005);
        if (unit_neg != " [err]") return false;

        return true;
    }

    // --- The Ultimate Gatekeeper: Static Assertions ---
    // If any engineering changes break the timing unit calculations or string views, 
    // the code will reject compilation right here with an explicit error message.
    static_assert(run_compile_time_tests(), "Unit Test Failure: timer.hpp math logic or threshold ranges collapsed!");

    static void run_runtime_tests() {
        std::cout << "Running timer runtime tests...\n\n";

        // --- Test Case 1: Execution Count Precision ---
        {
            uint32_t call_count = 0;
            constexpr uint16_t target_iters = 45;

            // Pass a tracking lambda to verify execution parity
            time_it("Count Test", [&]() {
                call_count++;
                }, target_iters, true); // printout=true to bypass file logs for testing

            assert(call_count == target_iters && "Failure: time_it failed to execute the callable exactly N times!");
        }

        // --- Test Case 2: State Retention & Side-Effects ---
        {
            int accumulator = 0;
            constexpr uint16_t iterations = 10;

            // Verify that lvalue execution doesn't corrupt or reset state mid-loop
            time_it("Accumulator Test", [&]() {
                accumulator += 2;
                }, iterations, true);

            assert(accumulator == (iterations * 2) && "Failure: Lambda state mutations were corrupted or unaligned!");
        }

        // --- Test Case 3: Zero-Iteration Boundary Graceful Exit ---
        {
            bool lambda_executed = false;

            // This should hit the interface guard, return early, and never execute the lambda
            time_it("Zero Intercept Test", [&]() {
                lambda_executed = true;
                }, 0, true);

            assert(!lambda_executed && "Failure: time_it executed a lambda even when num_iters was 0!");
        }

        std::cout << "\nAll runtime tests passed successfully!\n";
    }

} // namespace timer::unittests

int main() {
    timer::unittests::run_runtime_tests();
    return 0; 
}