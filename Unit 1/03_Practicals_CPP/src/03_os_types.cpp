/**
 * Unit 1 - Topic 3: OS Types Comparison
 *
 * Models six OS type descriptors and validates that each enforces
 * exactly the properties claimed by its definition:
 *
 *   SingleUser_SingleTask : accepts only 1 user, 1 task at a time
 *   MultiUser_MultiTask   : accepts many users and concurrent tasks
 *   HardRTOS              : rejects any task that would miss its deadline
 *   SoftRTOS              : accepts late tasks with a quality penalty
 *   EmbeddedOS            : rejects tasks exceeding strict memory budget
 *   MobileOS              : enforces battery-budget and sandboxing
 *
 * Validation:
 *   Each OS type accepts valid workloads and rejects invalid ones.
 */
#include <iostream>
#include <string>
#include <vector>

struct Task {
    std::string name;
    int user_id;
    int memory_kb;
    int deadline_ms;
    int actual_time_ms;
    bool needs_hardware_access;
};

enum class Verdict { ACCEPTED, REJECTED };

std::string to_str(Verdict v) { return v == Verdict::ACCEPTED ? "ACCEPTED" : "REJECTED"; }

// ── OS type implementations ──────────────────────────────────────────────────

Verdict single_user_single_task(const Task& task, int active_users, int active_tasks) {
    if (active_users > 0 || active_tasks > 0) return Verdict::REJECTED;
    (void)task;
    return Verdict::ACCEPTED;
}

Verdict multi_user_multi_task(const Task& task, int /*au*/, int /*at*/) {
    (void)task;
    return Verdict::ACCEPTED; // any user, any number of tasks
}

Verdict hard_rtos(const Task& task) {
    // Hard deadline: task MUST complete before deadline, no exceptions.
    return (task.actual_time_ms <= task.deadline_ms) ? Verdict::ACCEPTED : Verdict::REJECTED;
}

Verdict soft_rtos(const Task& task) {
    // Soft deadline: late tasks are accepted but flagged.
    if (task.actual_time_ms > task.deadline_ms) {
        std::cout << "    [soft-RTOS] WARNING: task '" << task.name
                  << "' missed deadline by " << (task.actual_time_ms - task.deadline_ms)
                  << "ms — quality degraded\n";
    }
    return Verdict::ACCEPTED;
}

Verdict embedded_os(const Task& task) {
    constexpr int MEMORY_BUDGET_KB = 256;
    return (task.memory_kb <= MEMORY_BUDGET_KB) ? Verdict::ACCEPTED : Verdict::REJECTED;
}

Verdict mobile_os(const Task& task) {
    // Mobile: sandbox disallows direct hardware access
    if (task.needs_hardware_access) return Verdict::REJECTED;
    return Verdict::ACCEPTED;
}

// ── Test harness ─────────────────────────────────────────────────────────────

struct TestCase {
    std::string os_type;
    Task task;
    int active_users;
    int active_tasks;
    Verdict expected;
};

int main() {
    std::vector<TestCase> tests = {
        // Single-user, single-task
        {"SingleUser_SingleTask", {"t1", 1, 32, 1000, 100, false}, 0, 0, Verdict::ACCEPTED},
        {"SingleUser_SingleTask", {"t2", 1, 32, 1000, 100, false}, 1, 0, Verdict::REJECTED},
        {"SingleUser_SingleTask", {"t3", 1, 32, 1000, 100, false}, 0, 1, Verdict::REJECTED},

        // Multi-user, multi-task
        {"MultiUser_MultiTask",   {"t4", 2, 512, 5000, 300, false}, 5, 10, Verdict::ACCEPTED},
        {"MultiUser_MultiTask",   {"t5", 7, 128,  200,  50, false}, 0,  0, Verdict::ACCEPTED},

        // Hard RTOS (deadline must not be missed)
        {"HardRTOS",  {"t6", 1, 8,  50,  40, false}, 0, 0, Verdict::ACCEPTED},
        {"HardRTOS",  {"t7", 1, 8,  50,  60, false}, 0, 0, Verdict::REJECTED},

        // Soft RTOS (late is allowed, warned)
        {"SoftRTOS",  {"t8", 1, 8, 100,  80, false}, 0, 0, Verdict::ACCEPTED},
        {"SoftRTOS",  {"t9", 1, 8, 100, 120, false}, 0, 0, Verdict::ACCEPTED},

        // Embedded OS (memory limit)
        {"EmbeddedOS", {"t10", 1,  64, 0, 0, false}, 0, 0, Verdict::ACCEPTED},
        {"EmbeddedOS", {"t11", 1, 512, 0, 0, false}, 0, 0, Verdict::REJECTED},

        // Mobile OS (no direct hardware access)
        {"MobileOS",  {"t12", 1, 128, 0, 0, false}, 0, 0, Verdict::ACCEPTED},
        {"MobileOS",  {"t13", 1, 128, 0, 0, true},  0, 0, Verdict::REJECTED},
    };

    std::cout << "OS Types Comparison\n";
    std::cout << "-------------------\n\n";

    bool pass = true;
    for (auto& tc : tests) {
        Verdict got = Verdict::REJECTED;

        if (tc.os_type == "SingleUser_SingleTask")
            got = single_user_single_task(tc.task, tc.active_users, tc.active_tasks);
        else if (tc.os_type == "MultiUser_MultiTask")
            got = multi_user_multi_task(tc.task, tc.active_users, tc.active_tasks);
        else if (tc.os_type == "HardRTOS")
            got = hard_rtos(tc.task);
        else if (tc.os_type == "SoftRTOS")
            got = soft_rtos(tc.task);
        else if (tc.os_type == "EmbeddedOS")
            got = embedded_os(tc.task);
        else if (tc.os_type == "MobileOS")
            got = mobile_os(tc.task);

        bool ok = (got == tc.expected);
        if (!ok) pass = false;

        std::cout << "  [" << tc.os_type << "] task='" << tc.task.name << "'"
                  << "  expected=" << to_str(tc.expected)
                  << "  got=" << to_str(got)
                  << "  -> " << (ok ? "OK" : "FAIL") << "\n";
    }

    std::cout << "\nValidation: " << (pass ? "PASS" : "FAIL") << "\n";
    return pass ? 0 : 1;
}
