# Rafal Wochna 279752
include("matrix_utils.jl")
using .MatrixUtils
include("matrix.jl")
using .blocksys
using BenchmarkTools  

function run_test(matrix_file::String, vector_file::Union{String, Nothing}, output_file::String, option::Int)
    println("Wczytywanie macierzy z pliku: $matrix_file")
    mat = read_matrix(matrix_file)
    println("Macierz wczytana pomyślnie.")
    x_exact = nothing

    if vector_file != nothing
        println("Wczytywanie wektora z pliku: $vector_file")
        b = read_vector(vector_file)
        println("Wektor wczytany pomyślnie.")
    else
        println("Generowanie wektora b na podstawie macierzy i x = [1, 1, ..., 1]^T...")
        b = generate_vector(mat)
        x_exact = ones(Float64, mat.n)  
        println("Wektor b wygenerowany pomyślnie.")
    end

    x = nothing  # Rozwiązanie
    LU = nothing  # Rozkład LU
    p = nothing   # Permutacja
    elapsed_time = 0.0  # Czas wykonania
    memory_allocated = 0  # Zmienna do przechowywania informacji o pamięci

    if option == 1
        println("Rozpoczynanie eliminacji Gaussa...")
        memory_allocated = @allocated elapsed_time = @elapsed x = solve_gauss(mat, b)
        println("Eliminacja Gaussa zakończona.")
        write_solution_with_time(output_file, x, x_exact, elapsed_time, memory_allocated)
    elseif option == 2
        println("Rozpoczynanie eliminacji Gaussa z częściowym wyborem...")
        memory_allocated = @allocated elapsed_time = @elapsed x = solve_gauss_partial(mat, b)
        println("Eliminacja Gaussa z częściowym wyborem zakończona.")
        write_solution_with_time(output_file, x, x_exact, elapsed_time, memory_allocated)
    elseif option == 3
        println("Rozpoczynanie rozkładu LU i rozwiązywanie układu...")
        memory_allocated = @allocated elapsed_time = @elapsed x = solve_lu(compute_lu!(mat), b)
        println("Rozkład LU i rozwiązanie układu zakończone.")
        write_solution_with_time(output_file, x, x_exact, elapsed_time, memory_allocated)
    elseif option == 4
        println("Rozpoczynanie rozkładu LU z częściowym wyborem i rozwiązywanie układu...")
        memory_allocated = @allocated elapsed_time = @elapsed begin
            p = compute_lu_partial!(mat)
            x = solve_lu_partial(mat, p, b)
        end
        println("Rozkład LU z częściowym wyborem i rozwiązanie układu zakończone.")
        write_solution_with_time(output_file, x, x_exact, elapsed_time, memory_allocated)
    else
        println("Nieprawidłowa opcja. Wybierz 1, 2, 3 lub 4.")
        return
    end
end


function main()
    if length(ARGS) < 3
        println("Użycie: julia test.jl <plik_macierzy> <plik_wektora|none> <plik_wynikowy> <opcja>")
        println("Opcje:")
        println("1 - Eliminacja Gaussa")
        println("2 - Eliminacja Gaussa z częściowym wyborem")
        println("3 - Rozkład LU i rozwiązanie układu")
        println("4 - Rozkład LU z częściowym wyborem i rozwiązanie układu")
        return
    end

    matrix_file = ARGS[1]
    vector_file = ARGS[2] != "none" ? ARGS[2] : nothing
    output_file = ARGS[3]
    option = parse(Int, ARGS[4])

    run_test(matrix_file, vector_file, output_file, option)
end

main()