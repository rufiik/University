with Mathlib;  

package body Ada_Wrapper is

   function Gcd_Ada (A, B : U64) return U64 is
   begin
      return Mathlib.Gcd (A, B);
   end Gcd_Ada;

   function Smallest_Prime_Divisor_Ada (N : U64) return U64 is
   begin
      return Mathlib.Smallest_Prime_Divisor (N);
   end Smallest_Prime_Divisor_Ada;

   function Euler_Totient_Ada (N : U64) return U64 is
   begin
      return Mathlib.Euler_Totient (N);
   end Euler_Totient_Ada;

   function Solve_Linear_Diophantine_Ada
     (A, B, C : U64) return Diophantine_Solution_Ada
   is
      Solution : Mathlib.Diophantine_Solution;
   begin
      Solution := Mathlib.Solve_Linear_Diophantine (A, B, C);
      return (
         Has_Solution => Solution.Has_Solution /= 0,
         X            => Solution.X,
         Y            => Solution.Y
      );
   end Solve_Linear_Diophantine_Ada;

end Ada_Wrapper;