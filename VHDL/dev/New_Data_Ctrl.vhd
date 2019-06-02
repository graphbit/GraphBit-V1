----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:15:45 05/14/2019 
-- Design Name: 
-- Module Name:    New_Data_Ctrl - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity New_Data_Ctrl is
    Port ( CLK : in  STD_LOGIC;										-- 100MHz Clock
           DATA : in  STD_LOGIC_VECTOR (7 downto 0);			-- Data coming from mux communication
           FLAG : in  STD_LOGIC;										-- Flag indicating new data is available
           DINB : in  STD_LOGIC_VECTOR (7 downto 0);			-- Data coming from the DPRAM
           CLKB : out  STD_LOGIC;									-- Clk of the DPRAM
           DATA_DP : out  STD_LOGIC_VECTOR (7 downto 0);		-- Data towards the DPRAM
           MADDRB : out  STD_LOGIC_VECTOR (12 downto 0);		-- Adress to the 'B' side of the DPRAM
           MWEB : out  STD_LOGIC;									-- Memory Write Enable (for DPRAM)
           SUPP : out  STD_LOGIC_VECTOR (3 downto 0));			-- LEDs for debugging purposes
end New_Data_Ctrl;

architecture Behavioral of New_Data_Ctrl is

type state_t is (size0,size1,cmd,getrow, getline, dataloop);
type mode_t is (none, text, graph, pixel);

-- State machine signals
signal State_s : state_t := size0; 									-- For the state machine
signal Mode_s: mode_t := none;										-- Current mode.
signal Count_s : STD_LOGIC_VECTOR(15 downto 0) := x"0000";	-- Current count
signal Row_s : STD_LOGIC_VECTOR(7 downto 0) := X"00";			-- Current X axis position
signal Line_s : STD_LOGIC_VECTOR(7 downto 0) := X"00";		-- Current Y axis position
signal Checksum_s : STD_LOGIC_VECTOR(7 downto 0) := x"00";	-- Checksum

-- Mux signals
signal DataIn_G_s : STD_LOGIC_VECTOR(7 downto 0);				-- Data going in the graph module
signal DataIn_T_s : STD_LOGIC_VECTOR(7 downto 0);				-- Data going in the text module
signal DataOut_G_s : STD_LOGIC_VECTOR(7 downto 0);				-- Data coming out of the graph module
signal DataOut_T_s : STD_LOGIC_VECTOR(7 downto 0);				-- Data coming out of the text module
signal Add_mem_G_s: STD_LOGIC_VECTOR(13 downto 0);				-- Adress to the DPRAM from graph module
signal Add_mem_T_s: STD_LOGIC_VECTOR(13 downto 0);				-- Adress to the DPRAM from the text module
signal WR_mem_G_s : STD_LOGIC;										-- Write signal from the graph module
signal WR_mem_T_s : STD_LOGIC;										-- Write signal from the text module
signal CS_mem_G_s : STD_LOGIC;										-- CS signal from graph (to MWEB)
signal CS_mem_T_s : STD_LOGIC;										-- CS signal from text  (to MWEB)
signal Clear_G_s : STD_LOGIC;											-- Clear graph module
signal Clear_T_s : STD_LOGIC;											-- Clear text module
signal ENA_G_s : STD_LOGIC;											-- Enable graph mode
signal ENA_T_s : STD_LOGIC;											-- Enable text mode
signal Data_Clk : STD_LOGIC;											-- Data clock

-- Misc signals
signal Pixel_Read_s : STD_LOGIC_VECTOR(7 downto 0);
signal Dump_s : STD_LOGIC;

-- Graph mode
	COMPONENT OLD_MDGRAPH
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
		ADR_GRAPH : out  STD_LOGIC_VECTOR (13 downto 0);
		CS_GRAPH : OUT std_logic;
		WR_GRAPH : OUT std_logic;
		DATA_GRAPH : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;
	
	COMPONENT OLD_MDTEXT
	PORT(
		CLK : IN std_logic;
		LINE : IN std_logic_vector(7 downto 0);
		ROW : IN std_logic_vector(7 downto 0);
		CLEAR : IN std_logic;
		CLK_DATA : IN std_logic;
		ENA_TEXT : IN std_logic;
		DATA : IN std_logic_vector(7 downto 0);          
		ADR_TEXT : OUT std_logic_vector(13 downto 0);
		WR_TEXT : OUT std_logic;
		CS_TEXT : OUT std_logic;
		DATA_TEXT : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;
	
begin


pDataAnalysis : process(CLK) is  
variable timeout_v : integer range 0 to 50000 := 0;
variable RxByteNb_v : std_logic_vector(15 downto 0) := x"0000";
variable flag_v : std_logic_vector(1 downto 0) := "00";
begin
	if (rising_edge(CLK)) then
		flag_v(1) := flag_v(0);											-- Detect rising edge of the communication flag
		flag_v(0) := FLAG;
		if (flag_v = "01") then											-- Receiving new data from communication mux
			timeout_v := 0;												-- Reset timeout counter
			case State_s is 
				-- First part of the data size
				when size0 =>
					RxByteNb_v(15 downto 8) := DATA;
					RxByteNb_v(7 downto 0) := x"00";
					Checksum_s <= DATA;
					Count_s <= x"0001";
					Line_s <= x"01";
					Row_s <= x"01";
					Data_Clk <= '0';
					Mode_s <= none;
					State_s <= size1;										-- Next step is getting the second part of the data size
					
					
				-- Second part of the data size
				when size1 =>
					RxByteNb_v(15 downto 8) := RxByteNb_v(15 downto 8);
					RxByteNb_v(7 downto 0) := DATA;
					Checksum_s <= Checksum_s + DATA;
					Count_s <= Count_s + '1';
					Line_s <= x"01";
					Row_s <= x"01";
					Data_Clk <= '0';
					Mode_s <= none;
					State_s <= cmd;										-- Next step is getting the command
					
					
				-- Command (text? graph? pixel?)
				when cmd =>
					Checksum_s <= Checksum_s + DATA;
					Line_s <= x"01";
					Row_s <= x"01";
					Count_s <= Count_s + '1';
					Data_Clk <= '0';
					RxByteNb_v := RxByteNb_v;
					if (DATA = x"01") then								-- Text command
						Mode_s <= text;
						State_s <= getline;
					elsif (DATA = x"02") then							-- Graph command
						Mode_s <= graph;
						State_s <= dataloop;
					elsif (DATA = x"03") then							-- Pixel command
						Mode_s <= pixel;
						State_s <= getline;
					else														-- Unknown command
						Mode_s <= none;
						State_s <= dataloop;
					end if;
					
					
				-- Acquiring the row
				when getline =>
					Line_s <= DATA;										-- Starting position (Y axis)
					Row_s <= x"01";
					Mode_s <= Mode_s;
					Checksum_s <= Checksum_s + DATA;
					Count_s <= Count_s + '1';
					RxByteNb_v := RxByteNb_v;
					Data_Clk <= '0';
					state_s <= getrow;
					
					
				-- Acquiring the line
				when getrow =>
					Row_s <= DATA;											-- Starting position (X axis)
					Line_s <= Line_s;
					Mode_s <= Mode_s;
					Checksum_s <= Checksum_s + DATA;
					Count_s <= Count_s + '1';
					RxByteNb_v := RxByteNb_v;
					Data_Clk <= '0';
					state_s <= dataloop;
					
					
				-- Acquiring new data
				when dataloop =>
					if (Count_s /= (RxByteNb_v - 1)) then			-- Have we reached the last data?
						Checksum_s <= Checksum_s + DATA;
						Count_s <= Count_s + '1';
						State_s <= dataloop;
						Data_Clk <= '1';
						RxByteNb_v := RxByteNb_v;
						Mode_s <= Mode_s;
						-- Handle rows and lines
						if (Mode_s = text) then
							if (Row_s = x"1E" and Line_s = x"A0") then	-- Last byte of the 30x160 screen
								Row_s <= x"01";
								Line_s <= x"01";
							elsif (Row_s = x"1E") then					-- Last byte of the row
								Row_s <= x"01";
								Line_s <= Line_s + '1';					-- Increment line
							else
								Row_s <= Row_s + '1';					-- Increment row
								Line_s <= Line_s;
							end if;
						elsif (Mode_s = graph) then
							if (Row_s = x"1F" and Line_s = x"A0") then	-- Last byte of the 30x160 screen
								Row_s <= x"01";
								Line_s <= x"01";
							elsif (Row_s = x"1F") then					-- Last byte of the row
								Row_s <= x"02";
								Line_s <= Line_s + '1';					-- Increment line
							else
								Row_s <= Row_s + '1';					-- Increment row
								Line_s <= Line_s;
							end if;
						else
							-- Mode pixel
							if (Row_s = x"F0" and Line_s = x"A0") then	-- Last pixel of the 240x160 screen
								Row_s <= x"01";
								Line_s <= x"01";
							elsif (Row_s <= x"F0") then				-- Last pixel of the row
								Row_s <= x"01";
								Line_s <= Line_s + '1';					-- Increment line
							else
								Row_s <= Row_s + '1';					-- Increment row
								Line_s <= Line_s;
							end if;
						end if;
					else
					
					
				-- Handle checksum
						Count_s <= x"0000";
						Row_s <= x"01";
						Line_s <= x"01";
						State_s <= size0;
						RxByteNb_v := x"FFFF";
						Data_Clk <= '0';
						Mode_s <= none;
						-- CHECKSUM HANDLING
						-- TO IMPLEMENT
						Checksum_s <= x"00";
					end if;
					
				-- Reset state machine
				when others =>
				Count_s <= x"0000";
				State_s <= size0;
				Mode_s <= none;
				Data_Clk <= '0';
				Line_s <= x"01";
				Row_s <= x"01";
				Checksum_s <= x"00";
				RxByteNb_v := x"FFFF";					
			end case;
		else
			Data_Clk <= '0';
			
			-- Handle timeout
			-- 50000 * 10ns = 500us timeout
			if (timeout_v = 50000) then
				Count_s <= x"0000";
				State_s <= size0;
				Mode_s <= none;
				Line_s <= x"01";
				Row_s <= x"01";
				Checksum_s <= x"00";
				timeout_v := 0;
				RxByteNb_v := x"FFFF";
			else
				timeout_v := timeout_v + 1;
				Count_s <= Count_s;
				State_s <= State_s;
				Mode_s <= Mode_s;
				Line_s <= Line_s;
				Row_s <= Row_s;
				Checksum_s <= Checksum_s;
				RxByteNb_v := RxByteNb_v;
			end if;
		end if;	-- FLAG = '1'
	end if;	-- rising_edge(CLK)
end process;

pMux : process(CLK) is
begin
	if (mode_s /= none) then
		if (mode_s = text) then
			ENA_T_s <= '1';
			ENA_G_s <= '0';
			DATA_DP <= DataOut_T_s;
			MADDRB <= Add_Mem_T_s(12 downto 0);
			CLKB <= WR_mem_T_s;
			MWEB <= CS_mem_T_s;
			Pixel_Read_s <= x"00";
			DataIn_T_s <= DATA;
			DataIn_G_s <= x"00";
		else
			ENA_G_s <= '1';
			ENA_T_s <= '0';
			DATA_DP <= DataOut_G_s;
			MADDRB <= Add_Mem_G_s(12 downto 0);
			CLKB <= WR_mem_G_s;
			MWEB <= CS_mem_G_s;
			Pixel_Read_s <= DINB;
			DataIn_G_s <= DATA;
			DataIn_T_s <= x"00";
			if (mode_s = graph) then
				Dump_s <= '1';
			else																-- Pixel mode
				Dump_s <= '0';
			end if;
		end if;
	end if;
end process;

Inst_MDGRAPH: OLD_MDGRAPH PORT MAP(
		CLK => CLK,
		ENA_GRAPH => ENA_G_s,
		DUMP => Dump_s,
		CLK_DATA => Data_Clk,
		CLEAR => Clear_G_s,
		DATA => DataIn_G_s,
		ROW => Row_s,
		LINE => Line_s,
		PIXEL_READ => Pixel_Read_s,
		ADR_GRAPH => Add_mem_G_s,
		CS_GRAPH => CS_mem_G_s,
		WR_GRAPH => WR_mem_G_s,
		DATA_GRAPH => DataOut_G_s
	);
	
	Inst_MDTEXT: OLD_MDTEXT PORT MAP(
		CLK => CLK,
		LINE => Line_s,
		ROW => Row_s,
		CLEAR => Clear_T_s,
		CLK_DATA => Data_Clk,
		ENA_TEXT => ENA_T_s,
		DATA => DataIn_T_s,
		ADR_TEXT => Add_mem_T_s,
		WR_TEXT => WR_mem_T_s,
		CS_TEXT => CS_mem_T_s,
		DATA_TEXT => DataOut_T_s
	);

end Behavioral;

