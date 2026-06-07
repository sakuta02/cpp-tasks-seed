#!/bin/bash

set -euo pipefail

CHECKS='-*,clang-analyzer-*,performance-*,bugprone-*'
FILES=()

for pattern in *.cpp *.cxx *.cc; do
  if [[ -e "$pattern" ]]; then
    FILES+=("$pattern")
  fi
done

if [[ ${#FILES[@]} -eq 0 ]]; then
  echo "No source files to analyze."
  exit 0
fi

for f in "${FILES[@]}"; do
  echo "Running clang-tidy on $f"
  clang-tidy "$f" -checks="$CHECKS" --quiet -- \
    -std=c++17 \
    -Wall \
    -Wpedantic \
    -I. \
    ${CXX_EXTRA_FLAGS:-}
done
