#!/bin/bash

# Plik wynikowy
OUTPUT_FILE="results.csv"

# Liczba powtórzeń
REPEATS=10

# Nagłówek w pliku wynikowym
echo "k,max_flow_edmonds,time_edmonds,augmenting_paths_edmonds,max_flow_dinic,time_dinic,augmenting_paths_dinic" > $OUTPUT_FILE

# Eksperymenty dla każdego k
for k in {1..16}; do
    for ((rep=1; rep<=REPEATS; rep++)); do
        # Uruchomienie programu zad5 i zapis wyników do pliku
        ./comp --size $k >> $OUTPUT_FILE
    done
done

echo "Eksperymenty zakończone. Wyniki zapisano w pliku $OUTPUT_FILE."