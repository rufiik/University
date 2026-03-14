include("matrixgen.jl")
using .matrixgen

ns = [10, 100, 1000, 2500, 5000, 7500, 10000, 25000, 50000, 100000]
letters = ["B","C","D","E","F", "G", "H", "I", "J","K"]
ls = [2, 4, 5, 10, 2, 4 ,5 ,10 ,2, 4] 

ck = 1.0

for (idx, letter) in enumerate(letters)
    l = ls[idx]
    for n in ns
        if n % l == 0
            filename = "$(letter)_$(n).txt"
            println("Generuję macierz dla n = $n, l = $l do pliku $filename")
            blockmat(n, l, ck, filename)
        else
            println("Pominięto n = $n (niepodzielne przez l = $l) dla litery $letter")
        end
    end
end