/**
 * Unit 1 - Topic 1: OS Layered Architecture
 *
 * Simulates a four-layer OS stack:
 *   Layer 3: User Applications
 *   Layer 2: System Programs  (shell, compilers, utilities)
 *   Layer 1: Kernel           (scheduler, memory, fs, I/O)
 *   Layer 0: Hardware         (CPU, memory, disk)
 *
 * Rules enforced (same as real layered OS):
 *   - Each layer may only call the layer immediately below it.
 *   - Skipping a layer is an illegal access and is rejected.
 *
 * Validation:
 *   - All legitimate top-to-bottom traversals succeed.
 *   - All illegal skip-layer accesses are rejected.
 */
#include <functional>
#include <iostream>
#include <string>
#include <vector>

enum Layer { HARDWARE = 0, KERNEL = 1, SYSTEM_PROGRAMS = 2, USER_APP = 3 };

std::string layer_name(Layer l) {
    switch (l) {
        case HARDWARE: return "Hardware";
        case KERNEL: return "Kernel";
        case SYSTEM_PROGRAMS: return "System Programs";
        case USER_APP: return "User Application";
    }
    return "Unknown";
}

// Returns true if the call from 'caller' to 'callee' is legal.
bool is_legal_call(Layer caller, Layer callee) {
    return static_cast<int>(caller) == static_cast<int>(callee) + 1;
}

struct CallResult {
    bool ok;
    std::string message;
};

CallResult simulate_call(Layer from, Layer to) {
    if (is_legal_call(from, to)) {
        return {true, layer_name(from) + " -> " + layer_name(to) + " : ALLOWED"};
    } else {
        return {false, layer_name(from) + " -> " + layer_name(to) + " : BLOCKED (illegal skip)"};
    }
}

// Full request path: User App -> System Programs -> Kernel -> Hardware
std::vector<std::pair<Layer, Layer>> legal_request = {
    {USER_APP, SYSTEM_PROGRAMS},
    {SYSTEM_PROGRAMS, KERNEL},
    {KERNEL, HARDWARE}
};

// Illegal direct calls that the layered model must block
std::vector<std::pair<Layer, Layer>> illegal_requests = {
    {USER_APP, KERNEL},     // skips System Programs
    {USER_APP, HARDWARE},   // skips two layers
    {SYSTEM_PROGRAMS, HARDWARE} // skips Kernel
};

int main() {
    std::cout << "OS Layered Architecture Simulation\n";
    std::cout << "-----------------------------------\n\n";

    bool pass = true;

    std::cout << "Legal call chain (request flows down one layer at a time):\n";
    for (auto& [from, to] : legal_request) {
        auto r = simulate_call(from, to);
        std::cout << "  " << r.message << "\n";
        if (!r.ok) pass = false;
    }

    std::cout << "\nIllegal call attempts (must all be blocked):\n";
    for (auto& [from, to] : illegal_requests) {
        auto r = simulate_call(from, to);
        std::cout << "  " << r.message << "\n";
        if (r.ok) pass = false; // these MUST fail
    }

    std::cout << "\nResponse returns from Hardware back up to User App:\n";
    for (auto it = legal_request.rbegin(); it != legal_request.rend(); ++it) {
        auto r = simulate_call(it->second, it->first);
        std::cout << "  " << layer_name(it->second) << " -> " << layer_name(it->first)
                  << " : RESPONSE DELIVERED\n";
    }

    std::cout << "\nValidation: " << (pass ? "PASS" : "FAIL") << "\n";
    return pass ? 0 : 1;
}
