# #!/bin/bash
# # Liczba powtórzeń
for k in 1 10 100; do
# Eksperymenty dla n ∈ {1000, 2000, ..., 50000} 
    for n in {100..10000..100}; do
        for ((i=1; i<=k; i++)); do
            # ./DataGenerator $n rand | ./QuickSort wyniki/results_quickRandom_k${k}.txt
            # ./DataGenerator $n asc | ./QuickSort wyniki/results_quickAsc_k${k}.txt
            # ./DataGenerator $n desc | ./QuickSort wyniki/results_quickDesc_k${k}.txt

            # ./DataGenerator $n rand | ./DualPivotQS wyniki/results_dualPivotQSRandom_k${k}.txt
            # ./DataGenerator $n asc | ./DualPivotQS wyniki/results_dualPivotQSAsc_k${k}.txt
            # ./DataGenerator $n desc | ./DualPivotQS wyniki/results_dualPivotQSDesc_k${k}.txt

            # ./DataGenerator $n rand | ./DualPivotQSold wyniki/results_dualPivotQSoldRandom_k${k}.txt
            # ./DataGenerator $n asc | ./DualPivotQSold wyniki/results_dualPivotQSoldAsc_k${k}.txt
            # ./DataGenerator $n desc | ./DualPivotQSold wyniki/results_dualPivotQSoldDesc_k${k}.txt

            # ./DataGenerator $n rand | ./QuickSortold wyniki/results_quickoldRandom_k${k}.txt
            ./DataGenerator $n desc | ./QuickSortold wyniki2/results_quickoldDesc_k${k}.txt
            # ./DataGenerator $n desc | ./QuickSortold wyniki/results_quickoldDesc_k${k}.txt
        done
    done
done