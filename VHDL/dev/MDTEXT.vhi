
-- VHDL Instantiation Created from source file MDTEXT.vhd -- 18:02:59 05/20/2019
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT MDTEXT
	PORT(
		CLK : IN std_logic;
		LINE : IN std_logic_vector(7 downto 0);
		ROW : IN std_logic_vector(7 downto 0);
		CLEAR : IN std_logic;
		CLK_DATA : IN std_logic;
		ENA_TEXT : IN std_logic;
		DATA : IN std_logic_vector(7 downto 0);          
		ADR_TEXT : OUT std_logic_vector(12 downto 0);
		WR_TEXT : OUT std_logic;
		CS_TEXT : OUT std_logic;
		DATA_TEXT : OUT std_logic_vector(7 downto 0)
		);
	END COMPONENT;

	Inst_MDTEXT: MDTEXT PORT MAP(
		CLK => ,
		LINE => ,
		ROW => ,
		CLEAR => ,
		CLK_DATA => ,
		ENA_TEXT => ,
		DATA => ,
		ADR_TEXT => ,
		WR_TEXT => ,
		CS_TEXT => ,
		DATA_TEXT => 
	);


