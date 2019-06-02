#include "LCD.h"
/* --------------------------------------------------------------------------------------------- */
/* Private Defines */

/* Added size to the I2C Send Buffer 
 * The send buffer is of size (dataSize + LCD_I2C_SENDBUFFER_SIZEADD),
 * where dataSize is taken from the arguments of the function LCD_Write_I2C, 
 * and LCD_I2C_SENDBUFFER_SIZEADD is equal to :
 * - 2 bytes for the packet size indicator 
 * - 1 byte for the command 
 * - 1 byte for the checksum */
#define LCD_I2C_SENDBUFFER_SIZEADD 0x04U

 /* Added size to the UART Send Buffer
  * The send buffer is of size (dataSize + LCD_UART_SENDBUFFER_SIZEADD),
  * where dataSize is taken from the arguments of the function LCD_Write_UART,
  * and LCD_UART_SENDBUFFER_SIZEADD is equal to :
  * - 2 bytes for the packet size indicator
  * - 1 byte for the command
  * - 1 byte for the checksum */
#define LCD_UART_SENDBUFFER_SIZEADD 0x04U

/* Added size to the parallel send buffer 
 * The send buffer is of size (dataSize + LCD_PARALLEL_SENDBUFFER_SIZEADD),
 * where dataSize is taken from the arguments of the function LCD_Write_Parallel, 
 * and LCD_PARALLEL_SENDBUFFER_SIZEADD is equal to :
 * - 2 bytes for the packet size indicator 
 * - 1 byte for the command
 * - 1 byte for the checksum */
#define LCD_PARALLEL_SENDBUFFER_SIZEADD 0x04U

/* Parallel Adress */
/* The following is implementation-defined behavior, and will only work on the 
 * TSO Dallas development kit, 2007 and 2016 models. 
 * This is the definitions of a pointer to an external address. */
/* Adress bit 15 points to external elements */
/* Adress decoder 74AHCT138 has CS2 as output #2 (Y2),
 * so its inputs must be 010, so A10 at 0, A9 at 1 and A8 at 0 */ 
/* A15  A14  A13  A12  A11  A10  A9  A8  A7  A6  A5  A4  A3  A2  A1  A0
 *  1    0    0    0    0    0   1   0   0   0   0   0   0   0   0   0
 * 0x8200 points to CS2 */
/* volatile uint8_t __xdata* const P_Adress = (uint8_t __xdata*)0x8200 */
/* 0x8200 is an address in xdata to a uint8_t element 
 * P_Adress is a pointer that will remain constant (so always pointing to
 * the same address), pointing to an address in xdata to a uint8_t element 
 * that can change anytime 
 * We assign 0x8200 to P_Adress. In order to write to CS2, dereference the pointer */
/* Example : *P_Adress = 0x80U */ 
uint8_t xdata* const P_Adress = (uint8_t xdata*)0x8200U;


/* --------------------------------------------------------------------------------------------- */
/* Create and initialize the LCD structure */
LCD_STRUCT_T LCD_Struct =
{
	NOT_INIT,		/* LCD_MODE_T Mode */
	LCD_NOT_INIT,	/* LCD_STATE_T State */
	NO_ERROR,		/* LCD_ERROR_T Error */
	NULL,			/* LCD_ARRAY_T Array */
};


/* --------------------------------------------------------------------------------------------- */
/* Private Function Declarations */

static LCD_ERROR_T LCD_Write_Empty(const LCD_COMMAND_T Command, const uint8_t* pData, const size_t dataSize);
//static LCD_ERROR_T LCD_Write_I2C(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize);
//static LCD_ERROR_T LCD_Write_UART(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize);
static LCD_ERROR_T LCD_Write_Parallel(const LCD_COMMAND_T Command, const uint8_t* pData, const size_t dataSize);
LCD_ERROR_T(*LCD_Write)(const LCD_COMMAND_T Command, const uint8_t* pData, const size_t dataSize) = LCD_Write_Empty;


/* Commands */
static LCD_ERROR_T LCD_Clear_Empty(void);
//static LCD_ERROR_T LCD_Clear_I2C(void);
//static LCD_ERROR_T LCD_Clear_UART(void);
//static LCD_ERROR_T LCD_Clear_Parallel(void);
LCD_ERROR_T(*LCD_Clear)(void) = LCD_Clear_Empty;

/* Text functions */

/* Graphic functions */
//LCD_ERROR_T LCD_FillLine(uint8_t* pArray, uint8_t StartPixel, uint8_t StopPixel);

/* Array functions */
//static LCD_ERROR_T LCD_Create_Array_Struct(LCD_ARRAY_T* structArray, uint8_t lines, uint8_t columns, uint8_t coordX, uint8_t coordY);
//static LCD_ERROR_T LCD_Delete_Array_Struct(LCD_ARRAY_T* structArray);
//static LCD_ERROR_T LCD_Create_Array(LCD_ARRAY_T* structArray);
//static LCD_ERROR_T LCD_Free_Array(LCD_ARRAY_T* structArray);

/* Default LCD error handler */
static void LCD_Default_ErrorHandler(uint16_t lineNumber);
void(*LCD_ErrorHandler)(uint16_t lineNumber) = LCD_Default_ErrorHandler;

/* --------------------------------------------------------------------------------------------- */
/* Public Functions Definitions */

/* --------------------------------------------------------------------------------------------- */
/* Initialization functions */
/* Contains : 
 *			-	LCD_ERROR_T LCD_Init_I2C(I2C_HANDLER* hi2c, TIM_HANDLER* htim)          -- UNIMPLEMENTED
 *			-	LCD_ERROR_T LCD_Init_UART(UART_HANDLER* hi2c, TIM_HANDLER* htim)	-- UNIMPLEMENTED
 *			-	LCD_ERROR_T LCD_Init_Parallel(TIM_HANDLER* htim)                        -- IMPLEMENTATION
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      LCD_Init_Parallel                                               
* LCD_ERROR_T LCD_Init_Parallel(void)
* Author : Pascal-Emmanuel Lachance & Charles L�vesque-Matte
* Date : 2019-05-22
* 
* Initializes the LCD module in parallel mode.
* 
* Called by : none
* Function calls : 
*			- LCD_Clear()
*			
* Parameters : none
* Returns: LCD_ERROR_T Init_I2C_Error
*			- E_PARALLEL_INIT: Error during Parallel library initialization of the parallel bus
*			
* Example call : LCD_Init_Parallel();
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
LCD_ERROR_T LCD_Init_Parallel(void)
{
	LCD_ERROR_T Init_Error = NO_ERROR;
	LCD_Struct.State = LCD_NOT_INIT;	/* Default state */
	LCD_Struct.Mode = PARALLEL_MODE;		
	
        /* Handle function pointers */
        LCD_Write = LCD_Write_Parallel;
        /* Clear the LCD */
        Init_Error = LCD_Clear();

        /* Module is ready to start operations */
        LCD_Struct.State = LCD_READY;
	return (Init_Error);
}

/* --------------------------------------------------------------------------------------------- */
/* Private Functions Definitions */

static LCD_ERROR_T LCD_Write_Empty(const LCD_COMMAND_T Command, const uint8_t* pData, const size_t dataSize)
{
  return (E_UNIMPLEMENTED);
}

static LCD_ERROR_T LCD_Write_Parallel(const LCD_COMMAND_T Command, const uint8_t* pData, const size_t dataSize)
{
  LCD_ERROR_T P_Error = NO_ERROR;
  if (LCD_Struct.State == LCD_READY)
  {
    LCD_Struct.State = LCD_BUSY;
          /* Check if dataSize is too big */
          if ((dataSize + LCD_PARALLEL_SENDBUFFER_SIZEADD) > dataSize)
          {
                  /* Initialize data structure */
                  /* (Variable lenghts array are illegal in C89, make sure the compiler used is based on C99) */
                  size_t write_i = 0x00U;
                  uint8_t* Data = (uint8_t*)malloc((dataSize + LCD_PARALLEL_SENDBUFFER_SIZEADD) * sizeof(uint8_t));
                  uint8_t Checksum = (dataSize + LCD_PARALLEL_SENDBUFFER_SIZEADD);
          
                  /* Dynamic stack memory allocation, we must check if it's valid */
                  if ((&Data[0] != NULL) && (&Data[0] != 0))
                  {
                          memset(Data, 0x0000, (dataSize + LCD_PARALLEL_SENDBUFFER_SIZEADD));
                          size_t temp_dataSize = (dataSize + LCD_PARALLEL_SENDBUFFER_SIZEADD); 
                          /* The amount of data is the first and second bytes sent */
                          Data[0] = (uint8_t)((temp_dataSize & 0xFF00U) >> 8U);
                          Data[1] = (uint8_t)(temp_dataSize & 0x00FFU);
                          /* Command is the second byte sent */
                          Data[2] = Command;
                  
                          /* Check pointer value */
                          /* It is possible for pData to be a NULL pointer, but only if dataSize is equal to 0 */
                          if (((pData != NULL) && (dataSize != 0)))
                          {
                                  /* Populating the array with pData */
                                  for (write_i = (LCD_PARALLEL_SENDBUFFER_SIZEADD - 0x01U); write_i <= (dataSize + (LCD_PARALLEL_SENDBUFFER_SIZEADD - 0x02U)); write_i++)
                                  {
                                          Data[write_i] = pData[(write_i - (LCD_PARALLEL_SENDBUFFER_SIZEADD - 0x01U))];
                                          Checksum += Data[write_i];
                                  }
                          
                                  /* Calculating checksum's 2's complement value and placing it at the end of the array */
                                  Checksum = (uint8_t)((0xFF - Checksum) + 0x01U);
                                  Data[dataSize + (LCD_PARALLEL_SENDBUFFER_SIZEADD - 0x01U)] = Checksum;
                                  
                                  /* Sending data on the parallel bus */
                                  for (write_i = 0x00U; write_i < (dataSize + LCD_PARALLEL_SENDBUFFER_SIZEADD); write_i++)
                                  {
                                          /* Send a byte */
                                          *P_Adress = Data[write_i];
                                  }
                                  /* End of normal behavior */
                                  free(Data);
                                  LCD_Struct.State = LCD_READY;
                          
                          }
                          else					/* (pData == NULL) && (dataSize != 0) */
                          {
                                  /* This functions accepts null pointers to a data buffer
                                          * But only if the datasize is equal to 0, indicating that 
                                          * we don't want to send any data beyond the command & checksum 
                                          * If the pointer is null but dataSize has a value, there was an error */
                                  free(Data);
                                  P_Error = E_WRITE_ARRAY;
                                  LCD_Struct.Error = E_WRITE_ARRAY;
                                  LCD_ErrorHandler(__LINE__);
                                  assert(pData != NULL);
                          }
                  }
                  else						/* (&Data[0] == NULL) || (&Data[0] == 0) */
                  {
                          /* Array pointer is invalid */
                          P_Error = E_WRITE_ARRAY;
                          LCD_Struct.Error = E_WRITE_ARRAY;
                          LCD_ErrorHandler(__LINE__);
                          assert(&Data[0] != NULL);
                          assert(&Data[0] != 0);
                  }
          }
          else						/* (dataSize + LCD_PARALLEL_SENDBUFFER_SIZEADD) > dataSize*/
          {
                  /* The data exceeds the data size limit (65535 - LCD_PARALLEL_SENDBUFFER_SIZEADD)*/
                  /* This if instruction may seem weird. But in the case of an overflow, the 
                          * result of the addition would be lower than the initial value of dataSize */
                  P_Error = E_WRITE_SIZE;
                  LCD_Struct.Error = E_WRITE_SIZE;
                  LCD_ErrorHandler(__LINE__);
          }
  }
  else							/* LCD_Struct.State != LCD_READY */
  {
          /* The module isn't ready to write */
          switch (LCD_Struct.State)	/* Check its state */
          {
          case LCD_BUSY:
                  /* Module is currently performing operations, try later */
                  P_Error = E_PARALLEL_WRITE_S_BUSY;
                  LCD_Struct.Error = E_PARALLEL_WRITE_S_BUSY;
                  LCD_ErrorHandler(__LINE__);
                  break;
          case LCD_FAILURE:
                  /* Module has critically failed */
                  P_Error = E_PARALLEL_WRITE_S_FAILURE;
                  LCD_Struct.Error = E_PARALLEL_WRITE_S_FAILURE;
                  LCD_ErrorHandler(__LINE__);
                  break;
          default:
                  /* You should never get here */
                  LCD_Struct.State = LCD_FAILURE;
                  P_Error = E_PARALLEL_WRITE_S_FAILURE;
                  LCD_Struct.Error = E_PARALLEL_WRITE_S_FAILURE;
                  LCD_ErrorHandler(__LINE__);
                  break;
          }
  }
	
	return (P_Error);
}



static LCD_ERROR_T LCD_Clear_Empty(void)
{
  return (E_UNIMPLEMENTED);
}
/* Error handler */

void LCD_Default_ErrorHandler(uint16_t lineNumber)
{
	//printf("Error at line %d : ", lineNumber);
	switch (LCD_Struct.Error)
	{
	case NO_ERROR:
		break;
		/* Everything else to be implemented */
	default:
		break;
	}
	//printf("\n");
}