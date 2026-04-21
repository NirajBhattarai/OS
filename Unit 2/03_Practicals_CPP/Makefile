CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread -Wall -Wextra
SRC_DIR = src
BIN_DIR = bin

TARGET_NAMES = \
	01_process_states \
	02_pcb_context_switch \
	03_threads_race_and_mutex \
	04_ipc_bounded_buffer \
	05_cpu_scheduling

TARGETS = $(addprefix $(BIN_DIR)/,$(TARGET_NAMES))

all: $(TARGETS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

run: all
	@for t in $(TARGET_NAMES); do \
		echo "\n>>> Running $$t"; \
		./$(BIN_DIR)/$$t; \
	done

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean run
