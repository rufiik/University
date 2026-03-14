# Rafal Wochna 279752
include("matrix_utils.jl")
using .MatrixUtils
include("matrix.jl")
using .blocksys


function main()
    println("=== Program do macierzy ===")
    println("Wybierz operację:")
    println("1. Wczytaj macierz z pliku")
    println("2. Wczytaj wektor z pliku")
    println("3. Wygeneruj wektor b = A*x (x = [1, 1, ..., 1]^T)")
    println("4. Rozwiąż układ równań metodą eliminacji Gaussa")
    println("5. Rozwiąż układ równań metodą eliminacji Gaussa z częściowym wyborem elementu głównego")
    println("6. Rozkład LU")
    println("7. Rozkład LU z częściowym wyborem elementu głównego")
    println("8. Rozwiąż układ równań za pomocą rozkładu LU bez częściowego wyboru")
    println("9. Rozwiąż układ równań za pomocą rozkładu LU z częściowym wyborem")
    println("10. Zapisz rozwiązanie do pliku (z opcjonalnym błędem względnym)")
    println("0. Wyjdź")

    mat = nothing
    b = nothing
    x = nothing
    x_exact = nothing 
    LU = nothing
    p = nothing

    while true
        print("\nWybierz opcję: ")
        choice = readline()

        if choice == "1"
            print("Podaj nazwę pliku z macierzą: ")
            filename = readline()
            mat = read_matrix(filename)
            println("Macierz wczytana pomyślnie.")

        elseif choice == "2"
            print("Podaj nazwę pliku z wektorem: ")
            filename = readline()
            b = read_vector(filename)
            b_generated = false  # Wektor b został wczytany, a nie wygenerowany
            println("Wektor wczytany pomyślnie.")

        elseif choice == "3"
            if mat === nothing
                println("Najpierw wczytaj macierz (opcja 1).")
            else
                b = generate_vector(mat)
                x_exact = ones(Float64, mat.n)  
                println("Wektor b wygenerowany pomyślnie.")
                # println("Wektor b: ", b)
            end

        elseif choice == "4"
            if mat === nothing || b === nothing
                println("Najpierw wczytaj macierz i wektor (opcje 1 i 2).")
            else
                x = solve_gauss(mat, b)
                println("Układ rozwiązany metodą eliminacji Gaussa.")
                # println("Rozwiązanie: ", x)
                mat = nothing
                b = nothing
            end

        elseif choice == "5"
            if mat === nothing || b === nothing
                println("Najpierw wczytaj macierz i wektor (opcje 1 i 2).")
            else
                x = solve_gauss_partial(mat, b)
                println("Układ rozwiązany metodą eliminacji Gaussa z częściowym wyborem.")
                # println("Rozwiązanie: ", x)
                mat = nothing
                b = nothing
            end

        elseif choice == "6"
            if mat === nothing
                println("Najpierw wczytaj macierz (opcja 1).")
            else
                compute_lu!(mat)
                println("Rozkład LU został wygenerowany.")
                LU = mat
                mat = nothing
                p = nothing
            end
        elseif choice == "7"
            if mat === nothing
                println("Najpierw wczytaj macierz (opcja 1).")
            else
                p = compute_lu_partial!(mat)
                println("Rozkład LU z częściowym wyborem został wygenerowany.")
                LU = mat
                mat = nothing
            end
        elseif choice == "8"
            if LU === nothing || b === nothing
                println("Najpierw wygeneruj rozkład LU i wczytaj wektor b (opcje 6 i 2).")
            else
                x = solve_lu(LU, b)
                println("Układ rozwiązany za pomocą rozkładu LU.")
                # println("Rozwiązanie: ", x)
                LU = nothing
                b = nothing
            end
  
        elseif choice == "9"
            if LU === nothing || b === nothing || p === nothing
                println("Najpierw wygeneruj rozkład LU z częściowym wyborem i wczytaj wektor b (opcje 7 i 2).")
            else
                x = solve_lu_partial(LU, p, b)
                println("Układ rozwiązany za pomocą rozkładu LU z częściowym wyborem.")
                # println("Rozwiązanie: ", x)
                LU = nothing
                b = nothing
            end
        elseif choice == "10"
            if x === nothing
                println("Najpierw rozwiąż układ równań (np. opcje 4, 5, 6 lub 7).")
            else
                print("Podaj nazwę pliku do zapisu rozwiązania: ")
                filename = readline()
                write_solution(filename, x, x_exact)  
                println("Rozwiązanie zapisane do pliku: ", filename)
            end
        elseif choice == "0"
            println("Koniec programu.")
            break

        else
            println("Nieprawidłowa opcja. Spróbuj ponownie.")
        end
    end
end

main()