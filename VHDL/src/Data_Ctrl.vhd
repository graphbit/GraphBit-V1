----------------------------------------------------------------------------------
-- Company: GraphBit Inc.
-- Engineer: Charles Lévesque-Matte & Pascal-Emmanuel Lachance
-- 
-- Create Date:    08:49:32 04/25/2019 
-- Design Name: LCD Controller
-- Module Name:    Data_Ctrl - Behavioral 
-- Project Name: Ecran Graphique 
-- Target Devices: Xilinx Spartan 3E
-- Tool versions: ISE 14.7
-- Description: Ce module prend les données des modules de communications et les formatte 
--              pour la mémoire dual port.
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity Data_Ctrl is
    Port ( CLK : in STD_LOGIC;                       -- Clk 100 MHz
           DATA : in STD_LOGIC_VECTOR(7 downto 0);   -- Data comming from mux communication
           FLAG : in STD_LOGIC;                      -- A data is ready to be processed
           DINB : in STD_LOGIC_VECTOR(7 downto 0);   -- Data read on the LCD
           CLKB : out STD_LOGIC;                     -- To be able to shift address on 'b side of the memory
           DATA_DP: out STD_LOGIC_VECTOR(7 downto 0); -- Data going into the dp ram
           MADDRB : out STD_LOGIC_VECTOR(12 downto 0);    -- Address inside the 'b' side of the dp ram
           MWEB : out STD_LOGIC;
			  LED : out STD_LOGIC_VECTOR(7 downto 0));                      -- memory write enable
          
           
           -- Allocation of signal pin
end Data_Ctrl;

architecture Behavioral of Data_Ctrl is
-- ***************************** Component's declaration **********************************************

--*****************************************************************************************************
-- ****************************** Declaration of signals **********************************************
signal RxByteNb_s: STD_LOGIC_VECTOR(15 downto 0) := x"FFFF";   -- Number of bytes to be sent by the microcontroller
signal Flag_Text_s:  STD_LOGIC := '0';
signal Flag_Graph_s:  STD_LOGIC := '0';
signal StartLine_s : STD_LOGIC_VECTOR(7 downto 0) := x"00";						-- Display starting point from communication peripheral
signal StartRow_s : STD_LOGIC_VECTOR(7 downto 0) := x"00";						-- Display starting point from communication peripheral
signal Line_s: STD_LOGIC_VECTOR(7 downto 0);
signal Row_s :STD_LOGIC_VECTOR(7 downto 0);
signal Clear_Graph_s:  STD_LOGIC;										-- Clear the data of the graph module
signal Clear_Text_s:  STD_LOGIC;											-- Clear the data of the text module
signal DataIn_G : STD_LOGIC_VECTOR(7 downto 0);
signal DataIn_T : STD_LOGIC_VECTOR(7 downto 0);
signal DataOut_T: STD_LOGIC_VECTOR(7 downto 0);
signal DataOut_G: STD_LOGIC_VECTOR(7 downto 0);
signal DataToMod : STD_LOGIC_VECTOR(7 downto 0);
signal Dump_s: STD_LOGIC := '0';
signal Clk_Data_s: STD_LOGIC;
signal Clk_Data_flag_s : STD_LOGIC;
-- For the mux process
signal Pixel_Read_s: STD_LOGIC_VECTOR(7 downto 0);
signal Add_Mem_Graph_s: STD_LOGIC_VECTOR(12 downto 0);			-- Adress signal from the graph module
signal Add_Mem_Text_s: STD_LOGIC_VECTOR(12 downto 0);				-- Adress signal from the text module
signal Wr_mem_Graph_s : STD_LOGIC;										-- Write signal from the graph module
signal Wr_mem_Text_s : STD_LOGIC;										-- Write signal from the text module
signal Cs_Mem_Graph_s: STD_LOGIC;										-- CS signal from the graph module
signal Cs_Mem_Text_s: STD_LOGIC;											-- CS signal from the text module
signal Data_From_Mem_s : STD_LOGIC_VECTOR(7 downto 0);

signal Data_DP_s: STD_LOGIC_VECTOR(7 downto 0) := x"00";


   	COMPONENT MDGRAPH
	PORT(
		CLK : IN std_logic;
		ENA_GRAPH : IN std_logic;
		DUMP : IN std_logic;
		CLK_DATA : IN std_logic;
		CLEAR : IN std_logic;
		DATA : IN std_logic_vector(7 downto 0);
		ROW : IN std_logic_vector(7 downto 0);
		LINE : IN std_logic_vector(7 downto 0);
		PIXEL_READ : IN std_logic_vector(7 downto 0);          
		ADR_GRAPH : OUT std_logic_vector(12 downto 0);
		CS_GRAPH : OUT std_logic;
		WR_GRAPH : OUT std_logic;
		DATA_GRAPH : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;
   

begin
Data_From_Mem_s <= DINB ;
DATA_DP <= Data_DP_s;

   pDataAnalysis: process(CLK) is 
   -- variables declaration 
variable countByte : integer range 0 to 604 :=0;
      begin
      if (rising_edge(CLK)) then 
         if(FLAG = '1') then    -- There's a communication on the parallele port ( priority 1)
				LED(0) <= '1';						-- Light up first led
            DataToMod <= DATA;       -- The data is ready to go in the dual port ram
				if (Clk_Data_Flag_s = '1') then
					Clk_Data_flag_s <= '0';
				end if;
            if(countByte = RxByteNb_s) then       -- Test the max value first 
               countByte := 0;                -- reset counter
					RxByteNb_s <= x"FFFF";
            else
               if(countByte = 0) then        -- First byte ? Must be the number of byte to be sent
						LED(1) <= '1';
                  RxByteNb_s(15 downto 8) <= DATA;  -- first part of the int 
                  countByte := countByte + 1;      -- couting the number of byte sent
						Flag_Text_s <= Flag_Text_s;     -- ENABLE flag remain the same
                  Flag_Graph_s <= Flag_Graph_s;
               elsif(countByte = 1) then
						LED(1) <= '1';
                  RxByteNb_s(7 downto 0) <= DATA;   --  second part of the int
                  countByte := countByte + 1;
						Flag_Text_s <= Flag_Text_s;     -- ENABLE flag remain the same
                  Flag_Graph_s <= Flag_Graph_s;
               elsif(countByte = 2) then          -- byte of command
						LED(2) <= '1';
                  case DATA is
                     when  x"01" =>
                        Flag_Text_s <= '1';           -- the user selected the text mode
								Flag_Graph_s <= '0';
								LED(5) <= '1';
                        Clear_Graph_s <= '1';			-- Clear the data of the graph module
                     when x"02" =>                  -- Dump graph mode
                        Flag_Graph_s <= '1';          -- the user selected the graph mode
								Flag_Text_s <= '0';
								LED(6) <= '1';
                        Dump_s <= '1';
                        Clear_Text_s <= '1';				-- Clear the data of the text module
                     when others => 
                        Flag_Text_s <= Flag_Text_s;     -- They should remain the same since the user didn't 
                        Flag_Graph_s <= Flag_Graph_s;   -- send the right command
                  end case;  
                  countByte := countByte + 1 ;      -- we still have to count  
					 elsif (countByte = (RxByteNb_s - '1')) then
						-- TO IMPLEMENT
						-- CHECKSUM
						LED(4) <= '1';
						Flag_Text_s <= '0';     -- Reset enable flags
                  Flag_Graph_s <= '0';
						countByte := 0;      -- Restart count
                else			-- Normal data reception
						LED(3) <= '1';
						Clk_Data_flag_s <= '1';               -- generate a clk signal for 
						Flag_Text_s <= Flag_Text_s;     -- ENABLE flag remain the same
                  Flag_Graph_s <= Flag_Graph_s;
						countByte := countByte + 1;
						 if(Flag_Text_s = '1' and countByte = 3) then
								StartLine_s <= DATA;
						 elsif(Flag_Text_s = '1' and countByte = 4) then
								StartRow_s <= DATA;
						 else    -- Normal data
							DataToMod <= DATA;      -- we gather the data to dump it on the screen
						 end if;
                end if;
              end if;
				else
				LED(6 downto 0) <= "0000000";
            end if;
			end if;
      end process pDataAnalysis;
      
      pDumpManage: process(Clk, Dump_s) is
      -- variables
         begin
			if (rising_edge(Clk)) then
				if (StartLine_s /= x"00") then
					line_s <= StartLine_s;
				elsif (StartRow_s /= x"00") then
					row_s <= StartRow_s;
				else
					if(Dump_s = '0') then 
							if ((row_s = x"F0") and (line_s = x"A0")) then -- last pixel of the screen 240x160
								row_s <= x"01";
								line_s <=  x"01";
							elsif (row_s = x"F0")then  -- Last pixel of line 240
								line_s <= line_s + 1;
								row_s <= x"01";
							else
								row_s <= row_s + 1; -- pixel increment
							end if;
						 else   
							if ((row_s = x"1E") and (line_s = x"A0")) then -- Last byte of the screen 30x160
								row_s <= x"01";
								line_s <=  x"01";
							elsif (row_S = x"1E")then  -- last byte of the line
								line_s <= line_s + 1;
								row_s <= x"01";
							else
								row_s <= row_s + 1; -- byte increment
							end if;
						 end if;
					 end if;
				 end if;
         end process pDumpManage;
         
         pDataClkManager: process(CLK) is
         begin    
				if(rising_edge(CLK)) then 
					if (Clk_Data_Flag_s = '1') then
						Clk_Data_s <= '1';
					else
						Clk_data_s <= '0';
					end if;
				end if;
         end process pDataClkManager;
            
			pDatatoDPChecker: process(CLK) is
			begin
				if (rising_edge(CLK)) then
					if (Data_DP_s = x"00") then
						LED(7) <= '1';
					else
						LED(7) <= '0';
					end if;
				end if;
			end process pDatatoDPChecker;
   
   Inst_MDGRAPH: MDGRAPH PORT MAP(
		CLK => CLK,
		ENA_GRAPH => Flag_Graph_s,
		DUMP => '1',
		CLK_DATA => Clk_Data_s,
		CLEAR => '0',
		DATA => DataToMod,
		ROW => Row_s,
		LINE => Line_s,
		PIXEL_READ => Pixel_Read_s,
		ADR_GRAPH => MADDRB,
		CS_GRAPH => MWEB,
		WR_GRAPH => CLKB,
		DATA_GRAPH => Data_DP_s
	);
end Behavioral;

