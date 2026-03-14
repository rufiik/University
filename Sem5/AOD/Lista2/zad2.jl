# Rafał Wochna 279752 
using JuMP
using GLPK
using Printf

function production_plan(processing_time, max_demand, machine_hours, selling_price, variable_cost, material_cost)
    model = Model(GLPK.Optimizer)

    num_products = length(max_demand)  # Liczba produktów
    num_machines = length(machine_hours)  # Liczba maszyn

    # Zmienne decyzyjne: ilość produkcji każdego produktu
    @variable(model, x[1:num_products] >= 0)  # x[i] - ilość produkcji produktu Pi w kilogramach

    # Funkcja celu: maksymalizacja zysku
    @objective(model, Max, sum((selling_price[i] - material_cost[i]) * x[i] for i in 1:num_products) -
                          sum(variable_cost[j] * sum(processing_time[i][j] * x[i] for i in 1:num_products) / 60 for j in 1:num_machines))

    # Ograniczenia: czas pracy maszyn
    for j in 1:num_machines
        @constraint(model, sum(processing_time[i][j] * x[i] for i in 1:num_products) <= machine_hours[j] * 60)
    end

    # Ograniczenia: maksymalny popyt
    for i in 1:num_products
        @constraint(model, x[i] <= max_demand[i])
    end

    # Rozwiązanie modelu
    optimize!(model)

    # Wyświetlenie wyników
    if termination_status(model) == MOI.OPTIMAL
        println("Optymalny plan produkcji:")
        for i in 1:num_products
            println("Produkt P$i: ", value(x[i]), " kg")
        end
        @printf("Maksymalny zysk: \$%.2f\n", objective_value(model))
    else
        println("Nie znaleziono optymalnego rozwiązania.")
    end
end

# Dane wejściowe
# processing_time - macierz czasów obróbki (w minutach) produktów na maszynach
# max_demand - tablica maksymalnego popytu na produkty
# machine_hours - tablica dostępności maszyn w godzinach
# selling_price - tablica cen sprzedaży za kilogram produktów
# variable_cost - tablica kosztów pracy maszyn w $/godzinę
# material_cost - tablica kosztów materiałów na kilogram produktu

processing_time = [
    [5, 10, 6],  
    [3, 6, 4],   
    [4, 5, 3],  
    [4, 2, 1]    
]
max_demand = [400, 100, 150, 500]  
machine_hours = [60, 60, 60]  
selling_price = [9, 7, 6, 5]  
variable_cost = [2, 2, 3]  
material_cost = [4, 1, 1, 1]  
production_plan(processing_time, max_demand, machine_hours, selling_price, variable_cost, material_cost)