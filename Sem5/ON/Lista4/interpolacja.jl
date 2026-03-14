# Rafal Wochna 279752

module interpolacja
using Plots
gr()
function ilorazyRoznicowe(x::Vector{Float64}, f::Vector{Float64})
    n = length(x) - 1
    fx = copy(f)  

    for j in 1:n
        for i in n+1:-1:j+1
            fx[i] = (fx[i] - fx[i-1]) / (x[i] - x[i-j])
        end
    end

    return fx
end

function warNewton(x::Vector{Float64}, fx::Vector{Float64}, t::Float64)
    n = length(x) - 1
    nt = fx[n+1]  

    for k in n:-1:1
        nt = fx[k] + (t - x[k]) * nt
    end

    return nt
end

function naturalna(x::Vector{Float64}, fx::Vector{Float64})::Vector{Float64}
    n = length(x) - 1
    a = zeros(Float64, n+1) 
    a[n+1] = fx[n+1]
    for i in n:-1:1
        a[i] = fx[i] - x[i] * a[i+1]
        for j in i+1:n
            a[j] -= x[i] * a[j+1]
        end
    end

    return a
end

function rysujNnfx(f, a::Float64, b::Float64, n::Int; wezly::Symbol = :rownoodlegle)
    x = if wezly == :rownoodlegle
        h = (b - a) / n
        [a + k * h for k in 0:n]
    elseif wezly == :czebyszew
        [0.5 * (a + b) + 0.5 * (b - a) * cos((2k + 1) * π / (2n + 2)) for k in 0:n]
    else
        throw(ArgumentError("Nieznany typ węzłów: $wezly"))
    end

    y = Float64[f(xk) for xk in x]

    fx = ilorazyRoznicowe(x, y)

    function Nn(t)
        warNewton(x, fx, t)
    end

    t = range(a, b, length=100 * (n + 1))  
    ft = [f(tk) for tk in t]    
    Nt = [Nn(tk) for tk in t]    

    plot(t, ft, label="f(x)", lw=2, color=:blue)  
    plot!(t, Nt, label="Wielomian interpolacyjny Nn(x)", lw=2, color=:red) 
end

end # interpolacja module