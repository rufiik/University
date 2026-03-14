#Rafal Wochna 279752
using LinearAlgebra

function matcond(n::Int, c::Float64)
# Function generates a random square matrix A of size n with
# a given condition number c.
# Inputs:
#	n: size of matrix A, n>1
#	c: condition of matrix A, c>= 1.0
#
# Usage: matcond(10, 100.0)
#
# Pawel Zielinski
        if n < 2
         error("size n should be > 1")
        end
        if c< 1.0
         error("condition number  c of a matrix  should be >= 1.0")
        end
        (U,S,V)=svd(rand(n,n))
        return U*diagm(0 =>[LinRange(1.0,c,n);])*V'
end

function hilb(n::Int)
# Function generates the Hilbert matrix  A of size n,
#  A (i, j) = 1 / (i + j - 1)
# Inputs:
#	n: size of matrix A, n>=1
#
#
# Usage: hilb(10)
#
# Pawel Zielinski
        if n < 1
         error("size n should be >= 1")
        end
        return [1 / (i + j - 1) for i in 1:n, j in 1:n]
end
# Funkcje pomocnicze
function relative_error(x_exact, x_approx)
    return norm(x_exact - x_approx) / norm(x_exact)
end

# Eksperymenty dla macierzy Hilberta
function experiment_hilbert()
    println("Eksperymenty dla macierzy Hilberta:")
    for n in 2:13
        H = hilb(n)
        x_exact = ones(n)  # Dokładne rozwiązanie
        b = H * x_exact    # Wektor b

        # Rozwiązanie za pomocą eliminacji Gaussa
        x_gauss = H \ b
        error_gauss = relative_error(x_exact, x_gauss)

        # Rozwiązanie za pomocą odwrotności macierzy
        x_inv = inv(H) * b
        error_inv = relative_error(x_exact, x_inv)

        println("n = $n, cond(H) = $(cond(H)), rank(H) = $(rank(H))")
        println("  Błąd względny (Gauss): $error_gauss")
        println("  Błąd względny (inv): $error_inv")
    end
end

# Eksperymenty dla macierzy losowych
function experiment_random()
    println("\nEksperymenty dla macierzy losowych:")
    n_values = [5, 10, 20]
    c_values = [1, 10, 10^3, 10^7, 10^12, 10^16]

    for n in n_values
        for c in c_values
            R = matcond(n, Float64(c))  # Konwersja c na Float64
            x_exact = ones(n)  # Dokładne rozwiązanie
            b = R * x_exact    # Wektor b

            # Rozwiązanie za pomocą eliminacji Gaussa
            x_gauss = R \ b
            error_gauss = relative_error(x_exact, x_gauss)

            # Rozwiązanie za pomocą odwrotności macierzy
            x_inv = inv(R) * b
            error_inv = relative_error(x_exact, x_inv)

            println("n = $n, c = $c, cond(R) = $(cond(R)), rank(R) = $(rank(R))")
            println("  Błąd względny (Gauss): $error_gauss")
            println("  Błąd względny (inv): $error_inv")
        end
    end
end

# Uruchomienie eksperymentów
experiment_hilbert()
experiment_random()