----------------------------------------------------------------------------------
-- Company: GraphBit Inc.
-- Engineer: Charles Lévesque-Matte & Pascal-Emmanuel Lachance
-- 
-- Create Date:    10:44:42 2019-04-29
-- Design Name:    ECRAN LCD
-- Module Name:    ModeGraph
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

entity MDGRAPH is
    Port ( CLK : in  STD_LOGIC;
           ENA_GRAPH : in  STD_LOGIC;
           DUMP : in  STD_LOGIC;
           CLK_DATA : in  STD_LOGIC;
           CLEAR : in  STD_LOGIC;
           DATA : in  STD_LOGIC_VECTOR (7 downto 0);
           ROW : in  STD_LOGIC_VECTOR (7 downto 0);
           LINE : in  STD_LOGIC_VECTOR (7 downto 0);
           PIXEL_READ : in  STD_LOGIC_VECTOR (7 downto 0);
           ADR_GRAPH : out  STD_LOGIC_VECTOR (12 downto 0);
           CS_GRAPH : out  STD_LOGIC;
           WR_GRAPH : out  STD_LOGIC;
           DATA_GRAPH : out  STD_LOGIC_VECTOR (7 downto 0));
end MDGRAPH;

architecture MDGRAPH of MDGRAPH is
signal line_s : integer := 1;
signal row_s : integer := 1;
signal add_graph : integer range 0 to 4095:= 0;
signal pixel_position : integer := 0;

type state_type is (wait_data,clear_data,rx_data,m_read_clk,read_swap,mod_pixel,wr_mem,inc_add);
signal state: state_type;

signal data_in : STD_LOGIC_VECTOR (7 downto 0);
signal data_ready : STD_LOGIC_VECTOR (7 downto 0);

signal data_flag : STD_LOGIC := '0';

begin

DATA_GRAPH <= data_ready(3 downto 0) & data_ready(7 downto 4);
ADR_GRAPH  <= CONV_STD_LOGIC_VECTOR( add_graph,13);

 pDetect: process(CLK,CLK_DATA,ENA_GRAPH,DATA) is    -- Data detection process
 variable detect: std_ulogic_vector(1 downto 0):= "00";
   begin
      if(rising_edge(CLK)) then 
         detect(1) := detect(0);   -- record last value of sync in detect(1)
         detect(0) := CLK_DATA;    -- record current sync in detect(0)
         if(detect = "01") then    -- we have a rising edge
            if(ENA_GRAPH = '1') then 
               data_in <= DATA;
               data_flag <= '1';
               line_s <= conv_integer(LINE); 
               row_s <= conv_integer(ROW);
            else
               data_flag <= '0';
               line_s <= line_s;
               row_s <= row_s;
            end if;   
         else
            data_flag <= '0';   
         end if;  
      end if;   
   end process pDetect; 
   
   
 pStateMachine : process(CLK,State) is
   begin
   if(falling_edge(CLK)) then 
      case State is 
         when wait_data =>
            WR_GRAPH <= '0';
            CS_GRAPH <= '0';
            if (CLEAR = '1')then       -- State clear
               State <= clear_data;         -- 
               CS_GRAPH <= '1';
               data_ready <= "00000000";
            else 
               if(data_flag = '1') then -- Received data 
                  State <= rx_data;
               else
                  State <= wait_data;   -- wait for data 
               end if;
            end if;
          when rx_data =>
            if(DUMP = '1') then 
               add_graph <= ((30 *(line_s - 1)) + (row_s - 1));   -- moving through addres
               State <= wr_mem;
               data_ready <= data_in;
               CS_GRAPH <= '1';
             else                     -- reading address[pixel] 
					add_graph <=  (((30 *(line_s - 1)) + ((row_s - 1))/8));
					pixel_position <= ((row_s - 1) - (((row_s - 1)/8)*8));
					State <= m_read_clk;    
				 end if; 
          when m_read_clk =>  -- clkgen memory read  [pixel] 
            WR_GRAPH <= '1';
            State <= read_swap;
          when read_swap =>
            WR_GRAPH <= '0';
            data_ready <= PIXEL_READ(3 downto 0) & PIXEL_READ(7 downto 4);  -- 
            State <= mod_pixel;
          when mod_pixel =>
            case pixel_position is 
            -- It seems weird that data_in is always set to (0)
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
            State <= wr_mem;
            CS_GRAPH <= '1';
            when wr_mem =>
               WR_GRAPH <= '1';
               State <= wait_data;
            when clear_data =>
               WR_GRAPH <= '1';
               State <= inc_add;
            when inc_add =>
               if(add_graph = 4799) then 
                  add_graph <= 0;
               else
               add_graph <= add_graph + 1;
               end if;
               State <= wait_data;
            when others =>
               State <= wait_data;
         end case;
      end if;    
   end process pStateMachine;
   
   end MDGRAPH;
