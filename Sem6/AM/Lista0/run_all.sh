#!/bin/bash

for file in ./dane/*.tsp; do
    echo "Przetwarzam $file"
    ./Prim "$file"
done

for file in ./dane/*.tsp; do
    echo "Przetwarzam $file"
    ./randomTSP "$file"
done