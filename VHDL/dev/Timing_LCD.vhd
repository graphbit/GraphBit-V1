----------------------------------------------------------------------------------
-- Company: Cegep Limoilou
-- Engineer: Charles Lévesque-Matte & Pascal-Emmanuel Lachance
-- 
-- Create Date:    17:18:57 03/21/2019 
-- Design Name: 
-- Module Name:    Timing_LCD - Behavioral 
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
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;


entity Timing_LCD is
    Port ( LATCH : buffer  STD_LOGIC :='0';
           SHIFT : buffer  STD_LOGIC :='0';
           FRM : out STD_LOGIC :='0';
           DF : out  STD_LOGIC :='0';
           CLK : in  STD_LOGIC;
           DISPLAYOFF : buffer  STD_LOGIC := '1';
           VEE_ON : buffer  STD_LOGIC := '0';
           MADDRA : buffer  STD_LOGIC_VECTOR( 13 downto 0):="00000000000000"
			  );
end Timing_LCD;

architecture Behavioral of Timing_LCD is

constant clkDivide: integer := 74;  -- (100Mhz/74)/2 = 675.675Khz 

signal shift_counter : integer   :=0;
signal shift_data : integer   := 1;
signal start_flag : std_logic := '0';
signal latch_counter : integer   := 1;
signal clkDiv: integer  := 0;
signal DF_s: STD_LOGIC := '0';

signal addra : STD_LOGIC_VECTOR( 13 downto 0):=  "00000000000000";
-------------------------------------------------------------------------------
begin

DISPLAYOFF <= '1';
MADDRA <= addra;
DF <= DF_s;
			
SIGNAL_VEEON: process(CLK,start_flag)                          -- set up power for LCD
     
variable clkStart: integer range 0 to 5500000 := 0; 
   begin
      if (rising_edge(CLK)) and (start_flag ='0')   then
         if (clkStart = 1) then -- 5500000   55 ms @100Mhz
            start_flag <= '1';
            VEE_ON <= '1';
            clkStart := clkStart;
         else
            clkStart := clkStart + 1;
            start_flag <= start_flag;
            VEE_ON <= VEE_ON;
         end if;
      end if;   
   end process;
   
SIGNAL_SHIFT : process (CLK,start_flag)  --set up timing for SHIFT
begin                
	if (rising_edge(CLK))  and (start_flag ='1')  then
		if (clkDiv = clkDivide) then
			clkDiv <= 0;
			SHIFT <= not SHIFT;
		else
			SHIFT <= SHIFT;
			clkDiv <= clkDiv + 1;
		end if;   
	end if;
end process;
      
SIGNAL_COUNTER : process (SHIFT)                  -- set up CLK latch and line
begin
	if (rising_edge(SHIFT)) then
		if (shift_counter = 60) then
         if (latch_counter = 160)  then
			   latch_counter <= 1;
			else
			   latch_counter <= latch_counter + 1;			
		   end if;		
		   shift_counter <= 1;
		else
			shift_counter <= shift_counter +1;
			latch_counter <= latch_counter;
		end if;
		
		shift_data <= shift_data +1;
		
		if (shift_data = 9600) then
		   shift_data <= 1;
		end if;
	end if;
end process;


 
process(SHIFT)          -- set up addr for port A
begin
  if(rising_edge(SHIFT)) then
     if( addra = X"257F")then --"10010110000000" -- 9600
       addra <=  "00000000000000";
    else
        addra <= addra +1;
	 end if; 	
end if;   
end process;

SIGNAL_DATA : process (SHIFT)                  -- set up DATA on BUS
begin
	if (rising_edge(SHIFT)) then
		-- DATA_BUS <= image(shift_data); 	
	end if;
end process;
      
SIGNAL_LATCH : process(CLK)                          -- set up timing for LATCH
begin
	if (rising_edge(CLK))  then
		if (shift_counter = 60) then
			LATCH <= '1';
		else
			LATCH <= '0';
		end if;
	end if;   
end process;

SIGNAL_FRM: process (SHIFT,LATCH,Latch_Counter,CLK) -- set up timing for for FRM
begin
  if (falling_edge(SHIFT)) then
      if(latch_counter = 1) then
			FRM <= '1';
      else
         FRM <= '0';  
      end if; 
  end if;
end process;
 
SIGNAL_DF : process(SHIFT)
 begin
   if (rising_edge(SHIFT)) then
      if((latch_counter = 1) and (shift_counter = 60)) then
        DF_s <= not DF_s;  
		else
		  DF_s <= DF_s;
      end if;  
  end if;   
end process;  

end Behavioral;

