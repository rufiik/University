# Rafał Wochna 279752 
using JuMP
using GLPK

function container_monitoring(m, n, containers, k)
    model = Model(GLPK.Optimizer)

    # Zmienne decyzyjne: czy w kwadracie (i, j) znajduje się kamera
    @variable(model, x[1:m, 1:n], Bin)  # x[i, j] = 1, jeśli w kwadracie (i, j) jest kamera, 0 w przeciwnym razie

    # Funkcja celu: minimalizacja liczby kamer
    @objective(model, Min, sum(x[i, j] for i in 1:m, j in 1:n))

    # Ograniczenie: kamery nie mogą być umieszczone w kwadratach zajmowanych przez kontenery
    for (i, j) in containers
        @constraint(model, x[i, j] == 0)
    end

  # Ograniczenie: każdy kontener musi być monitorowany przez co najmniej jedną kamerę
for (ci, cj) in containers
    @constraint(model, 
        sum(x[i, cj] for i in max(1, ci-k):min(m, ci+k)) +  # Pionowy zasięg kamery
        sum(x[ci, j] for j in max(1, cj-k):min(n, cj+k)) -  # Poziomy zasięg kamery
        x[ci, cj] >= 1)  # Unikamy podwójnego liczenia kamery w miejscu kontenera
end

    # Rozwiązanie modelu
    optimize!(model)

    # Wyświetlenie wyników
    if termination_status(model) == MOI.OPTIMAL
        println("Optymalne rozmieszczenie kamer:")
        for i in 1:m
            for j in 1:n
                if value(x[i, j]) > 0.5
                    println("Kamera w kwadracie (", i, ", ", j, ")")
                end
            end
        end
        println("Minimalna liczba kamer: ", objective_value(model))
    else
        println("Nie znaleziono optymalnego rozwiązania.")
    end
end
# Dane wejściowe
# m - liczba wierszy planszy
# n - liczba kolumn planszy
# containers - lista pozycji kontenerów (wiersz, kolumna)
# k - zasięg kamery w każdą stronę
# 1 przyklad
m = 10  
n = 10  
containers = [(1, 2), (3, 5), (5, 7), (7, 3), (9, 9), (2, 8), (6, 4), (8, 6), (4, 10), (10, 1)] 
k = 1 
container_monitoring(m, n, containers, k)
# 2 przyklad
m = 10  
n = 10  
containers = [(1, 2), (3, 5), (5, 7), (7, 3), (9, 9), (2, 8), (6, 4), (8, 6), (4, 10), (10, 1)]  
k = 2 
container_monitoring(m, n, containers, k)