# Rafal Wochna 279752
module MatrixUtils
using SparseArrays
using LinearAlgebra


export read_matrix, read_vector,
       generate_vector,write_solution,
       first_column, last_column,
       first_row, last_row, generate_vector,
       BlockMatrix, write_solution_with_time
# Struktura przechowująca macierz w formacie rzadkim
struct BlockMatrix
    n::Int
    l::Int
    A::SparseMatrixCSC{Float64, Int64} 
end       
# Funkcja do wczytywania macierzy z pliku
function read_matrix(filename::String)
    open(filename) do f
        # Pierwsza linia: n l
        line = readline(f)
        parts = split(line)
        n = parse(Int, parts[1])
        l = parse(Int, parts[2])
        
        # Przygotowanie danych dla formatu rzadkiego
        I = Int[]
        J = Int[]
        V = Float64[]
        
        # Czytanie elementów niezerowych
        while !eof(f)
            line = readline(f)
            isempty(line) && continue
            parts = split(line)
            length(parts) < 3 && continue
            
            i = parse(Int, parts[1])
            j = parse(Int, parts[2])
            val = parse(Float64, parts[3])
            
            # Sprawdzenie poprawności indeksów
            if i < 1 || i > n || j < 1 || j > n
                error("Nieprawidłowe indeksy: ($i, $j) dla macierzy rozmiaru $n x $n")
            end
            
            push!(I, i)
            push!(J, j)
            push!(V, val)
        end
        for k in 0:(div(n-1, l))
    # Zakres wierszy i kolumn dla bloku Ck
    row_start = k*l + 1
    row_end = min(n, (k+1)*l)
    col_start = k*l + 1
    col_end = min(n, (k+1)*l)
    
        # Zera pod przekątną w Ck
        for i in row_start+1:row_end
            for j in col_start:i-1
                push!(I, i)
                push!(J, j)
                push!(V, 0.0)
            end
        end
        
        # l-1 zer na prawo od przekątnej w Ck (dla metod z częściowym wyborem)
        for i in row_start:row_end
            for j in i+1:min(i+l-1, col_end)
                push!(I, i)
                push!(J, j)
                push!(V, 0.0)
            end
        end
    end

    # Zera w blokach Bk 
    for k in 1:(div(n-1, l))
        # Blok Bk: wiersze (k-1)*l+1:k*l, kolumny k*l+1:min(n, (k+1)*l)
        row_start = (k-1)*l + 1
        row_end = k*l
        col_start = k*l + 1
        col_end = min(n, (k+1)*l)
        for i in row_start:row_end
            for j in col_start:col_end
                push!(I, i)
                push!(J, j)
                push!(V, 0.0)
            end
        end
    end
        # Tworzenie macierzy rzadkiej
        A = sparse(I, J, V, n, n)
        
        return BlockMatrix(n, l, A)
    end
end

# Funkcja do wczytywania wektora z pliku
function read_vector(filename::String)
    open(filename) do f
        n_line = readline(f)
        n = parse(Int, n_line)
        b = zeros(Float64, n)
        for i in 1:n
            line = readline(f)
            b[i] = parse(Float64, line)
        end
        return b
    end
end

# Funkcja zwracająca indeks pierwszej kolumny w wierszu i, która może zawierać niezerowe elementy
function first_column(mat::BlockMatrix, rowIndex::Int)
    if rowIndex % mat.l == 1
        return max(1, rowIndex - mat.l)
    end
    return max(1, rowIndex - ((rowIndex - 1) % mat.l) - 1)
end

# Funkcja zwracająca indeks ostatniej kolumny w wierszu i, która może zawierać niezerowe elementy
function last_column(mat::BlockMatrix, rowIndex::Int)
    return min(mat.n, mat.l + rowIndex)
end
# Funkcja zwracająca indeks pierwszego wiersza w kolumnie j, która może zawierać niezerowe elementy
function first_row(mat::BlockMatrix, columnIndex::Int)
    return max(1, columnIndex - mat.l)
end

# Funkcja zwracająca indeks ostatniego wiersza w kolumnie j, która może zawierać niezerowe elementy
function last_row(mat::BlockMatrix, columnIndex::Int)
    if columnIndex % mat.l == 0
        return min(mat.n, columnIndex + mat.l)
    end
    return min(mat.n, columnIndex + mat.l - ((columnIndex - 1) % mat.l))
end

# Funkcja mnożenia macierzy BlockMatrix przez wektor
function multiply_matrix_vector(mat::BlockMatrix, vec::Vector{Float64})
    if length(vec) != mat.n
        error("Niezgodne rozmiary: macierz ma rozmiar $(mat.n), a wektor ma długość $(length(vec))")
    end

    result = zeros(Float64, mat.n)
    for i in 1:mat.n
        for j in first_column(mat, i):last_column(mat, i) # iteraja tylko po niezerowych elementach
            result[i] += mat.A[i, j] * vec[j]
        end
    end

    return result
end

# Funkcja generująca wektor b = A*x, gdzie x = [1, 1, ..., 1]^T
function generate_vector(mat::BlockMatrix)
    n = mat.n
    x = ones(Float64, n)  
    b = multiply_matrix_vector(mat, x)       
    return b
end

# Funkcja do zapisywania rozwiązania do pliku (z opcjonalnym błędem względnym)
function write_solution(filename::String, x::Vector{Float64}, x_exact::Union{Nothing, Vector{Float64}}=nothing)
    open(filename, "w") do f
        # Jeśli podano dokładne rozwiązanie, oblicz i zapisz błąd względny
        if x_exact !== nothing
            error = norm(x - x_exact) / norm(x_exact)
            println(f, error) 
        end
        for xi in x
            println(f, xi)
        end
    end
end


function format_memory(bytes::Int)
    if bytes < 1024
        return string(bytes, " B")
    elseif bytes < 1024^2
        return string(round(bytes / 1024, digits=2), " KB")
    elseif bytes < 1024^3
        return string(round(bytes / 1024^2, digits=2), " MB")
    else
        return string(round(bytes / 1024^3, digits=2), " GB")
    end
end
# Funkcja do zapisywania rozwiązania do pliku (z opcjonalnym błędem względnym i czasem wykonania)
function write_solution_with_time(filename::String, x::Vector{Float64}, x_exact::Union{Nothing, Vector{Float64}}=nothing, elapsed_time::Float64=0.0, memory_allocated::Int=0)
    open(filename, "w") do f
        # Jeśli podano dokładne rozwiązanie, oblicz i zapisz błąd względny
        if x_exact !== nothing
            error = norm(x - x_exact) / norm(x_exact)
            println(f, error)
        end
        println(f, elapsed_time, " s")
        println(f, format_memory(memory_allocated))
        # for xi in x
        #     println(f, xi)
        # end
    end 
end

end # module MatrixUtils

