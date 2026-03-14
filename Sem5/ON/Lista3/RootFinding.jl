# Rafal Wochna 279752
module RootFinding

export mbisekcji, mstycznych, msiecznych
"""
Funkcja mbisekcji implementuje metodę bisekcji do znajdowania miejsca zerowego funkcji f w przedziale [a, b].
Założenia:
- f jest funkcją ciągłą w przedziale [a, b].
- f(a) * f(b) < 0, czyli funkcja zmienia znak na końcach przedziału.
Parametry:
- f: funkcja, dla której szukamy miejsca zerowego.
- a: początek przedziału (Float64).
- b: koniec przedziału (Float64).
- delta: dokładność względem argumentu (Float64).
- epsilon: dokładność względem wartości funkcji (Float64).
Zwraca:
- r: przybliżone miejsce zerowe (Float64).
- fr: wartość funkcji w punkcie r (Float64).
- it: liczba wykonanych iteracji (Int).
- error_code: kod błędu (Int), 0 jeśli brak błędu, 1 jeśli funkcja nie zmienia znaku w przedziale.
"""

function mbisekcji(f, a::Float64, b::Float64, delta::Float64, epsilon::Float64)
    fa::Float64 = f(a)
    fb::Float64 = f(b)
    if sign(fa) == sign(fb)
        return (NaN, NaN, 0, 1)  
    end
    it = 0 
    p::Float64 = (b - a)/2
    r::Float64 = a + p
    fr::Float64 = f(r)
    while (b - a) / 2 > delta && abs(fr) > epsilon
        it += 1
        if sign(fa) != sign(fr)  
            b = r
            fb = fr
        else 
            a = r
            fa = fr
        end
        p= (b - a) / 2
        r = a + p
        fr = f(r)
    end

    return (r, fr, it, 0)  
end

"""
Funkcja mstycznych implementuje metodę stycznych (Newtona) do znajdowania miejsca zerowego funkcji f.
Założenia:
- f jest funkcją różniczkowalną w przedziale [a, b].
- f'(r) ≠ 0, gdzie r jest pierwiastkiem funkcji f (pierwiastek jest jednokrotny, tzn. f(r) = 0 i f'(r) ≠ 0).

Parametry:
- f: funkcja, dla której szukamy miejsca zerowego.
- pf: pochodna funkcji f.
- x0: początkowe przybliżenie (Float64).
- delta: dokładność względem argumentu (Float64).
- epsilon: dokładność względem wartości funkcji (Float64).
- maxit: maksymalna liczba iteracji (Int).
Zwraca:
- r: przybliżone miejsce zerowe (Float64).
- fr: wartość funkcji w punkcie r (Float64).
- it: liczba wykonanych iteracji (Int).
- error_code: kod błędu (Int), 0 jeśli brak błędu, 1 jeśli nie osiągnięto dokładności w maxit iteracji, 2 jeśli pochodna bliska zeru.
"""
function mstycznych(f, pf, x0::Float64, delta::Float64, epsilon::Float64, maxit::Int)
    x = x0
    fx::Float64 = f(x)
    it = 0
    if abs(fx) < epsilon
        return (x, fx, it, 0) 
    end
    while abs(fx) > epsilon && it < maxit
        pfx::Float64 = pf(x)  
        if abs(pfx) < delta
            return (x, fx, it, 2)  
        end

        x_new::Float64 = x - fx / pfx
        if abs(x_new - x) < delta
            return (x_new, f(x_new), it + 1, 0) 
        end
        x = x_new
        fx = f(x)
        it += 1
    end
    if it >= maxit
        return (x, fx, it, 1) 
    end
    return (x, fx, it, 0)
end
"""
Funkcja msiecznych implementuje metodę siecznych do znajdowania miejsca zerowego funkcji f.
Założenia:
- f jest funkcją ciągłą w przedziale [x0, x1].
Parametry:
- f: funkcja, dla której szukamy miejsca zerowego.
- x0: pierwsze początkowe przybliżenie (Float64).
- x1: drugie początkowe przybliżenie (Float64).
- delta: dokładność względem argumentu (Float64).
- epsilon: dokładność względem wartości funkcji (Float64).
- maxit: maksymalna liczba iteracji (Int).
Zwraca:
- r: przybliżone miejsce zerowe (Float64).
- fr: wartość funkcji w punkcie r (Float64).
- it: liczba wykonanych iteracji (Int).
- error_code: kod błędu (Int), 0 jeśli brak błędu, 1 jeśli nie osiągnięto dokładności w maxit iteracji lub dzielenie przez wartość bliską zeru.

"""
function msiecznych(f, x0::Float64, x1::Float64, delta::Float64, epsilon::Float64, maxit::Int)
    fx0::Float64 = f(x0)
    fx1::Float64 = f(x1)
    it = 0
    if abs(fx0) < epsilon
        return (x0, fx0, it, 0)  
    end
    if abs(fx1) < epsilon
        return (x1, fx1, it, 0)  
    end
    while it < maxit
        if abs(fx0) > abs(fx1)
             x0, x1 = x1, x0  
            fx0, fx1 = fx1, fx0  
        end
        if abs(fx1 - fx0) < epsilon 
            return (x1, fx1, it, 1)  
        end
        x_new::Float64 = x1 - fx1 * (x1 - x0) / (fx1 - fx0)
        fx_new::Float64 = f(x_new)

        if abs(fx_new) < epsilon || abs(x_new - x1) < delta
            return (x_new, fx_new, it + 1, 0)  
        end

        x0, x1 = x1, x_new
        fx0, fx1 = fx1, fx_new
        it += 1
    end

    return (x1, fx1, it, 1)  
end

end  # module RootFinding
