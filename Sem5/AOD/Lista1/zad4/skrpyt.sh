#!/bin/bash

# Folder z plikami wejściowymi
INPUT_FOLDER="./4"
OUTPUT_FILE="./4/output/all_results.txt"

# Tworzymy folder na wyniki, jeśli nie istnieje
mkdir -p "$(dirname "$OUTPUT_FILE")"

# Czyścimy plik wyjściowy, jeśli istnieje
> "$OUTPUT_FILE"

# Iterujemy po wszystkich plikach w folderze ./4/
for input_file in "$INPUT_FOLDER"/*; do
    # Pobieramy nazwę pliku bez ścieżki
    filename=$(basename -- "$input_file")
    # Dodajemy nagłówek do pliku wyjściowego
    echo "=== Wyniki dla pliku: $filename ===" >> "$OUTPUT_FILE"
    # Uruchamiamy program ./Bipartite i zapisujemy wynik oraz czas działania
    { ./Bipartite < "$input_file"; } 2>> "$OUTPUT_FILE" >> "$OUTPUT_FILE"
    echo "" >> "$OUTPUT_FILE" # Dodajemy pustą linię między wynikami
done

echo "Wszystkie wyniki zostały zapisane w pliku $OUTPUT_FILE."