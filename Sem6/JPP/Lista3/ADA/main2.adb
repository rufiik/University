with Ada.Text_IO; use Ada.Text_IO;
with Interfaces;  use Interfaces;
with DH;

procedure Main2 is
   N : constant Unsigned_64 := 1234567891;
   
   package My_DH is new DH (N);
   use My_DH;
   use My_DH.My_Ring;

   Setup : Setup_Type := Create_Setup;
   User1 : User_Type  := Create_User (Setup);
   User2 : User_Type  := Create_User (Setup);

   U1_Pub : My_Ring.Element := Get_Public_Key (User1);
   U2_Pub : My_Ring.Element := Get_Public_Key (User2);

   Message   : My_Ring.Element := To_Element (12345);
   Encrypted : My_Ring.Element;
   Decrypted : My_Ring.Element;
begin

   Set_Key (User1, U2_Pub);
   Set_Key (User2, U1_Pub);


   Encrypted := Encrypt (User1, Message);
   Decrypted := Decrypt (User2, Encrypted);

   Put ("Generator: "); 
   My_Ring.Put (Get_Generator (Setup)); 
   New_Line;
   
   Put ("User1 Public Key: "); 
   My_Ring.Put (U1_Pub); 
   New_Line;
   
   Put ("User2 Public Key: "); 
   My_Ring.Put (U2_Pub); 
   New_Line;
   
   Put ("Original Message: "); 
   My_Ring.Put (Message); 
   New_Line;
   
   Put ("Encrypted Message: "); 
   My_Ring.Put (Encrypted); 
   New_Line;
   
   Put ("Decrypted Message: "); 
   My_Ring.Put (Decrypted); 
   New_Line;
end Main2;