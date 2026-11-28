#!/usr/bin/env bash
# Executa lru_main.out para varios k e n = 2^P_MIN ... 2^P_MAX
# Uso: ./run_lru.sh
set -euo pipefail

cd "$(dirname "$0")"

KS=(3 4 5)
P_MIN=10
P_MAX=16

make -s lru_main.out

for k in "${KS[@]}"; do
    for p in $(seq "$P_MIN" "$P_MAX"); do
        ./lru_main.out "$((1 << p))" "$k"
        echo
    done
done
