with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Interfaces; use Interfaces;

with C_Wrapper;
with Go_Wrapper;
with Ada_Wrapper;

procedure Test_All is

   A : constant Unsigned_64 := 30;
   B : constant Unsigned_64 := 18;
   C : constant Unsigned_64 := 6;
   N : constant Unsigned_64 := 77;

   -- zmienne dla Go
   Go_Has_Solution : Integer_32;
   Go_X, Go_Y      : Unsigned_64;

   -- zmienne dla Ady
   Ada_Solution : Ada_Wrapper.Diophantine_Solution_Ada;

   -- zmienne dla C
   C_Solution : C_Wrapper.Diophantine_Solution_C;

   procedure Print_U64 (Value : Unsigned_64) is
   begin
      Put (Unsigned_64'Image (Value));
   end Print_U64;

begin
   -- ADA
   Put_Line ("=== ADA ===");
   Put ("GCD("); Print_U64 (A); Put (", "); Print_U64 (B);
   Put ("): "); Print_U64 (Ada_Wrapper.Gcd_Ada (A, B)); New_Line;

   Put ("Smallest prime divisor of "); Print_U64 (N);
   Put (": "); Print_U64 (Ada_Wrapper.Smallest_Prime_Divisor_Ada (N)); New_Line;

   Put ("Euler's totient of "); Print_U64 (N);
   Put (": "); Print_U64 (Ada_Wrapper.Euler_Totient_Ada (N)); New_Line;

   Ada_Solution := Ada_Wrapper.Solve_Linear_Diophantine_Ada (A, B, C);
   Put ("Diophantine: has_solution:");
   Put (" x="); Print_U64 (Ada_Solution.X);
   Put (" y="); Print_U64 (Ada_Solution.Y); New_Line;

   New_Line;

   -- GO 
   Put_Line ("=== GO ===");
   Put ("GCD("); Print_U64 (A); Put (", "); Print_U64 (B);
   Put ("): "); Print_U64 (Go_Wrapper.Gcd_Go (A, B)); New_Line;

   Put ("Smallest prime divisor of "); Print_U64 (N);
   Put (": "); Print_U64 (Go_Wrapper.Smallest_Prime_Divisor_Go (N)); New_Line;

   Put ("Euler's totient of "); Print_U64 (N);
   Put (": "); Print_U64 (Go_Wrapper.Euler_Totient_Go (N)); New_Line;

   Go_Wrapper.Solve_Linear_Diophantine_Go (A, B, C, Go_Has_Solution, Go_X, Go_Y);
   Put ("Diophantine: has_solution:");
   Put (" x="); Print_U64 (Go_X);
   Put (" y="); Print_U64 (Go_Y); New_Line;

   New_Line;

   -- C
   Put_Line ("=== C ===");
   Put ("GCD("); Print_U64 (A); Put (", "); Print_U64 (B);
   Put ("): "); Print_U64 (C_Wrapper.Gcd_C (A, B)); New_Line;

   Put ("Smallest prime divisor of "); Print_U64 (N);
   Put (": "); Print_U64 (C_Wrapper.Smallest_Prime_Divisor_C (N)); New_Line;

   Put ("Euler's totient of "); Print_U64 (N);
   Put (": "); Print_U64 (C_Wrapper.Euler_Totient_C (N)); New_Line;

   C_Solution := C_Wrapper.Solve_Linear_Diophantine_C (A, B, C);
   Put ("Diophantine: has_solution:");
   Put (" x="); Print_U64 (C_Solution.X);
   Put (" y="); Print_U64 (C_Solution.Y); New_Line;

end Test_All;