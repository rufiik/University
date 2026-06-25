import ctypes
import os
from typing import Tuple, NamedTuple

class DiophantineSolution(NamedTuple):
    has_solution: bool
    x: int
    y: int

class AdaWrapper:
    
    def __init__(self, lib_path: str = "../../ADA/libmathADA.so"):
        if not os.path.exists(lib_path):
            lib_path = "./libmathADA.so"
        
        self.lib = ctypes.CDLL(lib_path)

        self._setup_functions()
    
    def _setup_functions(self):
        self.lib.gcd.argtypes = [ctypes.c_uint64, ctypes.c_uint64]
        self.lib.gcd.restype = ctypes.c_uint64
        

        self.lib.smallest_prime_divisor.argtypes = [ctypes.c_uint64]
        self.lib.smallest_prime_divisor.restype = ctypes.c_uint64
        

        self.lib.euler_totient.argtypes = [ctypes.c_uint64]
        self.lib.euler_totient.restype = ctypes.c_uint64
        
        class AdaDiophantineSolutionStruct(ctypes.Structure):
            _fields_ = [
                ("has_solution", ctypes.c_int),
                ("x", ctypes.c_uint64),
                ("y", ctypes.c_uint64)
            ]
        
        self.AdaDiophantineSolutionStruct = AdaDiophantineSolutionStruct
        self.lib.solve_linear_diophantine.argtypes = [
            ctypes.c_uint64, ctypes.c_uint64, ctypes.c_uint64
        ]
        self.lib.solve_linear_diophantine.restype = AdaDiophantineSolutionStruct
    
    def gcd(self, a: int, b: int) -> int:
        return self.lib.gcd(ctypes.c_uint64(a), ctypes.c_uint64(b))
    
    def smallest_prime_divisor(self, n: int) -> int:
        return self.lib.smallest_prime_divisor(ctypes.c_uint64(n))
    
    def euler_totient(self, n: int) -> int:
        return self.lib.euler_totient(ctypes.c_uint64(n))
    
    def solve_linear_diophantine(self, a: int, b: int, c: int) -> DiophantineSolution:
        result = self.lib.solve_linear_diophantine(
            ctypes.c_uint64(a),
            ctypes.c_uint64(b),
            ctypes.c_uint64(c)
        )
        return DiophantineSolution(
            has_solution=bool(result.has_solution),
            x=result.x,
            y=result.y
        )