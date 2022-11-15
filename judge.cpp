#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>

/* ====================== SETTINGS ====================== */

// Compiler settings
const std::string compiler_flags = "g++ -std=c++17";

// Verdict messages
const std::string ac = "\x1b[42m Accepted \x1b[0m\n";
const std::string wa = "\x1b[41m Wrong Answer \x1b[0m\n";
const std::string par = "\x1b[44m Partial Score \x1b[0m\n";
const std::string tle = "\x1b[100m Time Limit Exceeded \x1b[0m\n";
const std::string skipped = "\x1b[100m Skipped \x1b[0m\n";

// Scoring mode
const bool ICPC = false;

// Runtime settings
const float time_limit = 1000; // Measured in miliseconds
const bool pause_at_non_AC_testcase = false;

/* ==================== MAIN PROGRAM ==================== */
const std::string AC = "Accepted";
const std::string WA = "Wrong Answer";
const std::string TLE = "Time Limit Exceeded";
const std::string PARTIAL = "Partial Score";

struct checker_response {
    std::string verdict;
    float points;
    std::string response;
};

// Perform check on "/testcase/bruteforce.out" and "/testcase/candidate.out"
checker_response perform_check(std::chrono::milliseconds exec_time) {
    using namespace std;

    ifstream tst("./testcase/input.inp", fstream::in);
    ifstream brf("./testcase/bruteforce.out", fstream::in);
    ifstream cnd("./testcase/candidate.out", fstream::in);

    if(rand() % 2) return {AC, 1.0, ""};
    return {WA, 0.0, ""};

    if(exec_time.count() > time_limit) return {TLE, 0.0, ""};

    int n; tst >> n;
    int a,b; cnd >> a >> b;
    if(a + b == n) return {AC, 1.0, to_string(a) + " + " + to_string(b) + " = " + to_string(n)};
    return {WA, 0.0, to_string(a) + " + " + to_string(b) + " != " + to_string(n)};
}

int main() {

/* Initialization */

    // Set score precision
    srand(time(NULL));
    std::cout << std::fixed << std::setprecision(2);

    // Compile test_gen.cpp, bruteforce.cpp, and candidate.cpp
    std::cout << "Compiling candidate.cpp...\n";
    system((compiler_flags + " candidate.cpp -o candidate").c_str());
    std::cout << "Compiling bruteforce.cpp...\n";
    system((compiler_flags + " bruteforce.cpp -o bruteforce").c_str());
    std::cout << "Compiling test generator...\n";
    system((compiler_flags + " test_gen.cpp -o test_gen").c_str());

/* Testing phase */
    int testcase_cnt = 0;
    std::cout << "Enter no. of tests to perform: ";
    std::cin >> testcase_cnt;

    // Some runtime information
    int ac_verdict = 0, wa_verdict = 0, tle_verdict = 0;
    int brf_time = 0, cnd_time = 0;
    float total_points = 0;
    for(int test = 1; test <= testcase_cnt; ++ test) {
        std::cout << "=============== Testcase #" << test << " ===============\n";

        // Generate tests & run both bruteforce and candidate
        system(std::string("./test_gen").c_str());

            // Measure execution time of bruteforce.cpp
            auto brf_start = std::chrono::high_resolution_clock::now();
            system(std::string("./bruteforce").c_str());
            auto brf_end = std::chrono::high_resolution_clock::now();

            // Measure execution time of candidate.cpp
            auto cnd_start = std::chrono::high_resolution_clock::now();
            system(std::string("./candidate").c_str());
            auto cnd_end = std::chrono::high_resolution_clock::now();

        // Get duration
        auto duration_brf = std::chrono::duration_cast<std::chrono::milliseconds>(brf_end - brf_start);
        auto duration_cnd = std::chrono::duration_cast<std::chrono::milliseconds>(cnd_end - cnd_start);

        // Give verdict based on checker's response
        checker_response response = perform_check(duration_cnd);
        std::cout << "Judge's verdict: ";
        if((wa_verdict > 0 || tle_verdict > 0) && ICPC) { // ICPC mode is enabled, so no need to check for more testcases
            std::cout << skipped;
            continue;
        }
        if(response.verdict == AC) { // Accepted verdict
            ++ ac_verdict;
            std::cout << ac;
        }
        if(response.verdict == WA) { // Wrong Answer verdict
            ++ wa_verdict;
            std::cout << wa;
        }
        if(response.verdict == TLE) { // Time Limit Exceeded verdict
            ++ tle_verdict;
            std::cout << tle;
        }
        if(response.verdict == PARTIAL) { // Partial Score verdict
            ++ wa_verdict;
            std::cout << par;
        }

        if(!ICPC) { // ICPC mode is enabled, same as above
            total_points += response.points;
            brf_time += duration_brf.count(), cnd_time += duration_cnd.count();
            std::cout << "Points: " << response.points << " / " << float(1) << "\n";
            std::cout << "Execution time: " << duration_brf.count() << " ms (bruteforce), " << duration_cnd.count() << " ms (candidate)\n";
        }
        std::cout << "Checker's response: " << response.response << "\n";

        // Allow user to pause and view the current testcase
        if(pause_at_non_AC_testcase && (response.verdict == WA || response.verdict == TLE || response.verdict == PARTIAL)) {
            std::cout << "This test did not receive an AC (Accepted) verdict. Do you wish to continue (Y/N)? ";
            char inp; std::cin >> inp;
            if(inp == 'N') {
                std::cout << "- You can view the current testcase in \"/testcase/input.inp\" -\n";
                break;
            }
        }
    }

    /* Output the final verdict */
    std::cout << "=============== Result ===============\n";
    std::cout << "Final verdict: ";
    bool more_info = false;
    if(ac_verdict == testcase_cnt) {
        std::cout << ac;
        more_info = true;
    }
    else if(wa_verdict >= tle_verdict) std::cout << wa;
    else std::cout << tle;

    if(more_info) {
        if(!ICPC) std::cout << "Points: " << total_points << " / " << (float) testcase_cnt << "\n";
        std::cout << "Total execution time: " << brf_time << " ms (bruteforce), " << cnd_time << " ms (candidate)\n";
        std::cout << "Accepted verdicts: " << ac_verdict << "\n";
        std::cout << "Wrong Answer / Partial Score verdicts: " << wa_verdict << "\n";
        std::cout << "Time Limit Exceeded verdicts: " << tle_verdict << "\n";
    }
    std::cout << "=============== TASJudge ===============";
}