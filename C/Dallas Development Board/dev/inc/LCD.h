#ifndef LCD_H
#define LCD_H
/* --------------------------------------------------------------------------------------------- */
/* Includes */
#include <assert.h>						/* For the assert() macro of the standard library */
#include <stdlib.h>						/* For dynamic heap memory allocation */
//#include <stdbool.h>					/* For boolean type declaration */
//#include <stdint.h>						/* For standard integer type redeclaration */
#include <string.h>						/* For character arrays in text mode */

#define uint8_t unsigned char
#define bool uint8_t
#define size_t unsigned int
#define true 1
#define false 0


#define NDEBUG
#ifndef NDEBUG
//#include <stdio.h>						/* For the printf library used to display error messages */
#else
//#define printf(__e, ...) ((void)0)			/* Disable printf and stdio in production code */
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
	E_PARALLEL_INIT,					/* Error during parallel initialization */
	/* Writing errors */
	E_WRITE_ARRAY,						/* Generic writing array couldn't be initialized */
	E_I2C_WRITE_S_BUSY,					/* Attempt to write in I2C while module state is busy */
	E_UART_WRITE_S_BUSY,				/* Attempt to write in UART while module state is busy */
	E_SPI_WRITE_S_BUSY,					/* Attempt to write in SPI while module state is busy */
	E_PARALLEL_WRITE_S_BUSY,			/* Attempt to write in Parallel while module state is busy */
	E_PARALLEL_WRITE_S_INIT,			/* Attempt to write in Parallel while module state is not yet initialized */
	E_I2C_WRITE_S_FAILURE,				/* Attempt to write in I2C while module has state critically failed */
	E_UART_WRITE_S_FAILURE,				/* Attempt to write in UART while module has state critically failed */
	E_PARALLEL_WRITE_S_FAILURE,			/* Attempt to write in Parallel while module state has critically failed */
	E_I2C_WRITE_COMMAND,				/* Invalid command passed to the I2C write function */
	E_UART_WRITE_COMMAND,				/* Invalid command passed to the UART write function */
	E_PARALLEL_WRITE_COMMAND,			/* Invalid command passed to the parallel write function */
	E_PARALLEL,							/* Error while using a function of the parallel library */
	E_WRITE_SIZE,						/* Size exceeds maximum size of (65535 - LCD_I2C_SENDBUFFER_SIZEADD) */
	/* Text errors */
	E_WRITE_TEXT_SIZE,					/* Incorrect size of character array */
	E_WRITE_TEXT_LINE,					/* Incorrect line number */
	E_WRITE_TEXT_COLUMN,				/* Incorrect column number */
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
}LCD_ERROR_T;

typedef enum
{
	NOT_INIT,							/* Default mode */
	I2C_MODE,							/* I2C Mode; code can't access other modes */
	UART_MODE,							/* UART Mode; code can't access other modes */
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
	LCD_CLEAR = 0x00,
	LCD_TEXT = 0x01,
	LCD_GRAPH = 0x02,
	LCD_PIXEL = 0x03,
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
	LCD_ARRAY_T* Array;
}LCD_STRUCT_T;

/* --------------------------------------------------------------------------------------------- */
/* Functions */

/* Initialization functions */
LCD_ERROR_T LCD_Reset(void);
LCD_ERROR_T LCD_Init_I2C(void); 
LCD_ERROR_T LCD_Init_UART(void); 
LCD_ERROR_T LCD_Init_Parallel(void); 

extern LCD_ERROR_T(*LCD_Write)(const LCD_COMMAND_T Command, const uint8_t pData[], const size_t dataSize);


#endif