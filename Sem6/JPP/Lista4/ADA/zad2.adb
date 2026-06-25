with Ada.Text_IO; use Ada.Text_IO;
with Ada.Command_Line;
with Ada.Numerics.Discrete_Random;

procedure zad2 is

   type Message is record
      From   : Integer;
      To     : Integer;
      Msg_ID : Integer;
   end record;

   protected Console is
      procedure Log (Text : String);
   end Console;

   protected body Console is
      procedure Log (Text : String) is
      begin
         Put_Line (Text);
      end Log;
   end Console;

   task type User_Task (ID : Integer) is
      entry Init (Total_Msgs : Integer; Max_Users : Integer);
      entry Receive (Msg : Message);
      entry Shutdown; 
      entry Get_Stats (Sent, Received : out Integer);
   end User_Task;

   type User_Access is access User_Task;
   type User_Array is array (Integer range <>) of User_Access;
   type User_Array_Ptr is access User_Array;

   task type Server_Task is
      entry Set_Users (Arr : User_Array_Ptr);
      entry Transfer (Msg : Message);
      entry Notify_Sender_Done; 
      entry Wait_For_Finish;    
   end Server_Task;

   type Server_Access is access Server_Task;
   Global_Server : Server_Access;

   task body Server_Task is
      Users : User_Array_Ptr;
      Senders_Finished : Integer := 0;
      Total_Users : Integer;
   begin
      accept Set_Users (Arr : User_Array_Ptr) do
         Users := Arr;
         Total_Users := Users'Length;
      end Set_Users;

      loop
         select
            accept Transfer (Msg : Message) do
               Console.Log ("[SERVER] Przesyłam: U" & Integer'Image(Msg.From) & " -> U" & Integer'Image(Msg.To));
               Users(Msg.To).Receive (Msg);
            end Transfer;
         or
            accept Notify_Sender_Done do
               Senders_Finished := Senders_Finished + 1;
               Console.Log ("[SERVER] Nadawca potwierdził koniec. Gotowych:" & Integer'Image(Senders_Finished));
            end Notify_Sender_Done;
         or
            when Senders_Finished = Total_Users =>
               accept Wait_For_Finish do
                  Console.Log ("[SERVER] Wszyscy nadawcy skończyli. Zamykam odbiorców...");
                  for I in Users'Range loop
                     Users(I).Shutdown;
                  end loop;
               end Wait_For_Finish;
               exit;
            end select;
      end loop;
   end Server_Task;

   task body User_Task is
      Rec_Count, S_Count : Integer := 0;
      M_Total, M_Max : Integer;
      
      task type Sender_Subtask;
      task body Sender_Subtask is
         subtype DR is Integer range 0 .. M_Max - 1;
         package RG is new Ada.Numerics.Discrete_Random (DR);
         G : RG.Generator;
      begin
         RG.Reset (G);
         for I in 0 .. M_Total - 1 loop
            declare
               Target : DR := RG.Random (G);
            begin
               Console.Log ("[U" & Integer'Image(ID) & "] Wysyłam ID:" & Integer'Image(I) & " do U" & Integer'Image(Target));
               Global_Server.Transfer ((From => ID, To => Integer(Target), Msg_ID => I));
               S_Count := S_Count + 1;
            end;
            delay 0.01;
         end loop;
         Global_Server.Notify_Sender_Done;
      end Sender_Subtask;

      Worker : access Sender_Subtask;
   begin
      accept Init (Total_Msgs : Integer; Max_Users : Integer) do
         M_Total := Total_Msgs;
         M_Max := Max_Users;
      end Init;

      Worker := new Sender_Subtask;

      loop
         select
            accept Receive (Msg : Message) do
               Rec_Count := Rec_Count + 1;
               Console.Log ("         [U" & Integer'Image(ID) & "] ODEBRAŁEM od U" & Integer'Image(Msg.From));
            end Receive;
         or
            accept Shutdown; 
            exit;
         end select;
      end loop;

      accept Get_Stats (Sent, Received : out Integer) do
         Sent := S_Count;
         Received := Rec_Count;
      end Get_Stats;
   end User_Task;

   N : Integer := 5;
   M : Integer := 5;
   Users_List : User_Array_Ptr;
   TR, SV, RV : Integer := 0;

begin
   if Ada.Command_Line.Argument_Count = 2 then
      N := Integer'Value (Ada.Command_Line.Argument (1));
      M := Integer'Value (Ada.Command_Line.Argument (2));
   end if;

   Global_Server := new Server_Task;
   Users_List := new User_Array(0 .. N - 1);

   for I in 0 .. N - 1 loop
      Users_List(I) := new User_Task(I);
   end loop;

   Global_Server.Set_Users (Users_List);

   for I in 0 .. N - 1 loop
      Users_List(I).Init (M, N);
   end loop;

   Global_Server.Wait_For_Finish;

   Console.Log ("");
   Console.Log ("STATYSTYKI KOŃCOWE ");
   for I in 0 .. N - 1 loop
      Users_List(I).Get_Stats (SV, RV);
      TR := TR + RV;
      Console.Log ("Użytkownik" & Integer'Image(I) & ": Odebrał" & Integer'Image(RV));
   end loop;

   Console.Log ("Suma wysłanych:  " & Integer'Image(N * M));
   Console.Log ("Suma odebranych: " & Integer'Image(TR));

end zad2;