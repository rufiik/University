with Ada.Text_IO;         use Ada.Text_IO;
with Interfaces;          use Interfaces;
with Mathlib;             use Mathlib;

procedure Main is
   A : U64 := 15;
   B : U64 := 10;
   C : U64 := 5;

   G : U64;
   SPD : U64;
   Phi : U64;

   Solution : Diophantine_Solution;
begin
   -- GCD
   G := GCD (A, B);
   Put_Line ("GCD(" & U64'Image(A) & ", " & U64'Image(B) & ") =" & U64'Image(G));

   -- najmniejszy dzielnik pierwszy
   SPD := Smallest_Prime_Divisor (A);
   Put_Line ("Smallest prime divisor of" & U64'Image(A) & " =" & U64'Image(SPD));

   -- funkcja Eulera
   Phi := Euler_Totient (A);
   Put_Line ("Euler totient of" & U64'Image(A) & " =" & U64'Image(Phi));

   -- równanie diofantyczne
   Solution := Solve_Linear_Diophantine (A, B, C);

   if Solution.Has_Solution then
      Put_Line ("Solution to" &
                U64'Image(A) & "x - " &
                U64'Image(B) & "y = " &
                U64'Image(C) & ":");

      Put_Line ("x =" & I64'Image(Solution.X) &
                " y =" & I64'Image(Solution.Y));
   else
      Put_Line ("No solution in natural numbers.");
   end if;

end Main;