with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Float_Random; use Ada.Numerics.Float_Random;
with Random_Seeds; use Random_Seeds;
with Ada.Real_Time; use Ada.Real_Time;
with Ada.Synchronous_Task_Control; use Ada.Synchronous_Task_Control;
with Ada.Unchecked_Deallocation;
procedure  Mutex_Template is


-- Processes 

  Nr_Of_Processes : constant Integer :=5;

  Min_Steps : constant Integer := 50 ;
  Max_Steps : constant Integer := 100 ;

  Min_Delay : constant Duration := 0.1;
  Max_Delay : constant Duration := 0.5;

-- States of a Process 

type Process_State is (
  Local_Section,
  Entry_Protocol_1,
  Entry_Protocol_2,
  Entry_Protocol_3,
  Entry_Protocol_4,
  Critical_Section,
  Exit_Protocol
);
Flag : array (0 .. Nr_Of_Processes-1) of Integer := (others => 0);
-- Możesz dodać prostą ochronę na flagi (opcjonalnie, jeśli chcesz mieć atomowość):
protected Flag_Access is
  procedure Set_Flag(I : Integer; V : Integer);
  function Get_Flag(I : Integer) return Integer;
end Flag_Access;

protected body Flag_Access is
  procedure Set_Flag(I : Integer; V : Integer) is
  begin
    Flag(I) := V;
  end;
  function Get_Flag(I : Integer) return Integer is
  begin
    return Flag(I);
  end;
end Flag_Access;

-- 2D Board display board

  Board_Width  : constant Integer := Nr_Of_Processes;
  Board_Height : constant Integer := Process_State'Pos( Process_State'Last ) + 1;

-- Timing

  Start_Time : Time := Clock;  -- global startnig time

-- Random seeds for the tasks' random number generators
 
  Seeds : Seed_Array_Type( 1..Nr_Of_Processes ) := Make_Seeds( Nr_Of_Processes );

-- Types, procedures and functions

  -- Postitions on the board
  type Position_Type is record	
    X: Integer range 0 .. Board_Width - 1; 
    Y: Integer range 0 .. Board_Height - 1; 
  end record;	   

  -- traces of Processes
  type Trace_Type is record 	      
    Time_Stamp:  Duration;	      
    Id : Integer;
    Position: Position_Type;      
    Symbol: Character;	      
  end record;	      

  type Trace_Array_type is  array(0 .. Max_Steps*2) of Trace_Type;

  type Traces_Sequence_Type is record
    Last: Integer := -1;
    Trace_Array: Trace_Array_type ;
  end record; 


  procedure Print_Trace( Trace : Trace_Type ) is
    Symbol : String := ( ' ', Trace.Symbol );
  begin
    Put_Line(
        Duration'Image( Trace.Time_Stamp ) & " " &
        Integer'Image( Trace.Id ) & " " &
        Integer'Image( Trace.Position.X ) & " " &
        Integer'Image( Trace.Position.Y ) & " " &
        ( ' ', Trace.Symbol ) -- print as string to avoid: '
      );
  end Print_Trace;

  procedure Print_Traces( Traces : Traces_Sequence_Type ) is
  begin
    for I in 0 .. Traces.Last loop
      Print_Trace( Traces.Trace_Array( I ) );
    end loop;
  end Print_Traces;


  -- task Printer collects and prints reports of traces and the line with the parameters

  task Printer is
    entry Report( Traces : Traces_Sequence_Type );
  end Printer;
  
  task body Printer is 
  begin
  
    -- Collect and print the traces
    
    for I in 1 .. Nr_Of_Processes loop -- range for TESTS !!!
        accept Report( Traces : Traces_Sequence_Type ) do
          -- Put_Line("I = " & I'Image );
          Print_Traces( Traces );
        end Report;
      end loop;

    -- Prit the line with the parameters needed for display script:

    Put(
      "-1 "&
      Integer'Image( Nr_Of_Processes ) &" "&
      Integer'Image( Board_Width ) &" "&
      Integer'Image( Board_Height ) &" "       
    );
    for I in Process_State'Range loop
      Put( I'Image &";" );
    end loop;
    Put_Line("EXTRA_LABEL;"); -- Place labels with extra info here (e.g. "MAX_TICKET=...;" for Backery). 

  end Printer;


  -- Processes
  type Process_Type is record
    Id: Integer;
    Symbol: Character;
    Position: Position_Type;    
  end record;


  task type Process_Task_Type is	
    entry Init(Id: Integer; Seed: Integer; Symbol: Character);
    entry Start;
  end Process_Task_Type;	

  task body Process_Task_Type is
    G : Generator;
    Process : Process_Type;
    Time_Stamp : Duration;
    Nr_of_Steps: Integer;
    Traces: Traces_Sequence_Type; 

    procedure Store_Trace is
    begin  
      Traces.Last := Traces.Last + 1;
      Traces.Trace_Array( Traces.Last ) := ( 
          Time_Stamp => Time_Stamp,
          Id => Process.Id,
          Position => Process.Position,
          Symbol => Process.Symbol
        );
    end Store_Trace;

    procedure Change_State( State: Process_State ) is
    begin
      Time_Stamp := To_Duration ( Clock - Start_Time ); -- reads global clock
      Process.Position.Y := Process_State'Pos( State );
      Store_Trace;
    end;
    

  begin
    accept Init(Id: Integer; Seed: Integer; Symbol: Character) do
      Reset(G, Seed); 
      Process.Id := Id;
      Process.Symbol := Symbol;
      -- Initial position 
      Process.Position := (
          X => Id,
          Y => Process_State'Pos( LOCAL_SECTION )
        );
      -- Number of steps to be made by the Process  
      Nr_of_Steps := Min_Steps + Integer( Float(Max_Steps - Min_Steps) * Random(G));
      -- Time_Stamp of initialization
      Time_Stamp := To_Duration ( Clock - Start_Time ); -- reads global clock
      Store_Trace; -- store starting position
    end Init;
    
    -- wait for initialisations of the remaining tasks:
    accept Start do
      null;
    end Start;

--    for Step in 0 .. Nr_of_Steps loop
-- ...istniejący kod...
for Step in 0 .. Nr_of_Steps/4 - 1  loop  -- TEST !!!
    Put_Line("Proces" & Integer'Image(Process.Id) & " wchodzi do LOCAL_SECTION");
    delay Min_Delay+(Max_Delay-Min_Delay)*Duration(Random(G));
    Put_Line("Proces" & Integer'Image(Process.Id) & " kończy LOCAL_SECTION");

    -- ENTRY_PROTOCOL_1: flag[self] := 1
    Put_Line("Proces" & Integer'Image(Process.Id) & " ustawia flagę na 1 (ENTRY_PROTOCOL_1)");
    Flag_Access.Set_Flag(Process.Id, 1);
    Change_State(Entry_Protocol_1);

    -- await(all flag[j] in {0,1,2})
    Put_Line("Proces" & Integer'Image(Process.Id) & " zaczyna czekać w ENTRY_PROTOCOL_1");
    declare
        function In_012(V : Integer) return Boolean is (V = 0 or V = 1 or V = 2);
    begin
        loop
            declare
                Ok : Boolean := True;
            begin
                for J in 0 .. Nr_Of_Processes-1 loop
                    if not In_012(Flag_Access.Get_Flag(J)) then
                        Ok := False;
                        Put_Line("Proces" & Integer'Image(Process.Id) & " czeka w ENTRY_PROTOCOL_1 na proces " &
                                 Integer'Image(J) & " (flaga=" & Integer'Image(Flag_Access.Get_Flag(J)) & ")");
                        exit;
                    end if;
                end loop;
                exit when Ok;
                delay 0.001;
            end;
        end loop;
    end;
    Put_Line("Proces" & Integer'Image(Process.Id) & " kończy czekać w ENTRY_PROTOCOL_1");

    -- ENTRY_PROTOCOL_3: flag[self] := 3
    Put_Line("Proces" & Integer'Image(Process.Id) & " ustawia flagę na 3 (ENTRY_PROTOCOL_3)");
    Flag_Access.Set_Flag(Process.Id, 3);
    Change_State(Entry_Protocol_3);

    -- if any flag[j] = 1
    declare
        Any1 : Boolean := False;
    begin
        for J in 0 .. Nr_Of_Processes-1 loop
            if J /= Process.Id and then Flag_Access.Get_Flag(J) = 1 then
                Any1 := True;
                exit;
            end if;
        end loop;
        if Any1 then
            -- flag[self] := 2
            Put_Line("Proces" & Integer'Image(Process.Id) & " ustawia flagę na 2 (ENTRY_PROTOCOL_2)");
            Flag_Access.Set_Flag(Process.Id, 2);
            Change_State(Entry_Protocol_2);

            -- await(no flag[j] = 4)
            Put_Line("Proces" & Integer'Image(Process.Id) & " zaczyna czekać w ENTRY_PROTOCOL_2 na brak flagi 4");
            loop
                declare
                    Found : Boolean := False;
                    Blokujacy : Integer := -1;
                begin
                    for J in 0 .. Nr_Of_Processes-1 loop
                        if Flag_Access.Get_Flag(J) = 4 then
                            Found := True;
                            Blokujacy := J;
                            Put_Line("Proces" & Integer'Image(Process.Id) & " czeka w ENTRY_PROTOCOL_2 na proces " &
                                     Integer'Image(Blokujacy) & " (flaga=4)");
                            exit;
                        end if;
                    end loop;
                    exit when not Found;
                    delay 0.001;
                end;
            end loop;
            Put_Line("Proces" & Integer'Image(Process.Id) & " kończy czekać w ENTRY_PROTOCOL_2");
        end if;
    end;

    -- flag[self] := 4
    Put_Line("Proces" & Integer'Image(Process.Id) & " ustawia flagę na 4 (ENTRY_PROTOCOL_4)");
    Flag_Access.Set_Flag(Process.Id, 4);
    Change_State(Entry_Protocol_4);

    -- await(all flag[j] in {0,1} for j < self)
    Put_Line("Proces" & Integer'Image(Process.Id) & " zaczyna czekać w ENTRY_PROTOCOL_4 na niższych");
    declare
        function In_01(V : Integer) return Boolean is (V = 0 or V = 1);
    begin
        if Process.Id > 0 then
            loop
                declare
                    Ok : Boolean := True;
                begin
                    for J in 0 .. Process.Id-1 loop
                        if not In_01(Flag_Access.Get_Flag(J)) then
                            Ok := False;
                            Put_Line("Proces " & Integer'Image(Process.Id) &
                                     " czeka na nizszy proces " & Integer'Image(J) &
                                     " (flaga=" & Integer'Image(Flag_Access.Get_Flag(J)) & ")");
                            -- Diagnostyka: wypisz flagi wszystkich procesów
                            Put("Stany flag: ");
                            for K in 0 .. Nr_Of_Processes-1 loop
                                Put(Integer'Image(K) & "=" & Integer'Image(Flag_Access.Get_Flag(K)) & " ");
                            end loop;
                            New_Line;
                            exit;
                        end if;
                    end loop;
                    exit when Ok;
                    delay 0.001;
                end;
            end loop;
        end if;
    end;
    Put_Line("Proces" & Integer'Image(Process.Id) & " kończy czekać w ENTRY_PROTOCOL_4");

    -- CRITICAL_SECTION
    Put_Line("Proces" & Integer'Image(Process.Id) & " wchodzi do CRITICAL_SECTION");
    Change_State(Critical_Section);
    delay Min_Delay+(Max_Delay-Min_Delay)*Duration(Random(G));
    Put_Line("Proces" & Integer'Image(Process.Id) & " wychodzi z CRITICAL_SECTION");

    -- EXIT_PROTOCOL
    Put_Line("Proces" & Integer'Image(Process.Id) & " wchodzi do EXIT_PROTOCOL");
    Change_State(EXIT_PROTOCOL);
    declare
        function In_014(V : Integer) return Boolean is (V = 0 or V = 1 or V = 4);
    begin
        if Process.Id < Nr_Of_Processes-1 then
            loop
                declare
                    Ok : Boolean := True;
                    Blokujacy : Integer := -1;
                begin
                    for J in Process.Id+1 .. Nr_Of_Processes-1 loop
                        if not In_014(Flag_Access.Get_Flag(J)) then
                            Ok := False;
                            Blokujacy := J;
                            Put_Line("Proces" & Integer'Image(Process.Id) &
                                     " nie moze zejsc z flagi 4 do 0, bo proces" &
                                     Integer'Image(Blokujacy) & " ma flage=" &
                                     Integer'Image(Flag_Access.Get_Flag(Blokujacy)));
                            for P in 0 .. Nr_Of_Processes-1 loop
                                if Flag_Access.Get_Flag(P) = 4 then
                                    declare
                                        Czeka_Na_Wyzszego : Boolean := False;
                                        Kogo : Integer := -1;
                                    begin
                                        for J2 in P+1 .. Nr_Of_Processes-1 loop
                                            if not In_014(Flag_Access.Get_Flag(J2)) then
                                                Czeka_Na_Wyzszego := True;
                                                Kogo := J2;
                                                exit;
                                            end if;
                                        end loop;
                                        if Czeka_Na_Wyzszego then
                                            Put_Line("Proces" & Integer'Image(P) & " (flaga=4) czeka na wyzszego proces " &
                                                     Integer'Image(Kogo) & " (flaga=" & Integer'Image(Flag_Access.Get_Flag(Kogo)) & ")");
                                        else
                                            Put_Line("Proces" & Integer'Image(P) & " (flaga=4) NIE czeka na wyzszego (moze zejsc do 0)");
                                        end if;
                                    end;
                                end if;
                            end loop;
                            exit;
                        end if;
                    end loop;
                    exit when Ok;
                    delay 0.001;
                end;
            end loop;
        end if;
    end;
    Put_Line("Proces" & Integer'Image(Process.Id) & " kończy EXIT_PROTOCOL, ustawia flagę na 0");
    Flag_Access.Set_Flag(Process.Id, 0);

    Change_State(LOCAL_SECTION); -- starting LOCAL_SECTION
end loop;
    
    Printer.Report( Traces );
  end Process_Task_Type;


-- local for main task

  Process_Tasks: array (0 .. Nr_Of_Processes-1) of Process_Task_Type; -- for tests
  Symbol : Character := 'A';

begin 
  -- init tarvelers tasks
  for I in Process_Tasks'Range loop
    Process_Tasks(I).Init( I, Seeds(I+1), Symbol );   -- `Seeds(I+1)` is ugly :-(
    Symbol := Character'Succ( Symbol );
  end loop;

  -- start tarvelers tasks
  for I in Process_Tasks'Range loop
    Process_Tasks(I).Start;
  end loop;

end Mutex_Template;

