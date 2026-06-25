with Ada.Text_IO; use Ada.Text_IO;
with Interfaces;  use Interfaces;
with RSA;

procedure Main is
   N : constant Unsigned_64 := 10007 * 10009;
   
   package My_RSA is new RSA (N);
   use My_RSA;
   use My_RSA.My_Ring;
   
   P : Element := To_Element(10007);
   Q : Element := To_Element(10009);
   Message : Element := To_Element(12345);
   Encrypted, Decrypted : Element;
   Obj : RSA_Object;
   
begin
   Initialize(Obj, P, Q);
   
   Put("Public Key: ");
   Put(Unsigned_64'Image(Value(Get_Public_Key(Obj))));
   New_Line;
   
   Put("Modulo: ");
   Put(Unsigned_64'Image(Get_Modulo(Obj)));
   New_Line;
   
   Put("Original Message: ");
   Put(Value(Message)'Image);
   New_Line;
   
   Encrypted := Encrypt(Obj, Message);
   Put("Encrypted: ");
   Put(Value(Encrypted)'Image);
   New_Line;
   
   Decrypted := Decrypt(Obj, Encrypted);
   Put("Decrypted: ");
   Put(Value(Decrypted)'Image);
   New_Line;
   
end Main;