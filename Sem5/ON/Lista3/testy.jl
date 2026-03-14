# Rafal Wochna 279752
include("RootFinding.jl") 
using .RootFinding
using Test  

# Testy jednostkowe
@testset "Testy funkcji mbisekcji" begin

    # Test 1: Funkcja x^2 - 2, pierwiastek w sqrt(2)
    f1 = x -> x^2 - 2
    a1, b1 = 0.0, 2.0
    delta1, epsilon1 = 1e-7, 1e-7
    r1, fr1, it1, err1 = mbisekcji(f1, a1, b1, delta1, epsilon1)
    @test abs(r1 - sqrt(2)) < delta1  # Sprawdzenie, czy wynik jest bliski sqrt(2)
    @test abs(fr1) < epsilon1  # Sprawdzenie, czy wartość funkcji w r1 jest bliska 0
    @test err1 == 0  # Sprawdzenie, czy nie wystąpił błąd

    # Test 2: Funkcja x^3 - x, pierwiastki w -1, 0, 1
    f2 = x -> x^3 - x
    a2, b2 = 0.5, 1.5
    delta2, epsilon2 = 1e-6, 1e-6
    r2, fr2, it2, err2 = mbisekcji(f2, a2, b2, delta2, epsilon2)
    @test abs(r2 - 1.0) < delta2  # Sprawdzenie, czy wynik jest bliski 1.0
    @test abs(fr2) < epsilon2  # Sprawdzenie, czy wartość funkcji w r2 jest bliska 0
    @test err2 == 0  # Sprawdzenie, czy nie wystąpił błąd

    # Test 3: Funkcja x^2 + 1, brak pierwiastków w R
    f3 = x -> x^2 + 1
    a3, b3 = -1.0, 1.0
    delta3, epsilon3 = 1e-6, 1e-6
    r3, fr3, it3, err3 = mbisekcji(f3, a3, b3, delta3, epsilon3)
    @test isnan(r3)  # Sprawdzenie, czy wynik to NaN
    @test err3 == 1  # Sprawdzenie, czy zwrócono błąd (brak zmiany znaku)

    # Test 4: Funkcja liniowa x - 5, pierwiastek w 5
    f4 = x -> x - 5
    a4, b4 = 4.0, 6.0
    delta4, epsilon4 = 1e-6, 1e-6
    r4, fr4, it4, err4 = mbisekcji(f4, a4, b4, delta4, epsilon4)
    @test abs(r4 - 5.0) < delta4  # Sprawdzenie, czy wynik jest bliski 5.0
    @test abs(fr4) < epsilon4  # Sprawdzenie, czy wartość funkcji w r4 jest bliska 0
    @test err4 == 0  # Sprawdzenie, czy nie wystąpił błąd

end

@testset "Testy funkcji mstycznych" begin

    # Test 1: Funkcja x^2 - 2, pierwiastek w sqrt(2)
    f1 = x -> x^2 - 2
    pf1 = x -> 2x  
    x0_1 = 1.0  
    delta1, epsilon1 = 1e-7, 1e-7
    maxit1 = 100
    r1, fr1, it1, err1 = mstycznych(f1, pf1, x0_1, delta1, epsilon1, maxit1)
    @test abs(r1 - sqrt(2)) < delta1  # Sprawdzenie, czy wynik jest bliski sqrt(2)
    @test abs(fr1) < epsilon1  # Sprawdzenie, czy wartość funkcji w r1 jest bliska 0
    @test err1 == 0  # Sprawdzenie, czy nie wystąpił błąd

    # Test 2: Funkcja x^3 - x, pierwiastek w 1
    f2 = x -> x^3 - x
    pf2 = x -> 3x^2 - 1  
    x0_2 = 0.9  
    delta2, epsilon2 = 1e-6, 1e-6
    maxit2 = 100
    r2, fr2, it2, err2 = mstycznych(f2, pf2, x0_2, delta2, epsilon2, maxit2)
    @test abs(r2 - 1.0) < delta2  # Sprawdzenie, czy wynik jest bliski 1.0
    @test abs(fr2) < epsilon2  # Sprawdzenie, czy wartość funkcji w r2 jest bliska 0
    @test err2 == 0  # Sprawdzenie, czy nie wystąpił błąd

    # Test 3: Funkcja x^2 + 1, brak pierwiastków w R
    f3 = x -> x^2 + 1
    pf3 = x -> 2x
    x0_3 = 0.0
    delta3, epsilon3 = 1e-6, 1e-6
    maxit3 = 100
    r3, fr3, it3, err3 = mstycznych(f3, pf3, x0_3, delta3, epsilon3, maxit3)
    @test err3 == 2  # Sprawdzenie, czy zwrócono błąd (pochodna bliska zeru)

    # Test 4: Funkcja liniowa x - 5, pierwiastek w 5
    f4 = x -> x - 5
    pf4 = x -> 1 
    x0_4 = 4.0  
    delta4, epsilon4 = 1e-6, 1e-6
    maxit4 = 100
    r4, fr4, it4, err4 = mstycznych(f4, pf4, x0_4, delta4, epsilon4, maxit4)
    @test abs(r4 - 5.0) < delta4  # Sprawdzenie, czy wynik jest bliski 5.0
    @test abs(fr4) < epsilon4  # Sprawdzenie, czy wartość funkcji w r4 jest bliska 0
    @test err4 == 0  # Sprawdzenie, czy nie wystąpił błąd


end
@testset "Testy funkcji msiecznych" begin

    # Test 1: Funkcja x^2 - 2, pierwiastek w sqrt(2)
    f1 = x -> x^2 - 2
    x0_1, x1_1 = 0.0, 2.0  
    delta1, epsilon1 = 1e-7, 1e-7
    maxit1 = 100
    r1, fr1, it1, err1 = msiecznych(f1, x0_1, x1_1, delta1, epsilon1, maxit1)
    @test abs(r1 - sqrt(2)) < delta1  # Sprawdzenie, czy wynik jest bliski sqrt(2)
    @test abs(fr1) < epsilon1  # Sprawdzenie, czy wartość funkcji w r1 jest bliska 0
    @test err1 == 0  # Sprawdzenie, czy nie wystąpił błąd

    # Test 2: Funkcja x^3 - x, pierwiastek w 1
    f2 = x -> x^3 - x
    x0_2, x1_2 = 0.5, 1.5 
    delta2, epsilon2 = 1e-6, 1e-6
    maxit2 = 100
    r2, fr2, it2, err2 = msiecznych(f2, x0_2, x1_2, delta2, epsilon2, maxit2)
    @test abs(r2 - 1.0) < delta2  # Sprawdzenie, czy wynik jest bliski 1.0
    @test abs(fr2) < epsilon2  # Sprawdzenie, czy wartość funkcji w r2 jest bliska 0
    @test err2 == 0  # Sprawdzenie, czy nie wystąpił błąd

    # Test 3: Funkcja x^2 + 1, brak pierwiastków w R
    f3 = x -> x^2 + 1
    x0_3, x1_3 = -1.0, 1.0 
    delta3, epsilon3 = 1e-6, 1e-6
    maxit3 = 100
    r3, fr3, it3, err3 = msiecznych(f3, x0_3, x1_3, delta3, epsilon3, maxit3)
    @test err3 == 1  # Sprawdzenie, czy zwrócono błąd (brak zbieżności)

    # Test 4: Funkcja liniowa x - 5, pierwiastek w 5
    f4 = x -> x - 5
    x0_4, x1_4 = 4.0, 6.0  
    delta4, epsilon4 = 1e-6, 1e-6
    maxit4 = 100
    r4, fr4, it4, err4 = msiecznych(f4, x0_4, x1_4, delta4, epsilon4, maxit4)
    @test abs(r4 - 5.0) < delta4  # Sprawdzenie, czy wynik jest bliski 5.0
    @test abs(fr4) < epsilon4  # Sprawdzenie, czy wartość funkcji w r4 jest bliska 0
    @test err4 == 0  # Sprawdzenie, czy nie wystąpił błąd

    # Test 5: Funkcja x^3, pochodna bliska zeru w x0 = x1 = 0
    f5 = x -> x^3
    x0_5, x1_5 = 0.0, 0.1  
    delta5, epsilon5 = 1e-6, 1e-6
    maxit5 = 100
    r5, fr5, it5, err5 = msiecznych(f5, x0_5, x1_5, delta5, epsilon5, maxit5)
    @test abs(r5) < delta5  # Sprawdzenie, czy wynik jest bliski 0
    @test abs(fr5) < epsilon5  # Sprawdzenie, czy wartość funkcji w r5 jest bliska 0
    @test err5 == 0  # Sprawdzenie, czy metoda zbiega do pierwiastka

end