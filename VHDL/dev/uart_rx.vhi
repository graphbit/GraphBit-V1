
-- VHDL Instantiation Created from source file uart_rx.vhd -- 15:50:57 05/28/2019
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT uart_rx
	PORT(
		RxD : IN std_logic;
		mclk : IN std_logic;          
		rdrf : OUT std_logic;
		rx_data : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

	Inst_uart_rx: uart_rx PORT MAP(
		RxD => ,
		mclk => ,
		rdrf => ,
		rx_data => 
	);


