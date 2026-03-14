# Rafal Wochna 279752
include("interpolacja.jl")
using .interpolacja
using Plots

# Przykład (a): f(x) = e^x
f1(x) = exp(x)
a1 = 0.0
b1 = 1.0

for n in [5, 10, 15]
    interpolacja.rysujNnfx(f1, a1, b1, n, wezly=:rownoodlegle)
    savefig("wykres_exp_rownoodlegle_n$n.png")
    println("Wykres dla f(x) = e^x, n = $n (węzły równoodległe) zapisany jako 'wykres_exp_rownoodlegle_n$n.png'")
end

# Przykład (b): f(x) = x^2 * sin(x)
f2(x) = x^2 * sin(x)
a2 = -1.0
b2 = 1.0

for n in [5, 10, 15]
    interpolacja.rysujNnfx(f2, a2, b2, n, wezly=:rownoodlegle)
    savefig("wykres_x2sinx_rownoodlegle_n$n.png")
    println("Wykres dla f(x) = x^2 * sin(x), n = $n (węzły równoodległe) zapisany jako 'wykres_x2sinx_rownoodlegle_n$n.png'")
end