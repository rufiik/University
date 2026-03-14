# Rafal Wochna 279752

include("RootFinding.jl") 
using .RootFinding
f = x -> 3x - exp(x)  
delta::Float64 = 10^-4
epsilon::Float64 = 10^-4

a1::Float64, b1::Float64 = 0.0, 1.0
a2::Float64, b2::Float64 = 1.0, 2.0

println("Rozwiązywanie równania 3x - e^x = 0 metodą bisekcji:")
r, fr, it, err = mbisekcji(f, a1, b1, delta, epsilon)

println("r = $r, f(r) = $fr, iteracje = $it, kod błędu = $err")

r, fr, it, err = mbisekcji(f, a2, b2, delta, epsilon)
println("r = $r, f(r) = $fr, iteracje = $it, kod błędu = $err")