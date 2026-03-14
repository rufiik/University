# Rafał Wochna 279752 
using JuMP
using GLPK
using Printf

function fuel_allocation(supply, demand, cost)
    model = Model(GLPK.Optimizer)

    num_airports = length(demand)  # Liczba lotnisk
    num_suppliers = length(supply)  # Liczba dostawców

    # Zmienne decyzyjne: ilość paliwa dostarczanego przez dostawcę i na lotnisko
    @variable(model, x[1:num_airports, 1:num_suppliers] >= 0)  # x[i, j] - ilość paliwa z firmy j na lotnisko i

    # Funkcja celu: minimalizacja kosztów
    @objective(model, Min, sum(cost[i][j] * x[i, j] for i in 1:num_airports, j in 1:num_suppliers))

    # Ograniczenia: zapotrzebowanie na lotniskach
    for i in 1:num_airports
        @constraint(model, sum(x[i, j] for j in 1:num_suppliers) == demand[i])
    end
    # Ograniczenia: dostępność paliwa u dostawców
    for j in 1:num_suppliers
        @constraint(model, sum(x[i, j] for i in 1:num_airports) <= supply[j])
    end

    optimize!(model)

    # Wyświetlenie wyników
    if termination_status(model) == MOI.OPTIMAL
        println("Optymalne rozwiązanie:")
        for i in 1:num_airports
            for j in 1:num_suppliers
                println("Lotnisko $i, Firma $j: ", value(x[i, j]), " galonów")
            end
        end
        @printf("Minimalny koszt: \$%.0f\n", objective_value(model))
    else
        println("Nie znaleziono optymalnego rozwiązania.")
    end
end


# supply - tablica dostępności paliwa u dostawców
# demand - tablica zapotrzebowania na lotniskach
# cost - macierz kosztów dostarczenia paliwa z dostawcy na lotnisko

supply = [275000, 550000, 660000] 
demand = [110000, 220000, 330000, 440000]  
cost = [
    [10, 7, 8],    
    [10, 11, 14],  
    [9, 12, 4],   
    [11, 13, 9]  
    ]
fuel_allocation(supply, demand, cost)