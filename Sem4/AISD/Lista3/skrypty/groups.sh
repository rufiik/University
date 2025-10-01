#!/bin/bash

# Liczba powtórzeń
m=50

# Zakres n (od 1000 do 50000, krok 1000)
# for n in {100..50000..100}; do
    # Wartości k
#     k_values=(1 10 $((n / 2)) $((n / 4)) $((3 * n / 4)) $((n - 10)) $n)

#     # Wartości groupSize
#     group_sizes=(3 5 7 9)

#     for groupSize in "${group_sizes[@]}"; do
#         for k in "${k_values[@]}"; do
#             for ((i=1; i<=m; i++)); do
#                 ./DataGenerator $n rand | ./SelectGroups $k dane/results_groups.txt $groupSize
#             done
#         done
#     done
# done

# Zakres n (od 1000 do 100000, krok 1000)
for n in {1000..100000..1000}; do
    # n_values=(10 $((n / 2)) $((n - 10)) -1)
    # Testy dla losowych elementów w tablicy
        for ((i=1; i<=m; i++)); do
            ./DataGenerator $n asc | ./BinarySearch  dane/results_binary_random.txt
        done
done