with Ada.Text_IO; use Ada.Text_IO;
with Interfaces; use Interfaces;
with Mathlib;

package body Ring is

   function To_Element (V : Unsigned_64) return Element is
   begin
      return (Val => V mod N);
   end To_Element;

   function Value (E : Element) return Unsigned_64 is
   begin
      return E.Val;
   end Value;

   function "+" (A, B : Element) return Element is
   begin
      return To_Element (A.Val + B.Val);
   end "+";

   function "-" (A, B : Element) return Element is
   begin
      return To_Element ((N + A.Val - B.Val) mod N);
   end "-";

   function "*" (A, B : Element) return Element is
   begin
      return To_Element (A.Val * B.Val);
   end "*";

   function "/" (A, B : Element) return Element is
      Sol : Mathlib.Diophantine_Solution;
      Inv : Unsigned_64;
   begin
      Sol := Mathlib.Solve_Linear_Diophantine (B.Val, N, 1);
      if not Sol.Has_Solution then
         raise Constraint_Error with "Brak odwrotności (dzielenie przez element nieodwracalny)";
      end if;
      Inv := Sol.X mod N;
      return To_Element (A.Val * Inv);
   end "/";

   function "=" (A, B : Element) return Boolean is
   begin
      return A.Val = B.Val;
   end "=";

   function "<" (A, B : Element) return Boolean is
   begin
      return A.Val < B.Val;
   end "<";

   function "<=" (A, B : Element) return Boolean is
   begin
      return A.Val <= B.Val;
   end "<=";

   function ">" (A, B : Element) return Boolean is
   begin
      return A.Val > B.Val;
   end ">";

   function ">=" (A, B : Element) return Boolean is
   begin
      return A.Val >= B.Val;
   end ">=";

   procedure Put (E : Element) is
   begin
      Put ("mod ");
      Put (N'Image);
      Put (": ");
      Put (Unsigned_64'Image(E.Val));
   end Put;
    function Get_Modulus return Unsigned_64 is
   begin
      return N;
   end Get_Modulus;

end Ring;