with Ada.Containers.Vectors;
with Ada.Numerics.Discrete_Random;

package body DH is
   use Interfaces;

   package Unsigned_Vectors is new Ada.Containers.Vectors
     (Index_Type => Natural, Element_Type => Unsigned_64);

   function Prime_Factors (Num_In : Unsigned_64) return Unsigned_Vectors.Vector is
      Num     : Unsigned_64 := Num_In;
      Factors : Unsigned_Vectors.Vector;
      I       : Unsigned_64 := 2;
   begin
      while I * I <= Num loop
         while Num mod I = 0 loop
            Factors.Append (I);
            Num := Num / I;
         end loop;
         I := I + 1;
      end loop;
      if Num > 1 then
         Factors.Append (Num);
      end if;
      return Factors;
   end Prime_Factors;

   function Power (A : My_Ring.Element; B : Unsigned_64) return My_Ring.Element is
      Result : My_Ring.Element := My_Ring.To_Element (1);
      Base   : My_Ring.Element := A;
      Exp    : Unsigned_64 := B;
   begin
      while Exp > 0 loop
         if Exp mod 2 = 1 then
            Result := Result * Base;
         end if;
         Base := Base * Base;
         Exp := Exp / 2;
      end loop;
      return Result;
   end Power;

   subtype Rand_Range is Unsigned_64 range 1 .. N - 1;
   package Rand is new Ada.Numerics.Discrete_Random (Rand_Range);
   Gen : Rand.Generator;

   function Is_Generator (Candidate : My_Ring.Element; Factors : Unsigned_Vectors.Vector) return Boolean is
   begin
      for Q of Factors loop
         if My_Ring.Value (Power (Candidate, (N - 1) / Q)) = 1 then
            return False;
         end if;
      end loop;
      return True;
   end Is_Generator;

   function Create_Setup return Setup_Type is
      Factors   : Unsigned_Vectors.Vector := Prime_Factors (N - 1);
      Candidate : My_Ring.Element;
      S         : Setup_Type;
   begin
      loop
         Candidate := My_Ring.To_Element (Rand.Random (Gen));
         if Is_Generator (Candidate, Factors) then
            S.Generator := Candidate;
            exit;
         end if;
      end loop;
      return S;
   end Create_Setup;

   function Get_Generator (S : Setup_Type) return My_Ring.Element is
   begin
      return S.Generator;
   end Get_Generator;

   function Create_User (S : Setup_Type'Class) return User_Type is
      U : User_Type;
   begin
      U.Secret := Rand.Random (Gen);
      U.Public_Key := Power (S.Generator, U.Secret);
      U.Key_Set := False;
      return U;
   end Create_User;

   function Get_Public_Key (U : User_Type) return My_Ring.Element is
   begin
      return U.Public_Key;
   end Get_Public_Key;

   procedure Set_Key (U : in out User_Type; Other_Public : My_Ring.Element) is
   begin
      U.Shared_Key := Power (Other_Public, U.Secret);
      U.Key_Set := True;
   end Set_Key;

   function Encrypt (U : User_Type; M : My_Ring.Element) return My_Ring.Element is
   begin
      if not U.Key_Set then
         raise Program_Error with "Klucz szyfrujacy nie zostal ustawiony!";
      end if;
      return M * U.Shared_Key;
   end Encrypt;

   function Decrypt (U : User_Type; C : My_Ring.Element) return My_Ring.Element is
   begin
      if not U.Key_Set then
         raise Program_Error with "Klucz szyfrujacy nie zostal ustawiony!";
      end if;
      return C / U.Shared_Key;
   end Decrypt;

begin
   Rand.Reset (Gen);
end DH;