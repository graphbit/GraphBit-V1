#include "LCD.h"
/* --------------------------------------------------------------------------------------------- */
/* Private Defines */

/* Timer Period = (Oscillator Frequency (in KHz) / Frequency of the time base(in Hz) * 1000) - 1
 * HSI_VALUE is Oscillator Frequency in Hz. Dividing it by 1000 gives frequency in KHz
 * (HSI_VALUE is given in stm32f4xx_hal_conf.h and set as system clock in system_stm32f4xx.c)
 * We want a 1000Hz Time Base
 * */
#define LCD_TIMER_PERIOD ((unsigned int)((HSI_VALUE / 1000U) / 1000U * 1000U) - 1U)

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

/* Added size to the SPI Send Buffer 
 * The send buffer is of size (dataSize + LCD_SPI_SENDBUFFER_SIZEADD),
 * where dataSize is taken from the arguments of the function LCD_Write_I2C, 
 * and LCD_SPI_SENDBUFFER_SIZEADD is equal to :
 * - 2 bytes for the packet size indicator 
 * - 1 byte for the command 
 * - 1 byte for the checksum */
#define LCD_SPI_SENDBUFFER_SIZEADD 0x04U


/* Added size to the parallel send buffer 
 * The send buffer is of size (dataSize + LCD_PARALLEL_SENDBUFFER_SIZEADD),
 * where dataSize is taken from the arguments of the function LCD_Write_Parallel, 
 * and LCD_PARALLEL_SENDBUFFER_SIZEADD is equal to :
 * - 2 bytes for the packet size indicator 
 * - 1 byte for the command
 * - 1 byte for the checksum */
#define LCD_PARALLEL_SENDBUFFER_SIZEADD 0x04U

/* --------------------------------------------------------------------------------------------- */
/* Create and initialize the LCD structure */
LCD_STRUCT_T LCD_Struct =
{
	NOT_INIT, /* LCD_MODE_T Mode */
	LCD_NOT_INIT, /* LCD_STATE_T State */
	NO_ERROR, /* LCD_ERROR_T Error */
	NULL, /* I2C_HandleTypeDef hi2c */
	NULL, /* UART_HandleTypeDef huart */
	NULL,
	/* SPI_HandleTypeDef hspi */
	NULL, /* TIM_HandleTypeDef htim */
	NULL, /* LCD_ARRAY_T Array */
};



/* --------------------------------------------------------------------------------------------- */
/* Private Function Declarations */
static LCD_ERROR_T LCD_Write_Empty(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize);
static LCD_ERROR_T LCD_Write_I2C(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize);
static LCD_ERROR_T LCD_Write_UART(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize);
static LCD_ERROR_T LCD_Write_SPI(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize);
static LCD_ERROR_T LCD_Write_Parallel(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize);
LCD_ERROR_T(*LCD_Write)(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize) = LCD_Write_Empty;


/* Commands */
static LCD_ERROR_T LCD_Clear_Empty(void);
static LCD_ERROR_T LCD_Clear_I2C(void);
static LCD_ERROR_T LCD_Clear_UART(void);
static LCD_ERROR_T LCD_Clear_SPI(void);
static LCD_ERROR_T LCD_Clear_Parallel(void);
LCD_ERROR_T(*LCD_Clear)(void) = LCD_Clear_Empty;

/* Text functions */

/* Graphic functions */
LCD_ERROR_T LCD_FillLine(uint8_t* pArray, uint8_t StartPixel, uint8_t StopPixel);
/* Timer functions */
static LCD_ERROR_T LCD_Config_Timer(TIM_HANDLER* htim);
static LCD_ERROR_T LCD_Init_Timer(TIM_HANDLER* htim);
static void LCD_Timer_Interrupt_Empty(void);
static void LCD_Timer_Interrupt_I2C(void);
static void LCD_Timer_Interrupt_UART(void);
static void LCD_Timer_Interrupt_SPI(void);
static void LCD_Timer_Interrupt_Parallel(void);
static void(*LCD_Timer_Interrupt)(void) = LCD_Timer_Interrupt_Empty;
static LCD_ERROR_T LCD_Timer_Start(TIM_HANDLER* htim);
static LCD_ERROR_T LCD_Timer_Stop(TIM_HANDLER* htim);
void HAL_TIM_PeriodElapsedCallback(TIM_HANDLER *htim);

/* Array functions */
static LCD_ERROR_T LCD_Create_Array_Struct(LCD_ARRAY_T* structArray, uint8_t lines, uint8_t columns, uint8_t coordX, uint8_t coordY);
static LCD_ERROR_T LCD_Delete_Array_Struct(LCD_ARRAY_T* structArray);
static LCD_ERROR_T LCD_Create_Array(LCD_ARRAY_T* structArray);
static LCD_ERROR_T LCD_Free_Array(LCD_ARRAY_T* structArray);

/* Default LCD error handler */
static void LCD_Default_ErrorHandler(uint16_t lineNumber, LCD_ERROR_T Error);
void(*LCD_ErrorHandler)(uint16_t lineNumber, LCD_ERROR_T Error) = LCD_Default_ErrorHandler;

/* --------------------------------------------------------------------------------------------- */
/* Public Functions Definitions */

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      LCD_Reset                                                
* LCD_ERROR_T LCD_Reset(void)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-23
* 
* Reset the entire LCD module, clearing handlers, function pointers and dynamic arrays
* 
* Called by : none
* Function calls :
*			- LCD_Timer_Stop()
*			- LCD_Delete_Array_Struct()
*			
* Parameters : void
* Returns: LCD_ERROR_T Reset_Error
*			- E_TIM_STOP : If the timer couldn't be stopped properly
*			
* Example call : LCD_Reset();
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
LCD_ERROR_T LCD_Reset(void)
{
	LCD_ERROR_T Reset_Error = NO_ERROR;
	
	/* Default mode and state */
	LCD_Struct.State = LCD_NOT_INIT;
	LCD_Struct.Mode = NOT_INIT;
	
	/* Stop timer */
	if (LCD_Struct.htim != NULL)
	{
		Reset_Error = LCD_Timer_Stop(LCD_Struct.htim);		
	}
	
	/* Clear handlers */
	LCD_Struct.htim = NULL;
	LCD_Struct.hi2c = NULL;
	LCD_Struct.huart = NULL;
	LCD_Struct.hspi = NULL;
	
	/* Delete array structure if it exists */
	LCD_Delete_Array_Struct(LCD_Struct.Array);
	
	/* Reset function pointers */
	LCD_Timer_Interrupt = LCD_Timer_Interrupt_Empty;
	LCD_Write = LCD_Write_Empty;
	LCD_Clear = LCD_Clear_Empty;
	
	return (Reset_Error);
}

/* --------------------------------------------------------------------------------------------- */
/* Initialization functions */
/* Contains : 
 *			-	LCD_ERROR_T LCD_Init_I2C(I2C_HANDLER* hi2c, TIM_HANDLER* htim)
 *			-	LCD_ERROR_T LCD_Init_UART(UART_HANDLER* hi2c, TIM_HANDLER* htim)
 *			-	LCD_ERROR_T LCD_Init_SPI(SPI_HANDLER* hi2c, TIM_HANDLER* htim)
 *			-	LCD_ERROR_T LCD_Init_Parallel(TIM_HANDLER* htim)
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      LCD_Init_I2C                                               
* LCD_ERROR_T LCD_Init_I2C(I2C_HANDLER* hi2c, TIM_HANDLER* htim)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-20
* 
* Initializes the LCD module in I2C mode.
* 
* Called by : none
* Function calls : 
*			- LCD_Config_Timer()
*			- LCD_Clear()
*			
* Parameters : I2C_HANDLER* hi2c, TIM_HANDLER* htim
*			- hi2c : pointer to a I2C_HandleTypeDef structure from the HAL librairies
*			- htim : pointer to a TIM_HandleTypeDef structure from the HAL librairies
* Returns: LCD_ERROR_T Init_I2C_Error
*			- E_I2C_INIT : If the I2C_HANDLER hi2c pointer is invalid
*			- E_TIM_HAL_ERROR : Error during HAL initialization of the timer
*			- E_TIM_HAL_ERROR_CLKSOURCE : Error during HAL initialization of the timer clocksource
*			- E_TIM_HAL_ERROR_SLAVE : Error during HAL initialization of the timer slave
*			- E_TIM_HAL_ERROR_MASTER : Error during HAL initialization of the timer master
*			- E_TIM_INIT : Invalid TIM_HANDLER htim pointer
*			
* Example call : LCD_Init_I2C(&hi2c1, &htim2);
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
LCD_ERROR_T LCD_Init_I2C(I2C_HANDLER* hi2c, TIM_HANDLER* htim)
{
	LCD_ERROR_T Init_I2C_Error = NO_ERROR;
	LCD_Struct.State = LCD_NOT_INIT; /* Default state */
	
	if ((hi2c != NULL) && (hi2c != 0))  /* Check the validity of I2C handler pointer */
	{
		LCD_Struct.hi2c = hi2c; /* Assigns the I2C pointer to the LCD structure */
		LCD_Struct.huart = NULL; /* Unassign the UART handler pointer */
		LCD_Struct.hspi = NULL; /* Unassign the SPI handler pointer */
		LCD_Struct.htim = NULL; /* Unassign the timer handler pointer */
		LCD_Struct.Mode = I2C_MODE;		
		
		/* Initialize Timer */
		Init_I2C_Error = LCD_Config_Timer(htim);
		if (Init_I2C_Error == NO_ERROR)
		{
			/* Handle function pointers */
			LCD_Write = LCD_Write_I2C;
			LCD_Clear = LCD_Clear_I2C;
		
			/* Clear the LCD */
			Init_I2C_Error = LCD_Clear();
			
			/* Module is ready to commence operations */
			LCD_Struct.State = LCD_READY;
		}
	}
	else								/* (hi2c == NULL) || (hi2c == 0) */
	{
		/* The I2C Handler pointer is invalid */
		Init_I2C_Error = E_I2C_INIT;
		LCD_Struct.Error = E_I2C_INIT;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		assert(hi2c != NULL);
		assert(hi2c != 0);
	}
	return (Init_I2C_Error);
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      LCD_Init_UART                                               
* LCD_ERROR_T LCD_Init_UART(UART_HANDLER* huart, TIM_HANDLER* htim)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-05-02
* 
* Initializes the LCD module in UART mode.
* 
* Called by : none
* Function calls : 
*			- LCD_Config_Timer()
*			- LCD_Clear()
*			
* Parameters : UART_HANDLER* huart, TIM_HANDLER* htim
*			- huart : pointer to a UART_HandleTypeDef structure from the HAL librairies
*			- htim : pointer to a TIM_HandleTypeDef structure from the HAL librairies
* Returns: LCD_ERROR_T Init_I2C_Error
*			- E_UART_INIT : If the UART_HANDLER hi2c pointer is invalid
*			- E_TIM_HAL_ERROR : Error during HAL initialization of the timer
*			- E_TIM_HAL_ERROR_CLKSOURCE : Error during HAL initialization of the timer clocksource
*			- E_TIM_HAL_ERROR_SLAVE : Error during HAL initialization of the timer slave
*			- E_TIM_HAL_ERROR_MASTER : Error during HAL initialization of the timer master
*			- E_TIM_INIT : Invalid TIM_HANDLER htim pointer
*			
* Example call : LCD_Init_UART(&huart2, &htim2);
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
LCD_ERROR_T LCD_Init_UART(UART_HANDLER* huart, TIM_HANDLER* htim)
{
	LCD_ERROR_T Init_Uart_Error = NO_ERROR;
	LCD_Struct.State = LCD_NOT_INIT; /* Default state */
	
	if ((huart != NULL) && huart != 0)
	{
		LCD_Struct.hi2c = NULL; /* Unassigns the I2C pointer to the LCD structure */
		LCD_Struct.huart = huart; /* Assign the UART handler pointer to the LCD structures */
		LCD_Struct.hspi = NULL; /* Unassign the SPI handler pointer */
		LCD_Struct.htim = NULL; /* Unassign the timer handler pointer */
		LCD_Struct.Mode = UART_MODE;		
	}

	/* Initialize Timer */
	Init_Uart_Error = LCD_Config_Timer(htim);
	if (Init_Uart_Error == NO_ERROR)
	{
		/* Handle function pointers */
		LCD_Write = LCD_Write_UART;
		LCD_Clear = LCD_Clear_UART;

		/* Clear the LCD */
		Init_Uart_Error = LCD_Clear();

		/* Module is ready to commence operations */
		LCD_Struct.State = LCD_READY;
	}

	else								/* (huart == NULL) || (huart == 0) */
	{
		/* The HUART Handler pointer is invalid */
		Init_Uart_Error = E_UART_INIT;
		LCD_Struct.Error = E_UART_INIT;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		assert(huart != NULL);
		assert(huart != 0);
	}
	
	return (Init_Uart_Error);
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      LCD_Init_SPI                                               
* LCD_ERROR_T LCD_Init_SPI(SPI_HANDLER* hspi, TIM_HANDLER* htim)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-05-08
* 
* Initializes the LCD module in SPI mode.
* 
* Called by : none
* Function calls : 
*			- LCD_Config_Timer()
*			- LCD_Clear()
*			
* Parameters : SPI_HANDLER* hspi, TIM_HANDLER* htim
*			- hspi : pointer to a SPI_HandleTypeDef structure from the HAL librairies
*			- htim : pointer to a TIM_HandleTypeDef structure from the HAL librairies
* Returns: LCD_ERROR_T Init_SPI_Error
*			- E_SPI_INIT : If the SPI_HANDLER hspi pointer is invalid
*			- E_TIM_HAL_ERROR : Error during HAL initialization of the timer
*			- E_TIM_HAL_ERROR_CLKSOURCE : Error during HAL initialization of the timer clocksource
*			- E_TIM_HAL_ERROR_SLAVE : Error during HAL initialization of the timer slave
*			- E_TIM_HAL_ERROR_MASTER : Error during HAL initialization of the timer master
*			- E_TIM_INIT : Invalid TIM_HANDLER htim pointer
*			
* Example call : LCD_Init_UART(&spi1, &htim2);
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
LCD_ERROR_T LCD_Init_SPI(SPI_HANDLER* hspi, TIM_HANDLER* htim)
{
	LCD_ERROR_T Init_SPI_Error = NO_ERROR;
	LCD_Struct.State = LCD_NOT_INIT; /* Default state */
	
	if ((hspi != NULL) && (hspi != 0))  /* Check the validity of spi handler pointer */
	{
		LCD_Struct.hspi = NULL; /* Assigns the spi pointer to the LCD structure */
		LCD_Struct.huart = NULL; /* Unassign the UART handler pointer */
		LCD_Struct.hspi = hspi; /* Unassign the SPI handler pointer */
		LCD_Struct.htim = NULL; /* Unassign the timer handler pointer */
		LCD_Struct.Mode = SPI_MODE;		
		
		/* Initialize Timer */
		Init_SPI_Error = LCD_Config_Timer(htim);
		if (Init_SPI_Error == NO_ERROR)
		{
			/* Handle function pointers */
			LCD_Write = LCD_Write_SPI;
			LCD_Clear = LCD_Clear_SPI;
		
			/* Clear the LCD */
			Init_SPI_Error = LCD_Clear();
			
			/* Module is ready to commence operations */
			LCD_Struct.State = LCD_READY;
		}
	}
	else								/* (hspi == NULL) || (hspi == 0) */
	{
		/* The I2C Handler pointer is invalid */
		Init_SPI_Error = E_SPI_INIT;
		LCD_Struct.Error = E_SPI_INIT;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		assert(hspi != NULL);
		assert(hspi != 0);
	}
	return (Init_SPI_Error);
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      LCD_Init_Parallel                                               
* LCD_ERROR_T LCD_Init_Parallel(TIM_HANDLER* htim)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-26
* 
* Initializes the LCD module in parallel mode.
* Initializes the parallel library Parallel.h
* 
* Called by : none
* Function calls : 
*			- Parallel_InitData()
*			- Parallel_InitAdress()
*			- Parallel_InitChipSelect()
*			- Parallel_Init()
*			- LCD_Config_Timer()
*			- LCD_Clear()
*			
* Parameters : TIM_HANDLER* htim
*			- htim : pointer to a TIM_HandleTypeDef structure from the HAL librairies
* Returns: LCD_ERROR_T Init_I2C_Error
*			- E_PARALLEL_INIT: Error during Parallel library initialization of the parallel bus
*			- E_TIM_HAL_ERROR : Error during HAL initialization of the timer
*			- E_TIM_HAL_ERROR_CLKSOURCE : Error during HAL initialization of the timer clocksource
*			- E_TIM_HAL_ERROR_SLAVE : Error during HAL initialization of the timer slave
*			- E_TIM_HAL_ERROR_MASTER : Error during HAL initialization of the timer master
*			- E_TIM_INIT : Invalid TIM_HANDLER htim pointer
*			
* Example call : LCD_Init_Parallel(&htim2);
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
LCD_ERROR_T LCD_Init_Parallel(TIM_HANDLER* htim)
{
	LCD_ERROR_T Init_Error = NO_ERROR;
	P_ERROR_T Parallel_Init_Error = P_NO_ERROR;
	LCD_Struct.State = LCD_NOT_INIT; /* Default state */
	
	LCD_Struct.hi2c = NULL; /* Assigns the I2C pointer to the LCD structure */
	LCD_Struct.huart = NULL; /* Unassign the UART handler pointer */
	LCD_Struct.hspi = NULL; /* Unassign the SPI handler pointer */
	LCD_Struct.htim = NULL; /* Unassign the timer handler pointer */
	LCD_Struct.Mode = PARALLEL_MODE;		
	
	/* Initialize the parallel bus */
	Parallel_Init_Error = Parallel_InitReadWrite(GPIOB, GPIO_PIN_0, NULL, 0x0000U);
	if (Parallel_Init_Error == P_NO_ERROR)
	{
		Parallel_Init_Error = Parallel_InitData(0x08U, GPIOE, GPIO_PIN_8, GPIOE, GPIO_PIN_9, GPIOE, GPIO_PIN_10, GPIOE, GPIO_PIN_11, GPIOE, GPIO_PIN_12, GPIOE, GPIO_PIN_13, GPIOE, GPIO_PIN_14, GPIOE, GPIO_PIN_15);
		if (Parallel_Init_Error == P_NO_ERROR)
		{
			Parallel_Init_Error = Parallel_InitAdress(0x00U);
			if (Parallel_Init_Error == P_NO_ERROR)
			{
				/* On the parallel extension board, there is a routing error connecting CS2 (STM side) to CS1 (connector side). */
				Parallel_Init_Error = Parallel_InitChipSelect(0x03U, GPIOB, GPIO_PIN_11, GPIOB, GPIO_PIN_13, GPIOB, GPIO_PIN_12);
				if (Parallel_Init_Error == P_NO_ERROR)
				{
					Parallel_Init_Error = Parallel_Init();
					if (Parallel_Init_Error == P_NO_ERROR)
					{
						/* Initialize Timer */
						Init_Error = LCD_Config_Timer(htim);
						if (Init_Error == NO_ERROR)
						{
							/* Handle function pointers */
							LCD_Write = LCD_Write_Parallel;
							/* Clear the LCD */
							Init_Error = LCD_Clear();
			
							/* Module is ready to start operations */
							LCD_Struct.State = LCD_READY;
						}
					}
				}
			}
		}
	}
	if (Parallel_Init_Error != P_NO_ERROR)
	{
		/* Error during initialisation of the bus */
		Init_Error = E_PARALLEL_INIT;
		LCD_Struct.Error = E_PARALLEL_INIT;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
	}
	
	return (Init_Error);
}

/* --------------------------------------------------------------------------------------------- */
/* Private Functions Definitions */
static LCD_ERROR_T LCD_Write_Empty(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize)
{
	return (E_UNINITIALIZED);	
}
static LCD_ERROR_T LCD_Write_I2C(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize)
{
	LCD_ERROR_T I2C_Error = NO_ERROR;
	if (LCD_Struct.State == LCD_READY)
	{
		LCD_Struct.State = LCD_BUSY;
		HAL_StatusTypeDef I2C_State = HAL_OK;
		/* Check if dataSize is too big */
		if ((dataSize + LCD_I2C_SENDBUFFER_SIZEADD) > dataSize)
		{
			/* Initialize data structure */
			/* (Variable length array are illegal in C89, make sure the compiler used is based on C99) */
			size_t write_i = 0x00U;
			uint8_t Data[(dataSize + LCD_I2C_SENDBUFFER_SIZEADD)];
			uint8_t Checksum = (dataSize + LCD_I2C_SENDBUFFER_SIZEADD);
			Checksum += Command;
		
			/* Dynamic stack memory allocation, we must check if it's valid */
			if ((&Data[0] != NULL) && (&Data[0] != 0))
			{
				size_t temp_dataSize = (dataSize + LCD_I2C_SENDBUFFER_SIZEADD); 
				/* The amount of data is the first and second bytes sent */
				Data[0] = (uint8_t)((temp_dataSize & 0xFF00U) >> 8U);
				Data[1] = (uint8_t)(temp_dataSize & 0x00FFU);
				/* Command is the second byte sent */
				Data[2] = Command;
			
				/* Check pointer value */
				/* It is possible for pData to be a NULL pointer, but only if dataSize is equal to 0 */
				if ((pData != NULL) || ((pData == NULL) && (dataSize != 0)))
				{
					if (pData != NULL)
					{
						/* Populating the array with pData */
						for (write_i = (LCD_I2C_SENDBUFFER_SIZEADD - 0x01U); write_i <= (dataSize + (LCD_I2C_SENDBUFFER_SIZEADD - 0x02U)); write_i++)
						{
							Data[write_i] = pData[(write_i - (LCD_I2C_SENDBUFFER_SIZEADD - 0x01U))];
							Checksum += Data[write_i];
						}
					}
				
					/* Calculating checksum's 2's complement value and placing it at the end of the array */
					Checksum = (~Checksum) + 0x01U;
					Data[dataSize + (LCD_I2C_SENDBUFFER_SIZEADD - 0x01U)] = Checksum;
					/* Calculating a timeout value */
					uint32_t timeoutValue = 0x03U * + (uint32_t)(dataSize /  0x04U);
				
					/* Sending data via I2C */
					I2C_State = HAL_I2C_Master_Transmit(LCD_Struct.hi2c, LCD_I2C_ADRESS, &Data[0], (uint16_t)(dataSize + LCD_I2C_SENDBUFFER_SIZEADD), timeoutValue);
				
					if (I2C_State == HAL_OK)
					{
						/* Operations with the module have been completed successfully, reset state */
						LCD_Struct.State = LCD_READY;
					}
					else			/* I2C_State != HAL_OK */
					{
						/* There was a HAL error */
						switch (I2C_State)
						{
						case HAL_BUSY:
							I2C_Error = E_I2C_HAL_BUSY;
							break;
						case HAL_TIMEOUT:
							I2C_Error = E_I2C_HAL_TIMEOUT;
							break;
						case HAL_ERROR:
							/* Nested switch case, to check which error arose */
							switch (LCD_Struct.hi2c->ErrorCode)
							{
							case HAL_I2C_ERROR_AF:		/* Acknowledge failure */
								/* This error is the one most likely to occur in HAL_I2C_Master_Transmit */
								I2C_Error = E_I2C_HAL_ERROR_AF;
								LCD_Struct.Error = E_I2C_HAL_ERROR_AF;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_I2C_ERROR_BERR:	/* Bus error */
								I2C_Error = E_I2C_HAL_ERROR_BERR;
								LCD_Struct.Error = E_I2C_HAL_ERROR_BERR;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_I2C_ERROR_ARLO:	/* Arbitration lost */
								I2C_Error = E_I2C_HAL_ERROR_ARLO;
								LCD_Struct.Error = E_I2C_HAL_ERROR_ARLO;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_I2C_ERROR_OVR:		/* Overrun */
								I2C_Error = E_I2C_HAL_ERROR_OVR;
								LCD_Struct.Error = E_I2C_HAL_ERROR_OVR;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_I2C_ERROR_DMA:		/* DMA */
								I2C_Error = E_I2C_HAL_ERROR_DMA;
								LCD_Struct.Error = E_I2C_HAL_ERROR_DMA;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							default:					/* Timeout error or magic */ 
														/* This shouldn't ever happen */
								I2C_Error = E_I2C_HAL_ERROR;
								LCD_Struct.Error = E_I2C_HAL_ERROR;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							}
							break;
						default:
							I2C_Error = E_I2C_HAL_ERROR; /* This shouldn't ever happen */
							LCD_Struct.Error = E_I2C_HAL_ERROR;
							LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
							break;
						}
					}
				}
				else				/* (pData == NULL) && (dataSize != 0) */
				{
					/* This functions accepts null pointers to a data buffer
						* But only if the datasize is equal to 0, indicating that 
						* we don't want to send any data beyond the command & checksum 
						* If the pointer is null but dataSize has a value, there was an error */
					I2C_Error = E_WRITE_ARRAY;
					LCD_Struct.Error = E_WRITE_ARRAY;
					LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
					assert(pData != NULL);
				}
			}
			else					/* (&Data[0] == NULL) || (&Data[0] == 0) */
			{
				/* Array pointer is invalid */
				I2C_Error = E_WRITE_ARRAY;
				LCD_Struct.Error = E_WRITE_ARRAY;
				LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
				assert(&Data[0] != NULL);
				assert(&Data[0] != 0);
			}
		}
		else						/* (dataSize + LCD_I2C_SENDBUFFER_SIZEADD) > dataSize*/
		{
			/* The data exceeds the data size limit (65535 - LCD_I2C_SENDBUFFER_SIZEADD)*/
			/* This if instruction may seem weird. But in the case of an overflow, the 
			 * result of the addition would be lower than the initial value of dataSize */
			I2C_Error = E_WRITE_SIZE;
			LCD_Struct.Error = E_WRITE_SIZE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		}
	}
	else							/* LCD_Struct.State != LCD_READY */
	{
		/* The module isn't ready to write */
		switch (LCD_Struct.State)	/* Check its state */
		{
		case LCD_BUSY:
			/* Module is currently performing operations, try later */
			I2C_Error = E_I2C_WRITE_S_BUSY;
			LCD_Struct.Error = E_I2C_WRITE_S_BUSY;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		case LCD_FAILURE:
			/* Module has critically failed */
			I2C_Error = E_I2C_WRITE_S_FAILURE;
			LCD_Struct.Error = E_I2C_WRITE_S_FAILURE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		default:
			/* You should never get here */
			LCD_Struct.State = LCD_FAILURE;
			I2C_Error = E_I2C_WRITE_S_FAILURE;
			LCD_Struct.Error = E_I2C_WRITE_S_FAILURE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		}
	}	
	return (I2C_Error);
}

static LCD_ERROR_T LCD_Write_UART(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize)
{
	LCD_ERROR_T UART_Error = NO_ERROR;
	if (LCD_Struct.State == LCD_READY)
	{
		LCD_Struct.State = LCD_BUSY;
		HAL_StatusTypeDef UART_State = HAL_OK;
		/* Check if dataSize is too big */
		if ((dataSize + LCD_UART_SENDBUFFER_SIZEADD) > dataSize)
		{
			/* Initialize data structure */
			/* (Variable length array are illegal in C89, make sure the compiler used is based on C99) */
			size_t write_i = 0x00U;
			uint8_t Data[(dataSize + LCD_UART_SENDBUFFER_SIZEADD)];
			uint8_t Checksum = (dataSize + LCD_UART_SENDBUFFER_SIZEADD);
			Checksum += Command;

			/* Dynamic stack memory allocation, we must check if it's valid */
			if ((&Data[0] != NULL) && (&Data[0] != 0))
			{
				size_t temp_dataSize = (dataSize + LCD_UART_SENDBUFFER_SIZEADD);
				/* The amount of data is the first and second bytes sent */
				Data[0] = (uint8_t)((temp_dataSize & 0xFF00U) >> 8U);
				Data[1] = (uint8_t)(temp_dataSize & 0x00FFU);
				/* Command is the second byte sent */
				Data[2] = Command;
				/* Check pointer value */
				/* It is possible for pData to be a NULL pointer, but only if dataSize is equal to 0 */
				if ((pData != NULL) || ((pData == NULL) && (dataSize != 0)))
				{
					if (pData != NULL)
					{
						/* Populating the array with pData */
						for (write_i = (LCD_UART_SENDBUFFER_SIZEADD - 0x01U); write_i <= (dataSize + (LCD_UART_SENDBUFFER_SIZEADD - 0x02U)); write_i++)
						{
							Data[write_i] = pData[(write_i - (LCD_UART_SENDBUFFER_SIZEADD - 0x01U))];
							Checksum += Data[write_i];
						}
					}

					/* Calculating checksum's 2's complement value and placing it at the end of the array */
					Checksum = (~Checksum) + 0x01U;
					Data[dataSize + (LCD_UART_SENDBUFFER_SIZEADD - 0x01U)] = Checksum;
					/* Calculating a timeout value */
					uint32_t timeoutValue = 0x03U * + (uint32_t)(dataSize / 0x04U);

					/* Sending data via UART */
					UART_State = HAL_UART_Transmit(LCD_Struct.huart, &Data[0], (uint16_t)(dataSize + LCD_UART_SENDBUFFER_SIZEADD), timeoutValue);

					if (UART_State == HAL_OK)
					{
						/* Operations with the module have been completed successfully, reset state */
						LCD_Struct.State = LCD_READY;
					}
					else			/* UART_State != HAL_OK */
					{
						/* There was a HAL error */
						switch (UART_State)
						{
						case HAL_BUSY:
							UART_Error = E_UART_HAL_BUSY;
							LCD_Struct.Error = E_UART_HAL_BUSY;
							LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
							break;
						case HAL_TIMEOUT:
							UART_Error = E_UART_HAL_TIMEOUT;
							LCD_Struct.Error = E_UART_HAL_TIMEOUT;
							LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
							break;
						case HAL_ERROR:
							/* Nested switch case, to check which error arose */
							switch (LCD_Struct.huart->ErrorCode)
							{
							case HAL_UART_ERROR_PE:	/* Parity error */
								UART_Error = E_UART_HAL_ERROR_PE;
								LCD_Struct.Error = E_UART_HAL_ERROR_PE;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_UART_ERROR_NE:		/* Noise Error */
								UART_Error = E_UART_HAL_ERROR_NE;
								LCD_Struct.Error = E_UART_HAL_ERROR_NE;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_UART_ERROR_FE: /* Frame Error*/
								UART_Error = E_UART_HAL_ERROR_FE;
								LCD_Struct.Error = E_UART_HAL_ERROR_FE;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_UART_ERROR_ORE: /* Overrun error*/
								UART_Error = E_UART_HAL_ERROR_ORE;
								LCD_Struct.Error = E_UART_HAL_ERROR_ORE;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_UART_ERROR_DMA:		/* DMA */
								UART_Error = E_UART_HAL_ERROR_DMA;
								LCD_Struct.Error = E_UART_HAL_ERROR_DMA;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							default:					/* Timeout error or magic */
														/* This shouldn't ever happen */
								UART_Error = E_UART_HAL_ERROR;
								LCD_Struct.Error = E_UART_HAL_ERROR;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							}
							break;
						default:
							UART_Error = E_UART_HAL_ERROR; /* This shouldn't ever happen */
							LCD_Struct.Error = E_UART_HAL_ERROR;
							LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
							break;
						}
					}
				}
				else				/* (pData == NULL) && (dataSize != 0) */
				{
					/* This functions accepts null pointers to a data buffer
						* But only if the datasize is equal to 0, indicating that
						* we don't want to send any data beyond the command & checksum
						* If the pointer is null but dataSize has a value, there was an error */
					UART_Error = E_WRITE_ARRAY;
					LCD_Struct.Error = E_WRITE_ARRAY;
					LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
					assert(pData != NULL);
				}
			}
			else					/* (&Data[0] == NULL) || (&Data[0] == 0) */
			{
				/* Array pointer is invalid */
				UART_Error = E_WRITE_ARRAY;
				LCD_Struct.Error = E_WRITE_ARRAY;
				LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
				assert(&Data[0] != NULL);
				assert(&Data[0] != 0);
			}
		}
		else						/* (dataSize + LCD_UART_SENDBUFFER_SIZEADD) > dataSize*/
		{
			/* The data exceeds the data size limit (65535 - LCD_UART_SENDBUFFER_SIZEADD)*/
									/* This if instruction may seem weird. But in the case of an overflow, the
									* result of the addition would be lower than the initial value of dataSize */
			UART_Error = E_WRITE_SIZE;
			LCD_Struct.Error = E_WRITE_SIZE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		}
	}
	else							/* LCD_Struct.State != LCD_READY */
	{
		/* The module isn't ready to write */
		switch (LCD_Struct.State)	/* Check its state */
		{
		case LCD_BUSY:
			/* Module is currently performing operations, try later */
			UART_Error = E_UART_WRITE_S_BUSY;
			LCD_Struct.Error = E_UART_WRITE_S_BUSY;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		case LCD_FAILURE:
			/* Module has critically failed */
			UART_Error = E_UART_WRITE_S_FAILURE;
			LCD_Struct.Error = E_UART_WRITE_S_FAILURE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		default:
			/* You should never get here */
			LCD_Struct.State = LCD_FAILURE;
			UART_Error = E_UART_WRITE_S_FAILURE;
			LCD_Struct.Error = E_UART_WRITE_S_FAILURE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		}
	}
	return (UART_Error);
}
	

static LCD_ERROR_T LCD_Write_SPI(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize)
{
	LCD_ERROR_T SPI_Error = NO_ERROR;
	if (LCD_Struct.State == LCD_READY)
	{
		LCD_Struct.State = LCD_BUSY;
		HAL_StatusTypeDef SPI_State = HAL_OK;
		/* Check if dataSize is too big */
		if ((dataSize + LCD_SPI_SENDBUFFER_SIZEADD) > dataSize)
		{
			/* Initialize data structure */
			/* (Variable length array are illegal in C89, make sure the compiler used is based on C99) */
			size_t write_i = 0x00U;
			uint8_t Data[(dataSize + LCD_SPI_SENDBUFFER_SIZEADD)];
			uint8_t Checksum = (dataSize + LCD_SPI_SENDBUFFER_SIZEADD);
			Checksum += Command;
		
			/* Dynamic stack memory allocation, we must check if it's valid */
			if ((&Data[0] != NULL) && (&Data[0] != 0))
			{
				size_t temp_dataSize = (dataSize + LCD_SPI_SENDBUFFER_SIZEADD); 
				/* The amount of data is the first and second bytes sent */
				Data[0] = (uint8_t)((temp_dataSize & 0xFF00U) >> 8U);
				Data[1] = (uint8_t)(temp_dataSize & 0x00FFU);
				/* Command is the second byte sent */
				Data[2] = Command;
			
				/* Check pointer value */
				/* It is possible for pData to be a NULL pointer, but only if dataSize is equal to 0 */
				if ((pData != NULL) || ((pData == NULL) && (dataSize != 0)))
				{
					if (pData != NULL)
					{
						/* Populating the array with pData */
						for (write_i = (LCD_SPI_SENDBUFFER_SIZEADD - 0x01U); write_i <= (dataSize + (LCD_SPI_SENDBUFFER_SIZEADD - 0x02U)); write_i++)
						{
							Data[write_i] = pData[(write_i - (LCD_SPI_SENDBUFFER_SIZEADD - 0x01U))];
							Checksum += Data[write_i];
						}
					}
				
					/* Calculating checksum's 2's complement value and placing it at the end of the array */
					Checksum = (~Checksum) + 0x01U;
					Data[dataSize + (LCD_SPI_SENDBUFFER_SIZEADD - 0x01U)] = Checksum;
					/* Calculating a timeout value */
					uint32_t timeoutValue = 0x03U * + (uint32_t)(dataSize /  0x04U);
				
					/* Sending data via SPI */
					SPI_State = HAL_SPI_Transmit(LCD_Struct.hspi, &Data[0], (uint16_t)(dataSize + LCD_SPI_SENDBUFFER_SIZEADD), timeoutValue);
				
					if (SPI_State == HAL_OK)
					{
						/* Operations with the module have been completed successfully, reset state */
						LCD_Struct.State = LCD_READY;
					}
					else			/* SPI_State != HAL_OK */
					{
						/* There was a HAL error */
						switch (SPI_State)
						{
						case HAL_BUSY:
							SPI_Error = E_SPI_HAL_BUSY;
							break;
						case HAL_TIMEOUT:
							SPI_Error = E_SPI_HAL_TIMEOUT;
							break;
						case HAL_ERROR:
							/* Nested switch case, to check which error arose */
							switch (LCD_Struct.hspi->ErrorCode)
							{
							case HAL_SPI_ERROR_MODF:		/* Acknowledge failure */
								/* This error is the one most likely to occur in HAL_SPI_Master_Transmit */
								SPI_Error = E_SPI_HAL_ERROR_MODF;
								LCD_Struct.Error = E_SPI_HAL_ERROR_MODF;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_SPI_ERROR_CRC:	/* Bus error */
								SPI_Error = E_SPI_HAL_ERROR_CRC;
								LCD_Struct.Error = E_SPI_HAL_ERROR_CRC;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_SPI_ERROR_OVR:	/* Arbitration lost */
								SPI_Error = E_SPI_HAL_ERROR_OVR;
								LCD_Struct.Error = E_SPI_HAL_ERROR_OVR;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_SPI_ERROR_FRE:		/* Overrun */
								SPI_Error = E_SPI_HAL_ERROR_OVR;
								LCD_Struct.Error = E_SPI_HAL_ERROR_OVR;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							case HAL_SPI_ERROR_DMA:		/* DMA */
								SPI_Error = E_SPI_HAL_ERROR_DMA;
								LCD_Struct.Error = E_SPI_HAL_ERROR_DMA;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							default:					/* Timeout error or magic */ 
														/* This shouldn't ever happen */
								SPI_Error = E_SPI_HAL_ERROR;
								LCD_Struct.Error = E_SPI_HAL_ERROR;
								LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
								break;
							}
							break;
						default:
							SPI_Error = E_SPI_HAL_ERROR; /* This shouldn't ever happen */
							LCD_Struct.Error = E_SPI_HAL_ERROR;
							LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
							break;
						}
					}
				}
				else				/* pData == NULL */
				{
					/* This functions accepts null pointers to a data buffer
						* But only if the datasize is equal to 0, indicating that 
						* we don't want to send any data beyond the command & checksum 
						* If the pointer is null but dataSize has a value, there was an error */
					SPI_Error = E_WRITE_ARRAY;
					LCD_Struct.Error = E_WRITE_ARRAY;
					LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
					assert(pData != NULL);
				}
			}
			else					/* (&Data[0] == NULL) || (&Data[0] == 0) */
			{
				/* Array pointer is invalid */
				SPI_Error = E_WRITE_ARRAY;
				LCD_Struct.Error = E_WRITE_ARRAY;
				LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
				assert(&Data[0] != NULL);
			}
		}
		else						/* (dataSize + LCD_SPI_SENDBUFFER_SIZEADD) > dataSize*/
		{
			/* The data exceeds the data size limit (65535 - LCD_SPI_SENDBUFFER_SIZEADD)*/
			/* This if instruction may seem weird. But in the case of an overflow, the 
			 * result of the addition would be lower than the initial value of dataSize */
			SPI_Error = E_WRITE_SIZE;
			LCD_Struct.Error = E_WRITE_SIZE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		}
	}
	else							/* LCD_Struct.State != LCD_READY */
	{
		/* The module isn't ready to write */
		switch (LCD_Struct.State)	/* Check its state */
		{
		case LCD_BUSY:
			/* Module is currently performing operations, try later */
			SPI_Error = E_SPI_WRITE_S_BUSY;
			LCD_Struct.Error = E_SPI_WRITE_S_BUSY;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		case LCD_FAILURE:
			/* Module has critically failed */
			SPI_Error = E_SPI_WRITE_S_FAILURE;
			LCD_Struct.Error = E_SPI_WRITE_S_FAILURE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		default:
			/* You should never get here */
			LCD_Struct.State = LCD_FAILURE;
			SPI_Error = E_SPI_WRITE_S_FAILURE;
			LCD_Struct.Error = E_SPI_WRITE_S_FAILURE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		}
	}	
	return (SPI_Error);
}

/* MISRA-C : 2004 prohibits the multiple break statement within a loop (Rule 14.6)
 * We're creating a specific deviation for this function, LCD_Write_Parallel, that can exit the data transfer loop
 * if an error arises either while transmitting data on the parallel bus.
 * This is done in order to immediatly handle an error in the loop if it arises, and is more secure than not having two break statements. */
static LCD_ERROR_T LCD_Write_Parallel(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize)
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
			uint8_t Data[(dataSize + LCD_PARALLEL_SENDBUFFER_SIZEADD)];
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
					for (write_i = (LCD_PARALLEL_SENDBUFFER_SIZEADD - 0x01U); write_i < (dataSize + (LCD_PARALLEL_SENDBUFFER_SIZEADD - 0x01U)); write_i++)
					{
						Data[write_i] = pData[(write_i - (LCD_PARALLEL_SENDBUFFER_SIZEADD - 0x01U))];
						Checksum += Data[write_i];
					}
				
					/* Calculating checksum's 2's complement value and placing it at the end of the array */
					Checksum = (uint8_t)((0xFF - Checksum) + 0x01U);
					Data[dataSize + (LCD_PARALLEL_SENDBUFFER_SIZEADD - 0x01U)] = Checksum;
					
					P_ERROR_T Parallel_Error = P_NO_ERROR;
					/* Sending data on the parallel bus */
					for (write_i = 0x00U; write_i < (dataSize + LCD_PARALLEL_SENDBUFFER_SIZEADD); write_i++)
					{
						/* Send a byte */
						Parallel_Error = Parallel_Write(Data[write_i], (Command & 0x01U), CS2);
						if (Parallel_Error != P_NO_ERROR)
						{
							/* Immediatly exit loop if there's an error */
							P_Error = E_PARALLEL;
							LCD_Struct.Error = E_PARALLEL;
							LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
							assert(Parallel_Error == P_NO_ERROR);
							break;
						}
						Parallel_Error = Parallel_CS(CS2, PIN_LOW);
						if (Parallel_Error != P_NO_ERROR)
						{
							/* Immediatly exit loop if there's an error */
							P_Error = E_PARALLEL;
							LCD_Struct.Error = E_PARALLEL;
							LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
							assert(Parallel_Error == P_NO_ERROR);
							break;
						}
					}
					/* Clear the parallel bus */
					Parallel_Error = Parallel_Write(0x00U, 0x00U, CS0);
					if (Parallel_Error != P_NO_ERROR)
					{
						P_Error = E_PARALLEL;
						LCD_Struct.Error = E_PARALLEL;
						LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
						assert(Parallel_Error == P_NO_ERROR);
					}
					Parallel_Error = Parallel_CS(NOCS, PIN_LOW);
					if (Parallel_Error != P_NO_ERROR)
					{
						P_Error = E_PARALLEL;
						LCD_Struct.Error = E_PARALLEL;
						LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
						assert(Parallel_Error == P_NO_ERROR);
					}
					/* End of normal behavior */
					LCD_Struct.State = LCD_READY;
				
				}
				else					/* (pData == NULL) && (dataSize != 0) */
				{
					/* This functions accepts null pointers to a data buffer
						* But only if the datasize is equal to 0, indicating that 
						* we don't want to send any data beyond the command & checksum 
						* If the pointer is null but dataSize has a value, there was an error */
					P_Error = E_WRITE_ARRAY;
					LCD_Struct.Error = E_WRITE_ARRAY;
					LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
					assert(pData != NULL);
				}
			}
			else						/* (&Data[0] == NULL) || (&Data[0] == 0) */
			{
				/* Array pointer is invalid */
				P_Error = E_WRITE_ARRAY;
				LCD_Struct.Error = E_WRITE_ARRAY;
				LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
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
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
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
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		case LCD_FAILURE:
			/* Module has critically failed */
			P_Error = E_PARALLEL_WRITE_S_FAILURE;
			LCD_Struct.Error = E_PARALLEL_WRITE_S_FAILURE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		default:
			/* You should never get here */
			LCD_Struct.State = LCD_FAILURE;
			P_Error = E_PARALLEL_WRITE_S_FAILURE;
			LCD_Struct.Error = E_PARALLEL_WRITE_S_FAILURE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			break;
		}
	}
	
	return (P_Error);
}

/* --------------------------------------------------------------------------------------------- */
/* Command functions */
static LCD_ERROR_T LCD_Clear_Empty(void)
{
	return (E_UNINITIALIZED);	
}
static LCD_ERROR_T LCD_Clear_I2C(void)
{
	/* Send command 0x00 and no data */
	return (LCD_Write_I2C(LCD_CLEAR, NULL, 0x00U));
}
static LCD_ERROR_T LCD_Clear_UART(void)
{
	/* Send command 0x00 and no data */
	return (LCD_Write_UART(LCD_CLEAR, NULL, 0x00U));
}
static LCD_ERROR_T LCD_Clear_SPI(void)
{
	/* Send command 0x00 and no data */
	return (LCD_Write_SPI(LCD_CLEAR, NULL, 0x00U));
}
static LCD_ERROR_T LCD_Clear_Parallel(void)
{
	uint8_t ClearedArray[LCD_X_SIZE * LCD_Y_SIZE];
	
	/* Clears the created array, setting all its values to 0x00*/
	memset(ClearedArray, 0x00U, (LCD_X_SIZE * LCD_Y_SIZE));
		
	/* Send command graph and an empty screen */
	return (LCD_Write_Parallel(LCD_GRAPH, &ClearedArray[0], (LCD_X_SIZE * LCD_Y_SIZE)));
}


/* --------------------------------------------------------------------------------------------- */
/* Text functions */
LCD_ERROR_T LCD_WriteText(char* pText, size_t textSize, uint8_t line, uint8_t column)
{
	LCD_ERROR_T WriteText_Error = NO_ERROR;
	/* Check if string pointer is valid */
	if (pText != NULL)
	{
		/* Check if line and columns are valid */
		if ((line <= LCD_TEXT_MAX_LINES) && (column <= LCD_TEXT_MAX_COLUMNS))
		{
			/* Check if size of string is valid */
			if ((textSize <= (LCD_TEXT_MAX_LINES * LCD_TEXT_MAX_COLUMNS)) && (textSize > 0x00U))
			{
				/* Check if all characters within the string are valid */
				size_t i;
				for (i = 0x0000U; i < textSize; i++)
				{
					if (pText[i] >= 0x80)
					{
						WriteText_Error = E_WRITE_TEXT_CHARACTER;
						break;
					}
				}
				if (WriteText_Error == NO_ERROR)
				{
					size_t RemainingText = textSize;
					uint8_t currentLine = line;
					/* Replace all accentuated letters with their non-accentuated counterparts */
					do
					{
						size_t lenght;
						char string[LCD_TEXT_MAX_COLUMNS];
						/* strchr, from <string.h>, looks for the first instance of the character '\n' ([LINE FEED], 0x0A) in the 
						 * string pointed by &pText[(textSize - RemainingText)]. If no instances are found,
						 * it returns a null pointer. If the "\n" character is found, it returns a pointer to
						 * it's position in the string. */
						/* strrchr looks for the last instance of the character ' ' ([SPACE], 0x20) in the 
						 * string copied in the variable string. */
						const char* nextLine = strchr(&pText[(textSize - RemainingText)], '\n');
						if (nextLine == NULL)			/* No Line feed in the current string */
						{
							lenght = LCD_TEXT_MAX_COLUMNS - column;
							strncpy(string, &pText[(textSize - RemainingText)], lenght);
							nextLine = strrchr(string, ' ');
							if (nextLine == NULL)		/* No spaces in the current string */
							{
								RemainingText -= (lenght - 0x01U);
								WriteText_Error = LCD_WriteLine(string, lenght, currentLine, column);
								currentLine++;
							}
							else
							{
								lenght = (nextLine - string) + 0x01U;
								RemainingText -= lenght;
								WriteText_Error = LCD_WriteLine(string, lenght, currentLine, column);
								currentLine++;
							}
						}
						else
						{
							/* Get the lenght by taking the position of the '\n' character in memory minus 
							 * the start position of the string in memory. */
							lenght = (nextLine - &pText[(textSize - RemainingText)] + 1);
							/* The next "\n" is too far away */
							if (lenght > (LCD_TEXT_MAX_COLUMNS - column))
							{
								lenght = LCD_TEXT_MAX_COLUMNS - column;
								strncpy(string, &pText[(textSize - RemainingText)], lenght);
								nextLine = strrchr(string, 0x20);
								if (nextLine == NULL)		/* No spaces in the current string */
								{
									RemainingText -= (lenght - 0x01U);
									WriteText_Error = LCD_WriteLine(string, lenght, currentLine, column);
									currentLine++;
								}
								else
								{
									lenght = (nextLine - string) + 0x01U;
									RemainingText -= lenght;
									WriteText_Error = LCD_WriteLine(string, lenght, currentLine, column);
									currentLine++;
								}
							}
							else			/* The next '\n' is close enough */
							{
								strncpy(string, &pText[(textSize - RemainingText)], (lenght + 1));
								RemainingText -= lenght;
								WriteText_Error = LCD_WriteLine(string, lenght, currentLine, column);
								currentLine++;
							}
						}
						if (WriteText_Error != NO_ERROR)
						{
							break;
						}
					} while ((RemainingText > 0x0000U) && (RemainingText <= textSize));
				}
			}
			else						/* (textSize > (LCD_TEXT_MAX_LINES * LCD_TEXT_MAX_COLUMNS)) || (textSize == 0x00U) */
			{
				/* String is too long or null */
				WriteText_Error = E_WRITE_TEXT_SIZE;
				LCD_Struct.Error = E_WRITE_TEXT_SIZE;
				LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			}
		}
		else							/* (line > LCD_TEXT_MAX_LINES) || (column > LCD_TEXT_MAX_COLUMNS) */
		{
			/* line is invalid and out of bounds */
			WriteText_Error = E_WRITE_TEXT_LINECOL;
			LCD_Struct.Error = E_WRITE_TEXT_LINECOL;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		}
	}
	else								/* pText == NULL */
	{
		/* String pointer is invalid */
		WriteText_Error = E_WRITE_ARRAY;
		LCD_Struct.Error = E_WRITE_ARRAY;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		assert(pText != NULL);
	}
	
	return (WriteText_Error);
}
LCD_ERROR_T LCD_WriteLine(char* pText, size_t textSize, uint8_t line, uint8_t column)
{
	LCD_ERROR_T WriteLine_Error = NO_ERROR;

	/* Check if character pointer is valid */
	if (pText != NULL)
	{
		/* Check if line is valid */
		if (line <= LCD_TEXT_MAX_LINES)
		{
			/* Check if column is valid */
			if (textSize <= (LCD_TEXT_MAX_COLUMNS - column))
			{
				/* New Data Buffer, with 2 extra lines, for line & column */
				uint8_t Data[(textSize + 0x02U)];
				size_t write_i = 0x00U;
				
				/* Dynamic stack memory allocation, we must check if it's valid */
				if ((&Data[0] != NULL) && (&Data[0] != 0))
				{
					/* Clear array */
					memset(&Data[0], 0x0000, sizeof(Data));
					
					/* Add line and column data to the array */
					Data[0] = line;
					Data[1] = column;
					
					/* Populating the array with pText*/
					for (write_i = 0x02U; write_i <= (textSize + 0x01U); write_i++)
					{
						Data[write_i] = pText[(write_i - 0x02U)];
					}
						
					/* Select command to write */
					uint8_t command = LCD_TEXT;
					/* Write data */
					WriteLine_Error = LCD_Write(command, Data, (textSize + 0x01U));
				}
				else					/* (&Data[0] == NULL) || (&Data[0] == 0) */
				{
					/* Array pointer is invalid */
					WriteLine_Error = E_WRITE_ARRAY;
					LCD_Struct.Error = E_WRITE_ARRAY;
					LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
					assert(&Data[0] != NULL);
				}
			}
			else						/* textSize > (LCD_TEXT_MAX_COLUMNS - column) */
			{
				/* Columns is out of bounds */
				WriteLine_Error = E_WRITE_TEXT_SIZE;
				LCD_Struct.Error = E_WRITE_TEXT_SIZE;
				LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			}
		}
		else							/* line > LCD_TEXT_MAX_LINES*/
		{
			/* Line is out of bounds */
			WriteLine_Error = E_WRITE_TEXT_LINECOL;
			LCD_Struct.Error = E_WRITE_TEXT_LINECOL;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		}
	}
	else								/* pText == NULL*/
	{
		/* Invalid string pointer */
		WriteLine_Error = E_WRITE_ARRAY;
		LCD_Struct.Error = E_WRITE_ARRAY;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		assert(pText != NULL);
	}
	
	return (WriteLine_Error);
}

/* --------------------------------------------------------------------------------------------- */
/* Graphic functions */

LCD_ERROR_T LCD_FillLine(uint8_t* pArray, uint8_t StartPixel, uint8_t StopPixel)
{
	LCD_ERROR_T FillLine_Error = NO_ERROR;
	
	static const uint8_t maskStart[8] = { 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };
	static const uint8_t maskStop[8] = { 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF };
	
	if (pArray != NULL)
	{
		if (StartPixel >= StopPixel)
		{
			uint8_t fillLine_i;
			/* Get the Start and Stop Byte by dividing the bit number by the size of a byte */
			uint8_t StartByte = StartPixel / sizeof(uint8_t);
			uint8_t StopByte = StopPixel / sizeof(uint8_t);
			/* Get the Start and Stop Bit by doing the modulo of the size of the byte */
			uint8_t StartBit = StartPixel % sizeof(uint8_t);
			uint8_t StopBit = StopPixel % sizeof(uint8_t);
			
			/* Check to see if the bits to access are on the same byte or many bytes apart */
			if (StopByte > StartByte)
			{
				
				/* Mask the bits in the start and stop bytes */
				pArray[StartByte] &= maskStart[StartBit];
				pArray[StopByte] &= maskStop[StopBit];
				
				/* Fill with 0xFF all the remaining bytes */
				for (fillLine_i = (StartByte + 1U); fillLine_i < StopByte; fillLine_i++)
				{
					pArray[fillLine_i] = 0xFFU;
				}
			}
			else						/* The two bits to access are on the same byte */
			{
				/* We're about to do a lot of changes to a single byte
				 * Let us create a local variable that will be loaded into a register
				 * instead of always accessing the pointer through another pointer */
				uint8_t tempFillByte = pArray[StartByte];
				
				/* Set individually all the bits from the start bit to the stop bit */
				for (fillLine_i = StartBit; fillLine_i < StopBit; fillLine_i++)
				{
					tempFillByte |= 0x80U >> fillLine_i;
				}
				/* Put the temporary byte data back into the array */
				pArray[StartByte] = tempFillByte;
			}
		}
		else							/* StartPixel < StopPixel */
		{
			/* Negative size line? */
			FillLine_Error = E_DRAW_SIZE;
			LCD_Struct.Error = E_DRAW_SIZE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		}
	}
	else								/* pArray == NULL */
	{
		/* The array is non-existant */
		FillLine_Error = E_ARRAY_NONEXISTANT;
		LCD_Struct.Error = E_ARRAY_NONEXISTANT;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
	}
	
	
	return (FillLine_Error);
}

/* --------------------------------------------------------------------------------------------- */
/* Timer functions */
static LCD_ERROR_T LCD_Config_Timer(TIM_HANDLER* htim)
{
#ifdef HAL_TIM_MODULE_ENABLED
	LCD_ERROR_T Config_Tim_Error = NO_ERROR;
	
	/* Is a timer handler passed? */
	if (htim != NULL)
	{
		/* Init the timer normally */
		Config_Tim_Error = LCD_Init_Timer(htim);
	}
	else								/* htim == NULL */
	{
		/* No timer handler was passed to the function
		 * We must init our own timer. 
		 * This library uses Timer 2 for this purpose 
		 * Note that if the HAL library doesn't exist, the timer cannot be used
		 * These lines of code will be automatically disabled if the HAL is absent */
		
		/* Instance of clock config definitions */
		TIM_ClockConfigTypeDef sClockSourceConfig;
		TIM_SlaveConfigTypeDef sSlaveConfig;
		TIM_MasterConfigTypeDef sMasterConfig;
		
		/* Init timer 2 in counting mode */
		htim->Instance = TIM2;
		htim->Init.Prescaler = 0x0000U;
		htim->Init.CounterMode = TIM_COUNTERMODE_UP;
		htim->Init.Period = 0x0000U;
		htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		if (HAL_TIM_Base_Init(htim) == HAL_OK)
		{
			/* Configure clock source */
			sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
			if (HAL_TIM_ConfigClockSource(htim, &sClockSourceConfig) == HAL_OK)
			{
				/* Configure timer slave */
				sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
				sSlaveConfig.InputTrigger = TIM_TS_ITR0;
				if (HAL_TIM_SlaveConfigSynchronization(htim, &sSlaveConfig) == HAL_OK)
				{
					/* Configure timer master */
					sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
					sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
					if (HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig) == HAL_OK)
					{
						/* Init the timer within the LCD module */
						Config_Tim_Error = LCD_Init_Timer(htim);
					}
					else				/* HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig) != HAL_OK */
					{
						/* Error during HAL initialization of the master timer */
						Config_Tim_Error = E_TIM_HAL_ERROR_MASTER;
						LCD_Struct.Error = E_TIM_HAL_ERROR_MASTER;
						LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
					}
				}
				else					/* HAL_TIM_SlaveConfigSynchronization(htim, &sSlaveConfig) != HAL_OK */
				{
					/* Error during HAL initialization of the slave timer */
					Config_Tim_Error = E_TIM_HAL_ERROR_SLAVE;
					LCD_Struct.Error = E_TIM_HAL_ERROR_SLAVE;
					LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
				}
			}
			else						/* HAL_TIM_ConfigClockSource(htim, &sClockSourceConfig) != HAL_OK */
			{
				/* Error during HAL initialization of the timer clock source */
				Config_Tim_Error = E_TIM_HAL_ERROR_CLKSOURCE;
				LCD_Struct.Error = E_TIM_HAL_ERROR_CLKSOURCE;
				LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			}
		}
		else							/* HAL_TIM_Base_Init(htim) != HAL_OK */
		{
			/* Error during HAL initialization of the timer */
			Config_Tim_Error = E_TIM_HAL_ERROR;
			LCD_Struct.Error = E_TIM_HAL_ERROR;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		}
		
	}
	
	
#else
	LCD_ERROR_T Config_Tim_Error = E_TIM_NOHAL;
#endif
	return (Config_Tim_Error);
}

static LCD_ERROR_T LCD_Init_Timer(TIM_HANDLER* htim)
{
#ifdef HAL_TIM_MODULE_ENABLED
	LCD_ERROR_T Init_Tim_Error = NO_ERROR;
	
	/* Check the validity of the timer handler pointer */
	if ((htim != NULL) && (htim != 0))
	{
		/* Assigns the Timer pointer to the LCD structure */
		LCD_Struct.htim = htim;
		
		/* Set a period value for the counter */
		LCD_Struct.htim->Init.Period = LCD_TIMER_PERIOD;
		TIM_Base_SetConfig(LCD_Struct.htim->Instance, &LCD_Struct.htim->Init);
		
		/* Change timers settings according to the current mode */
		switch (LCD_Struct.Mode)
		{
		case I2C_MODE:
			LCD_Timer_Interrupt = LCD_Timer_Interrupt_I2C;
			break;
		case UART_MODE:
			LCD_Timer_Interrupt = LCD_Timer_Interrupt_UART;
			break;
		case SPI_MODE:
			LCD_Timer_Interrupt = LCD_Timer_Interrupt_SPI;
			break;
		case PARALLEL_MODE:
			LCD_Timer_Interrupt = LCD_Timer_Interrupt_Parallel;
			break;
		default:
			/* No mode is selected, can't init timer */
			/* Display error message if mode is invalid */
			Init_Tim_Error = E_UNKNOWN_MODE;
			LCD_Struct.Error = E_UNKNOWN_MODE;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			assert(LCD_Struct.Mode != NOT_INIT);
			break;
		}
	}
	else								/* (htim == NULL) || (htim == 0) */
	{
		/* Invalid timer handler pointer */
		Init_Tim_Error = E_TIM_INIT;
		LCD_Struct.Error = E_TIM_INIT;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		assert(htim != NULL);
		assert(htim != 0);
	}
	
#else
	LCD_ERROR_T Init_Tim_Error = E_TIM_NOHAL;
#endif
	return (Init_Tim_Error);
}

static void LCD_Timer_Interrupt_Empty(void)
{
	/* Nothing is in here 
	 * This function exist for default initialization, in order to avoid null function pointer 
	 * If your program has brought you here, your initialization is probably wrong */
}
static void LCD_Timer_Interrupt_I2C(void)
{
	
}
static void LCD_Timer_Interrupt_UART(void)
{
	
}
static void LCD_Timer_Interrupt_SPI(void)
{
	
}
static void LCD_Timer_Interrupt_Parallel(void)
{
	
}

static LCD_ERROR_T LCD_Timer_Start(TIM_HANDLER* htim)
{
#ifdef HAL_TIM_MODULE_ENABLED
	LCD_ERROR_T Timer_Start_Error = NO_ERROR;
	/* Check the validity of timer handler pointer */
	if ((htim != NULL) && (htim != 0))
	{
		/* Start the time base with interrupts enabled */
		if (HAL_TIM_Base_Start_IT(htim) != HAL_OK)
		{
			/* If there was an error during the timer start sequence */
			Timer_Start_Error = E_TIM_START;
			LCD_Struct.Error = E_TIM_START;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		}
	}
	else								/* (htim == NULL) || (htim == 0) */
	{
		/* Invalid timer handler pointer */
		Timer_Start_Error = E_TIM_START;
		LCD_Struct.Error = E_TIM_START;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		assert(htim != NULL);
	}
#else
	LCD_ERROR_T Timer_Start_Error = E_TIM_NOHAL;
#endif
	return (Timer_Start_Error);
}
static LCD_ERROR_T LCD_Timer_Stop(TIM_HANDLER* htim)
{
#ifdef HAL_TIM_MODULE_ENABLED
	LCD_ERROR_T Timer_Stop_Error = NO_ERROR;
	/* Check the validity of timer handler pointer */
	if ((htim != NULL) && (htim != 0))
	{
		/* Stop the time base with interrupts enabled */
		if (HAL_TIM_Base_Stop_IT(htim) != HAL_OK)
		{
			/* If there was an error during the timer stop sequence*/
			Timer_Stop_Error = E_TIM_STOP;
			LCD_Struct.Error = E_TIM_STOP;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		}
		
	}
	else								/* (htim == NULL) || (htim == 0) */
	{
		/* Invalid timer handler pointer */
		Timer_Stop_Error = E_TIM_STOP;
		LCD_Struct.Error = E_TIM_STOP;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		assert(htim != NULL);
	}
#else
	LCD_ERROR_T Timer_Stop_Error = E_TIM_NOHAL;
#endif
	return (Timer_Stop_Error);
}

/* HAL Function, initialized as weak in stm32f4xx_hal_tim.c like this :
 * __weak void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 * 
 * The __weak keyword allows redifinition such as this one. 
 * HAL_TIM_PeriodElapsedCallback is called everytime a timer set in counter mode overloads
 * We're having it call our own interrupt function if the timer calling the event is ours.
 * */
#ifdef HAL_TIM_MODULE_ENABLED			/* Check if HAL timer module exists */
void HAL_TIM_PeriodElapsedCallback(TIM_HANDLER *htim)
{
	if (htim == LCD_Struct.htim)
	{
		LCD_Timer_Interrupt();
	}
}
#endif

/* --------------------------------------------------------------------------------------------- */
/* Array generation functions */

/* The following 4 functions are non-compliant with MISRA-C:2004 guidelines, breaking rule 20.4 :
 * "Dynamic heap memory allocation shall not be used"
 * Specific deviations have been raised for all 4 of the functions and security measures have been
 * taken seriously. 
 * Noncompliant functions:
 * LCD_ERROR_T LCD_Create_Array_Struct(LCD_ARRAY_T* structArray, uint8_t lines, uint8_t columns, uint8_t coordX, uint8_t coordY);
 * LCD_ERROR_T LCD_Create_Array(LCD_ARRAY_T* structArray);
 * LCD_ERROR_T LCD_Free_Array(LCD_ARRAY_T* structArray);
 * LCD_ERROR_T LCD_Free_Array_Struct(LCD_ARRAY_T* structArray);
 *
 * Prohibited functions of <stdlib> used:
 * void* calloc (size_t num, size_t size);		-	Used in LCD_Create_Array 
 * void* malloc (size_t size);					-	Used in LCD_Create_Array_Struct
 * void free (void* ptr);						-	Used in LCD_Free_Array & LCD_Free_Array_Struct
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */


/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      LCD_Create_Array_Struct                                               
static LCD_ERROR_T LCD_Create_Array_Struct(LCD_ARRAY_T* structArray, uint8_t lines, uint8_t columns, uint8_t coordX, uint8_t coordY)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-23
* 
* Creates a structure containing a bidimentional array by dynamically allocating heap memory.
* This bidimentional array is used in order to map the LCD screen during calculations.
* This function places a pointer to the created structure inside of the LCD_Struct general structure.
* The array is destroyed after its lifetime is complete using the function LCD_Free_Array()
* The array created is mapped as such:
*		array[lines][columns]
* The created structure is mapped as such:
*		uint8_t lines;
*		uint8_t columns;
*		uint8_t coordX;
*		uint8_t coordY;
*		uint8_t progress;
*		uint8_t attempts;
*		bool ArrayDefined;
*		uint8_t** pArray;
* 
* Called by : none
* Function calls :
*			- LCD_Create_Array()
*			- LCD_Free_Array()
*
* Parameters : 
*			- LCD_ARRAY_T* structArray : A structure pointer of LCD_ARRAY_T type. This array will be 
*			  created with dynamic memory allocation within this functions. It's elements will be 
*			  initialized with the parameters passed to the function, and the array within the LCD_ARRAY_T
*			  struct will be created with the function LCD_Create_Array()
*			- uint8_t lines : amount of lines to the bidimentionnal array
*			- uint8_t columns : amount of columns to the bidimentionnal array
*			- uint8_t coordX : offset in the X axis
*			- uint8_t coordY : offset in the Y axis
*
* Returns: LCD_ERROR_T CreateArray_Error :
*			- E_ARRAY_NOTEMPTY : The passed pointer hasn't been cleared before this function is called
*			- E_ARRAY_COORD_X : The passed parameters of columns or coordX is invalid
*			- E_ARRAY_COORD_Y : The passed parameters of lines or coordY is invalid
*			- E_ARRAY_STRUCT : Parameter structArray is invalid (during array creation)
*			- E_ARRAY_MEMORY : Couldn't allocate memory
*			- E_ARRAY_MAXATTEMPTS : Couldn't allocate memory more than 3 times, enter critical failure
* 
* Example call : LCD_Create_Array_Struct(LCD_Struct.Array, 160U, 240U, 0U, 0U);
*
* This function is noncompliant with MISRA-C:2004 (Rule 20.4), a specific deviation has been made.
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/* MISRA-C : 2004 prohibits the usage of dynamic heap memory allocation (Rule 20.4)
 * We're creating a specific deviation for this function, LCD_Create_Array_Struct, that creates a dynamic
 * structure. This function also calls another function (LCD_Create_Array), that creates a dynamic bidimentionnal array
 * This is done for optimization reasons for speed, calculation, communication and memory usage.
 * We're checking and double-checking every pointer, and making sure all values are valid before using them.
 * Any doubt of invalidity instantly aborts the creation of the structure.
 * We're also freeing the memory after use, in order to prevent memory hogging.
 * In the case of an error during array generation, its memory is free'd by the function LCD_Free_Array */
static LCD_ERROR_T LCD_Create_Array_Struct(LCD_ARRAY_T* structArray, uint8_t lines, uint8_t columns, uint8_t coordX, uint8_t coordY)
{
	LCD_ERROR_T CreateArray_Error = NO_ERROR;
	/* Check if pointer is empty before allocating memory */
	if (structArray == NULL)
	{
		/* Check if asked coordinates & columns or lines fit in screen 
		 * (if sum is smaller than max value) */
		if ((coordX + columns) <= LCD_ARRAY_MAX_COORDX)
		{
			if ((coordY + lines) > LCD_ARRAY_MAX_COORDY)
			{
				/* Create the structure */
				structArray = (LCD_ARRAY_T*) malloc(sizeof(LCD_ARRAY_T));
	
				/* Populate the structure */
				structArray->coordX = coordX;
				structArray->coordY = coordY;
				structArray->lines = lines;
				structArray->columns = columns;
					
				/* Create the array */
				CreateArray_Error = LCD_Create_Array(structArray);
				
				/* Check for errors during array creation */
				if (CreateArray_Error == NO_ERROR)	
				{
					if (structArray->ArrayDefined == 1U)
					{
						/* Everything was generated successfully! */
						LCD_Struct.Array = structArray;
					}
					else				/* structArray-> defined != 1 */
					{
						/* Something went wrong while generating array */
						/* Free the array if there was an error */
						LCD_Free_Array(structArray);
						CreateArray_Error = E_ARRAY_MEMORY;
						LCD_Struct.Error = E_ARRAY_MEMORY;
						LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
						assert(CreateArray_Error == NO_ERROR);		
					}
					
				}
				else					/* CreateArray_Error != NO_ERROR */
				{
					/* There was an error during array creation 
										 * We must free the array! */
					LCD_Free_Array(structArray);
				}
			}
			else						/* ((coordX + columns) <= LCD_ARRAY_MAX_COORDX) */
			{
				/* Array out of bounds in Y coordinate */
				CreateArray_Error = E_ARRAY_COORD_Y;
				LCD_Struct.Error = E_ARRAY_COORD_Y;
				LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			}
		}
		else							/* ((coordX + columns) > LCD_ARRAY_MAX_COORDX */
		{
			/* Array out of bounds in X coordinate */
			CreateArray_Error = E_ARRAY_COORD_X;
			LCD_Struct.Error = E_ARRAY_COORD_X;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			
		}
	}
	else								/* structArray != NULL */
	{
		/* Structure isn't empty */
		CreateArray_Error = E_ARRAY_NOTEMPTY;
		LCD_Struct.Error = E_ARRAY_NOTEMPTY;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		assert(structArray == NULL);
	}
	
	return (CreateArray_Error);
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      LCD_Create_Array                                               
static LCD_ERROR_T LCD_Create_Array(LCD_ARRAY_T* structArray)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-23
* 
* Creates a bidimentional array by dynamically allocating heap memory.
* This bidimentional array is used in order to map the LCD screen during calculations.
* The array created is mapped as such:
*		array[lines][columns]
* The array is destroyed after its lifetime is complete using the function LCD_Free_Array()
* 
* Called by : LCD_Create_Array_Struct()
* Function calls : none
*
* Parameters : LCD_ARRAY_T* structArray
*			- A structure pointer towards a LCD_ARRAY_T structure created with the function 
*				LCD_Create_Array_Struct()
*			From this structure, the following variables are used:
*					- uint8_t lines : Size of the array in the X axis 
*					- uint8_t columns : Size of the array in the Y axis
*					- uint8_t attempts : Attempts at creating the array. If this exceeds 3, 
*						the module enters critical failure mode.
*					- bool ArrayDefined : Flag indicating if the array exists or not
*					- uint8_t** pArray : pointer storing the created array
*
* Returns: LCD_ERROR_T Create_Error :
*			- E_ARRAY_STRUCT : Parameter structArray is invalid
*			- E_ARRAY_MEMORY : Couldn't allocate memory
*			- E_ARRAY_MAXATTEMPTS : Couldn't allocate memory more than 3 times, enter critical failure
*			
* Example call : LCD_CreateArray(structArray);
*			
* This function is noncompliant with MISRA-C:2004 (Rule 20.4), a specific deviation has been made.
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/* MISRA-C : 2004 prohibits the usage of dynamic heap memory allocation (Rule 20.4)
 * We're creating a specific deviation for this function, LCD_Create_Array, that not only creates 
 * a dynamic array, but creates a dynamic array of dynamic arrays. 
 * This is done for optimization reasons for speed, calculation, communication and memory usage.
 * We're checking and double-checking every pointer, and making sure all values are valid before using them.
 * Any doubt of invalidity instantly aborts the creation of the array and calls LCD_Free_Array to free allocated memory.
 * We're also freeing the memory after use, in order to prevent memory hogging.*/
static LCD_ERROR_T LCD_Create_Array(LCD_ARRAY_T* structArray)
{
	LCD_ERROR_T Create_Error = NO_ERROR;
	/* Check if structure pointer is valid */
	if (structArray != NULL)
	{
		/* Generate an array of lines and assign each of it's value to 0 */
		structArray->pArray = (uint8_t**) calloc(structArray->lines, sizeof(uint8_t));
		/* Check if array of pointers is valid */
		if (structArray->pArray != NULL)
		{
			/* Create an array for every pointer in the array of pointers */
			uint8_t memory_i;
			for (memory_i = 0x00U; memory_i < structArray->lines; memory_i++)
			{					
				structArray->pArray[memory_i] = (uint8_t*) calloc(structArray->columns, sizeof(uint8_t));
				/* Check if created array is valid */
				if (structArray->pArray[memory_i] == NULL)
				{
					Create_Error = E_ARRAY_MEMORY;
					LCD_Struct.Error = E_ARRAY_MEMORY;
					LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
					structArray->attempts++;
					if (structArray->attempts > LCD_ARRAY_MAX_ATTEMPTS)
					{
						/* Couldn't create array even after many attempts. */
						/* Critical failure */
						Create_Error = E_ARRAY_MAXATTEMPTS;
						LCD_Struct.State = LCD_FAILURE;
						LCD_Struct.Error = E_ARRAY_MAXATTEMPTS;
						LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
						assert(structArray->attempts <= LCD_ARRAY_MAX_ATTEMPTS);
					}
					assert(structArray->pArray[memory_i] != NULL);
					assert(structArray->pArray[memory_i] != 0);
				}
				else
				{
					structArray->ArrayDefined = true;
				}
			}
		}
		else							/* structArray->pArray == NULL */
		{
			/* Invalid array pointer */
			Create_Error = E_ARRAY_MEMORY;
			LCD_Struct.Error = E_ARRAY_MEMORY;
			LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
			structArray->attempts++;
			if (structArray->attempts > LCD_ARRAY_MAX_ATTEMPTS)
			{
				/* Couldn't create array even after many attempts. */
				/* Critical failure */
				Create_Error = E_ARRAY_MAXATTEMPTS;
				LCD_Struct.State = LCD_FAILURE;
				LCD_Struct.Error = E_ARRAY_MAXATTEMPTS;
				LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
				assert(structArray->attempts <= LCD_ARRAY_MAX_ATTEMPTS);
			}
			assert(structArray->pArray != NULL);
			assert(structArray->pArray != 0);
		}
	}
	else								/* structArray == NULL */
	{
		/* Invalid structure pointer */
		Create_Error = E_ARRAY_STRUCT;
		LCD_Struct.Error = E_ARRAY_STRUCT;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		assert(structArray != NULL);
	}
	return (Create_Error);
}


/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      LCD_Free_Array                                               
static LCD_ERROR_T LCD_Free_Array(LCD_ARRAY_T* structArray)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-23
* 
* Frees the bidirectionnal array created in function LCD_Create_Array()
* The array created is mapped as such:
*		array[lines][columns]
* 
* Called by :
*			- LCD_Delete_Array_Struct()
*			- LCD_Create_Array_Struct()		(if an error occurs during struct creation)
* Function calls : none
*
* Parameters : LCD_ARRAY_T* structArray
*			- A structure pointer towards a LCD_ARRAY_T structure created with the function 
*				LCD_Create_Array_Struct()
*			From this structure, the following variables are used:
*					- uint8_t lines : Size of the array in the X axis 
*					- uint8_t columns : Size of the array in the Y axis
*					- uint8_t attempts : Attempts at creating the array. It is reset in this function.
*					- bool ArrayDefined : Flag indicating if the array exists or not
*					- uint8_t** pArray : pointer storing the created array
*
* Returns: LCD_ERROR_T Free_Error :
*			- E_ARRAY_STRUCT : Parameter structArray is invalid
*			
* Example call : LCD_Free_Array(structArray);
*			
* This function is noncompliant with MISRA-C:2004 (Rule 20.4), a specific deviation has been made.
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/* MISRA-C : 2004 prohibits the usage of dynamic heap memory allocation (Rule 20.4)
 * We're creating a specific deviation for this function, LCD_Free_Array, that is used to
 * free memory allocated within the function LCD_Create_Array.
 * This is done for optimization reasons for speed, calculation, communication and memory usage.
 * We're checking and double-checking every pointer, and making sure all values are valid before using them.
 * All memory is cleared before being deallocated in order to prevent memory leaks.
 * Any doubt of invalidity in the function LCD_Create_Array calls this function to free allocated memory.
 * At the end of the lifetime of the array created in LCD_Create_Array, this function is also called. */
static LCD_ERROR_T LCD_Free_Array(LCD_ARRAY_T* structArray)
{	
	LCD_ERROR_T Free_Error = NO_ERROR;
	/* Check if structure is valid */
	if (structArray != NULL)
	{
		/* Check if pointer is valid */
		if (structArray->pArray != NULL)
		{
			/* Clear the memory for every pointer in the array of pointers */
			uint8_t f_memory_i;
			for (f_memory_i = 0x00U; f_memory_i < structArray->lines; f_memory_i++)
			{
				/* Set all memory to 0 */
				if (structArray->pArray[f_memory_i] != NULL)
				{
					memset(structArray->pArray[f_memory_i], 0x00U, structArray->columns);
				}
				free(structArray->pArray[f_memory_i]);
			}
			/* Free the main array pointer memory */
			free(structArray->pArray);
			/* Array is no longer defined */
			structArray->ArrayDefined = 0x0U; 
			/* Reset amount of attempts */
			structArray->attempts = 0x00U;
		}
		else							/* structArray->pArray == NULL */
		{
			/* Pointer is invalid; the main array couldn't be generated.
			 * Nothing to do here, it doesn't have to be free'd */
		}
	}
	else								/* structArray == NULL */
	{
		/* Invalid structure pointer */
		Free_Error = E_ARRAY_STRUCT;
		LCD_Struct.Error = E_ARRAY_STRUCT;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
		assert(structArray != NULL);
	}
	return (Free_Error);
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      LCD_Delete_Array_Struct                                               
static LCD_ERROR_T LCD_Delete_Array_Struct(LCD_ARRAY_T* structArray)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-23
* 
* Destroy the struct Created in LCD_Create_Array_Struct()
* 
* Called by : none
* Function calls : LCD_Free_Array()
*
* Parameters : LCD_ARRAY_T* structArray
*			- A structure pointer towards a LCD_ARRAY_T structure created with the function 
*				LCD_Create_Array_Struct()
*
* Returns: LCD_ERROR_T Delete_Error :
*			- E_ARRAY_STRUCT : Parameter structArray is invalid
*			
* Example call : LCD_Delete_Array_Struct(structArray);
*			
* This function is noncompliant with MISRA-C:2004 (Rule 20.4), a specific deviation has been made.
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/* MISRA-C : 2004 prohibits the usage of dynamic heap memory allocation (Rule 20.4)
 * We're creating a specific deviation for this function, LCD_Delete_Array_Struct, that is used to
 * free memory allocated within the function LCD_Create_Array_Struct.
 * This is done for optimization reasons for speed, calculation, communication and memory usage.
 * All memory is cleared before being deallocated in order to prevent memory leaks.
 * At the end of the lifetime of the structure created in LCD_Create_Array_Struct, this function is called. */
static LCD_ERROR_T LCD_Delete_Array_Struct(LCD_ARRAY_T* structArray)
{
	LCD_ERROR_T Delete_Error = NO_ERROR;
	/* Check if struct pointer is valid */
	if (structArray != NULL)
	{
		/* Remove array pointer from LCD structure */
		LCD_Struct.Array = NULL;
		
		/* Free the array */
		LCD_Free_Array(structArray);
	
		/* Clear all values */
		structArray->coordX = 0x00U;
		structArray->coordY = 0x00U;
		structArray->lines = 0x00U;
		structArray->columns = 0x00U;
		structArray->progress = 0x00U;
		
		/* Free structure memory */
		free(structArray);
	}
	else								/* structArray == NULL */
	{
		/* Invalid structure pointer */
		Delete_Error = E_ARRAY_STRUCT;
		LCD_Struct.Error = E_ARRAY_STRUCT;
		LCD_ErrorHandler(__LINE__, LCD_Struct.Error);
	}
	
	return (Delete_Error);
}

/* Error handler */

void LCD_Default_ErrorHandler(uint16_t lineNumber, LCD_ERROR_T Error)
{
	printf("Error at line %d : ", lineNumber);
	switch (Error)
	{
	case NO_ERROR:
		break;
		/* Everything else to be implemented */
	default:
		break;
	}
	printf("\n");
}


/* TODO LIST 
 * Finish SPI_Write function */