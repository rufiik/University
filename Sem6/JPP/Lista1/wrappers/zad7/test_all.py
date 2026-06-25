#!/usr/bin/env python3
"""
Program testujący biblioteki z C, Ada i Go
"""
import sys
import os

# Dodaj ścieżki do wrapperów
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from c_wrapper import CWrapper
from ada_wrapper import AdaWrapper
from go_wrapper import GoWrapper


def test_library(name: str, wrapper, a: int, b: int, c: int, n: int):
    print(f"\n=== {name.upper()} ===")
    
    # Test GCD
    gcd_result = wrapper.gcd(a, b)
    print(f"GCD({a}, {b}): {gcd_result}")
    
    # Test Smallest Prime Divisor
    spd_result = wrapper.smallest_prime_divisor(n)
    print(f"Smallest prime divisor of {n}: {spd_result}")
    
    # Test Euler Totient
    et_result = wrapper.euler_totient(n)
    print(f"Euler's totient of {n}: {et_result}")
    
    # Test Diophantine
    diophantine = wrapper.solve_linear_diophantine(a, b, c)
    print(f"Diophantine: has_solution={diophantine.has_solution} "
          f"x={diophantine.x} y={diophantine.y}")


def main():
    a = 30
    b = 18
    c = 6
    n = 77
    
    print(f"Parametry testowe: a={a}, b={b}, c={c}, n={n}")
    
    # Ścieżki do bibliotek
    lib_paths = {
        'C': "../../C/libmathC.so",
        'Ada': "../../ADA/libmathADA.so",
        'Go': "../../GO/libmathGO.so"
    }
    
    # Próba załadowania każdej biblioteki
    wrappers = []
    
    # Biblioteka C
    try:
        c_wrapper = CWrapper(lib_paths['C'])
        wrappers.append(("C", c_wrapper))
        print(f"\n Biblioteka C załadowana z {lib_paths['C']}")
    except Exception as e:
        print(f"\n Błąd ładowania biblioteki C: {e}")
        # Spróbuj w bieżącym katalogu
        try:
            c_wrapper = CWrapper("./libmathC.so")
            wrappers.append(("C", c_wrapper))
            print(f" Biblioteka C załadowana z ./libmathC.so")
        except Exception as e2:
            print(f" Nadal nie działa: {e2}")
    
    # Biblioteka Ada
    try:
        ada_wrapper = AdaWrapper(lib_paths['Ada'])
        wrappers.append(("Ada", ada_wrapper))
        print(f" Biblioteka Ada załadowana z {lib_paths['Ada']}")
    except Exception as e:
        print(f"\n Błąd ładowania biblioteki Ada: {e}")
        try:
            ada_wrapper = AdaWrapper("./libmathADA.so")
            wrappers.append(("Ada", ada_wrapper))
            print(f" Biblioteka Ada załadowana z ./libmathADA.so")
        except Exception as e2:
            print(f" Nadal nie działa: {e2}")
    
    # Biblioteka Go
    try:
        go_wrapper = GoWrapper(lib_paths['Go'])
        wrappers.append(("Go", go_wrapper))
        print(f" Biblioteka Go załadowana z {lib_paths['Go']}")
    except Exception as e:
        print(f"\n Błąd ładowania biblioteki Go: {e}")
        try:
            go_wrapper = GoWrapper("./libmathGO.so")
            wrappers.append(("Go", go_wrapper))
            print(f" Biblioteka Go załadowana z ./libmathGO.so")
        except Exception as e2:
            print(f"Nadal nie działa: {e2}")
    
    # Testowanie załadowanych bibliotek
    if not wrappers:
        print("\n Żadna biblioteka nie została załadowana!")
        sys.exit(1)
    
    for name, wrapper in wrappers:
        test_library(name, wrapper, a, b, c, n)


if __name__ == "__main__":
    main()