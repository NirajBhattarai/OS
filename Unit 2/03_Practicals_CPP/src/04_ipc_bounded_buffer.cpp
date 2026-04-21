#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

constexpr int BUFFER_SIZE = 5;
constexpr int TOTAL_ITEMS = 100;

std::queue<int> buffer_q;
std::mutex mtx;
std::condition_variable not_full;
std::condition_variable not_empty;

long long produced_sum = 0;
long long consumed_sum = 0;
int produced_count = 0;
int consumed_count = 0;

void producer() {
    for (int item = 1; item <= TOTAL_ITEMS; ++item) {
        std::unique_lock<std::mutex> lock(mtx);
        not_full.wait(lock, [] { return static_cast<int>(buffer_q.size()) < BUFFER_SIZE; });

        buffer_q.push(item);
        produced_sum += item;
        produced_count++;

        lock.unlock();
        not_empty.notify_one();
    }
}

void consumer() {
    for (int i = 0; i < TOTAL_ITEMS; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        not_empty.wait(lock, [] { return !buffer_q.empty(); });

        int item = buffer_q.front();
        buffer_q.pop();
        consumed_sum += item;
        consumed_count++;

        lock.unlock();
        not_full.notify_one();
    }
}

int main() {
    std::thread p(producer);
    std::thread c(consumer);
    p.join();
    c.join();

    long long expected_sum = 1LL * TOTAL_ITEMS * (TOTAL_ITEMS + 1) / 2;
    bool pass = true;
    pass = pass && (produced_count == TOTAL_ITEMS);
    pass = pass && (consumed_count == TOTAL_ITEMS);
    pass = pass && (produced_sum == expected_sum);
    pass = pass && (consumed_sum == expected_sum);

    std::cout << "IPC Producer-Consumer\n";
    std::cout << "---------------------\n";
    std::cout << "Produced count: " << produced_count << "\n";
    std::cout << "Consumed count: " << consumed_count << "\n";
    std::cout << "Expected sum  : " << expected_sum << "\n";
    std::cout << "Consumed sum  : " << consumed_sum << "\n";
    std::cout << "Validation: " << (pass ? "PASS" : "FAIL") << "\n";

    return pass ? 0 : 1;
}
