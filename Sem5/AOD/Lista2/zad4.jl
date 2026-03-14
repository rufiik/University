# Rafał Wochna 279752 
using JuMP
using GLPK

function shortest_path_with_time_constraint(nodes, edges, start_node, end_node, max_time)
    model = Model(GLPK.Optimizer)

    num_nodes = length(nodes)  # Liczba wierzchołków

    # Zmienne decyzyjne: czy krawędź (i, j) jest częścią ścieżki
    @variable(model, z[1:size(edges, 1)], Bin)  # z[k] = 1, jeśli krawędź k jest w ścieżce, 0 w przeciwnym razie

    # Funkcja celu: minimalizacja całkowitego kosztu
    @objective(model, Min, sum(edges[k, 3] * z[k] for k in 1:size(edges, 1)))

    # Ograniczenie: całkowity czas przejazdu nie może przekroczyć max_time
    @constraint(model, sum(edges[k, 4] * z[k] for k in 1:size(edges, 1)) <= max_time)

    # Ograniczenia przepływu w wierzchołkach
    for i in nodes
        if i == start_node
            @constraint(model, sum(z[k] for k in 1:size(edges, 1) if edges[k, 1] == i) -
                                sum(z[k] for k in 1:size(edges, 1) if edges[k, 2] == i) == 1)
        elseif i == end_node
            @constraint(model, sum(z[k] for k in 1:size(edges, 1) if edges[k, 1] == i) -
                                sum(z[k] for k in 1:size(edges, 1) if edges[k, 2] == i) == -1)
        else
            @constraint(model, sum(z[k] for k in 1:size(edges, 1) if edges[k, 1] == i) -
                                sum(z[k] for k in 1:size(edges, 1) if edges[k, 2] == i) == 0)
        end
    end

    # Rozwiązanie modelu
    optimize!(model)

    # Wyświetlenie wyników
    if termination_status(model) == MOI.OPTIMAL
        println("Optymalna ścieżka:")
        for k in 1:size(edges, 1)
            if value(z[k]) > 0.5
                println("Krawędź: (", edges[k, 1], ", ", edges[k, 2], ") z kosztem ", edges[k, 3], " i czasem ", edges[k, 4])
            end
        end
        println("Minimalny koszt: ", objective_value(model))
    else
        println("Nie znaleziono optymalnego rozwiązania.")
    end
end

# Dane wejściowe
nodes = 1:10  # Wierzchołki grafu
edges = [
    1 2 3 4;
    1 3 4 9;
    1 4 7 10;
    1 5 8 12;
    2 3 2 3;
    3 4 4 6;
    3 5 2 2;
    3 10 6 11;
    4 5 1 1;
    4 7 3 5;
    5 6 5 6;
    5 7 3 3;
    5 10 5 8;
    6 1 5 8;
    6 7 2 2;
    6 10 7 11;
    7 3 4 6;
    7 8 3 5;
    7 9 1 1;
    8 9 1 2;
    9 10 2 2
]  # Krawędzie w formacie (i, j, koszt, czas)
start_node = 1  # Wierzchołek początkowy
end_node = 10  # Wierzchołek końcowy
max_time = 15  # Maksymalny czas przejazdu

# Wywołanie funkcji
shortest_path_with_time_constraint(nodes, edges, start_node, end_node, max_time)

# Mój własny egzemplarz problemu z 10 wierzchołkami
nodes = 1:10  # Wierzchołki grafu

# Krawędzie w formacie (i, j, koszt, czas)
edges = [
    1 2 4 2;
    2 3 5 3;
    3 4 6 2;
    4 10 8 3;
    1 5 3 1;
    5 6 4 2;
    6 10 5 3;
    1 7 1 10;
    7 8 3 2;
    8 9 2 3;
    9 10 4 2
]

start_node = 1  # Wierzchołek początkowy
end_node = 10   # Wierzchołek końcowy
max_time = 10   # Maksymalny czas przejazdu

# Wywołanie funkcji
shortest_path_with_time_constraint(nodes, edges, start_node, end_node, max_time)


