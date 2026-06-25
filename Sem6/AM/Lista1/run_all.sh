#!/bin/bash

DATA_DIR="dane"
RESULTS_DIR="wyniki"

mkdir -p "$RESULTS_DIR"

# KLUCZOWE: Ograniczamy każdy program do 1 wątku!
export OMP_NUM_THREADS=1

echo "Uruchamianie wszystkich instancji w tle..."

# Uruchom main dla wszystkich plików z algorytmem 1 (klasyczny)
for file in "$DATA_DIR"/*.tsp; do
    fname=$(basename "$file")
    echo "== Start: main (alg 1) dla $fname =="
    ./main "$file" 1 > "$RESULTS_DIR/log_main1_${fname}.txt" 2>&1 &
done

# Uruchom main dla wszystkich plików z algorytmem 2 (losowe sąsiedztwo)
for file in "$DATA_DIR"/*.tsp; do
    fname=$(basename "$file")
    echo "== Start: main (alg 2) dla $fname =="
    ./main "$file" 2 > "$RESULTS_DIR/log_main2_${fname}.txt" 2>&1 &
done

# Uruchom prim dla wszystkich plików
for file in "$DATA_DIR"/*.tsp; do
    fname=$(basename "$file")
    echo "== Start: prim dla $fname =="
    ./prim "$file" > "$RESULTS_DIR/log_prim_${fname}.txt" 2>&1 &
done

echo "Wszystkie programy (Alg 1, Alg 2 i Prim) pracują w tle. Czekam na zakończenie..."
wait
echo "GOTOWE! Wszystkie zadania zakończone."