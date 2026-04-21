#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

enum class State { New, Ready, Running, Waiting, Terminated };

std::string to_string(State s) {
    switch (s) {
        case State::New: return "New";
        case State::Ready: return "Ready";
        case State::Running: return "Running";
        case State::Waiting: return "Waiting";
        case State::Terminated: return "Terminated";
    }
    return "Unknown";
}

bool is_allowed_transition(State from, State to) {
    static const std::map<State, std::set<State>> allowed = {
        {State::New, {State::Ready}},
        {State::Ready, {State::Running}},
        {State::Running, {State::Ready, State::Waiting, State::Terminated}},
        {State::Waiting, {State::Ready}},
        {State::Terminated, {}}
    };

    auto it = allowed.find(from);
    if (it == allowed.end()) return false;
    return it->second.count(to) > 0;
}

int main() {
    std::vector<State> timeline = {
        State::New,
        State::Ready,
        State::Running,
        State::Waiting,
        State::Ready,
        State::Running,
        State::Terminated
    };

    std::cout << "Process State Simulation\n";
    std::cout << "------------------------\n";

    bool pass = true;
    for (size_t i = 0; i + 1 < timeline.size(); ++i) {
        State from = timeline[i];
        State to = timeline[i + 1];
        bool ok = is_allowed_transition(from, to);
        std::cout << to_string(from) << " -> " << to_string(to)
                  << " : " << (ok ? "OK" : "INVALID") << "\n";
        if (!ok) pass = false;
    }

    std::cout << "\nValidation: " << (pass ? "PASS" : "FAIL") << "\n";
    return pass ? 0 : 1;
}
