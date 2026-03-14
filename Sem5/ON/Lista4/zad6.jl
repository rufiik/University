# Rafal Wochna 279752
include("interpolacja.jl")
using .interpolacja
using Plots

# Przykład (a): f(x) = |x|
f1(x) = abs(x)
a1 = -1.0
b1 = 1.0

for n in [5, 10, 15]
    interpolacja.rysujNnfx(f1, a1, b1, n, wezly=:rownoodlegle)
    savefig("wykres_abs_rownoodlegle_n$n.png")
    println("Wykres dla f(x) = |x|, n = $n (węzły równoodległe) zapisany jako 'wykres_abs_rownoodlegle_n$n.png'")

    interpolacja.rysujNnfx(f1, a1, b1, n, wezly=:czebyszew)
    savefig("wykres_abs_czebyszew_n$n.png")
    println("Wykres dla f(x) = |x|, n = $n (węzły Czebyszewa) zapisany jako 'wykres_abs_czebyszew_n$n.png'")
end

# Przykład (b): f(x) = 1 / (1 + x^2)
f2(x) = 1 / (1 + x^2)
a2 = -5.0
b2 = 5.0

for n in [5, 10, 15]
    interpolacja.rysujNnfx(f2, a2, b2, n, wezly=:rownoodlegle)
    savefig("wykres_runge_rownoodlegle_n$n.png")
    println("Wykres dla f(x) = 1 / (1 + x^2), n = $n (węzły równoodległe) zapisany jako 'wykres_runge_rownoodlegle_n$n.png'")

    interpolacja.rysujNnfx(f2, a2, b2, n, wezly=:czebyszew)
    savefig("wykres_runge_czebyszew_n$n.png")
    println("Wykres dla f(x) = 1 / (1 + x^2), n = $n (węzły Czebyszewa) zapisany jako 'wykres_runge_czebyszew_n$n.png'")
end