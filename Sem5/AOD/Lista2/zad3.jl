# Rafał Wochna 279752 
using JuMP
using GLPK
using Printf

function production_storage_plan(cost_production, max_extra_production, cost_extra_production, demand, storage_cost, initial_storage, max_storage)
    model = Model(GLPK.Optimizer)

    num_periods = length(demand)  # Liczba okresów

    # Zmienne decyzyjne
    @variable(model, x[1:num_periods] >= 0)  # x[j] - produkcja normalna w okresie j
    @variable(model, y[1:num_periods] >= 0)  # y[j] - produkcja ponadwymiarowa w okresie j
    @variable(model, s[1:num_periods] >= 0)  # s[j] - magazynowanie w okresie j

    # Funkcja celu: minimalizacja kosztów
    @objective(model, Min, sum(cost_production[j] * x[j] + cost_extra_production[j] * y[j] + storage_cost * s[j] for j in 1:num_periods))

    # Ograniczenia: spełnienie zapotrzebowania w każdym okresie
    for j in 1:num_periods
        if j == 1
            @constraint(model, x[j] + y[j] + initial_storage - s[j] == demand[j])
        else
            @constraint(model, x[j] + y[j] + s[j-1] - s[j] == demand[j])
        end
    end

    # Ograniczenia: maksymalna produkcja normalna i ponadwymiarowa
    for j in 1:num_periods
        @constraint(model, x[j] <= 100)  # Maksymalna produkcja normalna
        @constraint(model, y[j] <= max_extra_production[j])  # Maksymalna produkcja ponadwymiarowa
    end

    # Ograniczenia: maksymalna pojemność magazynu
    for j in 1:num_periods
        @constraint(model, s[j] <= max_storage)
    end

    optimize!(model)

    # Wyświetlenie wyników
    if termination_status(model) == MOI.OPTIMAL
        println("Optymalny plan produkcji i magazynowania:")
        for j in 1:num_periods
            println("Okres $j: Produkcja normalna = ", value(x[j]), ", Produkcja ponadwymiarowa = ", value(y[j]), ", Magazynowanie = ", value(s[j]))
        end
        @printf("Minimalny koszt: \$%.2f\n", objective_value(model))
    else
        println("Nie znaleziono optymalnego rozwiązania.")
    end
end

# Dane wejściowe
# demand - tablica zapotrzebowania w każdym okresie
# cost_production - tablica kosztów produkcji normalnej w każdym okresie
# max_extra_production - tablica maksymalnej produkcji ponadwymiarowej w każdym okresie
# cost_extra_production - tablica kosztów produkcji ponadwymiarowej w każdym okresie
# storage_cost - koszt magazynowania za jednostkę
# initial_storage - początkowy stan magazynu
# max_storage - maksymalna pojemność magazynu

demand = [130, 80, 125, 195]  
cost_production = [6000, 4000, 8000, 9000] 
max_extra_production = [60, 65, 70, 60] 
cost_extra_production = [8000, 6000, 10000, 11000] 
storage_cost = 1500 
initial_storage = 15 
max_storage = 70 

production_storage_plan(cost_production, max_extra_production, cost_extra_production, demand, storage_cost, initial_storage, max_storage)
