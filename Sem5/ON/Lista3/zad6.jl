# Rafal Wochna 279752
include("RootFinding.jl")  
using .RootFinding
f1 = x -> exp(1 - x) - 1
pf1 = x -> -exp(1 - x)  # Pochodna funkcji f1
f2 = x -> x * exp(-x)
pf2 = x -> exp(-x) * (1 - x)  # Pochodna funkcji f2
delta = 10.0^-5
epsilon = 10.0^-5
println("Rozwiązywanie równania f1(x) = e^(1-x) - 1: ")

# Przedziały dla f1
a1_f1, b1_f1 = -2.0, 3.5
a2_f1, b2_f1 = 0.0, 2.0
a3_f1, b3_f1 = -1000.0, 1000.0

# Rozwiązania dla mbisekcji
r1_f1, fr1_f1, it1_f1, err1_f1 = mbisekcji(f1, a1_f1, b1_f1, delta, epsilon)
println("Przedział: [$a1_f1, $b1_f1], r = $r1_f1, f(r) = $fr1_f1, iteracje = $it1_f1, kod błędu = $err1_f1")
r2_f1, fr2_f1, it2_f1, err2_f1 = mbisekcji(f1, a2_f1, b2_f1, delta, epsilon)
println("Przedział: [$a2_f1, $b2_f1], r = $r2_f1, f(r) = $fr2_f1, iteracje = $it2_f1, kod błędu = $err2_f1")
r3_f1, fr3_f1, it3_f1, err3_f1 = mbisekcji(f1, a3_f1, b3_f1, delta, epsilon)
println("Przedział: [$a3_f1, $b3_f1], r = $r3_f1, f(r) = $fr3_f1, iteracje = $it3_f1, kod błędu = $err3_f1")
println("\nRozwiązywanie równania f2(x) = x * e^(-x):")
# Przedziały dla f2
a1_f2, b1_f2 = -1.0, 1.0
a2_f2, b2_f2 = -1000.0, 1.0
a3_f2, b3_f2 = -1.0, 1000.0
r1_f2, fr1_f2, it1_f2, err1_f2 = mbisekcji(f2, a1_f2, b1_f2, delta, epsilon)
println("Przedział: [$a1_f2, $b1_f2], r = $r1_f2, f(r) = $fr1_f2, iteracje = $it1_f2, kod błędu = $err1_f2")
r2_f2, fr2_f2, it2_f2, err2_f2 = mbisekcji(f2, a2_f2, b2_f2, delta, epsilon)
println("Przedział: [$a2_f2, $b2_f2], r = $r2_f2, f(r) = $fr2_f2, iteracje = $it2_f2, kod błędu = $err2_f2")
r3_f2, fr3_f2, it3_f2, err3_f2 = mbisekcji(f2, a3_f2, b3_f2, delta, epsilon)
println("Przedział: [$a3_f2, $b3_f2], r = $r3_f2, f(r) = $fr3_f2, iteracje = $it3_f2, kod błędu = $err3_f2")

# Rozwiązania dla mstycznych
maxit = 100  
println("Metoda Newtona dla f1(x) = e^(1-x) - 1:")
x0_values_f1 = [-10.0, 0.1, 0.5, 1.1, 2.0, 4.0, 20.0, 100.0] 
for x0 in x0_values_f1
    r_f1, fr_f1, it_f1, err_f1 = mstycznych(f1, pf1, x0, delta, epsilon, maxit)
    println("x0 = $x0, r = $r_f1, f(r) = $fr_f1, iteracje = $it_f1, kod błędu = $err_f1")
end
println("\nMetoda Newtona dla f2(x) = x * e^(-x):")
x0_values_f2 = [-10.0,0.1,0.5,1.0, 2.0, 4.0,  20.0, 100.0]  
for x0 in x0_values_f2
    r_f2, fr_f2, it_f2, err_f2 = mstycznych(f2, pf2, x0, delta, epsilon, maxit)
    println("x0 = $x0, r = $r_f2, f(r) = $fr_f2, iteracje = $it_f2, kod błędu = $err_f2")
end

# Sprawdzenie dla x0 = 1 dla f2
println("\nSprawdzenie dla x0 = 1 dla f2:")
x0 = 1.0
r_f2, fr_f2, it_f2, err_f2 = mstycznych(f2, pf2, x0, delta, epsilon, maxit)
println("x0 = $x0, r = $r_f2, f(r) = $fr_f2, iteracje = $it_f2, kod błędu = $err_f2")

# Rozwiązania dla msiecznych
println("Metoda siecznych dla f1(x) = e^(1-x) - 1:")
x0_x1_values_f1 = [
    (0.5, 1.5),
    (-1.0, 5.0),
    (0.0, 2.0),
    (-10.0, 10.0),
    (-1000.0, 1000.0)
]
for (x0, x1) in x0_x1_values_f1
    local r_f1, fr_f1, it_f1, err_f1  # Jawna deklaracja zmiennych jako lokalnych
    r_f1, fr_f1, it_f1, err_f1 = msiecznych(f1, x0, x1, delta, epsilon, maxit)
    println("x0 = $x0, x1 = $x1, r = $r_f1, f(r) = $fr_f1, iteracje = $it_f1, kod błędu = $err_f1")
end
println("\nMetoda siecznych dla f2(x) = x * e^(-x):")
x0_x1_values_f2 = [
    (0.1, 1.0),
    (-1.0, 1.0),
    (0.0, 2.0),
    (1.0, 2.0),
    (-1000.0, 1000.0)
]

for (x0, x1) in x0_x1_values_f2
    local r_f2, fr_f2, it_f2, err_f2  # Jawna deklaracja zmiennych jako lokalnych
    r_f2, fr_f2, it_f2, err_f2 = msiecznych(f2, x0, x1, delta, epsilon, maxit)
    println("x0 = $x0, x1 = $x1, r = $r_f2, f(r) = $fr_f2, iteracje = $it_f2, kod błędu = $err_f2")
end
