with Interfaces; use Interfaces;

package Mathlib is

   subtype U64 is Unsigned_64;
   subtype I64 is Integer_64;

   function Gcd (A, B : U64) return U64;
   function Smallest_Prime_Divisor (N : U64) return U64;
   function Euler_Totient (N : U64) return U64;

   type Diophantine_Solution is record
      Has_Solution : Integer_32;  -- int z C
      X, Y         : U64;
   end record;
   pragma Convention (C, Diophantine_Solution);

   function Solve_Linear_Diophantine (A, B, C : U64) return Diophantine_Solution;

private
   pragma Import (C, Gcd, "gcd");
   pragma Import (C, Smallest_Prime_Divisor, "smallest_prime_divisor");
   pragma Import (C, Euler_Totient, "euler_totient");
   pragma Import (C, Solve_Linear_Diophantine, "solve_linear_diophantine");

end Mathlib;