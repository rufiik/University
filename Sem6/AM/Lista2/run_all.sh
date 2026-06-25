#!/bin/bash

# Ścieżki do plików danych
FILES=( "dane/ca4663.tsp" "dane/eg7146.tsp" "dane/ei8246.tsp" "dane/mu1979.tsp" "dane/tz6117.tsp" )

# Katalog na wyniki
OUTPUT_DIR="wyniki4"
mkdir -p "$OUTPUT_DIR"

# Uruchomienie ./main dla każdego pliku równolegle
echo "Uruchamianie ./main..."
for FILE in "${FILES[@]}"; do
    BASENAME=$(basename "$FILE" .tsp) # Nazwa pliku bez rozszerzenia
    OUTPUT_FILE="$OUTPUT_DIR/${BASENAME}_main.out"
    echo "Przetwarzanie $FILE -> $OUTPUT_FILE"
    ./main "$FILE" > "$OUTPUT_FILE" &
done

# # Uruchomienie ./main2 dla każdego pliku równolegle
# echo "Uruchamianie ./main2..."
# for FILE in "${FILES[@]}"; do
#     BASENAME=$(basename "$FILE" .tsp) # Nazwa pliku bez rozszerzenia
#     OUTPUT_FILE="$OUTPUT_DIR/${BASENAME}_main2.out"
#     echo "Przetwarzanie $FILE -> $OUTPUT_FILE"
#     ./main2 "$FILE" > "$OUTPUT_FILE" &
# done

# Czekaj na zakończenie wszystkich procesów
wait

echo "Wszystkie testy zakończone. Wyniki zapisane w katalogu $OUTPUT_DIR."