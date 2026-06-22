#include "logger.hpp"
#include <sstream>
#include <cassert>
#include <string>
#include <filesystem>

namespace logger::unittests {

    // RAII helper to intercept cout into a local stringstream.
    struct RedirectConsole {
        std::stringstream ss;
        std::streambuf* old_buf;
    
        RedirectConsole() : old_buf(std::cout.rdbuf(ss.rdbuf())) {}
        ~RedirectConsole() { std::cout.rdbuf(old_buf); }
    
        std::string str() const { return ss.str(); }
    };

    static std::string read_file_content(const std::filesystem::path& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) return "";
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static void run_logger_tests() {
        std::cout << "Running logger subsystem tests...\n";

        const std::filesystem::path test_log_path = "test_execution.log";
    
        // Clean up any lingering test logs from a previous crashed run
        std::filesystem::remove(test_log_path);

        // =========================================================================
        // Test Case 1: File Output Fidelity and Format Parsing
        // =========================================================================
        {
            LOG_INFO("Matrix processing phase {} initialized successfully.", 1);
            LOG_ERROR("Cache miss violation threshold breached: {} > {}.", 98.2, 95.0);

            std::string log_contents = read_file_content(test_log_path);

            // Verify the log severities exist
            assert(log_contents.find("[INFO]") != std::string::npos && "Failure: INFO tag missing from log file!");
            assert(log_contents.find("[ERROR]") != std::string::npos && "Failure: ERROR tag missing from log file!");

            assert(log_contents.find("phase 1 initialized") != std::string::npos && "Failure: Formatting integer failed in log!");
            assert(log_contents.find("98.2 > 95") != std::string::npos && "Failure: Formatting floating-point values failed in log!");
        }

        // =========================================================================
        // Test Case 2: Multi-Threaded Thread-Safety (Stress Test)
        // =========================================================================
        {
            // This test ensures that when multiple execution threads log simultaneously,
            // mutex guards prevent race conditions or partial line overwrites.
            constexpr int num_threads = 8;
            constexpr int logs_per_thread = 50;
            std::vector<std::thread> workers;

            for (int i = 0; i < num_threads; ++i) {
                workers.emplace_back([i, logs_per_thread]() {
                    for (int j = 0; j < logs_per_thread; ++j) {
                        LOG_INFO("Thread #{} reporting loop iteration metric sequence token: {}", i, j);
                    }
                });
            }

            for (auto& worker : workers) {
                worker.join();
            }

            // Verification pass: Count the total number of valid logs written
            std::ifstream file(test_log_path);
            std::string line;
            int total_lines = 0;
            while (std::getline(file, line)) {
                if (line.find("reporting loop iteration") != std::string::npos) {
                    total_lines++;
                }
            }

            int expected_lines = num_threads * logs_per_thread;
            assert(total_lines == expected_lines && "Failure: Log entries were dropped or corrupted during race conditions!");
        }

        // =========================================================================
        // Test Case 3: Edge Case Anomalies (Empty Strings and Special Characters)
        // =========================================================================
        {
            // Verify that the engine doesn't crash on empty formats, raw braces, or escaped symbols
            LOG_INFO(""); 
            LOG_WARN("{} {} {} !!!", "@#$", "{}", "\n");

            std::string log_contents = read_file_content(test_log_path);
            assert(log_contents.find("@#$ {} \n !!!") != std::string::npos && "Failure: Logger failed to handle special character strings safely!");
        }

        std::filesystem::remove(test_log_path);

        std::cout << "All logger subsystem tests passed successfully!\n";
    }

}

int main() {
    logger::unittests::run_logger_tests(); 
    return 0; 
}