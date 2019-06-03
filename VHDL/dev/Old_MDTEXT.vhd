----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:24:54 04/20/2011 
-- Design Name: 
-- Module Name:    MDTEXT - MDTEXT 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: Module qui avec la ligne et la colonne prealablement envoye
--              genere un caracte a lecran  
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

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity OLD_MDTEXT is
    Port ( 
           CLK : in  STD_LOGIC;
           LINE : in  STD_LOGIC_VECTOR (7 downto 0);
           ROW : in  STD_LOGIC_VECTOR (7 downto 0);
           CLEAR : in  STD_LOGIC;
           CLK_DATA : in  STD_LOGIC:= '0';
           ENA_TEXT : in  STD_LOGIC;
           DATA : in  STD_LOGIC_VECTOR (7 downto 0);
           ADR_TEXT : out  STD_LOGIC_VECTOR (13 downto 0):="00000000000000";
           WR_TEXT : out  STD_LOGIC := '0';
           CS_TEXT : out  STD_LOGIC := '0';
           DATA_TEXT : out STD_LOGIC_VECTOR (7 downto 0));
end OLD_MDTEXT;
architecture OLD_MDTEXT of OLD_MDTEXT is


component rom_1024_8 is
port (
Clk : in std_logic;
en : in std_logic; -- Read enable
addr : in std_logic_vector(9 downto 0);
data : out std_logic_vector(7 downto 0)
);
end component;


constant ADRWIDTH : integer := 30;
-- font signal
signal Clk_font : std_logic := '0';
signal en_font :  std_logic := '0'; -- Read enable
signal addr_font :   integer := 0;
signal data_font :  std_logic_vector(7 downto 0) :="00000000";
signal cnt_adr :  integer := 0;
signal adr_std : std_logic_vector(9 downto 0) :="0000000000";
-- mem signal
signal ligne : integer := 1;
signal colonne : integer := 1;
signal adres_text : integer := 0;
signal data_in : STD_LOGIC_VECTOR (7 downto 0);
signal wr_text_s : STD_LOGIC;
--flags list
signal data_flag : STD_LOGIC := '0';
-- state signal
signal stCur   :  STD_LOGIC_VECTOR (4 downto 0):="00000";
signal stNext   : STD_LOGIC_VECTOR (4 downto 0):="00000";

begin


adr_std <= CONV_STD_LOGIC_VECTOR(addr_font ,10); -- conversion de integer-> std
en_font <= '1';  -- font toujours accessible
CS_TEXT <= '1';  -- memoire toujours en écriture dans ce mode
--DATA_TEXT <= data_font(3 downto 0) & data_font(7 downto 4);
ADR_TEXT <= CONV_STD_LOGIC_VECTOR( adres_text,14);
WR_TEXT <= wr_text_s;
InstFont : rom_1024_8  -- instantiation du component rom1024_8[font]
   port map(
      Clk =>Clk_font,
      en => en_font,
      addr => adr_std,
	  
      data => data_font
   );
   
process(CLEAR,data_font) -- cas d'un reset 
	
begin
   if ((CLEAR = '1') and (stCur = "10010"))then
      DATA_TEXT <= "00000000";
   else   
      DATA_TEXT <= data_font(3 downto 0) & data_font(7 downto 4);
   end if;   
end process;

process(CLK) -- process qui switch le state machine
begin
   if (CLK = '1' and CLK'Event) then
      stCur <= stNext;
   end if;
end process;

process(CLK,stCur,wr_text_s) -- genere le clock du font
begin
   if (( wr_text_s = '1') or (stCur < "00011"))then
      Clk_font <= '0';
   else
      Clk_font <= CLK;
   end if; 
end process;      


process (stCur,CLK,data_flag,ligne,colonne,CLEAR) -- state machine des differentes etapes
begin
if(CLK = '1' and CLK'Event)  then
   case stCur is
      when "00000" => -- attente du data
         wr_text_s <= '0';
         if (CLEAR = '1')then
            stNext <= "10010";
            addr_font <=  0;
            --wr_text_s <= not wr_text_s; 
         else
            if ( data_flag = '1')then
               stNext <= "00001";
            else
               stNext <= "00000";
            end if;
         end if;
      when "00001" => -- setup base adr
          addr_font <=  (conv_integer(data_in)*8);
          adres_text <=  ((240 *(ligne - 1)) + (colonne - 1));      
         stNext <= "00010";
      when "00010" =>  -- setup adr et acquisition de la font ligne 1
         wr_text_s <= '0';
         addr_font <=   addr_font; 
         adres_text <= adres_text; 
         --tata <= tata + 30;
         stNext <= "00011";
		 
      when "00011" => -- ecriture dans la memoie
          wr_text_s <= '1';
          stNext <= "00100";
		  
      when "00100" =>  -- setup adr et acquisition de la font ligne 2
         wr_text_s <= '0';
         addr_font <=    addr_font + 1;
         adres_text <=  adres_text + ADRWIDTH;
         stNext <= "00101";
      when "00101" => -- ecriture dans la memoire
         wr_text_s <= '1';
         stNext <= "00110";
       when "00110" =>  -- setup adr et acquisition de la font ligne 3
         wr_text_s <= '0';
         addr_font <=    addr_font + 1;
         adres_text <= adres_text + ADRWIDTH;
         stNext <= "00111";
      when "00111" => -- ecriture dans la memoire
         wr_text_s <= '1';
         stNext <= "01000";
       when "01000" =>  -- setup adr et acquisition de la font ligne 4
         wr_text_s <= '0';
         addr_font <=     addr_font + 1;
         adres_text <= adres_text + ADRWIDTH;
         stNext <= "01001";
      when "01001" => -- ecriture dans la memoire
         wr_text_s <= '1';
         stNext <= "01010";
      when "01010" =>  -- setup adr et acquisition de la font ligne 5
         wr_text_s <= '0';
         addr_font <=   addr_font + 1;
         adres_text <= adres_text + ADRWIDTH;
         stNext <= "01011";
      when "01011" => -- ecriture dans la memoire
          wr_text_s <= '1';
          stNext <= "01100";
      when "01100" =>  -- setup adr et acquisition de la font ligne 6
         wr_text_s <= '0';
         addr_font <= addr_font + 1;
         adres_text <=  adres_text + ADRWIDTH;
         stNext <= "01101";
      when "01101" => -- ecriture dans la memoire
          wr_text_s <= '1';
          stNext <= "01110";
       when "01110" =>  -- setup adr et acquisition de la font ligne 7
         wr_text_s <= '0';
         addr_font <= addr_font + 1;
         adres_text <=  adres_text + ADRWIDTH;
         stNext <= "01111";
      when "01111" => -- ecriture dans la memoire
          wr_text_s <= '1';
          stNext <= "10000";
       when "10000" =>  -- setup adr et acquisition de la font ligne 8
         wr_text_s <= '0';
         addr_font <= addr_font + 1;
         adres_text <= adres_text + ADRWIDTH;
         stNext <= "10001";
      when "10001" => -- ecriture dans la memoire
         wr_text_s <= '1';
         stNext <= "00000";
      when "10010" => -- clear ecran
         wr_text_s <= not wr_text_s;
         if (wr_text_s = '1')then
            if ( adres_text = 4799)then
               adres_text <= 0;  
            else
               adres_text <= adres_text + 1;    
            end if;
         end if;            
          stNext <= "00000";         
      when others =>
         stNext <= "00000";
   end case;
end if;
end process;

process(CLK,CLK_DATA,ENA_TEXT,DATA) -- process de détection de data recu
variable detect : std_ulogic_vector (1 downto 0):="00";
begin
   if rising_edge (CLK) then
      detect(1) := detect(0); -- record last value of sync in detect(1)
      detect(0) := CLK_DATA ; --record current sync in detect(0)
      if (detect = "01") then -- rising_edge        
         if(ENA_TEXT = '1')then
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
 
end OLD_MDTEXT;

