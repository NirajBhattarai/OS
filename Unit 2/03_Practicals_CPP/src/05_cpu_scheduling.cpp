#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Proc {
    std::string name;
    int arrival;
    int burst;
    int priority;
};

struct Metrics {
    std::string algo;
    std::vector<std::string> order;
    double avg_wait;
    double avg_turnaround;
};

double average(const std::vector<int>& values) {
    double sum = 0;
    for (int v : values) sum += v;
    return values.empty() ? 0.0 : sum / values.size();
}

Metrics fcfs(const std::vector<Proc>& input) {
    auto p = input;
    std::sort(p.begin(), p.end(), [](const Proc& a, const Proc& b) {
        return a.arrival < b.arrival;
    });

    int time = 0;
    std::vector<int> wait, tat;
    std::vector<std::string> order;

    for (const auto& pr : p) {
        time = std::max(time, pr.arrival);
        wait.push_back(time - pr.arrival);
        order.push_back(pr.name);
        time += pr.burst;
        tat.push_back(time - pr.arrival);
    }

    return {"FCFS", order, average(wait), average(tat)};
}

Metrics sjf_non_preemptive(const std::vector<Proc>& input) {
    int n = static_cast<int>(input.size());
    std::vector<bool> done(n, false);
    int completed = 0;
    int time = 0;
    std::vector<int> wait, tat;
    std::vector<std::string> order;

    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; ++i) {
            if (!done[i] && input[i].arrival <= time) {
                if (idx == -1 || input[i].burst < input[idx].burst) idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        done[idx] = true;
        completed++;
        wait.push_back(time - input[idx].arrival);
        order.push_back(input[idx].name);
        time += input[idx].burst;
        tat.push_back(time - input[idx].arrival);
    }

    return {"SJF", order, average(wait), average(tat)};
}

Metrics priority_non_preemptive(const std::vector<Proc>& input) {
    int n = static_cast<int>(input.size());
    std::vector<bool> done(n, false);
    int completed = 0;
    int time = 0;
    std::vector<int> wait, tat;
    std::vector<std::string> order;

    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; ++i) {
            if (!done[i] && input[i].arrival <= time) {
                if (idx == -1 || input[i].priority < input[idx].priority) idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        done[idx] = true;
        completed++;
        wait.push_back(time - input[idx].arrival);
        order.push_back(input[idx].name);
        time += input[idx].burst;
        tat.push_back(time - input[idx].arrival);
    }

    return {"Priority", order, average(wait), average(tat)};
}

Metrics round_robin(const std::vector<Proc>& input, int quantum) {
    int n = static_cast<int>(input.size());
    std::vector<int> remaining(n);
    std::vector<int> completion(n, 0);
    std::vector<int> arrived(n, 0);

    for (int i = 0; i < n; ++i) remaining[i] = input[i].burst;

    int time = 0;
    int completed = 0;
    std::queue<int> q;
    std::vector<std::string> order;

    auto enqueue_new = [&]() {
        for (int i = 0; i < n; ++i) {
            if (!arrived[i] && input[i].arrival <= time) {
                q.push(i);
                arrived[i] = 1;
            }
        }
    };

    enqueue_new();

    while (completed < n) {
        if (q.empty()) {
            time++;
            enqueue_new();
            continue;
        }

        int idx = q.front();
        q.pop();

        int run = std::min(quantum, remaining[idx]);
        order.push_back(input[idx].name);
        time += run;
        remaining[idx] -= run;

        enqueue_new();

        if (remaining[idx] > 0) {
            q.push(idx);
        } else {
            completion[idx] = time;
            completed++;
        }
    }

    std::vector<int> wait, tat;
    for (int i = 0; i < n; ++i) {
        int turnaround = completion[i] - input[i].arrival;
        int waiting = turnaround - input[i].burst;
        tat.push_back(turnaround);
        wait.push_back(waiting);
    }

    return {"RoundRobin", order, average(wait), average(tat)};
}

void print_metrics(const Metrics& m) {
    std::cout << std::left << std::setw(12) << m.algo
              << " avg_wait=" << std::setw(6) << std::fixed << std::setprecision(2) << m.avg_wait
              << " avg_tat=" << std::setw(6) << std::fixed << std::setprecision(2) << m.avg_turnaround
              << " order=";
    for (size_t i = 0; i < m.order.size(); ++i) {
        std::cout << m.order[i] << (i + 1 < m.order.size() ? "->" : "");
    }
    std::cout << "\n";
}

int main() {
    std::vector<Proc> procs = {
        {"P1", 0, 6, 2},
        {"P2", 0, 2, 1},
        {"P3", 0, 8, 4},
        {"P4", 0, 3, 3}
    };

    Metrics m_fcfs = fcfs(procs);
    Metrics m_sjf = sjf_non_preemptive(procs);
    Metrics m_pri = priority_non_preemptive(procs);
    Metrics m_rr = round_robin(procs, 2);

    std::cout << "CPU Scheduling Comparison\n";
    std::cout << "-------------------------\n";
    print_metrics(m_fcfs);
    print_metrics(m_sjf);
    print_metrics(m_pri);
    print_metrics(m_rr);

    bool pass = true;
    pass = pass && !m_fcfs.order.empty() && (m_fcfs.order.front() == "P1");
    pass = pass && !m_sjf.order.empty() && (m_sjf.order.front() == "P2");
    pass = pass && !m_pri.order.empty() && (m_pri.order.front() == "P2");
    pass = pass && (m_sjf.avg_wait <= m_fcfs.avg_wait);

    std::cout << "Validation: " << (pass ? "PASS" : "FAIL") << "\n";
    return pass ? 0 : 1;
}
