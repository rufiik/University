with Interfaces; use Interfaces;

package Ada_Wrapper is

   subtype U64 is Unsigned_64;
   subtype I64 is Integer_64;

   function Gcd_Ada (A, B : U64) return U64;
   function Smallest_Prime_Divisor_Ada (N : U64) return U64;
   function Euler_Totient_Ada (N : U64) return U64;

   type Diophantine_Solution_Ada is record
      Has_Solution : Boolean;
      X, Y         : U64;
   end record;

   function Solve_Linear_Diophantine_Ada
     (A, B, C : U64) return Diophantine_Solution_Ada;

end Ada_Wrapper;