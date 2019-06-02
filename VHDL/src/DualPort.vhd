----------------------------------------------------------------------------------
-- Company: GraphBit Inc.
-- Engineer: 
-- Modified by: Charles Lévesque-Matte & Pascal-Emmanuel Lachance
-- Create Date:    10:36:31 04/07/2011 
----------------------------------------------------------------------------------
-- Company: CEGEP LIMOILOU
-- Engineer: OLIVIER BERROUARD ET PIERRE POULIOT
-- 
-- Create Date:    10:36:31 04/07/2011 
-- Design Name:    ECRAN LCD
-- Module Name:    MUX_MEMORY - MUX_MEMORY 
-- Project Name:   controler.ise
-- Target Devices: XC3S100e-TQ144
-- Tool versions: ISE 10.1
-- Description: module top de la memoire necessaire assignement des signaux
--              provenant de timing_top.
--              
-- Dependencies: NONE
--
-- Revision: revision 1.0
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

  
entity DualPort is 
    Port ( CLK_Shift : in  STD_LOGIC;                     -- Fetch the next data in the memory
           DATA_IN : in  STD_LOGIC_VECTOR (7 downto 0);   -- Data coming from cpu  side 'b'
           DATA_OUT : out  STD_LOGIC_VECTOR (3 downto 0); -- Data going to the LCD side 'a'
           MWEB : in  STD_LOGIC;                           -- memory write enable0
			  CLKB : in  STD_LOGIC;                           -- Clock for 'b' side    
			  MADDRA : in STD_LOGIC_VECTOR (13 downto 0);    -- 'a' side address 
			  MADDRB : in STD_LOGIC_VECTOR (12 downto 0);    -- 'b' side address
           DATA_READ : out STD_LOGIC_VECTOR (7 downto 0)  -- Data read from the LCD
			  );
end DualPort;

architecture DualPort of DualPort is

signal ena:  std_logic := '0';

component MemVideo is
	port (
	addra: IN std_logic_VECTOR(13 downto 0);
	addrb: IN std_logic_VECTOR(12 downto 0);
	clka: IN std_logic;
	clkb: IN std_logic;
	dinb: IN std_logic_VECTOR(7 downto 0);
	douta: OUT std_logic_VECTOR(3 downto 0);
   doutb: OUT std_logic_VECTOR(7 downto 0);
	ena: IN std_logic;
	web: IN std_logic);
end component;
	
begin

InstMemVideo: MemVideo
		port map (
			addra => MADDRA,
			addrb => MADDRB,
			clka => CLK_Shift,
			clkb => CLKB,
			dinb => DATA_IN,
			douta => DATA_OUT,
         doutb => DATA_READ, 
			ena => ena,
			web => MWEB);	
ena <= '1';        -- toujours allumer pour lire en permanence sur 1 coup de clock	  

end DualPort;