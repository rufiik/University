with Interfaces;

package Mathlib is
   use Interfaces;

   subtype U64 is Unsigned_64;
   subtype I64 is Integer_64;

   function GCD (A, B : U64) return U64;
   pragma Export (C, GCD, "gcd");

   function Smallest_Prime_Divisor (N : U64) return U64;
   pragma Export (C, Smallest_Prime_Divisor, "smallest_prime_divisor");

   function Euler_Totient (N : U64) return U64;
   pragma Export (C, Euler_Totient, "euler_totient");

   type Diophantine_Solution is record
      Has_Solution : Boolean;
      X, Y         : U64;
   end record;
   function Solve_Linear_Diophantine
     (A, B, C : U64) return Diophantine_Solution;
   pragma Export (C, Solve_Linear_Diophantine, "solve_linear_diophantine");
   type Extended_GCD_Result is record
      GCD : U64;
      X   : I64;
      Y   : I64;
   end record;
   procedure Extended_GCD (A, B : U64; X, Y : out I64);
   pragma Export (C, Extended_GCD, "extended_gcd");
end Mathlib;
