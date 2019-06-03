
-- VHDL Instantiation Created from source file Mux_Comm.vhd -- 15:50:14 05/28/2019
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT Mux_Comm
	PORT(
		DATA_PARALLELE : IN std_logic_vector(7 downto 0);
		DATA_UART : IN std_logic_vector(7 downto 0);
		CLK : IN std_logic;          
		FLAG_PARALLELE : OUT std_logic;
		FLAG_UART : OUT std_logic;
		FLAG_COM : OUT std_logic;
		DOUT : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

	Inst_Mux_Comm: Mux_Comm PORT MAP(
		FLAG_PARALLELE => ,
		DATA_PARALLELE => ,
		FLAG_UART => ,
		DATA_UART => ,
		FLAG_COM => ,
		DOUT => ,
		CLK => 
	);


