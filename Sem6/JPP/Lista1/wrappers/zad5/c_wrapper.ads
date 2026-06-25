with Interfaces; use Interfaces;

package C_Wrapper is

   subtype U64 is Unsigned_64;
   subtype I64 is Integer_64;

   function Gcd_C (A, B : U64) return U64;
   function Smallest_Prime_Divisor_C (N : U64) return U64;
   function Euler_Totient_C (N : U64) return U64;

   type Diophantine_Solution_C is record
      Has_Solution : Integer_32;  -- int z C
      X, Y         : U64;
   end record;
   pragma Convention (C, Diophantine_Solution_C);

   function Solve_Linear_Diophantine_C
     (A, B, C : U64) return Diophantine_Solution_C;

private
   pragma Import (C, Gcd_C, "gcd");
   pragma Import (C, Smallest_Prime_Divisor_C, "smallest_prime_divisor");
   pragma Import (C, Euler_Totient_C, "euler_totient");
   pragma Import (C, Solve_Linear_Diophantine_C, "solve_linear_diophantine");

end C_Wrapper;