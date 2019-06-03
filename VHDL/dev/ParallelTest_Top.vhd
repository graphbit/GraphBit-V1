----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    12:45:10 05/05/2019 
-- Design Name: 
-- Module Name:    ParallelTest_Top - Behavioral 
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ParallelTest_Top is
    Port ( DATA_FROM_CPU : in  STD_LOGIC_VECTOR (7 downto 0);
			  RX : in STD_LOGIC;
           --A0_CPU : in  STD_LOGIC;
           CS_CPU : in  STD_LOGIC;
           CLK : in  STD_LOGIC;
           DATA_OUT : out STD_LOGIC_VECTOR (3 downto 0);
			  SHIFT : out STD_LOGIC;
			  LATCH : out STD_LOGIC;
			  FRM : out STD_LOGIC;
			  DF : out STD_LOGIC;
			  SHDN : out STD_LOGIC;
			  DISP : out STD_LOGIC;
			  SUPP : out STD_LOGIC;
			  SUPPIN : in STD_LOGIC
			);
			  
		-- For GraphBit
		attribute loc : string;
    attribute loc of CLK: signal is "P54";
    --attribute loc of A0_CPU: signal is "P86"; 
		attribute loc of DATA_FROM_CPU: signal is "P2, P3, P4, P5, P14, P15, P16, P17";
    --attribute loc of DATA_FROM_CPU: signal is "P81, P91, P82, P92, P87, P93, P88, P94";
	   attribute loc of RX : signal is "P126";
    attribute loc of CS_CPU: signal is "P22";
		attribute loc of DATA_OUT : signal is "P94, P93, P92, P91";
		attribute loc of SHIFT : signal is "P98";
		attribute loc of LATCH : signal is "P105";
		attribute loc of FRM : signal is "P106";
		attribute loc of DF : signal is "P104";
		attribute loc of DISP : signal is "P103";
		attribute loc of SHDN : signal is "P112";
		attribute loc of SUPP : signal is "P32";
		attribute loc of SUPPIN : signal is "P35";
		
		-- For Basys
--		attribute loc : string;
--      attribute loc of CLK: signal is "P54";
--      --attribute loc of A0_CPU: signal is "P86"; 
--		attribute loc of DATA_FROM_CPU: signal is "P81, P91, P82, P92, P87, P93, P88, P94";
--      --attribute loc of DATA_FROM_CPU: signal is "P81, P91, P82, P92, P87, P93, P88, P94";
--      attribute loc of CS_CPU: signal is "P81";
--		attribute loc of DATA_OUT : signal is "P58, P74, P59, P75";
--		attribute loc of SHIFT : signal is "P85";
--		attribute loc of LATCH : signal is "P86";
--		attribute loc of FRM : signal is "P77";
--		attribute loc of DF : signal is "P76";
--		attribute loc of LED : signal is "P2, P3, P4, P5, P7, P8, P14, P15";
			  
end ParallelTest_Top;

architecture Behavioral of ParallelTest_Top is

COMPONENT PARALLELE
	PORT(
		CLK : IN std_logic;
		A0 : IN std_logic;
		DATA_FROM_CPU : IN std_logic_vector(7 downto 0);
		CS : IN std_logic;          
		A0_OUT : OUT std_logic;
		DATA_PARALLELE : OUT std_logic_vector(7 downto 0);
		FLAG_PARALLELE : OUT std_logic
		);
	END COMPONENT;
	
COMPONENT NEW_UART_RX
	PORT(
		RxD : IN std_logic;
		mclk : IN std_logic;          
		rdrf : OUT std_logic;
		rx_data : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;
	
COMPONENT Mux_Comm
	PORT(
		DATA_PARALLELE : IN std_logic_vector(7 downto 0);
		DATA_UART : IN std_logic_vector(7 downto 0);
		CLK : IN std_logic;          
		FLAG_PARALLELE : IN std_logic;
		FLAG_UART : IN std_logic;
		FLAG_COM : OUT std_logic;
		DOUT : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

COMPONENT New_Data_Ctrl
	PORT(
		CLK : IN std_logic;
		DINB : IN std_logic_vector(7 downto 0);          
		DATA : IN std_logic_vector(7 downto 0);
		FLAG : IN std_logic;
		CLKB : OUT std_logic;
		DATA_DP : OUT std_logic_vector(7 downto 0);
		MADDRB : OUT std_logic_vector(12 downto 0);
		MWEB : OUT std_logic;
		SUPP : out std_logic_vector(3 downto 0)
		);
	END COMPONENT; 
	
	COMPONENT Timing_LCD
	PORT(
		CLK : IN std_logic;          
		LATCH : BUFFER std_logic;
		SHIFT : BUFFER std_logic;
		FRM : OUT std_logic;
		DF : OUT std_logic;
		DISPLAYOFF : BUFFER std_logic;
		VEE_ON : BUFFER std_logic;
		MADDRA : BUFFER std_logic_vector(13 downto 0)
		);
	END COMPONENT;
	
	COMPONENT DualPort
	PORT(
		CLK_Shift : IN std_logic;
		DATA_IN : IN std_logic_vector(7 downto 0);
		MWEB : IN std_logic;
		CLKB : IN std_logic;
		MADDRA : IN std_logic_vector(13 downto 0);
		MADDRB : IN std_logic_vector(12 downto 0);          
		DATA_OUT : OUT std_logic_vector(3 downto 0);
		DATA_READ : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

signal Flag : STD_LOGIC;
signal Data : STD_LOGIC_VECTOR(7 downto 0);
signal Flag_Parallel : STD_LOGIC;
signal Data_Parallel : STD_LOGIC_VECTOR (7 downto 0);
signal Flag_UART : STD_LOGIC;
signal Data_UART : STD_LOGIC_VECTOR(7 downto 0);
signal DataOut_s : STD_LOGIC_VECTOR (3 downto 0);
signal Shift_s : STD_LOGIC;
signal Latch_s : STD_LOGIC;

signal unconnectedDisplayOff_s : STD_LOGIC;
signal unconnectedVEEOn_s : STD_LOGIC;		
signal unconnectedA0 : STD_LOGIC;
signal unconnectedFlag : STD_LOGIC;

signal DP_in_s : STD_LOGIC_VECTOR (7 downto 0);
signal DINB_s : STD_LOGIC_VECTOR (7 downto 0);
signal CLKB_s : STD_LOGIC;
signal MWEB_S : STD_LOGIC;
signal addra : STD_LOGIC_VECTOR (13 downto 0); -- addresse du port A
signal addrb : STD_LOGIC_VECTOR (12 downto 0); -- addresse du port a

begin
DATA_OUT <= DataOut_s;
SHIFT <= Shift_s;
LATCH <= Latch_s;
DISP <= '1';
SHDN <= '1';

Inst_PARALLELE: PARALLELE PORT MAP(
		CLK => CLK,
		A0 => '0',
		DATA_FROM_CPU => DATA_FROM_CPU,
		CS => CS_CPU,
		A0_OUT => unconnectedA0,
		DATA_PARALLELE => Data_Parallel,
		FLAG_PARALLELE => Flag_Parallel
	);
	
Inst_uart_rx: new_UART_RX PORT MAP(
		RxD => RX,
		mclk => CLK,
		rdrf => Flag_UART,
		rx_data => Data_UART 
	);

Inst_Mux_Comm: Mux_Comm PORT MAP(
		FLAG_PARALLELE => Flag_Parallel,
		DATA_PARALLELE => Data_Parallel,
		FLAG_UART => Flag_UART,
		DATA_UART => Data_UART,
		FLAG_COM => Flag,
		DOUT => Data,
		CLK => CLK
	);
	
	SUPP <= Flag;
	
Inst_Data_Ctrl: New_Data_Ctrl PORT MAP(
		CLK => CLK,
		DATA => Data,
		FLAG => Flag,
		DINB => DINB_s,
		CLKB => CLKB_s,
		DATA_DP => DP_in_s,
		MADDRB => addrb,
		MWEB => MWEB_S
		--SUPP => SUPP
	); 

Inst_Timing_LCD: Timing_LCD PORT MAP(
		LATCH => Latch_s,
		SHIFT => Shift_s,
		FRM => FRM,
		DF => DF,
		CLK => CLK,
		DISPLAYOFF => unconnectedDisplayOff_s,
		VEE_ON => unconnectedVEEOn_s,
		MADDRA => addra
	);
	
Inst_DualPort: DualPort PORT MAP(
		CLK_Shift => Shift_s,
		DATA_IN => DP_In_s,
		DATA_OUT => DataOut_s,
		MWEB => MWEB_S,
		CLKB => CLKB_s,
		MADDRA => addra,
		MADDRB => addrb,
		DATA_READ => DINB_s
	);



end Behavioral;

