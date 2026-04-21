#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

constexpr int THREADS = 8;
constexpr int INCREMENTS = 20000;
long long counter = 0;
std::mutex mtx;

void unsafe_increment() {
    for (int i = 0; i < INCREMENTS; ++i) {
        long long temp = counter;
        std::this_thread::yield();
        counter = temp + 1;
    }
}

void safe_increment() {
    for (int i = 0; i < INCREMENTS; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        long long temp = counter;
        counter = temp + 1;
    }
}

long long run_unsafe_once() {
    counter = 0;
    std::vector<std::thread> workers;
    for (int i = 0; i < THREADS; ++i) workers.emplace_back(unsafe_increment);
    for (auto& t : workers) t.join();
    return counter;
}

long long run_safe_once() {
    counter = 0;
    std::vector<std::thread> workers;
    for (int i = 0; i < THREADS; ++i) workers.emplace_back(safe_increment);
    for (auto& t : workers) t.join();
    return counter;
}

int main() {
    const long long expected = 1LL * THREADS * INCREMENTS;

    long long unsafe_value = expected;
    bool race_observed = false;
    for (int attempt = 1; attempt <= 5; ++attempt) {
        unsafe_value = run_unsafe_once();
        if (unsafe_value != expected) {
            race_observed = true;
            break;
        }
    }

    long long safe_value = run_safe_once();
    bool safe_correct = (safe_value == expected);

    std::cout << "Threads Race vs Mutex\n";
    std::cout << "---------------------\n";
    std::cout << "Expected count: " << expected << "\n";
    std::cout << "Unsafe count : " << unsafe_value << "\n";
    std::cout << "Safe count   : " << safe_value << "\n";

    bool pass = race_observed && safe_correct;
    std::cout << "Validation: " << (pass ? "PASS" : "FAIL") << "\n";

    return pass ? 0 : 1;
}
