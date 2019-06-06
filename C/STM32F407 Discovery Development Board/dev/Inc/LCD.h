#pragma once
/* --------------------------------------------------------------------------------------------- */
/* Includes */
#include <assert.h>						/* For the assert() macro of the standard library */
#include <stdlib.h>						/* For dynamic heap memory allocation */
#include <stdbool.h>					/* For boolean type declaration */
#include <stdint.h>						/* For standard integer type redeclaration */
#include <string.h>						/* For character arrays in text mode */
#include "Parallel.h"					/* For the parallel port */
#include "stm32f4xx_hal.h"				/* For HAL API peripherals and functions */

#ifndef NDEBUG
#include <stdio.h>						/* For the printf library used to display error messages */
#else
#define printf(__e, ...)((void)0)		/* Disable printf and stdio in production code */
#endif

/* --------------------------------------------------------------------------------------------- */
/* Definitions */
/* Text */
#define LCD_TEXT_MAX_LINES			20U
#define LCD_TEXT_MAX_COLUMNS		30U
/* I2C */
#define LCD_I2C_ADRESS				0x52U

/* Array */
#define LCD_ARRAY_MAX_ATTEMPTS		3U
#define LCD_ARRAY_MAX_COORDX		239U
#define LCD_ARRAY_MAX_COORDY		159U
#define LCD_X_SIZE		            30U
#define LCD_Y_SIZE					160U


/* In the case of a HAL peripheral not included or nonexistant, this code doesn't generate errors.
 * However, it might stop working properly and returns errors, but at least it won't break
 * everything else. */
#ifdef HAL_I2C_MODULE_ENABLED			/* Check if HAL definition of I2C exists */
#define I2C_HANDLER I2C_HandleTypeDef
#else									/* Otherwise create a pointer that will be pointing NULL */
#define I2C_HANDLER uint32_t
#endif

#ifdef HAL_UART_MODULE_ENABLED			/* Check if HAL definition of UART exists */
#define UART_HANDLER UART_HandleTypeDef
#else									/* Otherwise create a pointer that will be pointing NULL */
#define UART_HANDLER uint32_t
#endif

#ifdef HAL_SPI_MODULE_ENABLED			/* Check if HAL definition of SPI exists */
#define SPI_HANDLER SPI_HandleTypeDef
#else									/* Otherwise create a pointer that will be pointing NULL */
#define SPI_HANDLER uint32_t
#endif
#ifdef HAL_TIM_MODULE_ENABLED			/* Check if HAL definition of Timers exists */
#define TIM_HANDLER TIM_HandleTypeDef
#else									/* Otherwise create a pointer that will be pointing NULL */
#define TIM_HANDLER uint32_t
#endif

/* --------------------------------------------------------------------------------------------- */
/* Enumerated types */

typedef enum
{
	NO_ERROR,							/* Default state */
	E_UNIMPLEMENTED,					/* Function called is not implemented in the API yet */
	E_UNINITIALIZED,					/* Function called is not initialized */
	E_MODULE_BUSY,						/* Operations are being performed in another thread */
	E_MODULE_FAILED,					/* The module has failed and can't respond */
	E_UNKNOWN_MODE,						/* Attempting to access an unknown module mode */
	/* Initialization errors */
	E_I2C_INIT,							/* Error during I2C initialization */
	E_UART_INIT,						/* Error during UART initialization */
	E_SPI_INIT,							/* Error during SPI initialization */
	E_PARALLEL_INIT,					/* Error during parallel initialization */
	/* Timer errors */
	E_TIM_INIT,							/* Error during initialization of the timer */
	E_TIM_NOHAL,						/* No HAL definition of the timer */
	E_TIM_START,						/* Error while starting the timer */
	E_TIM_STOP,							/* Error while stopping the timer */
	E_TIM_HAL_ERROR,					/* TIM peripheral error */
	E_TIM_HAL_ERROR_CLKSOURCE,			/* TIM peripheral error while initializing clocksource */
	E_TIM_HAL_ERROR_SLAVE,				/* TIM peripheral error while initializing slave */
	E_TIM_HAL_ERROR_MASTER,				/* TIM peripheral error while initializing master */
	/* Writing errors */
	E_WRITE_ARRAY,						/* Generic writing array couldn't be initialized */
	E_I2C_WRITE_S_BUSY,					/* Attempt to write in I2C while module state is busy */
	E_UART_WRITE_S_BUSY,				/* Attempt to write in UART while module state is busy */
	E_SPI_WRITE_S_BUSY,					/* Attempt to write in SPI while module state is busy */
	E_PARALLEL_WRITE_S_BUSY,			/* Attempt to write in Parallel while module state is busy */
	E_PARALLEL_WRITE_S_INIT,			/* Attempt to write in Parallel while module state is not yet initialized */
	E_I2C_WRITE_S_FAILURE,				/* Attempt to write in I2C while module has state critically failed */
	E_UART_WRITE_S_FAILURE,				/* Attempt to write in UART while module has state critically failed */
	E_SPI_WRITE_S_FAILURE,				/* Attempt to write in SPI while module has state critically failed */
	E_PARALLEL_WRITE_S_FAILURE,			/* Attempt to write in Parallel while module state has critically failed */
	E_I2C_WRITE_COMMAND,				/* Invalid command passed to the I2C write function */
	E_UART_WRITE_COMMAND,				/* Invalid command passed to the UART write function */
	E_SPI_WRITE_COMMAND,				/* Invalid command passed to the SPI write function */
	E_PARALLEL_WRITE_COMMAND,			/* Invalid command passed to the parallel write function */
	E_PARALLEL,							/* Error while using a function of the parallel library */
	E_WRITE_SIZE,						/* Size exceeds maximum size of (65535 - LCD_I2C_SENDBUFFER_SIZEADD) */
	/* Text errors */
	E_WRITE_TEXT_SIZE,					/* Incorrect size of character array */
	E_WRITE_TEXT_LINECOL,				/* Incorrect line or column number */
	E_WRITE_TEXT_CHARACTER,				/* Invalid character */
	/* Graphic errors */
	E_DRAW_SIZE,						/* Invalid size for a drawing */
	/* Array errors */
	E_ARRAY_NONEXISTANT,					/* The array doesn't exist */
	E_ARRAY_STRUCT,						/* Couldn't access structure for array generation and handling */
	E_ARRAY_LINES,						/* Incorrect line number for array generation */
	E_ARRAY_COLUMNS,					/* Incorrect column number for array generation */
	E_ARRAY_COORD_X,					/* Incorrect X coordinate for array generation */
	E_ARRAY_COORD_Y,					/* Incorrect Y coordinate for array generation */
	E_ARRAY_MEMORY,						/* Couldn't create array pointer */
	E_ARRAY_MAXATTEMPTS,				/* Couldn't create array pointer even after many attemps, critical failure */
	E_ARRAY_NOTEMPTY,					/* Trying to allocate memory to a non-empty pointer */
	/* HAL errors */
	E_I2C_HAL_BUSY,						/* I2C peripheral currently busy */
	E_UART_HAL_BUSY,					/* UART peripheral currently busy*/
	E_SPI_HAL_BUSY,						/* SPI peripheral currently busy */
	E_I2C_HAL_TIMEOUT,					/* I2C peripheral timeout error */
	E_UART_HAL_TIMEOUT,					/* UART peripheral timeout error */
	E_SPI_HAL_TIMEOUT,					/* SPI peripheral timeout error */
	E_I2C_HAL_UNDEFINED,				/* I2C files from HAL aren't included properly */
	E_UART_HAL_UNDEFINED,				/* UART files from HAL aren't included properly */
	E_SPI_HAL_UNDEFINED,				/* SPI files from HAL aren't included properly */
	E_SPI_HAL_ERROR,					/* HAL SPI generic error */
	E_SPI_HAL_ERROR_MODF,				/* HAL SPI error #MODF	-- Mode Fault */
	E_SPI_HAL_ERROR_CRC,				/* HAL SPI error #CRC	-- Invalid Checksum */
	E_SPI_HAL_ERROR_OVR,				/* HAL SPI error #OVR	-- Overrun error */
	E_SPI_HAL_ERROR_FRE,				/* HAL SPI error #FRE	-- Frame Format Error */
	E_SPI_HAL_ERROR_DMA,				/* HAL SPI error #DMA	-- DMA error */
	E_I2C_HAL_ERROR,					/* HAL I2C generic error */
	E_I2C_HAL_ERROR_AF,					/* HAL I2C error #AF	-- Acknowledge Failure */
	E_I2C_HAL_ERROR_BERR,				/* HAL I2C error #BERR	-- Bus Error */
	E_I2C_HAL_ERROR_ARLO,				/* HAL I2C error #ARLO	-- Arbitration lost */
	E_I2C_HAL_ERROR_OVR,				/* HAL I2C error #OVR	-- Overrun error */
	E_I2C_HAL_ERROR_DMA,				/* HAL I2C error #DMA	-- DMA error */
	E_UART_HAL_ERROR,					/* HAL UART generic error */
	E_UART_HAL_ERROR_NONE,				/* HAL UART error #none -- No error */
	E_UART_HAL_ERROR_PE,				/* HAL UART error #PE   -- Parity error */
	E_UART_HAL_ERROR_NE,				/* HAL_UART error #NE   -- Noise error */
	E_UART_HAL_ERROR_FE,				/* HAL UART error #FE	-- Frame error */
	E_UART_HAL_ERROR_ORE,				/* HAL UART error #ORE  -- Overrun error */
	E_UART_HAL_ERROR_DMA,				/* HAL UART error #DMA	-- DMA transfer error */
}LCD_ERROR_T;

typedef enum
{
	NOT_INIT,							/* Default mode */
	I2C_MODE,							/* I2C Mode; code can't access other modes */
	UART_MODE,							/* UART Mode; code can't access other modes */
	SPI_MODE,							/* SPI Mode; code can't access other modes */
	PARALLEL_MODE						/* Parallel Mode; code can't access other modes */
}LCD_MODE_T;

typedef enum
{
	LCD_READY = 0x00U,							/* LCD module is ready to perform operations */
	LCD_BUSY = 0x01U,							/* LCD module is currently performing operations */
	LCD_FAILURE = 0x02U,						/* A critical error has disabled the LCD module */
	LCD_NOT_INIT = 0x02U						/* LCD module hasn't been initialized */
}LCD_STATE_T;

typedef enum
{
	LCD_CLEAR = 0x00U,
	LCD_TEXT = 0x01U,
	LCD_GRAPH = 0x02U,
	LCD_PIXEL = 0x03U,
}LCD_COMMAND_T;
/* --------------------------------------------------------------------------------------------- */
/* Structure declaration */
typedef struct
{
	uint8_t lines;
	uint8_t columns;
	uint8_t coordX;
	uint8_t coordY;
	uint8_t progress;
	uint8_t attempts;
	bool ArrayDefined;
	uint8_t** pArray;					/* Bidimentional array representing the screen, lines first, columns second */
}LCD_ARRAY_T;


typedef struct
{
	LCD_MODE_T Mode;
	LCD_STATE_T State;
	LCD_ERROR_T Error;
	I2C_HANDLER* hi2c;
	UART_HANDLER* huart;
	SPI_HANDLER* hspi;
	TIM_HANDLER* htim;
	LCD_ARRAY_T* Array;
}LCD_STRUCT_T;

/* --------------------------------------------------------------------------------------------- */
/* Functions */

/* Initialization functions */
LCD_ERROR_T LCD_Reset(void);
LCD_ERROR_T LCD_Init_I2C(I2C_HANDLER* hi2c, TIM_HANDLER* htim); 
LCD_ERROR_T LCD_Init_UART(UART_HANDLER* hi2c, TIM_HANDLER* htim); 
LCD_ERROR_T LCD_Init_SPI(SPI_HANDLER* hi2c, TIM_HANDLER* htim); 
LCD_ERROR_T LCD_Init_Parallel(TIM_HANDLER* htim); 

/* Command functions */
extern LCD_ERROR_T(*LCD_Clear)(void);
extern LCD_ERROR_T(*LCD_Write)(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize);

/* Text functions */
LCD_ERROR_T LCD_WriteText(char* pText, size_t textSize, uint8_t line, uint8_t column);
LCD_ERROR_T LCD_WriteLine(char* pText, size_t textSize, uint8_t line, uint8_t column);
/* Graphic functions */

/* Error Handler */
extern void(*LCD_ErrorHandler)(uint16_t lineNumber, LCD_ERROR_T Error);