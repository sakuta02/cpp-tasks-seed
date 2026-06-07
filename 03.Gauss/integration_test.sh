#!/bin/bash

set -euo pipefail

tmpdir="$(mktemp -d)"
trap 'rm -rf "$tmpdir"' EXIT

cat >"$tmpdir/small.csv" <<'EOF'
a,b,c
2,1,5
4,-6,-2
EOF

cat >"$tmpdir/small.expected.csv" <<'EOF'
x
1.750000
1.500000
EOF

python3 - "$tmpdir" <<'PY'
import pathlib
import random
import sys

outdir = pathlib.Path(sys.argv[1])
rng = random.Random(37945656)
n = 12
solution = [float(i + 1) for i in range(n)]
matrix = [[0.0] * n for _ in range(n)]

for i in range(n):
    matrix[i][i] = float(2 ** (i % 6))

for i in range(1, n):
    for j in range(i):
        factor = float(rng.randint(-3, 3))
        if factor == 0.0:
            continue
        for k in range(n):
            matrix[i][k] += factor * matrix[j][k]

rhs = []
for row in matrix:
    rhs.append(sum(a * x for a, x in zip(row, solution)))

with (outdir / "large.csv").open("w", encoding="utf-8") as f:
    header = ",".join(f"a{i}" for i in range(n)) + ",b\n"
    f.write(header)
    for row, b in zip(matrix, rhs):
        f.write(",".join(f"{value:.0f}" for value in row))
        f.write(f",{b:.0f}\n")

with (outdir / "large.expected.csv").open("w", encoding="utf-8") as f:
    f.write("x\n")
    for value in solution:
        f.write(f"{value:.6f}\n")
PY

./gauss "$tmpdir/small.csv" >"$tmpdir/small.actual.csv"
./gauss "$tmpdir/large.csv" >"$tmpdir/large.actual.csv"

cmp "$tmpdir/small.expected.csv" "$tmpdir/small.actual.csv"
cmp "$tmpdir/large.expected.csv" "$tmpdir/large.actual.csv"

echo "Integration tests passed."
