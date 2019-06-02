----------------------------------------------------------------------------------
-- Company: CEGEP LIMOILOU
-- Engineer: OLIVIER BERROUARD ET PIERRE POULIOT
-- 
-- Create Date:    14:23:32 05/05/2011 
-- Design Name:    ECRAN LCD
-- Module Name:    timing - timing 
-- Project Name:   controler.ise
-- Target Devices: XC3S100e-TQ144
-- Tool versions: ISE 10.1
-- Description: Programme qui gere laffichage de pixel en mode dump et pixel
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

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity OLD_MDGRAPH is
    Port ( CLK : in  STD_LOGIC;
           ENA_GRAPH : in  STD_LOGIC;
           DUMP : in  STD_LOGIC;
           CLK_DATA : in  STD_LOGIC;
           CLEAR : in  STD_LOGIC;
           DATA : in  STD_LOGIC_VECTOR (7 downto 0);
           ROW : in  STD_LOGIC_VECTOR (7 downto 0);
           LINE : in  STD_LOGIC_VECTOR (7 downto 0);
           PIXEL_READ : in  STD_LOGIC_VECTOR (7 downto 0);
           ADR_GRAPH : out  STD_LOGIC_VECTOR (13 downto 0);
           CS_GRAPH : out  STD_LOGIC;
           WR_GRAPH : out  STD_LOGIC;
           DATA_GRAPH : out  STD_LOGIC_VECTOR (7 downto 0));
end OLD_MDGRAPH;

architecture OLD_MDGRAPH of OLD_MDGRAPH is
signal ligne : integer := 1;
signal colonne : integer := 1;
signal adres_graph : integer := 0;
signal pixel_position : integer := 0;


signal data_in : STD_LOGIC_VECTOR (7 downto 0);
signal stCur   :  STD_LOGIC_VECTOR (4 downto 0):="00000";
signal stNext   : STD_LOGIC_VECTOR (4 downto 0):="00000";
signal data_ready : STD_LOGIC_VECTOR (7 downto 0);

signal data_flag : STD_LOGIC := '0';
signal transflag :  STD_LOGIC := '0';
signal transflag_done :  STD_LOGIC := '0';

begin

DATA_GRAPH <= data_ready(3 downto 0) & data_ready(7 downto 4);
ADR_GRAPH  <= CONV_STD_LOGIC_VECTOR( adres_graph,14);

process(CLK,ENA_GRAPH) -- process qui switch le state machine
begin
   if (CLK = '1' and CLK'Event) then
      stCur <= stNext;
   end if;
end process;

process(CLK,CLK_DATA,ENA_GRAPH,DATA) -- process de detection de data recu
variable detect : std_ulogic_vector (1 downto 0):="00";
begin
   if rising_edge (CLK) then
      detect(1) := detect(0); -- record last value of sync in detect(1)
      detect(0) := CLK_DATA ; --record current sync in detect(0)
      if (detect = "01") then -- rising_edge       
         if(ENA_GRAPH = '1')then
            data_in <= DATA;
            data_flag <= '1';
            ligne <= conv_integer(LINE); 
            colonne <= conv_integer(ROW);
         else
            data_flag <= '0';
            ligne <= ligne;
            colonne <= colonne;
         end if;   
      else
         data_flag <= '0';   
     end if;  
   end if;   
end process;


process (stCur,CLK,data_flag,ligne,colonne) -- state machine des differentes etapes
begin
if(CLK = '0' and CLK'Event)  then
   case stCur is
      when "00000" => -- attente du data
         WR_GRAPH <= '0';
         CS_GRAPH <= '0';
         if (CLEAR = '1')then -- mode effacement 
            stNext <= "00110";
            CS_GRAPH <= '1';
            data_ready <= "00000000";
         else
            if ( data_flag = '1')then -- cas dun data recu
                stNext <= "00001";              
            else
               stNext <= "00000";
            end if;
         end if;            
      when "00001" => 
         if(DUMP = '1')then -- setup base adr and data for dump
            adres_graph <= ((30 *(ligne - 1)) + (colonne - 1));
            stNext <= "00101";
            data_ready <= data_in;
            CS_GRAPH <= '1';
         else               -- setup de ladresse de lecture [pixel]
            adres_graph <=  (((30 *(ligne - 1)) + ((colonne - 1))/8));
            pixel_position <= ((colonne-1) - (((colonne-1)/8)*8));
            stNext <= "00010";           
         end if;          
      when "00010" =>  -- clkgen memory read  [pixel]
         WR_GRAPH <= '1';
         stNext <= "00011";
      when "00011" => -- lecture et swap de la lecture  [pixel]
         WR_GRAPH <= '0';      
         data_ready <= PIXEL_READ(3 downto 0) & PIXEL_READ(7 downto 4);
         stNext <= "00100";
      when "00100" =>  -- transformation du pixel  [pixel]
          case pixel_position is 
            when 0 =>
               data_ready(7) <= data_in(0);              
            when 1 =>
               data_ready(6) <= data_in(0);
            when 2 =>
               data_ready(5) <= data_in(0);
            when 3 =>
               data_ready(4) <= data_in(0);
            when 4 => 
               data_ready(3) <= data_in(0);
            when 5 => 
               data_ready(2) <= data_in(0);
            when 6 =>
               data_ready(1) <= data_in(0);
            when 7 =>
               data_ready(0) <= data_in(0);
            when others => NULL;
         end case;
         stNext <= "00101";
         CS_GRAPH <= '1';
      when "00101"=>  -- ecriture dans la memoire  [pixel] [dump]
         WR_GRAPH <= '1';
         stNext <= "00000";
      when "00110"=>  -- clear screen  [pixel] [dump]
         WR_GRAPH <= '1';
         stNext <= "00111";   
      when "00111"=>  -- increment adresse du clear screen
         if ( adres_graph = 4799)then
            adres_graph <= 0;  
         else
            adres_graph <= adres_graph + 1;    
         end if;  
          stNext <= "00000";
      when others =>
         stNext <= "00000";
   end case;
end if;         
end process;
 

end OLD_MDGRAPH;

