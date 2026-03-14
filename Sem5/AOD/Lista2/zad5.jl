# Rafał Wochna 279752 
using JuMP
using GLPK

function police_allocation(min_cars, max_cars, min_shift_cars, min_district_cars)
    model = Model(GLPK.Optimizer)

    num_districts, num_shifts = size(min_cars)  # Liczba dzielnic i zmian

    # Zmienne decyzyjne: liczba radiowozów przypisanych do każdej dzielnicy i zmiany
    @variable(model, x[1:num_districts, 1:num_shifts] >= 0, Int)  # x[i, j] - liczba radiowozów w dzielnicy i na zmianie j

    # Funkcja celu: minimalizacja całkowitej liczby radiowozów
    @objective(model, Min, sum(x[i, j] for i in 1:num_districts, j in 1:num_shifts))

    # Ograniczenia: minimalna i maksymalna liczba radiowozów dla każdej dzielnicy i zmiany
    for i in 1:num_districts
        for j in 1:num_shifts
            @constraint(model, x[i, j] >= min_cars[i, j])
            @constraint(model, x[i, j] <= max_cars[i, j])
        end
    end

    # Ograniczenia: minimalna liczba radiowozów na każdą zmianę
    for j in 1:num_shifts
        @constraint(model, sum(x[i, j] for i in 1:num_districts) >= min_shift_cars[j])
    end

    # Ograniczenia: minimalna liczba radiowozów na każdą dzielnicę
    for i in 1:num_districts
        @constraint(model, sum(x[i, j] for j in 1:num_shifts) >= min_district_cars[i])
    end

    # Rozwiązanie modelu
    optimize!(model)

    # Wyświetlenie wyników
    if termination_status(model) == MOI.OPTIMAL
        println("Optymalny przydział radiowozów:")
        for i in 1:num_districts
            for j in 1:num_shifts
                println("Dzielnica p$i, Zmiana $j: ", value(x[i, j]))
            end
        end
        println("Całkowita liczba wykorzystanych radiowozów: ", objective_value(model))
    else
        println("Nie znaleziono optymalnego rozwiązania.")
    end
end

# Dane wejściowe
# min_cars - macierz minimalnych liczb radiowozów dla każdej dzielnicy i zmiany
# max_cars - macierz maksymalnych liczb radiowozów dla każdej dzielnicy i zmiany
# min_shift_cars - tablica minimalnej liczby radiowozów na każdą zmianę
# min_district_cars - tablica minimalnej liczby radiowozów na każdą dzielnicę

min_cars = [
    2 4 3;  
    3 6 5;  
    5 7 6   
]
max_cars = [
    3 7 5;  
    5 7 10; 
    8 12 10 
]
min_shift_cars = [10, 20, 18] 
min_district_cars = [10, 14, 13]  

police_allocation(min_cars, max_cars, min_shift_cars, min_district_cars)
