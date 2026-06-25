with Ada.Text_IO; use Ada.Text_IO;
with Interfaces; use Interfaces;
with Ring;
procedure Test_Ring is
   package R7 is new Ring (7);
   use R7;
   package R8 is new Ring (8);
   A, B, C : Element;
   D       : Element;
   X : R8.Element;
   Y : R8.Element;
   
begin
   -- Testy dla Ring<7>
   New_Line;
   Put_Line("=== Testy dla Ring<7> ===");
   
   A := To_Element (3);
   B := A;
   C := To_Element (0);
   Put ("a = "); Put (A); Put (", b = "); Put (B); Put (", c = "); Put (C); New_Line;
   
   Put_Line ("a.get = " & Unsigned_64'Image(Value(A)));
   
   Put ("a + b = "); Put (A + B); New_Line;
   Put ("a - b = "); Put (A - B); New_Line;
   Put ("a * b = "); Put (A * B); New_Line;
   Put ("b / a = "); Put (B / A); New_Line;
   
   D := To_Element (2);
   D := D + A;
   Put ("d += a -> d = "); Put (D); New_Line;
   D := D - A;
   Put ("d -= a -> d = "); Put (D); New_Line;
   D := D * A;
   Put ("d *= a -> d = "); Put (D); New_Line;
   D := D / A;
   Put ("d /= a -> d = "); Put (D); New_Line;
   
   Put_Line ("a = b? " & Boolean'Image (A = B));
   Put_Line ("a /= b? " & Boolean'Image (A /= B));
   Put_Line ("a < b? " & Boolean'Image (A < B));
   Put_Line ("a > b? " & Boolean'Image (A > B));
   Put_Line ("a <= b? " & Boolean'Image (A <= B));
   Put_Line ("a >= b? " & Boolean'Image (A >= B));
   
   Put ("Wypisanie a przez Put: "); Put (A); New_Line;
   
   begin
      Put ("a / c = "); Put (A / C); New_Line;
   exception
      when Constraint_Error => Put_Line ("Wyjątek przy dzieleniu przez zero");
   end;
   
   New_Line;
   Put_Line("=== Testy dla Ring<8> ===");
   
   declare
      use R8;  
   begin
      X := To_Element (2);
      Y := To_Element (4);
      
      Put ("x = "); Put (X); New_Line;
      Put ("y = "); Put (Y); New_Line;
      
      Put ("y / x = ");
      Put (Y / X); 
      New_Line;
   
   end;
   
   New_Line;
   
end Test_Ring;