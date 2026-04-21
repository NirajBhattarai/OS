#include <array>
#include <iostream>
#include <string>

struct CPUContext {
    int pc;
    std::array<int, 4> regs;
};

struct PCB {
    int pid;
    std::string state;
    CPUContext snapshot;
};

void context_switch(CPUContext& cpu, PCB& current, PCB& next) {
    current.snapshot = cpu;
    current.state = "Ready";

    cpu = next.snapshot;
    next.state = "Running";
}

int main() {
    PCB p1{1, "Running", {100, {1, 2, 3, 4}}};
    PCB p2{2, "Ready", {220, {9, 8, 7, 6}}};

    CPUContext cpu = p1.snapshot;

    cpu.pc += 5;
    cpu.regs[0] = 42;

    context_switch(cpu, p1, p2);

    bool pass = true;
    pass = pass && (p1.snapshot.pc == 105);
    pass = pass && (p1.snapshot.regs[0] == 42);
    pass = pass && (p1.state == "Ready");
    pass = pass && (cpu.pc == 220);
    pass = pass && (cpu.regs[0] == 9);
    pass = pass && (p2.state == "Running");

    std::cout << "PCB and Context Switch Simulation\n";
    std::cout << "---------------------------------\n";
    std::cout << "Saved P1 PC: " << p1.snapshot.pc << "\n";
    std::cout << "Loaded CPU PC from P2: " << cpu.pc << "\n";
    std::cout << "Validation: " << (pass ? "PASS" : "FAIL") << "\n";

    return pass ? 0 : 1;
}
