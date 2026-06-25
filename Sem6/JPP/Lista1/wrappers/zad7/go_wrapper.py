import ctypes
import os
from typing import Tuple, NamedTuple

class DiophantineSolution(NamedTuple):

    has_solution: bool
    x: int
    y: int

class GoWrapper:
    
    def __init__(self, lib_path: str = "../../GO/libmathGO.so"):
        if not os.path.exists(lib_path):
            lib_path = "./libmathGO.so"
        
        self.lib = ctypes.CDLL(lib_path)
        self._setup_functions()
    
    def _setup_functions(self):
        self.lib.GCD.argtypes = [ctypes.c_uint64, ctypes.c_uint64]
        self.lib.GCD.restype = ctypes.c_uint64
        
        self.lib.SmallestPrimeDivisor.argtypes = [ctypes.c_uint64]
        self.lib.SmallestPrimeDivisor.restype = ctypes.c_uint64
        
        self.lib.EulerTotient.argtypes = [ctypes.c_uint64]
        self.lib.EulerTotient.restype = ctypes.c_uint64
        
        self.lib.SolveLinearDiophantine.argtypes = [
            ctypes.c_uint64,
            ctypes.c_uint64,
            ctypes.c_uint64,
            ctypes.POINTER(ctypes.c_int),
            ctypes.POINTER(ctypes.c_uint64),
            ctypes.POINTER(ctypes.c_uint64)
        ]
        self.lib.SolveLinearDiophantine.restype = None
    
    def gcd(self, a: int, b: int) -> int:
        return self.lib.GCD(ctypes.c_uint64(a), ctypes.c_uint64(b))
    
    def smallest_prime_divisor(self, n: int) -> int:
        return self.lib.SmallestPrimeDivisor(ctypes.c_uint64(n))
    
    def euler_totient(self, n: int) -> int:
        return self.lib.EulerTotient(ctypes.c_uint64(n))
    
    def solve_linear_diophantine(self, a: int, b: int, c: int) -> DiophantineSolution:
        has_solution = ctypes.c_int(0)
        x = ctypes.c_uint64(0)
        y = ctypes.c_uint64(0)
        
        self.lib.SolveLinearDiophantine(
            ctypes.c_uint64(a),
            ctypes.c_uint64(b),
            ctypes.c_uint64(c),
            ctypes.byref(has_solution),
            ctypes.byref(x),
            ctypes.byref(y)
        )
        
        return DiophantineSolution(
            has_solution=bool(has_solution.value),
            x=x.value,
            y=y.value
        )