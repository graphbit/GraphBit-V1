#pragma once

/* --------------------------------------------------------------------------------------------- */
/* Includes */
#include <assert.h>						/* For the assert() macro of the standard */
#include <stdbool.h>					/* For boolean type declaration */
#include <stdint.h>						/* For standard integer type redeclaration */
#include <stdarg.h>						/* For variadic functions */
#include "stm32f4xx_hal.h"				/* For HAL API peripherals and functions */

/* --------------------------------------------------------------------------------------------- */
/* Definitions */

#define P_DATA_READY	0x01U
#define P_ADRESS_READY	0x02U
#define P_CS_READY		0x04U
#define P_ALL_READY		0xFFU

#define PIN_HIGH		true
#define PIN_LOW			false

/* --------------------------------------------------------------------------------------------- */
/* Enumerated types */

typedef enum
{
	P_NO_ERROR,							/* Default state */
	P_E_INVALID_NUM,					/* Invalid value of argument num indicating number of pins */
	P_E_INVALID_PORT,					/* Invalid port pointer to GPIO_TypeDef */
	P_E_INVALID_PIN,					/* Invalid pin number */
	P_E_NOBUS,							/* None of the bus has been initialized before module init */
	P_E_DATABUS,						/* The data bus hasn't been initialized before module init */
	P_E_ADRESSBUS,						/* The adress bus hasn't been initialized before module init */
	P_E_CSBUS,							/* The Chip Select bus hasn't been initialized before module init */
	P_E_NOTREADY,						/* Attempt to write on the bus before initialization was complete */
	P_E_INVALID_CS,						/* Attempt to use a chip select that hasn't been initialized */
}P_ERROR_T;

typedef enum
{
	CS0 = 0x00U,
	CS1 = 0x01U,
	CS2 = 0x02U,
	CS3 = 0x03U,
	CS4 = 0x04U,
	CS5 = 0x05U,
	CS6 = 0x06U,
	NOCS = 0xFFU
}P_CS_T;

/* --------------------------------------------------------------------------------------------- */
/* Structures */

typedef struct
{
	GPIO_TypeDef* Port;
	uint32_t Pin;
	bool used;
	bool state;
}GPIO_T;

typedef struct
{
	GPIO_T Data[8];
	GPIO_T Adress[16];
	GPIO_T ChipSelect[6];
	GPIO_T Read;
	GPIO_T Write;
	uint8_t ready;
}PARALLEL_PORT_T;

/* --------------------------------------------------------------------------------------------- */
/* Functions */

P_ERROR_T Parallel_InitData(size_t num, ...);
P_ERROR_T Parallel_InitAdress(size_t num, ...);
P_ERROR_T Parallel_InitChipSelect(size_t num, ...);
P_ERROR_T Parallel_InitReadWrite(GPIO_TypeDef* ReadPort, uint32_t ReadPin, GPIO_TypeDef* WritePort, uint32_t WritePin);
P_ERROR_T Parallel_Init(void);

P_ERROR_T Parallel_Write(uint8_t Data, uint16_t Adress, P_CS_T ChipSelect);
P_ERROR_T Parallel_CS(P_CS_T ChipSelect, bool state);