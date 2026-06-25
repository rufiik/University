with Interfaces;
with Mathlib; use Mathlib;

package body RSA is
   use Interfaces;
   
   function GCD (A, B : Unsigned_64) return Unsigned_64 is
      X : Unsigned_64 := A;
      Y : Unsigned_64 := B;
      T : Unsigned_64;
   begin
      while Y /= 0 loop
         T := Y;
         Y := X mod Y;
         X := T;
      end loop;
      return X;
   end GCD;
   
   function Choose_Public_Key (Phi : Unsigned_64) return Element is
      BEST : constant Unsigned_64 := 65537;
      E : Unsigned_64 := 3;
   begin
      if GCD(BEST, Phi) = 1 then
         return To_Element(BEST);
      end if;
      while GCD(E, Phi) /= 1 loop
         E := E + 2;
      end loop;
      return To_Element(E);
   end Choose_Public_Key;
   
   function Calculate_Private_Key (E : Element; Phi : Unsigned_64) return Element is
      Sol : Diophantine_Solution;
   begin
      Sol := Solve_Linear_Diophantine(Value(E), Phi, 1);
      if not Sol.Has_Solution then
         raise Constraint_Error with "Nie można znaleźć klucza prywatnego";
      end if;
      return To_Element((Sol.X mod Phi + Phi) mod Phi);
   end Calculate_Private_Key;
   
   procedure Initialize (Obj : in out RSA_Object; P, Q : Element) is
   begin
      Obj.Modulo := Value(P) * Value(Q);
      Obj.Phi := (Value(P) - 1) * (Value(Q) - 1);
      Obj.Public_Key := Choose_Public_Key(Obj.Phi);
      Obj.Private_Key := Calculate_Private_Key(Obj.Public_Key, Obj.Phi);
   end Initialize;
   
   function Get_Modulo (Obj : RSA_Object) return Unsigned_64 is
   begin
      return Obj.Modulo;
   end Get_Modulo;
   
   function Get_Public_Key (Obj : RSA_Object) return Element is
   begin
      return Obj.Public_Key;
   end Get_Public_Key;
   
   function Encrypt (Obj : RSA_Object; M : Element) return Element is
      Result : Element := To_Element(1);
      Base   : Element := M;
      Exp    : Unsigned_64 := Value(Obj.Public_Key);
   begin
      while Exp > 0 loop
         if Exp mod 2 = 1 then
            Result := Result * Base;
         end if;
         Base := Base * Base;
         Exp := Exp / 2;
      end loop;
      return Result;
   end Encrypt;
   
   function Decrypt (Obj : RSA_Object; S : Element) return Element is
      Result : Element := To_Element(1);
      Base   : Element := S;
      Exp    : Unsigned_64 := Value(Obj.Private_Key);
   begin
      while Exp > 0 loop
         if Exp mod 2 = 1 then
            Result := Result * Base;
         end if;
         Base := Base * Base;
         Exp := Exp / 2;
      end loop;
      return Result;
   end Decrypt;
   
end RSA;