----------------------------------------------------------------------------------
-- Company: GraphBit Inc.
-- Engineer: Charles Lévesque-Matte & Pascal-Emmanuel Lachance
-- 
-- Create Date:    10:44:42 2019-04-29
-- Design Name:    ECRAN LCD
-- Module Name:    ModeText
-- Project Name:   LCD_controller
-- Target Devices: XC3S100e-TQ144
-- Tool versions: ISE 14.7
-- Description: 
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


entity MDTEXT is
    Port ( CLK : in  STD_LOGIC;
           LINE : in  STD_LOGIC_VECTOR (7 downto 0);
           ROW : in  STD_LOGIC_VECTOR (7 downto 0);
           CLEAR : in  STD_LOGIC;
           CLK_DATA : in  STD_LOGIC:= '0';
           ENA_TEXT : in  STD_LOGIC;
           DATA : in  STD_LOGIC_VECTOR (7 downto 0);
           ADR_TEXT : out  STD_LOGIC_VECTOR (12 downto 0):= "0000000000000";
           WR_TEXT : buffer  STD_LOGIC := '0';
           CS_TEXT : out  STD_LOGIC := '0';
           DATA_TEXT : out STD_LOGIC_VECTOR (7 downto 0));
end MDTEXT;
architecture MDTEXT of MDTEXT is

component rom_1024_8 is
   port (Clk : in std_logic;
         en : in std_logic; -- Read enable
         addr : in std_logic_vector(9 downto 0);
         data : out std_logic_vector(7 downto 0)
         );
end component;


constant ADRWIDTH : integer := 30;
-- font signal
signal Clk_font_s : std_logic := '0';
signal en_font_s :  std_logic := '0'; -- Read enable
signal addr_font_s :   integer := 0;
signal data_font_s :  std_logic_vector(7 downto 0) :="00000000";
signal adr_std_s : std_logic_vector(9 downto 0) :="0000000000";
-- mem signal
signal line_s : integer := 1;
signal row_s : integer := 1;
signal adres_text_s : integer := 0;
signal data_in_s : STD_LOGIC_VECTOR (7 downto 0);
-- flags list
signal data_flag_s : STD_LOGIC := '0';
-- State machine 
type state_type is (wait_data,add_base,font0_acq,font1_acq,font2_acq,font3_acq,font4_acq,font5_acq,font6_acq,font7_acq,wr_mem, font_final, clear_screen);
signal state: state_type;


begin

adr_std_s <= CONV_STD_LOGIC_VECTOR(addr_font_s ,10); -- conversion de integer-> std
en_font_s <= '1';  -- font toujours accessible
CS_TEXT <= '1';  -- memoire toujours en écriture dans ce mode
--DATA_TEXT <= data_font(3 downto 0) & data_font(7 downto 4);
ADR_TEXT <= CONV_STD_LOGIC_VECTOR( adres_text_s,13);

-- Clear screen and buffer
pReset: process(CLK, CLEAR,data_font_s) is 
   begin
		if (rising_edge(CLK)) then
			if((CLEAR = '1') and (State = clear_screen)) then 
				DATA_TEXT <= "00000000";
			else 
				DATA_TEXT <= data_font_s(3 downto 0) & data_font_s(7 downto 4);
			end if;
		end if;
 end process pReset;
 
 -- Generate font clock
 pClockFont : process(CLK,State,WR_TEXT) is
   begin
   if((WR_TEXT = '1') or (State = wr_mem)) then 
      Clk_font_s <= '0';
   else
      Clk_font_s<= CLK;
   end if;
end process;

pWriteFont : process(State,CLK,data_flag_s,line_s,row_s,CLEAR) is 
variable countFont : integer range 0 to 7 := 0;
   begin
   if(rising_edge(CLK)) then 
      case State is 
         when wait_data => 
            WR_TEXT <= '0';
            if(CLEAR = '1') then 
               State <= clear_screen;
               addr_font_s <= 0;
            else
               if(data_flag_s = '1')then
                  State <= add_base;
               else
                  State <= wait_data;
               end if;
            end if;
         when add_base =>
            addr_font_s <= (conv_integer(data_in_s)*8);
            adres_text_s <= ((240*(line_s - 1)) + (row_s - 1));
            State <= font0_acq;
         when font0_acq =>
            WR_TEXT <= '0';
            addr_font_s <= addr_font_s;
            adres_text_s <= adres_text_s;
            countFont := 1;                     -- countFont := countFont + 1
            State <= wr_mem;
         when wr_mem =>
            WR_TEXT <= '1';
            case countFont is
               when 0 =>
                  State <= font0_acq;
               when 1 =>
                  State <= font1_acq;
               when 2 =>
                  State <= font2_acq;
               when 3 =>
                  State <= font3_acq;
               when 4 =>
                  State <= font4_acq;
               when 5 =>
                  State <= font5_acq;
               when 6 =>
                  State <= font6_acq;
               when 7 =>
                  countFont := 0;
                  State <= font7_acq;
             end case;
          when font1_acq =>
            WR_TEXT <= '0';
            addr_font_s <= addr_font_s + 1;
            adres_text_s <= adres_text_s + ADRWIDTH;
            countFont := 2;
            State <= wr_mem;
          when font2_acq =>
            WR_TEXT <= '0';
            addr_font_s <=  addr_font_s + 1;
            adres_text_s <= adres_text_s + ADRWIDTH;
            countFont := 3;
            State <= wr_mem;
          when font3_acq =>
            WR_TEXT <= '0';
            addr_font_s <= addr_font_s + 1;
            adres_text_s <= adres_text_s + ADRWIDTH;
            countFont := 4;
            State <= wr_mem;
          when font4_acq =>
            WR_TEXT <= '0';
            addr_font_s <= addr_font_s + 1;
            adres_text_s <= adres_text_s + ADRWIDTH;
            countFont := 5;
            State <= wr_mem;
          when font5_acq =>
            WR_TEXT <= '0';
            addr_font_s <= addr_font_s + 1;
            adres_text_s <= adres_text_s + ADRWIDTH;
            countFont := 6;
            State <= wr_mem;
           when font6_acq =>
             WR_TEXT <= '0';
             addr_font_s <= addr_font_s + 1;
             adres_text_s <= adres_text_s + ADRWIDTH;
             countFont := 7;
             State <= wr_mem;
           when font7_acq =>
             WR_TEXT <= '0';
             addr_font_s <= addr_font_s + 1;
             adres_text_s <= adres_text_s + ADRWIDTH;
             countFont := 0;
             State <= font_final;
			  when font_final =>
				 WR_TEXT <= '1';
				 State <= wait_data;	
           when clear_screen =>
             WR_TEXT <= not WR_TEXT;
             if(WR_TEXT = '1') then
               if(adres_text_s = 4799)then
                  adres_text_s <= 0;
               else
                  adres_text_s <= adres_text_s + 1;
               end if;
             end if;
               State <= wait_data;
            when others =>
               State <= wait_data;
        end case;
    end if;
 end process pWriteFont;
 
process(CLK,CLK_DATA,ENA_TEXT,DATA) is -- process de détection de data recu
   begin
      if (rising_edge (CLK))then
         if (CLK_DATA = '1') then -- rising_edge        
            if(ENA_TEXT = '1')then
               data_in_s <= DATA;
               data_flag_s <= '1';
               line_s <= conv_integer(LINE); 
               row_s <= conv_integer(ROW);
            else
               data_flag_s <= '0';
					data_in_s <= data_in_s;
               line_s <= line_s;
               row_s <= row_s;
            end if;   
         else
            data_flag_s <= '0';
				data_in_s <= data_in_s;
				line_s <= line_s;
				row_s <= row_s;
         end if;       
      end if;   
end process;
            
            
-- Instance

InstFont : rom_1024_8  -- instantiation du component rom1024_8[font]
   port map(
      Clk => Clk_font_s,
      en => en_font_s,
      addr => adr_std_s,
      data => data_font_s
   );
end MDTEXT;