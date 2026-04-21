#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

echo "[1/2] Building all C++ programs..."
make clean >/dev/null 2>&1 || true
make

declare -a progs=(
  "01_process_states"
  "02_pcb_context_switch"
  "03_threads_race_and_mutex"
  "04_ipc_bounded_buffer"
  "05_cpu_scheduling"
)

echo "[2/2] Running validation suite..."

fail=0
for p in "${progs[@]}"; do
  echo "\n>>> Running $p"
  if ./bin/$p; then
    echo "[$p] PASS"
  else
    echo "[$p] FAIL"
    fail=1
  fi
done

if [[ $fail -eq 0 ]]; then
  echo "\nOverall Result: PASS"
  exit 0
else
  echo "\nOverall Result: FAIL"
  exit 1
fi
