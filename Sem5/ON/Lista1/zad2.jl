# Rafal Wochna 279752

#  - T::Type : typ zmiennoprzecinkowy (np. Float32)
function Kahan_machine_epsilon(T::Type)
    epsilon = T(3.0) *(( T(4.0)/T(3.0)) - T(1.0)) - T(1.0)
    return epsilon
end

# Lista typów do analizy
types = [Float16, Float32, Float64]

# Główna pętla raportująca wyniki
for T in types
    kahan_epsilon = Kahan_machine_epsilon(T)
    builtin_epsilon = eps(T)
    println("Typ: $T")
    println("  Epsilon obliczony metodą Kahana: $kahan_epsilon")
    println("  Wartość z funkcji eps:            $builtin_epsilon")
    println()
end