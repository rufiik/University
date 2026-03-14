# Rafal Wochna 279752

include("RootFinding.jl")  
using .RootFinding

f = x -> sin(x) - (1/2 * x)^2
pf = x -> cos(x) - x / 2  # Pochodna funkcji dla metody Newtona

delta = 1 / (2 * 10^5)
epsilon = 1 / (2 * 10^5)

println("Rozwiązywanie równania sin(x) - (1/2 * x)^2 = 0")

# 1. Metoda bisekcji
println("\nMetoda bisekcji:")
a, b = 1.5, 2.0
r_bisekcji, fr_bisekcji, it_bisekcji, err_bisekcji = mbisekcji(f, a, b, delta, epsilon)
println("r = $r_bisekcji, f(r) = $fr_bisekcji, iteracje = $it_bisekcji, kod błędu = $err_bisekcji")

# 2. Metoda Newtona
println("\nMetoda Newtona:")
x0_newton = 1.5
r_newton, fr_newton, it_newton, err_newton = mstycznych(f, pf, x0_newton, delta, epsilon, 100)
println("r = $r_newton, f(r) = $fr_newton, iteracje = $it_newton, kod błędu = $err_newton")

# 3. Metoda siecznych
println("\nMetoda siecznych:")
x0_sieczne, x1_sieczne = 1.0, 2.0
r_sieczne, fr_sieczne, it_sieczne, err_sieczne = msiecznych(f, x0_sieczne, x1_sieczne, delta, epsilon, 100)
println("r = $r_sieczne, f(r) = $fr_sieczne, iteracje = $it_sieczne, kod błędu = $err_sieczne")