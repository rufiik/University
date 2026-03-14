# Rafal Wochna 279752
module blocksys
include("matrix_utils.jl")
using ..MatrixUtils
using SparseArrays
using LinearAlgebra

export solve_gauss, solve_gauss_partial, compute_lu!, compute_lu_partial!, solve_lu, solve_lu_partial
# Eliminacja Gaussa bez wyboru elementu głównego
function solve_gauss(mat::BlockMatrix, b::Vector{Float64})
    n = mat.n
    l = mat.l
    A = mat.A  

    # Etap I - Doprowadzenie macierzy do postaci górnotrójkątnej
    for k in 1:n-1
        # Sprawdzenie, czy element diagonalny jest zerowy
        if abs(A[k, k]) < 1e-12
            error("Element diagonalny zerowy w kroku $k. Potrzebny wybór elementu głównego.")
        end
        # Eliminacja wierszy poniżej wiersza k
        for i in k+1:min(k+l, n)  # Iteracja tylko po wierszach w bloku
                factor = A[i, k] / A[k, k]
                for j in k+1:last_column(mat, k)
                        A[i, j] -= factor * A[k, j]
                end
                b[i] -= factor * b[k]
        end
    end

    # Etap II - Rozwiązanie układu z macierzą górnotrójkątną
    x = zeros(Float64, n)
    for i in n:-1:1
        sum = 0.0
        for j in i+1:last_column(mat, i)
            sum += A[i, j] * x[j]
        end
        x[i] = (b[i] - sum) / A[i, i]
    end

    return x
end

function Base.getindex(mat::BlockMatrix, i::Int, j::Int)
        return mat.A[i, j]  # Pobieranie wartości z macierzy rzadkiej SparseMatrixCSC
    end

function solve_gauss_partial(A::BlockMatrix, b::Vector{Float64})
    n = A.n
    block_size = Int(A.l) 
    p = collect(1:n)  # Tablica permutacji

    # Etap I - Doprowadzenie A do macierzy górnotrójkątnej
    for column in 1:n-1
        # Znalezienie pivota (maksymalnego elementu w kolumnie)
        maxElem = 0.0
        maxIndex = column
        for k in column : last_row(A, column) 
            if abs(A[p[k], column]) > abs(maxElem)
                maxElem = A[p[k], column]
                maxIndex = k
            end
        end

        # Zamiana wierszy w tablicy permutacji
        p[column], p[maxIndex] = p[maxIndex], p[column]

        # Eliminacja
        for i in column+1 : last_row(A, column)
            if A[p[i], column] != 0.0  
                coeff = A[p[i], column] / A[p[column], column]  
                for j in column+1: last_column(A, column + block_size)
                    A.A[p[i], j] -= coeff * A[p[column], j]  
                end
                b[p[i]] -= coeff * b[p[column]]  # Aktualizacja wektora b
            end
        end
    end

    # Etap II - Rozwiązanie układu z macierzą górnotrójkątną (uwzględniając permutacje)
    x = zeros(Float64, n)
    x[n] = b[p[n]] / A[p[n], n]
    for i in n-1:-1:1
        x[i] = b[p[i]]
        for j in i+1: last_column(A, i + block_size)
            x[i] -= A[p[i], j] * x[j]
        end
        x[i] /= A[p[i], i]
    end

    return x
end

function compute_lu!(mat::BlockMatrix)
    n = mat.n
    A = mat.A  

    for column in 1:n-1
        for i in column+1:last_row(mat, column)
            l = A[i, column] / A[column, column]
            A[i, column] = l  # Zapisujemy mnożnik w miejscu zerowanego elementu

            # Aktualizacja pozostałych elementów w wierszu
            for j in column+1:last_column(mat, column)
                A[i, j] -= l * A[column, j]
            end
        end
    end

    return mat 
end

function compute_lu_partial!(mat::BlockMatrix)
    n = mat.n
    A = mat.A  
    block_size = Int(mat.l)  
    p = collect(1:n)  # Tablica permutacji

    for column in 1:n-1
        # Znalezienie pivota (maksymalnego elementu w kolumnie)
        maxElem = 0.0
        maxIndex = column
        for k in column:last_row(mat, column)  
            if abs(A[p[k], column]) > abs(maxElem)
                maxElem = abs(A[p[k], column])
                maxIndex = k
            end
        end

        # Aktualizacja tablicy permutacji 
        if maxIndex != column
            p[column], p[maxIndex] = p[maxIndex], p[column]
        end

        # Eliminacja
        for i in column+1:last_row(mat, column)
            if A[p[i], column] != 0.0 
                coeff = A[p[i], column] / A[p[column], column] 
                A[p[i], column] = coeff  

                for j in column+1:last_column(mat, column+block_size)
                    A[p[i], j] -= coeff * A[p[column], j]  
                end
            end
        end
    end

    return p  # Zwracamy tablicę permutacji
end

function solve_lu(LU::BlockMatrix, b::Vector{Float64})
    n = LU.n
    # Etap I - Podstawienie w przód: L*y = b 
    for i in 1:n
        for j in first_column(LU, i):i-1
            b[i] -= LU[i, j] * b[j]
        end
    end

    # Etap II - Podstawienie od tyłu: U*x = y 
    for i in n:-1:1
        for j in i+1:last_column(LU, i)
            b[i] -= LU[i, j] * b[j]
        end
        b[i] /= LU[i, i]
    end

    return b
end

function solve_lu_partial(LU::BlockMatrix, p::Vector{Int}, b::Vector{Float64})
    n = LU.n
    x = zeros(Float64, n)

    # Etap I - Podstawienie w przód: L*y = pb
    for i in 1:n
        x[i] = b[p[i]]
        for j in first_column(LU, p[i]):i-1
            x[i] -= LU.A[p[i], j] * x[j]
        end
    end

    # Etap II - Podstawienie od tyłu: U*x = y
    x[n] /= LU.A[p[n], n]
    for i in n-1:-1:1
        for j in i+1:last_column(LU, i + LU.l)
            x[i] -= LU.A[p[i], j] * x[j]
        end
        x[i] /= LU.A[p[i], i]
    end

    return x
end


end # module blocksys