/**
 * Unit 1 - Topic 2: System Call Dispatch Simulation
 *
 * Simulates the kernel-side system call dispatch table.
 * Models the 9-step call flow from the notes:
 *   1. App calls library wrapper
 *   2. Library loads syscall number into register
 *   3. Library executes trap instruction
 *   4. CPU switches to kernel mode
 *   5. Kernel reads syscall number
 *   6. Kernel looks up dispatch table
 *   7. Handler executes
 *   8. CPU returns to user mode
 *   9. Return value delivered to caller
 *
 * Validation:
 *   - All registered syscalls dispatch to correct handler.
 *   - Unknown syscall number returns ENOSYS error.
 *   - Return value is preserved back to the caller.
 */
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <variant>

constexpr int ERR_NOSYS = -38;  // unimplemented syscall error code

struct SyscallResult {
    int retval;
    std::string handler_name;
};

// Each handler simulates a kernel service.
SyscallResult sys_read(int /*fd*/, size_t len) {
    return {static_cast<int>(len), "sys_read"};
}
SyscallResult sys_write(int /*fd*/, size_t len) {
    return {static_cast<int>(len), "sys_write"};
}
SyscallResult sys_open(const std::string& /*path*/) {
    return {3, "sys_open"};  // returns fake file descriptor 3
}
SyscallResult sys_close(int /*fd*/) {
    return {0, "sys_close"};
}
SyscallResult sys_getpid() {
    return {1234, "sys_getpid"};
}
SyscallResult sys_exit(int code) {
    return {code, "sys_exit"};
}

// Syscall dispatch table (mimics Linux x86-64 numbering)
struct DispatchEntry {
    int number;
    std::string name;
    std::function<SyscallResult()> invoke;
};

std::map<int, DispatchEntry> build_dispatch_table() {
    return {
        {0,  {0,  "read",   []{ return sys_read(1, 64); }}},
        {1,  {1,  "write",  []{ return sys_write(1, 13); }}},
        {2,  {2,  "open",   []{ return sys_open("/etc/passwd"); }}},
        {3,  {3,  "close",  []{ return sys_close(3); }}},
        {39, {39, "getpid", []{ return sys_getpid(); }}},
        {60, {60, "exit",   []{ return sys_exit(0); }}}
    };
}

struct KernelCallResult {
    bool dispatched;
    SyscallResult result;
};

KernelCallResult kernel_dispatch(int syscall_nr,
                                  const std::map<int, DispatchEntry>& table) {
    // Step 4-6: CPU in kernel mode, lookup dispatch table
    auto it = table.find(syscall_nr);
    if (it == table.end()) {
        return {false, {ENOSYS, "unknown_syscall"}};
    }
    // Step 7: execute handler
    SyscallResult r = it->second.invoke();
    return {true, r};
}

int main() {
    auto table = build_dispatch_table();

    std::cout << "System Call Dispatch Simulation\n";
    std::cout << "--------------------------------\n\n";

    struct TestCase {
        int nr;
        std::string desc;
        bool expect_success;
        int expect_retval;
    };

    std::vector<TestCase> tests = {
        {0,   "read(fd=1, len=64)",      true,  64},
        {1,   "write(fd=1, len=13)",     true,  13},
        {2,   "open('/etc/passwd')",     true,   3},
        {3,   "close(fd=3)",             true,   0},
        {39,  "getpid()",                true,  1234},
        {60,  "exit(0)",                 true,   0},
        {999, "unknown_syscall()",       false, ENOSYS},
    };

    bool pass = true;
    for (auto& t : tests) {
        // Steps 1-3: user app → library → trap
        std::cout << "  [trap] syscall " << t.nr << " (" << t.desc << ")\n";

        // Steps 4-8: kernel dispatch
        auto kr = kernel_dispatch(t.nr, table);

        bool ok = (kr.dispatched == t.expect_success) &&
                  (kr.result.retval == t.expect_retval);

        // Step 9: return to user
        std::cout << "    handler=" << kr.result.handler_name
                  << "  retval=" << kr.result.retval
                  << "  -> " << (ok ? "OK" : "MISMATCH") << "\n";

        if (!ok) pass = false;
    }

    std::cout << "\nValidation: " << (pass ? "PASS" : "FAIL") << "\n";
    return pass ? 0 : 1;
}
