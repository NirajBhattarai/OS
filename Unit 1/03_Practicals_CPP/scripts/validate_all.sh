#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

echo "============================================"
echo " Unit 1 OS — C++ Validation Suite"
echo "============================================"

echo ""
echo "[1/2] Building all C++ programs..."
make clean >/dev/null 2>&1 || true
make

declare -a progs=(
  "01_os_layers"
  "02_system_calls"
  "03_os_types"
  "04_os_structures"
  "05_virtual_machine"
)

echo ""
echo "[2/2] Running validation suite..."

fail=0
for p in "${progs[@]}"; do
  echo ""
  echo ">>> Running $p"
  if ./bin/$p; then
    echo "[$p] PASS"
  else
    echo "[$p] FAIL"
    fail=1
  fi
done

echo ""
echo "============================================"
if [[ $fail -eq 0 ]]; then
  echo " Overall Result: PASS"
  echo "============================================"
  exit 0
else
  echo " Overall Result: FAIL"
  echo "============================================"
  exit 1
fi
