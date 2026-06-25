with Interfaces; use Interfaces;
with Mathlib;

generic
   N : Interfaces.Unsigned_64; 
package Ring is
   type Element is private;

   function "+" (A, B : Element) return Element;
   function "-" (A, B : Element) return Element;
   function "*" (A, B : Element) return Element;
   function "/" (A, B : Element) return Element;

   function "=" (A, B : Element) return Boolean;
   function "<" (A, B : Element) return Boolean;
   function "<=" (A, B : Element) return Boolean;
   function ">" (A, B : Element) return Boolean;
   function ">=" (A, B : Element) return Boolean;

   function To_Element (V : Interfaces.Unsigned_64) return Element;
   function Value (E : Element) return Interfaces.Unsigned_64;
   function Get_Modulus return Unsigned_64;
   procedure Put (E : Element);

private
   type Element is record
      Val : Interfaces.Unsigned_64;
   end record;
end Ring;