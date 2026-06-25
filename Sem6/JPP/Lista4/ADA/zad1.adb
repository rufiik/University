with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Command_Line; use Ada.Command_Line;
with Ada.Calendar; use Ada.Calendar;
with Ada.Numerics.Discrete_Random;

procedure zad1 is
   
   Timeout_Base : constant := 100;
   Timeout_Max : constant := 500;
   
   package Random_Int is new Ada.Numerics.Discrete_Random (Integer);
   use Random_Int;
   Random_Seed : Random_Int.Generator;
   
   -- Logger Task z terminate
   task type Logger_Task is
      entry Log (Id : Integer; Msg : String);
   end Logger_Task;
   
   task body Logger_Task is
      Start_Time : Ada.Calendar.Time := Ada.Calendar.Clock;
   begin
      loop
         select
            accept Log (Id : Integer; Msg : String) do
               declare
                  Now : Ada.Calendar.Time := Ada.Calendar.Clock;
                  Elapsed : Duration := Now - Start_Time;
                  Milliseconds : Integer := Integer (Elapsed * 1000.0);
               begin
                  Put ("[");
                  Put (Milliseconds, Width => 6);
                  Put ("ms] Filozof");
                  Put (Id, Width => 2);
                  Put (": ");
                  Put_Line (Msg);
               end;
            end Log;
         or
            terminate;
         end select;
      end loop;
   end Logger_Task;
   
   function Milliseconds_To_Duration (MS : Integer) return Duration is
   begin
      return Duration (MS) / 1000.0;
   end Milliseconds_To_Duration;
   
   -- Fork
   protected type Fork is
      entry Take (Id : Integer; Was_Dirty : out Boolean);
      entry Try_Take (Id : Integer; Was_Dirty : out Boolean; Success : out Boolean);
      procedure Put (Id : Integer);
      procedure Set_Clean;
      procedure Set_Dirty;
      function Is_Clean return Boolean;
   private
      Locked : Boolean := False;
      Clean : Boolean := True;
      Owner : Integer := -1;
   end Fork;
   
   protected body Fork is
      
      entry Take (Id : Integer; Was_Dirty : out Boolean) when not Locked is
      begin
         Locked := True;
         Was_Dirty := not Clean;
         Owner := Id;
      end Take;
      
      entry Try_Take (Id : Integer; Was_Dirty : out Boolean; Success : out Boolean) 
        when not Locked is
      begin
         Success := True;
         Locked := True;
         Was_Dirty := not Clean;
         Owner := Id;
      end Try_Take;
      
      procedure Put (Id : Integer) is
      begin
         Clean := False;
         Owner := -1;
         Locked := False;
      end Put;
      
      procedure Set_Clean is
      begin
         Clean := True;
      end Set_Clean;
      
      procedure Set_Dirty is
      begin
         Clean := False;
      end Set_Dirty;
      
      function Is_Clean return Boolean is
      begin
         return Clean;
      end Is_Clean;
      
   end Fork;
   
   type Fork_Array is array (Integer range <>) of Fork;
   type Fork_Array_Access is access Fork_Array;
   
   type Failed_Meals_Array is array (Integer range <>) of Integer;
   type Failed_Meals_Access is access Failed_Meals_Array;
   
   type Logger_Access is access all Logger_Task;
   
   -- Philosopher Task
   task type Philosopher_Task is
      entry Start (Id : Integer; Meals_To_Eat : Integer; 
                   Failed : Failed_Meals_Access;
                   Forks : Fork_Array_Access;
                   Log : Logger_Access);
      entry Done;
   end Philosopher_Task;
   
   task body Philosopher_Task is
      My_Id : Integer;
      Target_Meals : Integer;
      Meals_Eaten : Integer := 0;
      Failed_Meals_Ref : Failed_Meals_Access;
      Forks_Ref : Fork_Array_Access;
      Logger_Ref : Logger_Access;
      
      Consecutive_Failures : Integer := 0;
      Total_Failed_Attempts : Integer := 0;
      Local_Failed : Integer := 0;
      
      Think_Min : constant := 50;
      Think_Max : constant := 200;
      Eat_Min : constant := 100;
      Eat_Max : constant := 300;
      Think_Gen : Random_Int.Generator;
      Eat_Gen : Random_Int.Generator;
      
      procedure Think is
         Think_Time : Integer;
      begin
         Think_Time := Random_Int.Random (Think_Gen) mod (Think_Max - Think_Min + 1) + Think_Min;
         
         if Consecutive_Failures > 3 then
            Think_Time := Think_Time / 2;
            Logger_Ref.Log (My_Id, 
               "Mysli... (glodny! czas skrocony do " & Integer'Image (Think_Time) & "ms)");
         elsif Consecutive_Failures > 0 then
            Think_Time := Think_Time * 7 / 10;
            Logger_Ref.Log (My_Id, 
               "Mysli... (lekko glodny, czas " & Integer'Image (Think_Time) & "ms)");
         else
            Logger_Ref.Log (My_Id, 
               "Mysli... (spokojnie, czas " & Integer'Image (Think_Time) & "ms)");
         end if;
         
         delay Duration (Think_Time) / 1000.0;
      end Think;
      
      procedure Eat (Meal_Number : Integer) is
         Eat_Time : Integer;
      begin
         Logger_Ref.Log (My_Id, 
            "JE OBIAD! (posilek " & Integer'Image (Meal_Number + 1) & 
            "/" & Integer'Image (Target_Meals) & ")");
         Eat_Time := Random_Int.Random (Eat_Gen) mod (Eat_Max - Eat_Min + 1) + Eat_Min;
         delay Duration (Eat_Time) / 1000.0;
      end Eat;
      
      function Take_Forks return Boolean is
         N : constant Integer := Forks_Ref'Length;
         Left_Fork : Integer;
         Right_Fork : Integer;
         First, Second : Integer;
         Was_Dirty1, Was_Dirty2 : Boolean;
         Success : Boolean;
         Timeout_Value : Integer;
         Start_Time : Ada.Calendar.Time;
         Elapsed : Duration;
      begin
         Left_Fork := My_Id;
         Right_Fork := (My_Id + 1) mod N;
         
         -- Hierarchia: bierz mniejszy numer pierwszy
         if Left_Fork < Right_Fork then
            First := Left_Fork;
            Second := Right_Fork;
         else
            First := Right_Fork;
            Second := Left_Fork;
         end if;
         
         -- Weź pierwszy widelec
         Forks_Ref (First).Take (My_Id, Was_Dirty1);
         
         -- Dynamiczny timeout
         Timeout_Value := Timeout_Base + (Consecutive_Failures * 20);
         if Timeout_Value > Timeout_Max then
            Timeout_Value := Timeout_Max;
         end if;
         
         if Consecutive_Failures > 5 then
            Logger_Ref.Log (My_Id, 
               "OSTRZEZENIE: Bardzo glodny! timeout=" & Integer'Image (Timeout_Value) & "ms");
         end if;
         
         Start_Time := Ada.Calendar.Clock;
         Elapsed := 0.0;
         
         while Elapsed < Milliseconds_To_Duration (Timeout_Value) loop
            Forks_Ref (Second).Try_Take (My_Id, Was_Dirty2, Success);
            
            if Success then
               if Was_Dirty1 then
                  Forks_Ref (First).Set_Clean;
                  Logger_Ref.Log (My_Id, "Wyczyszczono widelec" & Integer'Image (First));
               end if;
               if Was_Dirty2 then
                  Forks_Ref (Second).Set_Clean;
                  Logger_Ref.Log (My_Id, "Wyczyszczono widelec" & Integer'Image (Second));
               end if;
               
               Consecutive_Failures := 0;
               Logger_Ref.Log (My_Id, 
                  "Udalo sie wzac widelce" & Integer'Image (First) & 
                  " i" & Integer'Image (Second));
               return True;
            end if;
            
            delay 0.01;
            Elapsed := Ada.Calendar.Clock - Start_Time;
         end loop;
         
         -- Timeout
         Forks_Ref (First).Put (My_Id);
         Consecutive_Failures := Consecutive_Failures + 1;
         Total_Failed_Attempts := Total_Failed_Attempts + 1;
         Local_Failed := Local_Failed + 1;
         
         Logger_Ref.Log (My_Id, 
            "Nie udalo sie wzac widelcow (glod poziom" & 
            Integer'Image (Consecutive_Failures) & ", timeout" & 
            Integer'Image (Timeout_Value) & "ms)");
         return False;
      end Take_Forks;
      
   begin
      accept Start (Id : Integer; Meals_To_Eat : Integer; 
                  Failed : Failed_Meals_Access;
                  Forks : Fork_Array_Access;
                  Log : Logger_Access) do
         My_Id := Id;
         Target_Meals := Meals_To_Eat;
         Failed_Meals_Ref := Failed;
         Forks_Ref := Forks;
         Logger_Ref := Log;  
      end Start;
      
      Reset (Think_Gen);
      Reset (Eat_Gen);
      
      while Meals_Eaten < Target_Meals loop
         Think;
         
         if Take_Forks then
            Eat (Meals_Eaten);
            
            declare
               N : constant Integer := Forks_Ref'Length;
               Left : constant Integer := My_Id;
               Right : constant Integer := (My_Id + 1) mod N;
            begin
               Forks_Ref (Right).Put (My_Id);
               Forks_Ref (Left).Put (My_Id);
            end;
            
            Meals_Eaten := Meals_Eaten + 1;
            Logger_Ref.Log (My_Id, 
               "Skonczyl posilek" & Integer'Image (Meals_Eaten) & 
               "/" & Integer'Image (Target_Meals) & 
               " (lacznie glodowal" & Integer'Image (Total_Failed_Attempts) & " razy)");
         else
            Logger_Ref.Log (My_Id, 
               "GLODUJE! (nie udalo sie" & 
               Integer'Image (Local_Failed) & " razy)");
         end if;
      end loop;
      
      -- Zapisz końcową liczbę nieudanych prób
      if Failed_Meals_Ref /= null then
         Failed_Meals_Ref (My_Id) := Local_Failed;
      end if;
      
      Logger_Ref.Log (My_Id, 
         "ZAKONCZYL WSZYSTKIE POSILKI! (glodowal" & 
         Integer'Image (Total_Failed_Attempts) & " razy)");
      
      accept Done;
      
   end Philosopher_Task;
   
   type Philosopher_Access is access Philosopher_Task;
   type Philosopher_Pointer_Array is array (Integer range <>) of Philosopher_Access;
   
   -- Zmienne główne
   Num_Philosophers : Positive;
   Meals_Per_Philosopher : Positive;
   
   Log_Task : aliased Logger_Task;
   
   Total_Failed : Integer := 0;
   Max_Failed, Min_Failed : Integer;
   
begin
   Reset (Random_Seed);
   
   -- Sprawdź argumenty
   if Argument_Count /= 2 then
      Put_Line ("./philosophers <numOfPhilosophers> <mealsPerPhilosopher>");
      return;
   end if;
   
   begin
      Num_Philosophers := Positive'Value (Argument (1));
      Meals_Per_Philosopher := Positive'Value (Argument (2));
   exception
      when others =>
         Put_Line ("./philosophers <numOfPhilosophers> <mealsPerPhilosopher>");
         return;
   end;
   
   -- Dynamiczna alokacja
   declare
      Forks : Fork_Array_Access := new Fork_Array (0 .. Num_Philosophers - 1);
      Failed_Meals : Failed_Meals_Access := new Failed_Meals_Array (0 .. Num_Philosophers - 1);
      Philosophers_Tasks : Philosopher_Pointer_Array (0 .. Num_Philosophers - 1);
   begin
      -- Inicjalizacja Failed_Meals
      for I in 0 .. Num_Philosophers - 1 loop
         Failed_Meals (I) := 0;
      end loop;
      
      New_Line;
      Put_Line ("Liczba filozofow " & Integer'Image (Num_Philosophers));
      Put_Line ("Liczba posilkow na filozofa " & Integer'Image (Meals_Per_Philosopher));
      New_Line;
      
      -- Uruchom filozofów (dynamicznie)
      for I in 0 .. Num_Philosophers - 1 loop
         Philosophers_Tasks (I) := new Philosopher_Task;
         Philosophers_Tasks (I).Start (I, Meals_Per_Philosopher, 
                                       Failed_Meals, 
                                       Forks, 
                                       Log_Task'Access);
      end loop;
      
      -- Czekaj na zakończenie wszystkich filozofów
      for I in 0 .. Num_Philosophers - 1 loop
         Philosophers_Tasks (I).Done;
      end loop;
      
      -- Podsumowanie
      New_Line;
      Put_Line ("=====================================");
      Put_Line ("PODSUMOWANIE:");
      Total_Failed := 0;
      for I in 0 .. Num_Philosophers - 1 loop
         Put ("Filozof" & Integer'Image (I));
         Put (": nie udalo sie zjesc ");
         Put (Failed_Meals (I), Width => 1);
         Put_Line (" razy");
         Total_Failed := Total_Failed + Failed_Meals (I);
      end loop;
      
      New_Line;
      Put ("Czy system byl sprawiedliwy? ");
      
      Max_Failed := 0;
      Min_Failed := Meals_Per_Philosopher;
      for I in 0 .. Num_Philosophers - 1 loop
         if Failed_Meals (I) > Max_Failed then
            Max_Failed := Failed_Meals (I);
         end if;
         if Failed_Meals (I) < Min_Failed then
            Min_Failed := Failed_Meals (I);
         end if;
      end loop;
      
      if Max_Failed - Min_Failed <= 1 then
         Put_Line ("TAK - wszyscy filozofowie glodowali podobnie czesto");
      else
         Put_Line ("NIE - filozofowie glodowali z rozna czestotliwoscia (roznica " & 
                   Integer'Image (Max_Failed - Min_Failed) & ")");
      end if;
      
      Put_Line ("Laczna liczba nieudanych prob: " & Integer'Image (Total_Failed));
      Put_Line ("=====================================");
   
      
  end; -- koniec bloku declare
  
end zad1;