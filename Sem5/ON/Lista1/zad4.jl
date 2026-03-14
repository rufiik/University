# Rafal Wochna 279752

# Funkcja sprawdzająca warunek x * (1/x) != 1
function znajdz_liczbe()
    macheps = eps(Float64)
    x = 1.0 + macheps 
    while x < 2.0
        if x * (1 / x) != 1.0
            return x  
        end
        x += macheps
    end
    return nothing  
end

x = znajdz_liczbe()

if x !== nothing
    println("Znaleziono liczbę: x = $x")
    println("Sprawdzenie: x * (1/x) = $(x * (1/x))")
else
    println("Nie znaleziono liczby spełniającej warunek w przedziale (1, 2).")
end

