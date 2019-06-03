--  UART receive module
--
-- Modifier Daniel Cote 09/05/2012
-- 10/05/2012 Ajout du baud automatique.
--
-- Attend un ENTER (0x0D) pour ajuster sa vitesse
-- Vitesse permises: 9600, 19200, 57600, 115200
--
-- Apres le 0x0D tout les caracteres sont placee a la sortie rx_data
-- et le signal rdrf monte a 1 pour une clk.
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity uart_rx is
    port(
       RxD : in STD_LOGIC;                          -- Reception du data
       mclk : in STD_LOGIC;                         -- 100Mhz 
       rdrf : out STD_LOGIC;                        -- ReadReceiveFlag a 1 quand rxdata pret
       rx_data : out STD_LOGIC_VECTOR(7 downto 0)   -- Donnee recu OK quand rdrf est a 1 
        );
end uart_rx;

architecture uart_rx of uart_rx is

--type state_type is (mark0D, start0D, delay0D, shift0D, stop0D,test0D,mark,start,delay,shift,stop);
--signal state: state_type;
--signal rx0D: STD_LOGIC_VECTOR (107 downto 0);      -- Pour le 0x0D on rammasse 108 points
--                                                   -- a vitesse maximum (115200).
--signal baud_count0D: STD_LOGIC_VECTOR (15 downto 0);
--signal bit_count0D: STD_LOGIC_VECTOR (7 downto 0);
--
--constant bit_time0D: STD_LOGIC_VECTOR (15 downto 0) := X"0363";      -- Cherche 0x0D a 
--constant half_bit_time0D: STD_LOGIC_VECTOR (15 downto 0) := X"01B2"; -- vitesse maximum(115200)

--signal rxdata115200 : STD_LOGIC_VECTOR (7 downto 0);
--signal rxdata57600 : STD_LOGIC_VECTOR (7 downto 0);
--signal rxdata19200 : STD_LOGIC_VECTOR (7 downto 0);
--signal rxdata9600 : STD_LOGIC_VECTOR (7 downto 0);


type state_type is (mark,start,delay,shift,stop);
signal state: state_type;

signal rxbuff: STD_LOGIC_VECTOR (7 downto 0) := x"00";
signal baud_count: STD_LOGIC_VECTOR (15 downto 0);
signal bit_count: STD_LOGIC_VECTOR (3 downto 0);

signal clr: std_logic := '0';

signal  bit_time: STD_LOGIC_VECTOR (15 downto 0) := x"06C5";--X"0002";      --  57600:0x06C5  
signal  half_bit_time: STD_LOGIC_VECTOR (15 downto 0) := x"0363";--X"0001"; --  57600:0x0363  


begin

rx_data <= rxbuff;

uart2: process(mclk, clr)  --(clk, clr, rdrf_clr)      
  begin
    if clr = '1' then
--      state <= mark0D;
      state <= mark;
      rxbuff <= "00000000";
      baud_count <= X"0000";
      bit_count <= "0000";
      rdrf <= '0';
--      rx0D <= X"000000000000000000000000000"; -- 108 acquisitions (115200/9600 * 9bits)
                                              -- Acquisition a 115200 mais vitesse peut etre 9600.
--      baud_count0D <= X"0000";
--      bit_count0D <= "00000000";      
    elsif (mclk'event and mclk = '1') then
    
    case state is   

--         when mark0D =>         -- wait for start bit du caractere 0x0D
--            baud_count0D <= X"0000";
--            bit_count0D <= "00000000";
--            rdrf <= '0';
--            if RxD = '1' then        
--                state <= mark0D;        
--            else
--                state <= start0D;       -- go to start to receive 0x0D
--            end if; 
--        when start0D =>                 -- check for start bit de 0x0D
--            if baud_count0D >= half_bit_time0D then     
--               baud_count0D <= X"0000";
--               state <= delay0D;       
--              else 
--               baud_count0D <= baud_count0D + 1;
--               state <= start0D;       
--            end if;
--         when delay0D =>            
--            if baud_count0D >= bit_time0D then     
--               baud_count0D <= X"0000";
--               if bit_count0D < 108 then   
--                  state <= shift0D;      
--               else               
--                  state <= stop0D;       
--               end if;
--            else 
--               baud_count0D <= baud_count0D + 1;
--               state <= delay0D;       
--            end if;
--         when shift0D =>                -- get next bit
--            rx0D(107) <= RxD;
--            rx0D(106 downto 0) <= rx0D(107 downto 1);
--            bit_count0D <= bit_count0D + 1;
--            state <= delay0D;
--         when stop0D =>
--            rxdata115200 <= rx0D(7) & rx0D(6) & rx0D(5) & rx0D(4) & rx0D(3) & rx0D(2) & rx0D(1) & rx0D(0);
--            rxdata57600 <= rx0D(15) & rx0D(13) & rx0D(11) & rx0D(9) & rx0D(7) & rx0D(5) & rx0D(3) & rx0D(1);
--            rxdata19200 <= rx0D(50) & rx0D(44) & rx0D(38) & rx0D(32) & rx0D(26) & rx0D(20) & rx0D(14) & rx0D(8);
--            rxdata9600 <= rx0D(101) & rx0D(89) & rx0D(77) & rx0D(65) & rx0D(53) & rx0D(41) & rx0D(29) & rx0D(17);         
--            state <= test0D;         
--         when test0D =>              -- Verifie si on a 0x0D a une des vitesses
--            if rxdata115200 = X"0D" then 
--               bit_time <= X"0363";
--               half_bit_time <= X"01B2";
--               state <= mark;
--            elsif rxdata57600 = X"0D" then
--               bit_time <= X"06C5";
--               half_bit_time <= X"0363";
--               state <= mark;         
--            elsif rxdata19200 = X"0D" then
--               bit_time <= X"1450";
--               half_bit_time <= X"0A28";
--               state <= mark;
--            elsif rxdata9600 = X"0D" then
--               bit_time <= X"28A0";
--               half_bit_time <= X"1450";
--               state <= mark;
--            else               
--               bit_time <= X"06C5";
--               half_bit_time <= X"0363";               
--               state <= mark0D;         -- Pas de 0x0D de reconnu, on attend encore
--            end if;

            
         when mark =>                -- wait for start bit du premier caractere
            baud_count <= X"0000";
            bit_count <= "0000";
            rdrf <= '0';             -- Receive Data Ready Flag inactif.
            if RxD = '1' then        
                state <= mark;        
            else
                state <= start;      --   go to start
            end if; 
         when start =>               -- check for start bit
            if baud_count >= half_bit_time then     
               baud_count <= X"0000";
               state <= delay;       
              else 
               baud_count <= baud_count + 1;
               state <= start;       
            end if;
         when delay =>            
            if baud_count >= bit_time then     
               baud_count <= X"0000";
               if bit_count < 8 then   
                  state <= shift;      
               else               
                  state <= stop;       
               end if;
            else 
               baud_count <= baud_count + 1;
               state <= delay;       
            end if;
         when shift =>               -- get next bit
            rxbuff(7) <= RxD;
            rxbuff(6 downto 0) <= rxbuff(7 downto 1);
            bit_count <= bit_count + 1;
            state <= delay;
         when stop =>               
            rdrf <= '1';             -- Receive Data Ready Flag actif.
            state <= mark;         
         end case;            
    end if;
end process uart2; 
  

end uart_rx;
