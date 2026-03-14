# Rafal Wochna 279752

# Definicja funkcji f(x) i g(x)
function f(x::Float64)
    return sqrt(x^2 + 1) - 1
end

function g(x::Float64)
    return x^2 / (sqrt(x^2 + 1) + 1)
end
x_values = [8.0^(-i) for i in 1:10]

println("Wyniki dla Float64:")
for x in x_values
    println("x = $x")
    println("f(x) = ", f(x))
    println("g(x) = ", g(x))
    println()
end