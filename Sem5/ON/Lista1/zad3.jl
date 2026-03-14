# Rafal Wochna 279752

# Funkcja pomocnicza do wypisywania bitstringów i kroków
function wypisz_bitstring(przedzial_start, przedzial_koniec, n)
    println("\nPrzedział [$przedzial_start, $przedzial_koniec]:")
    x = przedzial_start
    for i in 1:n 
        println("Liczba $i: bitstring(x) = $(bitstring(x))")
        if i < n
            x_next = nextfloat(x)
            println("Krok δ = $(x_next - x)")
            x = x_next
        end
    end
end

# Wypisanie bitstringów i kroków dla przedziałów
println("\nBitstringi i kroki dla przedziałów:")

# Przedział [1, 2]
wypisz_bitstring(1.0, 2.0, 10)

# Przedział [0.5, 1]
wypisz_bitstring(0.5, 1.0, 10)

# Przedział [2, 4]
wypisz_bitstring(2.0, 4.0, 10)