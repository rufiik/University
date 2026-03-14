# Rafal Wochna 279752

#  - T::Type : typ zmiennoprzecinkowy (np. Float32)
function calculate_machine_epsilon(T::Type)
    epsilon = T(1.0)
    next_epsilon = epsilon / T(2)
    while one(T) + next_epsilon > one(T)
        epsilon = next_epsilon
        next_epsilon = epsilon / T(2)
    end
    return epsilon
end
function calculate_machine_eta(T::Type)
    eta = T(1.0)
    next_eta = eta / T(2)
    while next_eta > zero(T)
        eta = next_eta
        next_eta = eta / T(2)
    end
    return eta
end
function calculate_machine_max(T::Type)
    max_val = T(1.0)
    next_val = max_val
    while !isinf(next_val)
        max_val = next_val
        next_val = max_val * T(2)
    end
    move = max_val / T(2) # move : krok dostrajania (początkowo połowa max_val)
    while move > T(0.0) && max_val + move > max_val
        next_val = max_val + move
        if !isinf(next_val)
            max_val = next_val
        else
            move /= T(2)
        end
    end

    return max_val
end
# Lista typów do analizy
types = [Float16, Float32, Float64]

# Pętle raportujące wyniki
for T in types
    iter_epsilon = calculate_machine_epsilon(T)
    builtin_epsilon = eps(T)
    iter_eta = calculate_machine_eta(T)
    builtin_zero = nextfloat(zero(T))
    iter_max = calculate_machine_max(T)
    builtin_max = floatmax(T)
    println("Typ: $T")
    println("  Iteracyjnie obliczony epsilon: $iter_epsilon")
    println("  Wartość z funkcji eps:        $builtin_epsilon")
    println("  Iteracyjnie obliczony eta:     $iter_eta")
    println("  Wartość builtin(zero()):       $builtin_zero")
    println("  Iteracyjnie obliczony max:     $iter_max")
    println("  Wartość builtin(max):          $builtin_max")
    println()
end
println("Minimalne wartości znormalizowane:")
for T in [Float32, Float64]
    min_val = floatmin(T)
    println("Typ: $T")
    println("  Minimalna wartość znormalizowana (floatmin): $min_val")
    println()
end