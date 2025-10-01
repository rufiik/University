#!/bin/bash

# Liczba powtórzeń
m=50

# Wartości k


# Zakres n (od 1000 do 50000, krok 1000)
for n in {100..50000..100}; do
   k_values=(1 10 $((n / 2)) $((n / 4)) $((3 * n / 4)) $((n - 10)) $n)
    # Dla każdej wartości k
    for k in "${k_values[@]}"; do
        # Powtórz m razy
        for ((i=1; i<=m; i++)); do
            # Uruchom Select i zapisz wyniki
            ./DataGenerator $n rand |./Select $k dane/results_selectRand.txt
            # ./DataGenerator $n asc |./Select $k dane/results_selectAsc.txt
            # ./DataGenerator $n desc |./Select $k dane/results_selectDesc.txt
             
            # Uruchom RandomSelect i zapisz wyniki
            ./DataGenerator $n rand |./RandomSelect $k dane/results_randomSelectRand.txt
            # ./DataGenerator $n asc |./RandomSelect $k dane/results_randomSelectAsc.txt
            # ./DataGenerator $n desc |./RandomSelect $k dane/results_randomSelectDesc.txt
        done
    done
done

echo "Testy zakończone."