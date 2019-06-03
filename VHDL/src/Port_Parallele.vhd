----------------------------------------------------------------------------------
-- Company: GraphBit Inc.
-- Engineer: Charles Lévesque-Matte & Pascal- Emmanuel Lachance
-- 
-- Create Date:    11:15:59 04/23/2019 
-- Design Name: 
-- Module Name:    Port_Parallele - Behavioral 
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
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity PARALLELE is
    Port ( 
           CLK : in STD_LOGIC;
           A0 : in  STD_LOGIC;
           DATA_FROM_CPU : in  STD_LOGIC_VECTOR (7 downto 0);
           CS : in  STD_LOGIC;
           A0_OUT : out  STD_LOGIC;
           DATA_PARALLELE : out  STD_LOGIC_VECTOR (7 downto 0);
           FLAG_PARALLELE : out  STD_LOGIC := '0'
           );
end PARALLELE;

architecture PARALLELE of PARALLELE is

signal flag_pulse : std_logic := '0';
signal Compte : STD_LOGIC_VECTOR  (1 downto 0):= "00";
signal CSClean: std_logic_vector(1 downto 0) := "00";
begin


DATA_PARALLELE <= DATA_FROM_CPU;
A0_OUT <= A0;

process(CLK) -- détection de front montant sur le CS
begin  
	if(rising_edge (CLK)) then 
		CSClean(1) <= CSClean(0);
		CSClean(0) <= CS;
		if(CSClean = "01") then 
			FLAG_PARALLELE <= '1';        
		else
			FLAG_PARALLELE <= '0';
		end if; 
	end if;
end process;


end PARALLELE;
