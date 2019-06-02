----------------------------------------------------------------------------------
-- Company: GraphBit Inc
-- Engineer: Charles Lévesque-Matte & Pascal-Emmanuel Lachance
-- 
-- Create Date:    12:58:27 04/25/2019 
-- Design Name: LCD Control
-- Module Name:    Mux_Comm - Behavioral 
-- Project Name: LCD Control
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
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Mux_Comm is
    Port ( FLAG_PARALLELE : in STD_LOGIC;                        -- Take possession of the resource
           DATA_PARALLELE : in  STD_LOGIC_VECTOR (7 downto 0);       -- Data comming from the parallel port
           FLAG_UART : in  STD_LOGIC;                            -- Take possession of the resource
           DATA_UART : in  STD_LOGIC_VECTOR (7 downto 0);            -- Data comming from UART
           FLAG_COM : out  STD_LOGIC;                                -- Telling Data_Ctrl that there is data to gather
           DOUT : out  STD_LOGIC_VECTOR (7 downto 0);               -- Data going to the dual port ram
			  CLK: in STD_LOGIC);
           -- Pin assignation
end Mux_Comm;

architecture Behavioral of Mux_Comm is
-- signal declaration
signal Dout_s : std_logic_vector(7 downto 0) := x"00";
signal oldFlag_s : STD_LOGIC := '0';

begin

DOUT <= Dout_s;

   pMux : process(CLK, FLAG_PARALLELE,FLAG_UART) is 
   begin
		if (rising_edge(CLK)) then
			 if(FLAG_PARALLELE = '1') then       -- Priority #1 to the parallel port
				Dout_s <= DATA_PARALLELE;           -- Data sent to the next process
				if (oldFlag_s = '0') then			 -- On rising edge of the flag
					FLAG_COM <= '1';                  -- Data available
					oldFlag_s <= '1';
				else
					oldFlag_s <= '1';
					FLAG_COM <= '0';
				end if;
			 elsif(FLAG_UART = '1')then          -- Priority #2 to the UART port
				Dout_s <= DATA_UART;
				if (oldFlag_s = '0') then			 -- On rising edge of the flag
					FLAG_COM <= '1';               -- Data available
					oldFlag_s <= '1';
				else
					oldFlag_s <= '1';
					FLAG_COM <= '0';
				end if;
			 else
				oldFlag_s <= '0';
				FLAG_COM <= '0';
				Dout_s <= Dout_s;
			end if;
		end if;
    end process pMux;

end Behavioral;

