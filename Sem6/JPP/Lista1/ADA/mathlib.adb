with Interfaces;

package body Mathlib is
   use Interfaces;

   procedure Extended_GCD
     (A, B : U64; X, Y : out I64) is
      X1, Y1 : I64;
   begin
      if B = 0 then
         X := 1;
         Y := 0;
      else
         Extended_GCD (B, A mod B, X1, Y1);
         X := Y1;
         Y := X1 - I64 (A / B) * Y1;
      end if;
   end Extended_GCD;

   function GCD (A, B : U64) return U64 is
      X : U64 := A;
      Y : U64 := B;
      T : U64;
   begin
      if X = 0 then
         return Y;
      elsif Y = 0 then
         return X;
      end if;

      while Y /= 0 loop
         T := Y;
         Y := X mod Y;
         X := T;
      end loop;

      return X;
   end GCD;

   function Smallest_Prime_Divisor (N : U64) return U64 is
      I : U64 := 3;
   begin
      if N <= 1 then
         return 0;
      elsif N mod 2 = 0 then
         return 2;
      end if;

      while I * I <= N loop
         if N mod I = 0 then
            return I;
         end if;
         I := I + 2;
      end loop;

      return N;
   end Smallest_Prime_Divisor;

   function Euler_Totient (N : U64) return U64 is
      Result : U64 := N;
      X      : U64 := N;
      I      : U64 := 3;
   begin
      if X = 0 then
         return 0;
      end if;

      if X mod 2 = 0 then
         while X mod 2 = 0 loop
            X := X / 2;
         end loop;
         Result := Result - Result / 2;
      end if;

      while I * I <= X loop
         if X mod I = 0 then
            while X mod I = 0 loop
               X := X / I;
            end loop;
            Result := Result - Result / I;
         end if;
         I := I + 2;
      end loop;

      if X > 1 then
         Result := Result - Result / X;
      end if;

      return Result;
   end Euler_Totient;

   function Solve_Linear_Diophantine
     (A, B, C : U64) return Diophantine_Solution is

      Solution : Diophantine_Solution :=
        (Has_Solution => False, X => 0, Y => 0);

      G : U64 := GCD (A, B);
      X0, Y0 : I64;
      Factor : I64;
      Step_X, Step_Y : I64;
      T_Min_X, T_Min_Y, T : I64;

   begin
      if G = 0 or else C mod G /= 0 then
         return Solution;
      end if;

      if B = 0 then
         if C mod A = 0 then
            Solution.Has_Solution := True;
            Solution.X := U64 (C / A);
            Solution.Y := 0;
         end if;
         return Solution;
      end if;

      Extended_GCD (A, B, X0, Y0);

      Factor := I64 (C / G);
      X0 := X0 * Factor;
      Y0 := Y0 * Factor;

      Y0 := -Y0;

      Step_X := I64 (B / G);
      Step_Y := I64 (A / G);

      if Step_X = 0 or else Step_Y = 0 then
         return Solution;
      end if;

      -- x >= 0
      if Step_X > 0 then
         T_Min_X := (-X0 + Step_X - 1) / Step_X;
      else
         T_Min_X := (-X0) / Step_X;
      end if;

      -- y >= 0
      if Step_Y > 0 then
         T_Min_Y := (-Y0 + Step_Y - 1) / Step_Y;
      else
         T_Min_Y := (-Y0) / Step_Y;
      end if;

      if T_Min_X > T_Min_Y then
         T := T_Min_X;
      else
         T := T_Min_Y;
      end if;

      X0 := X0 + T * Step_X;
      Y0 := Y0 + T * Step_Y;

      if X0 >= 0 and then Y0 >= 0 then
         Solution.Has_Solution := True;
         Solution.X := U64 (X0); 
         Solution.Y := U64 (Y0); 
      end if;

      return Solution;
   end Solve_Linear_Diophantine;

end Mathlib;