//#include "LCD.h"
#include <string.h>

unsigned char xdata table[4800];
unsigned char xdata* const P_Adress = (unsigned char xdata*)0x8200U;
int main( void )
{
  memset(table, 0xF0, sizeof(table));
  //LCD_Init_Parallel();
  //LCD_Write(LCD_GRAPH, table, 48);
	{
		int i;
		unsigned char checksum;
		*P_Adress = (unsigned char)0x02U;
		checksum = 0x02U;
		*P_Adress = (unsigned char)((sizeof(table) & 0xFF00U) >> 8U);
		checksum += (unsigned char)((sizeof(table) & 0xFF00U) >> 8U);
		*P_Adress = (unsigned char)(sizeof(table) & 0x00FFU);
		checksum += (unsigned char)(sizeof(table) & 0x00FFU);
		for (i = 0; i < sizeof(table); i++)
		{
			*P_Adress = table[i];
			checksum += table[i];
		}
		*P_Adress = checksum;
	}
  while(1){}
}
