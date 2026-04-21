CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
SRC_DIR  = src
BIN_DIR  = bin

TARGET_NAMES = \
	01_os_layers \
	02_system_calls \
	03_os_types \
	04_os_structures \
	05_virtual_machine

TARGETS = $(addprefix $(BIN_DIR)/,$(TARGET_NAMES))

all: $(TARGETS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

run: all
	@for t in $(TARGET_NAMES); do \
		echo ""; \
		echo ">>> Running $$t"; \
		./$(BIN_DIR)/$$t; \
	done

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean run
