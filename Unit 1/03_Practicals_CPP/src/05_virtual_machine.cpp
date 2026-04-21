/**
 * Unit 1 - Topic 5: Virtual Machine Isolation
 *
 * Simulates a Type 1 hypervisor managing two virtual machines.
 * Each VM has:
 *   - An isolated address space (simulated as a byte array)
 *   - Its own saved CPU state (program counter + registers)
 *   - A saved snapshot that the hypervisor can restore
 *
 * Demonstrates:
 *   a) Memory isolation: writes inside VM1 are invisible to VM2.
 *   b) CPU state isolation: each VM saves/restores independent PC+registers.
 *   c) Snapshot: hypervisor saves VM state and restores it exactly.
 *
 * Validation:
 *   - VM1 memory write does NOT appear in VM2 address space.
 *   - Snapshot restore produces identical CPU state.
 *   - Each VM's PC advances independently.
 */
#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

constexpr size_t VM_MEMORY_BYTES = 4096; // 4 KB simulated RAM per VM
constexpr size_t NUM_REGS = 8;

struct CPUState {
    uint32_t pc{0};
    std::array<uint32_t, NUM_REGS> regs{};
};

struct VirtualMachine {
    int id;
    std::string os_name;
    std::array<uint8_t, VM_MEMORY_BYTES> memory{};
    CPUState cpu;
    CPUState snapshot_cpu;
    std::array<uint8_t, VM_MEMORY_BYTES> snapshot_memory{};

    void write_memory(size_t offset, uint8_t value) {
        if (offset < VM_MEMORY_BYTES) memory[offset] = value;
    }

    uint8_t read_memory(size_t offset) const {
        if (offset < VM_MEMORY_BYTES) return memory[offset];
        return 0;
    }

    void save_snapshot() {
        snapshot_cpu = cpu;
        snapshot_memory = memory;
    }

    void restore_snapshot() {
        cpu = snapshot_cpu;
        memory = snapshot_memory;
    }
};

// Hypervisor schedules and isolates VMs
struct Hypervisor {
    std::vector<VirtualMachine> vms;

    void create_vm(int id, const std::string& name) {
        VirtualMachine vm;
        vm.id = id;
        vm.os_name = name;
        vm.memory.fill(0);
        vms.push_back(vm);
    }

    VirtualMachine& get(int id) {
        for (auto& v : vms) if (v.id == id) return v;
        throw std::runtime_error("VM not found");
    }

    // Context switch: save current VM CPU state, load next (simulated)
    void context_switch(int from_id, int to_id) {
        auto& from = get(from_id);
        auto& to   = get(to_id);
        from.save_snapshot(); // hypervisor saves
        to.cpu.pc++;          // to-VM resumes where it left off
        std::cout << "  [Hypervisor] context switch VM" << from_id
                  << " -> VM" << to_id << "  (VM" << to_id
                  << " PC=" << to.cpu.pc << ")\n";
    }
};

int main() {
    std::cout << "Virtual Machine Isolation Simulation\n";
    std::cout << "-------------------------------------\n\n";

    Hypervisor hvsr;
    hvsr.create_vm(1, "Linux-Guest");
    hvsr.create_vm(2, "Windows-Guest");

    auto& vm1 = hvsr.get(1);
    auto& vm2 = hvsr.get(2);

    bool pass = true;

    // ── Test 1: Memory isolation ─────────────────────────────────────────────
    std::cout << "Test 1: Memory isolation\n";
    vm1.write_memory(100, 0xAB);
    vm2.write_memory(100, 0x00);

    uint8_t vm1_val = vm1.read_memory(100);
    uint8_t vm2_val = vm2.read_memory(100);

    std::cout << "  VM1 memory[100] = 0x" << std::hex << (int)vm1_val << std::dec << "\n";
    std::cout << "  VM2 memory[100] = 0x" << std::hex << (int)vm2_val << std::dec << "\n";

    if (vm1_val != 0xAB) { std::cout << "  FAIL: VM1 write lost\n"; pass = false; }
    if (vm2_val != 0x00) { std::cout << "  FAIL: VM1 write leaked into VM2\n"; pass = false; }
    if (vm1_val == 0xAB && vm2_val == 0x00)
        std::cout << "  -> Isolated correctly (VM1 write did NOT leak into VM2)\n";

    // ── Test 2: Independent CPU state ─────────────────────────────────────────
    std::cout << "\nTest 2: Independent CPU state\n";
    vm1.cpu.pc = 1000;
    vm1.cpu.regs[0] = 42;
    vm2.cpu.pc = 2000;
    vm2.cpu.regs[0] = 99;

    std::cout << "  VM1 PC=" << vm1.cpu.pc << " R0=" << vm1.cpu.regs[0] << "\n";
    std::cout << "  VM2 PC=" << vm2.cpu.pc << " R0=" << vm2.cpu.regs[0] << "\n";

    if (vm1.cpu.pc == vm2.cpu.pc) { std::cout << "  FAIL: PCs should be independent\n"; pass = false; }
    else std::cout << "  -> CPU states are independent\n";

    // ── Test 3: Snapshot save and restore ────────────────────────────────────
    std::cout << "\nTest 3: Snapshot save and restore\n";
    vm1.cpu.pc = 5000;
    vm1.cpu.regs[1] = 77;
    vm1.save_snapshot();
    std::cout << "  Snapshot saved: VM1 PC=" << vm1.cpu.pc << " R1=" << vm1.cpu.regs[1] << "\n";

    // Simulate VM1 running further
    vm1.cpu.pc = 5020;
    vm1.cpu.regs[1] = 0;
    std::cout << "  After running: VM1 PC=" << vm1.cpu.pc << " R1=" << vm1.cpu.regs[1] << "\n";

    // Restore snapshot
    vm1.restore_snapshot();
    std::cout << "  After restore: VM1 PC=" << vm1.cpu.pc << " R1=" << vm1.cpu.regs[1] << "\n";

    if (vm1.cpu.pc != 5000 || vm1.cpu.regs[1] != 77) {
        std::cout << "  FAIL: Snapshot restore incorrect\n";
        pass = false;
    } else {
        std::cout << "  -> Snapshot restored exactly\n";
    }

    // ── Test 4: Hypervisor context switch ─────────────────────────────────────
    std::cout << "\nTest 4: Hypervisor context switch\n";
    vm1.cpu.pc = 0;
    vm2.cpu.pc = 0;
    hvsr.context_switch(1, 2);
    if (vm2.cpu.pc != 1) {
        std::cout << "  FAIL: VM2 PC should have advanced\n";
        pass = false;
    } else {
        std::cout << "  -> Context switch executed correctly\n";
    }

    std::cout << "\nValidation: " << (pass ? "PASS" : "FAIL") << "\n";
    return pass ? 0 : 1;
}
