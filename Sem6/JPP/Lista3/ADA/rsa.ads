with Interfaces;
with Ring;

generic
   N : Interfaces.Unsigned_64;
package RSA is
   package My_Ring is new Ring (N);
   use My_Ring;
   
   type RSA_Object is tagged private;
   
   procedure Initialize (Obj : in out RSA_Object; P, Q : Element);
   function Get_Modulo (Obj : RSA_Object) return Interfaces.Unsigned_64;
   function Get_Public_Key (Obj : RSA_Object) return Element;
   function Encrypt (Obj : RSA_Object; M : Element) return Element;
   function Decrypt (Obj : RSA_Object; S : Element) return Element;
   
private
   type RSA_Object is tagged record
      Modulo      : Interfaces.Unsigned_64;
      Phi         : Interfaces.Unsigned_64;
      Public_Key  : Element;
      Private_Key : Element;
   end record;
end RSA;