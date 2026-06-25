with Interfaces; use Interfaces;

package Go_Wrapper is

   subtype U64 is Unsigned_64;
   subtype I64 is Integer_64;

   function Gcd_Go (A, B : U64) return U64;
   function Smallest_Prime_Divisor_Go (N : U64) return U64;
   function Euler_Totient_Go (N : U64) return U64;

   procedure Solve_Linear_Diophantine_Go
     (A, B, C      : U64;
      Has_Solution : out Integer_32;
      X, Y         : out U64);

private
   pragma Import (C, Gcd_Go, "GCD");
   pragma Import (C, Smallest_Prime_Divisor_Go, "SmallestPrimeDivisor");
   pragma Import (C, Euler_Totient_Go, "EulerTotient");
   pragma Import (C, Solve_Linear_Diophantine_Go, "SolveLinearDiophantine");

end Go_Wrapper;