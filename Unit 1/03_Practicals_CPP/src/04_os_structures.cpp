/**
 * Unit 1 - Topic 4: OS Kernel Structures — Monolithic vs Microkernel
 *
 * Compares two dispatch models in terms of:
 *   a) Call overhead (direct call vs message passing)
 *   b) Fault isolation (crash in one service vs kernel crash)
 *
 * Monolithic model:
 *   - All services (FS, net, driver) are direct C++ function calls.
 *   - If the FS function throws, it propagates to the entire kernel.
 *
 * Microkernel model:
 *   - Kernel only does IPC routing + process scheduling.
 *   - Services run as separate objects (simulate user-space servers).
 *   - A crashing server is caught; kernel continues running.
 *
 * Validation:
 *   - Monolithic: all normal calls succeed (fast path).
 *   - Microkernel: normal calls succeed via IPC.
 *   - Microkernel: crashing server does NOT crash the kernel.
 *   - Monolithic: a faulty call propagates immediately.
 */
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// ── Monolithic kernel simulation ─────────────────────────────────────────────

namespace monolithic {

struct KernelResult { std::string service; int retval; };

KernelResult fs_open(const std::string& path)  { return {"fs_open",  3}; }
KernelResult net_send(int bytes)               { return {"net_send", bytes}; }
KernelResult drv_read(int n)                   { return {"drv_read", n}; }

KernelResult faulty_service() {
    throw std::runtime_error("bug in monolithic service — whole kernel affected");
}

void run(std::vector<std::string>& log, bool& pass) {
    log.push_back("[Monolithic] fs_open('/etc') -> fd=" +
                  std::to_string(fs_open("/etc").retval) + "  [direct call]");
    log.push_back("[Monolithic] net_send(128)  -> " +
                  std::to_string(net_send(128).retval) + " bytes  [direct call]");
    log.push_back("[Monolithic] drv_read(64)   -> " +
                  std::to_string(drv_read(64).retval) + " bytes  [direct call]");

    // Simulate a bug in a monolithic service
    try {
        faulty_service();
        log.push_back("[Monolithic] faulty_service: NOT caught — kernel crash!");
        pass = false;
    } catch (const std::exception& e) {
        log.push_back(std::string("[Monolithic] faulty_service: EXCEPTION propagates -> ") + e.what());
        // In real monolithic kernel this crashes the OS. We just record it.
    }
}
} // namespace monolithic

// ── Microkernel simulation ────────────────────────────────────────────────────

namespace microkernel {

struct Message { std::string service; std::string payload; };
struct Reply   { bool ok; int retval; std::string from; };

// Simulate user-space servers
class Server {
public:
    std::string name;
    bool crashed{false};

    Reply handle(const Message& m) {
        if (crashed) return {false, -1, name + " [CRASHED]"};
        if (m.service == "fs_open")  return {true, 3, name};
        if (m.service == "net_send") return {true, 128, name};
        if (m.service == "drv_read") return {true, 64, name};
        return {false, -1, name};
    }
};

// Microkernel only does IPC routing
Reply kernel_ipc(Server& srv, const Message& m) {
    // Kernel catches server crash, isolates it
    if (srv.crashed) {
        return {false, -1, "kernel: server '" + srv.name + "' is down, restarting..."};
    }
    return srv.handle(m);
}

void run(std::vector<std::string>& log, bool& pass) {
    Server fs{"FileServer"}, net{"NetServer"}, drv{"DriverServer"};

    auto r1 = kernel_ipc(fs,  {{"fs_open"},  "path=/etc"});
    auto r2 = kernel_ipc(net, {{"net_send"}, "len=128"});
    auto r3 = kernel_ipc(drv, {{"drv_read"}, "n=64"});

    log.push_back("[Microkernel] fs_open  via IPC -> fd=" + std::to_string(r1.retval) +
                  "  server=" + r1.from);
    log.push_back("[Microkernel] net_send via IPC -> " + std::to_string(r2.retval) +
                  " bytes  server=" + r2.from);
    log.push_back("[Microkernel] drv_read via IPC -> " + std::to_string(r3.retval) +
                  " bytes  server=" + r3.from);

    // Simulate server crash — kernel must survive
    net.crashed = true;
    auto r4 = kernel_ipc(net, {{"net_send"}, "len=64"});
    log.push_back("[Microkernel] net_send after crash -> " + r4.from +
                  (r4.ok ? "  [unexpected: still OK]" : "  [kernel isolated crash — CORRECT]"));

    // Kernel itself still works after server crash
    auto r5 = kernel_ipc(fs, {{"fs_open"}, "path=/var"});
    bool kernel_survived = r5.ok;
    log.push_back("[Microkernel] fs_open after net crash -> fd=" + std::to_string(r5.retval) +
                  (kernel_survived ? "  [kernel alive — CORRECT]" : "  [FAIL]"));

    if (!kernel_survived) pass = false;
    if (r4.ok) pass = false; // crashed server should NOT return OK
}
} // namespace microkernel

// ── Main ─────────────────────────────────────────────────────────────────────

int main() {
    std::cout << "OS Structures: Monolithic vs Microkernel\n";
    std::cout << "----------------------------------------\n\n";

    bool pass = true;
    std::vector<std::string> mono_log, micro_log;

    std::cout << "Monolithic Kernel:\n";
    monolithic::run(mono_log, pass);
    for (auto& l : mono_log) std::cout << "  " << l << "\n";

    std::cout << "\nMicrokernel:\n";
    microkernel::run(micro_log, pass);
    for (auto& l : micro_log) std::cout << "  " << l << "\n";

    std::cout << "\nKey difference:\n";
    std::cout << "  Monolithic: fast direct calls, but a bug in any service is catastrophic.\n";
    std::cout << "  Microkernel: IPC overhead, but a crashed server leaves the kernel running.\n";

    std::cout << "\nValidation: " << (pass ? "PASS" : "FAIL") << "\n";
    return pass ? 0 : 1;
}
