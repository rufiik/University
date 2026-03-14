using Plots

function xsquared_rec(x::Float64, c::Float64)
    next_x::Float64 = x * x + c
    return next_x
end

# Funkcja do rysowania iteracji graficznej i zapisywania wykresu
function plot_iterative_graphics(c::Float64, x0::Float64, n::Int, filename::String)
    f(x) = x^2 + c  # Funkcja iteracyjna
    x_vals = LinRange(-2, 2, 1000)  # Zakres wartości x do rysowania funkcji

    # Rysowanie funkcji f(x) i linii y = x
    plot(x_vals, f.(x_vals), label="f(x) = x^2 + $c", legend=:top, xlabel="x", ylabel="y", title="Iteracja graficzna dla c = $c, x₀ = $x0")
    plot!(x_vals, x_vals, label="y = x", color=:red)

    # Iteracja graficzna
    x = x0
    for i in 1:n
        y = f(x)
        if i == 1
            plot!([x, x], [0, y], color=:blue, label=false)
        else
        # Rysowanie linii pionowej (x, f(x))
        plot!([x, x], [x, y], color=:blue, label=false)
        end
        # Rysowanie linii poziomej (f(x), y = x)
        plot!([x, y], [y, y], color=:green, label=false)
        x = y
    end

    # Zapisanie wykresu do pliku
    savefig(filename)
    println("Wykres zapisany jako $filename")
end

# Parametry
configs = [
    (-2.0, 1.0),               # c = -2, x0 = 1
    (-2.0, 2.0),               # c = -2, x0 = 2
    (-2.0, 1.99999999999999),  # c = -2, x0 = 1.99999999999999
    (-1.0, 1.0),               # c = -1, x0 = 1
    (-1.0, -1.0),              # c = -1, x0 = -1
    (-1.0, 0.75),              # c = -1, x0 = 0.75
    (-1.0, 0.25)               # c = -1, x0 = 0.25
]

# Liczba iteracji
n = 40

# Rysowanie iteracji graficznej dla każdej konfiguracji i zapisywanie wykresów
for (i, (c, x0)) in enumerate(configs)
    filename = "iteracja_graficzna_c$(c)_x0$(x0)_$(i).png"
    plot_iterative_graphics(c, x0, n, filename)
end