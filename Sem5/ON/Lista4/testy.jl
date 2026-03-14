# Rafal Wochna 279752
using Test
include("interpolacja.jl")  
using .interpolacja
using Plots

@testset "Testy funkcji ilorazyRoznicowe" begin
    # Test 1: Funkcja liniowa f(x) = 2x + 3
    x = [1.0, 2.0, 3.0]
    y = [5.0, 7.0, 9.0]  
    @test interpolacja.ilorazyRoznicowe(x, y) ≈ [5.0, 2.0, 0.0]  
    # Test 2: Funkcja kwadratowa f(x) = x^2
    x = [1.0, 2.0, 3.0]
    y = [1.0, 4.0, 9.0]  
    @test interpolacja.ilorazyRoznicowe(x, y) ≈ [1.0, 3.0, 1.0] 
    # Test 3: Pojedynczy punkt (przypadek brzegowy)
    x = [2.0]
    y = [4.0]
    @test interpolacja.ilorazyRoznicowe(x, y) ≈ [4.0]  
    # Test 4: Powtarzające się punkty (przypadek brzegowy)
    x = [1.0, 1.0, 2.0]
    y = [2.0, 2.0, 3.0]
    expected = [2.0, NaN, NaN]
    result = interpolacja.ilorazyRoznicowe(x, y)
    @test result[1] == expected[1]  
    @test isnan(result[2])       
    @test isnan(result[3]) 
end

@testset "Testy funkcji warNewton" begin
    # Test 1: Funkcja liniowa f(x) = 2x + 3
    x = [1.0, 2.0, 3.0]
    fx = [5.0, 2.0, 0.0]  
    @test interpolacja.warNewton(x, fx, 1.5) ≈ 6.0  
    @test interpolacja.warNewton(x, fx, 2.5) ≈ 8.0  
    # Test 2: Funkcja kwadratowa f(x) = x^2
    x = [1.0, 2.0, 3.0]
    fx = [1.0, 3.0, 1.0]  
    @test interpolacja.warNewton(x, fx, 1.5) ≈ 2.25 
    @test interpolacja.warNewton(x, fx, 2.5) ≈ 6.25  
end

@testset "Testy funkcji naturalna" begin
    # Test 1: Funkcja liniowa f(x) = 2x + 3
    x = [1.0, 2.0, 3.0]
    fx = [5.0, 2.0, 0.0] 
    a = interpolacja.naturalna(x, fx)
    @test a ≈ [3.0, 2.0, 0.0]  
    # Test 2: Funkcja kwadratowa f(x) = x^2
    x = [1.0, 2.0, 3.0]
    fx = [1.0, 3.0, 1.0]  
    a = interpolacja.naturalna(x, fx)
    @test a ≈ [0.0, 0.0, 1.0]  
    # Test 3: Funkcja stała f(x) = 4
    x = [1.0, 2.0, 3.0]
    fx = [4.0, 0.0, 0.0]  
    a = interpolacja.naturalna(x, fx)
    @test a ≈ [4.0, 0.0, 0.0]  
end
@testset "Testy funkcji rysujNnfx" begin
    # Funkcja liniowa f(x) = 2x + 3
    f1(x) = 2x + 3
    println("Rysowanie wykresu dla funkcji liniowej f(x) = 2x + 3")
    interpolacja.rysujNnfx(f1, 0.0, 3.0, 5, wezly=:rownoodlegle)
    savefig("wykres_liniowa_rownoodlegle.png")
    println("Wykres zapisany jako 'wykres_liniowa_rownoodlegle.png'")

    # Funkcja kwadratowa f(x) = x^2
    f2(x) = x^2
    println("Rysowanie wykresu dla funkcji kwadratowej f(x) = x^2")
    interpolacja.rysujNnfx(f2, -3.0, 3.0, 5, wezly=:rownoodlegle)
    savefig("wykres_kwadratowa_rownoodlegle.png")
    println("Wykres zapisany jako 'wykres_kwadratowa_rownoodlegle.png'")

    # Funkcja stała f(x) = 4
    f3(x) = 4
    println("Rysowanie wykresu dla funkcji stałej f(x) = 4")
    interpolacja.rysujNnfx(f3, 0.0, 3.0, 5, wezly=:rownoodlegle)
    savefig("wykres_stala_rownoodlegle.png")
    println("Wykres zapisany jako 'wykres_stala_rownoodlegle.png'")
end