# Rafal Wochna 279752

# Definicja funkcji f(x) i jej dokładnej pochodnej f'(x)
function f(x::Float64)
    return sin(x) + cos(3x)
end

function f_exact_derivative(x::Float64)
    return cos(x) - 3sin(3x)
end

# Punkt x0
x0 = Float64(1.0)

# Obliczanie przybliżonej pochodnej i błędów dla h = 2^(-n)
println("n | h | h+1 | Przybliżona pochodna | Dokładna pochodna | Błąd")
println("------------------------------------------------------")

for n in 0:54
    h = 2.0^(-n)  # h = 2^(-n)
    approx_derivative = (f(x0 + h) - f(x0)) / h  # Przybliżona pochodna
    exact_derivative = f_exact_derivative(x0)   # Dokładna wartość pochodnej
    error = abs(exact_derivative - approx_derivative)  # Błąd
    h_plus_1 = 2.0^(-(n-1))  # h + 1 = 2^(-(n-1))

    println("$n | $h | $h_plus_1 | $approx_derivative | $exact_derivative | $error")
end