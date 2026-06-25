with Interfaces;
with Ring;

generic
   N : Interfaces.Unsigned_64;
package DH is
   package My_Ring is new Ring (N);
   use My_Ring;
   type Setup_Type is tagged private;
   
   function Create_Setup return Setup_Type;
   function Get_Generator (S : Setup_Type) return My_Ring.Element;
   function Power (A : My_Ring.Element; B : Interfaces.Unsigned_64) return My_Ring.Element;
   type User_Type is tagged private;
   
   function Create_User (S : Setup_Type'Class) return User_Type;
   function Get_Public_Key (U : User_Type) return My_Ring.Element;
   procedure Set_Key (U : in out User_Type; Other_Public : My_Ring.Element);
   function Encrypt (U : User_Type; M : My_Ring.Element) return My_Ring.Element;
   function Decrypt (U : User_Type; C : My_Ring.Element) return My_Ring.Element;

private
   type Setup_Type is tagged record
      Generator : My_Ring.Element;
   end record;

   type User_Type is tagged record
      Secret     : Interfaces.Unsigned_64;
      Public_Key : My_Ring.Element;
      Shared_Key : My_Ring.Element;
      Key_Set    : Boolean := False;
   end record;
end DH;