#Rafal Wochna 279752
using Polynomials

# Współczynniki wielomianu P
P = [1, -210.0, 20615.0, -1256850.0,
     53327946.0, -1672280820.0, 40171771630.0, -756111184500.0,          
     11310276995381.0, -135585182899530.0,
     1307535010540395.0, -10142299865511450.0,
     63030812099294896.0, -311333643161390640.0,
     1206647803780373360.0, -3599979517947607200.0,
     8037811822645051776.0, -12870931245150988800.0,
     13803759753640704000.0, -8752948036761600000.0,
     2432902008176640000.0]

reversed_coefficients = P[end:-1:1]
poly = Polynomial(reversed_coefficients)

# Obliczanie miejsc zerowych
zeros = roots(poly)

# Sprawdzanie wyników dla 1 < k < 20
println("Sprawdzanie pierwiastków z_k dla 1 < k < 20:")
polynomialFactoredForm = fromroots(collect(1:20))
for k in 1:20
    zk = zeros[k]  # Pierwiastek z_k
    P_zk = abs(poly(zk))  # |P(zk)|
    p_zk = abs(polynomialFactoredForm(zk))  # |p(zk)|
    error_zk = abs(zk - k)  # |zk - k|
    println("k = $k, z_k = $zk")
    println("  |P(z_k)| = $P_zk")
    println("  |p(z_k)| = $p_zk")
    println("  |z_k - k| = $error_zk")
end


P_modified = copy(P)
P_modified[2] -= 2.0^(-23)  # Zmiana współczynnika -210
reversed_coefficients_modified = P_modified[end:-1:1]
poly_modified = Polynomial(reversed_coefficients_modified)
zeros_modified = roots(poly_modified)
println("\nSprawdzanie pierwiastków z_k dla 1 < k < 20 (zmodyfikowany wielomian):")
for k in 1:20
    zk = zeros_modified[k]  # Pierwiastek z_k
    P_zk = abs(poly_modified(zk))  # |P(zk)|
    p_zk = abs(polynomialFactoredForm(zk))  # |p(zk)|
    error_zk = abs(zk - k)  # |zk - k|
    println("k = $k, z_k = $zk")
    println("  |P(z_k)| = $P_zk")
    println("  |p(z_k)| = $p_zk")
    println("  |z_k - k| = $error_zk")
end