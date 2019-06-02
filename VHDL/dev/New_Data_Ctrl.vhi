
-- VHDL Instantiation Created from source file New_Data_Ctrl.vhd -- 21:07:05 05/14/2019
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT New_Data_Ctrl
	PORT(
		CLK : IN std_logic;
		DATA : IN std_logic_vector(7 downto 0);
		DINB : IN std_logic_vector(7 downto 0);    
		FLAG : INOUT std_logic;      
		CLKB : OUT std_logic;
		DATA_DP : OUT std_logic_vector(7 downto 0);
		MADDRB : OUT std_logic_vector(12 downto 0);
		MWEB : OUT std_logic;
		LED : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

	Inst_New_Data_Ctrl: New_Data_Ctrl PORT MAP(
		CLK => ,
		DATA => ,
		FLAG => ,
		DINB => ,
		CLKB => ,
		DATA_DP => ,
		MADDRB => ,
		MWEB => ,
		LED => 
	);


