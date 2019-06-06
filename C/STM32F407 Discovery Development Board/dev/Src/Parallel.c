#include "Parallel.h"

/* --------------------------------------------------------------------------------------------- */
/* Create an initialize the Parallel structure */
static PARALLEL_PORT_T Parallel_Struct = 
{ 
	{									/* Data bus */
		{ NULL, 0x00, false, false },	/* D0 */
		{ NULL, 0x00, false, false },	/* D1 */
		{ NULL, 0x00, false, false },	/* D2 */
		{ NULL, 0x00, false, false },	/* D3 */
		{ NULL, 0x00, false, false },	/* D4 */
		{ NULL, 0x00, false, false },	/* D5 */
		{ NULL, 0x00, false, false },	/* D6 */
		{ NULL, 0x00, false, false },	/* D7 */
	},
	{									/* Adress bus */
		{ NULL, 0x00, false, false },	/* A0 */
		{ NULL, 0x00, false, false },	/* A1 */
		{ NULL, 0x00, false, false },	/* A2 */
		{ NULL, 0x00, false, false },	/* A3 */
		{ NULL, 0x00, false, false },	/* A4 */
		{ NULL, 0x00, false, false },	/* A5 */
		{ NULL, 0x00, false, false },	/* A6 */
		{ NULL, 0x00, false, false },	/* A7 */
		{ NULL, 0x00, false, false },	/* A8 */
		{ NULL, 0x00, false, false },	/* A9 */
		{ NULL, 0x00, false, false },	/* A10 */
		{ NULL, 0x00, false, false },	/* A11 */
		{ NULL, 0x00, false, false },	/* A12 */
		{ NULL, 0x00, false, false },	/* A13 */
		{ NULL, 0x00, false, false },	/* A14 */
		{ NULL, 0x00, false, false },	/* A15 */
	},
	{									/* Chip Select */
		{ NULL, 0x00, false, false },	/* CS0 */
		{ NULL, 0x00, false, false },	/* CS1 */
		{ NULL, 0x00, false, false },	/* CS2 */
		{ NULL, 0x00, false, false },	/* CS3 */
		{ NULL, 0x00, false, false },	/* CS4 */
		{ NULL, 0x00, false, false },	/* CS5 */
	},
        { NULL, 0x00, false, false },           /* Read */
        { NULL, 0x00, false, false },           /* Write */
	0x00U								/* uint8_t ready */
};

/* --------------------------------------------------------------------------------------------- */
/* Private Function Declarations */

static void Parallel_ResetData(void);
static void Parallel_ResetAdress(void);
static void Parallel_ResetChipSelect(void);

static void Parallel_WriteIO(GPIO_T* GPIO, bool state);

/* --------------------------------------------------------------------------------------------- */
/* Public Function Definitions */

/* --------------------------------------------------------------------------------------------- */
/* Initialization functions */

/* The following 3 functions are non-compliant with MISRA-C:2004 guidelines, breaking rule 16.1 :
 * "Functions shall not be defined with a variable number of arguments"
 * And rule 14.6 :
 * "For any iteration statement there shall be at most one berak statement used for loop termination"
 * Specific deviations have been raised for all 3 of the functions and security measures have been
 * taken seriously. 
 * Noncompliant functions:
 * P_ERROR_T Parallel_InitData(size_t num, ...);
 * P_ERROR_T Parallel_InitAdress(size_t num, ...);
 * P_ERROR_T Parallel_InitChipSelect(size_t num, ...);
 *
 * Prohibited functions of <stdarg.h> used:
 * void va_start (va_list ap, paramN);			- Used in Parallel_InitData & Parallel_InitAdress & Parallel_InitChipSelect
 * type va_arg (va_list ap, type);				- Used in Parallel_InitData & Parallel_InitAdress & Parallel_InitChipSelect
 * void va_end (va_list ap);					- Used in Parallel_InitData & Parallel_InitAdress & Parallel_InitChipSelect
 *
 **/


/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      Parallel_InitData                                               
* P_ERROR_T Parallel_InitData(size_t num, ...)
* Author : Pascal-Emmanuel Lachance  & Charles Lévesque-Matte
* Date : 2019-04-25
* 
* This function is a variadic function, meaning it can take a variable number of parameters.
* The first parameter of the function is size_t num, and indicates the number of pins to configure
* Every other parameter must come in pair of a GPIO_TypeDef pointer (such as GPIOE) and a pin number (such as GPIO_PIN_10).
* For every parameter passed to the function, it places it in the Parallel_Struct structure, within
* the data array.
* The data array within the Parallel_Struct struct is mapped as such:
*		GPIO_TypeDef* Port
*		uint32_t Pin
*		bool used
*		bool state* 
* 
* Called by : none
* Function calls : none
*
* Parameters : 
*			- size_t num : number of pins to configure
*			- GPIO_TypeDef* Port0 : First port to map
*			- uint32_t Pin0 : First pin to map
*			- GPIO_TypeDef* Port1 : Second port to map
*			- uint32_t Pin1 : Second pin to map
*			...
*
* Returns: P_ERROR_T InitData_Error :
*			- P_E_INVALID_NUM : Number of arguments is invalid
*			- P_E_INVALID_PORT : Port is invalid (not a GPIO_TypeDef pointer?)
*			- P_E_INVALID_PIN : Pin is invalid
*			
* Example call : 
* Parallel_InitData(0x04U, GPIOE, GPIO_PIN_8, GPIOE, GPIO_PIN_9, GPIOE, GPIO_PIN_10, GPIOE, GPIO_PIN_11);
*			
* This function is noncompliant with MISRA-C:2004 (Rule 14.6 & 16.1), specific deviations have been made.
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/* MISRA-C : 2004 prohibits the usage of variadic functions (Rule 16.1)
 * We're creating a specific deviation for this function, Parallel_InitData, that takes as input
 * a variable amount of parameters.
 * This is done in order to make the librairy easier to use and more flexible.
 * We're checking every value after casting them to the proper type, and making sure all are valid before use.
 * Any doubt of invalidity instantly aborts the input process.
 * In the case of an error during data acquisition, all values are reset and an error flag is raised
 * In the case of an error during data acquisition, function va_end is called to stop the acquisition and free memory */
/* MISRA-C : 2004 prohibits the multiple break statement within a loop (Rule 14.6)
 * We're creating a specific deviation for this function, Parallel_InitData, that can exit the data acquisition loop
 * if an error arises either while receiving the port or the pin.
 * This is done in order to immediatly handle an error in the loop if it arises, and is more secure than not having two break statements.
 * In the case of an error during data acquisition, all values are reset and an error flag is raised
 * In the case of an error during data acquisition, function va_end is called to stop the acquisition and free memory */
P_ERROR_T Parallel_InitData(size_t num, ...)
{
	P_ERROR_T InitData_Error = P_NO_ERROR;
	va_list ap;
	/* Check if number of argument is correct */
	if ((num <= 8U) && (num > 0U))
	{
		uint8_t Data_variadic_i;
		/* Initialize va_list for num number of argument */
		va_start(ap, num);
		/* Read every argument passed */
		for (Data_variadic_i = 0x00U; Data_variadic_i < num; Data_variadic_i++)
		{
			/* Retrieve Port */
			Parallel_Struct.Data[Data_variadic_i].Port = (GPIO_TypeDef*) va_arg(ap, GPIO_TypeDef*);
			/* Immediatly check if port is valid */
			if (Parallel_Struct.Data[Data_variadic_i].Port == NULL)
			{
				/* Port isn't valid! */
				InitData_Error = P_E_INVALID_PORT;
				assert(Parallel_Struct.Data[Data_variadic_i].Port != NULL);
				assert(Parallel_Struct.Data[Data_variadic_i].Port != 0);
				va_end(ap);				/* End the variable list */
				break;					/* Exit the loop */
			}
			
			/* Retrieve Pin */
			Parallel_Struct.Data[Data_variadic_i].Pin = (uint32_t) va_arg(ap, uint32_t);
			/* Immediatly check if pin is valid */
			if (Parallel_Struct.Data[Data_variadic_i].Pin > 0x8000U)
			{							/* If the pin number is bigger than 15  */
				/* Pin number isn't valid */
				InitData_Error = P_E_INVALID_PIN;
				assert(Parallel_Struct.Data[Data_variadic_i].Pin <= 15U);
				va_end(ap);				/* End the variable list */
				break;					/* Exit the loop */
			}
			Parallel_Struct.Data[Data_variadic_i].used = true;
		}

		/* Check if an error was encountered within the loop */
		if (InitData_Error == P_NO_ERROR)
		{
			/* End the variadic list and clean memory */
			va_end(ap);
			Parallel_Struct.ready |= P_DATA_READY;
		}
		else							/* InitData_Error != P_NO_ERROR */
		{								/* There was an error during the loop */
			/* Reset all values in the data structure */
			Parallel_ResetData();
		}
	}
	else								/* (num > 8) || (num < 0) */
	{									/* number of argument invalid */
		InitData_Error = P_E_INVALID_NUM;
		assert((num <= 8U) && (num > 0U));
	}
	
	return (InitData_Error);
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      Parallel_InitAdress                                               
* P_ERROR_T Parallel_InitAdress(size_t num, ...)
* Author : Pascal-Emmanuel Lachance  & Charles Lévesque-Matte
* Date : 2019-04-25
* 
* This function is a variadic function, meaning it can take a variable number of parameters.
* The first parameter of the function is size_t num, and indicates the number of pins to configure
* Every other parameter must come in pair of a GPIO_TypeDef pointer (such as GPIOE) and a pin number (such as GPIO_PIN_10).
* For every parameter passed to the function, it places it in the Parallel_Struct structure, within
* the adress array.
* The adress array within the Parallel_Struct struct is mapped as such:
*		GPIO_TypeDef* Port
*		uint32_t Pin
*		bool used
*		bool state* 
* 
* Called by : none
* Function calls : none
*
* Parameters : 
*			- size_t num : number of pins to configure
*			- GPIO_TypeDef* Port0 : First port to map
*			- uint32_t Pin0 : First pin to map
*			- GPIO_TypeDef* Port1 : Second port to map
*			- uint32_t Pin1 : Second pin to map
*			...
*
* Returns: P_ERROR_T InitAdress_Error :
*			- P_E_INVALID_NUM : Number of arguments is invalid
*			- P_E_INVALID_PORT : Port is invalid (not a GPIO_TypeDef pointer?)
*			- P_E_INVALID_PIN : Pin is invalid
*			
* Example call : 
* Parallel_InitAdress(0x03U, GPIOD, GPIO_PIN_8, GPIOD, GPIO_PIN_9, GPIOD, GPIO_PIN_10);
*			
* This function is noncompliant with MISRA-C:2004 (Rule 14.6 & 16.1), specific deviations have been made.
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/* MISRA-C : 2004 prohibits the usage of variadic functions (Rule 16.1)
 * We're creating a specific deviation for this function, Parallel_InitAdress, that takes as input
 * a variable amount of parameters.
 * This is done in order to make the librairy easier to use and more flexible.
 * We're checking every value after casting them to the proper type, and making sure all are valid before use.
 * Any doubt of invalidity instantly aborts the input process.
 * In the case of an error during data acquisition, all values are reset and an error flag is raised
 * In the case of an error during data acquisition, function va_end is called to stop the acquisition and free memory */
/* MISRA-C : 2004 prohibits the multiple break statement within a loop (Rule 14.6)
 * We're creating a specific deviation for this function, Parallel_InitAdress, that can exit the data acquisition loop
 * if an error arises either while receiving the port or the pin.
 * This is done in order to immediatly handle an error in the loop if it arises, and is more secure than not having two break statements.
 * In the case of an error during data acquisition, all values are reset and an error flag is raised
 * In the case of an error during data acquisition, function va_end is called to stop the acquisition and free memory */
P_ERROR_T Parallel_InitAdress(size_t num, ...)
{
	P_ERROR_T InitAdress_Error = P_NO_ERROR;
	va_list ap;
	/* Check if number of argument is correct */
	if ((num <= 16U) && (num > 0U))
	{
		uint8_t Adress_variadic_i;
		/* Initialize va_list for num number of argument */
		va_start(ap, num);
		/* Read every argument passed */
		for (Adress_variadic_i = 0x00U; Adress_variadic_i < num; Adress_variadic_i++)
		{
			/* Retrieve Port */
			Parallel_Struct.Adress[Adress_variadic_i].Port = (GPIO_TypeDef*) va_arg(ap, GPIO_TypeDef*);
			/* Immediatly check if port is valid */
			if (Parallel_Struct.Adress[Adress_variadic_i].Port == NULL)
			{
				/* Port isn't valid! */
				InitAdress_Error = P_E_INVALID_PORT;
				assert(Parallel_Struct.Adress[Adress_variadic_i].Port != NULL);
				assert(Parallel_Struct.Adress[Adress_variadic_i].Port != 0);
				va_end(ap);				/* End the variable list */
				break;					/* Exit the loop */
			}
			
			/* Retrieve Pin */
			Parallel_Struct.Adress[Adress_variadic_i].Pin = (uint32_t) va_arg(ap, uint32_t);
			/* Immediatly check if pin is valid */
			if (Parallel_Struct.Adress[Adress_variadic_i].Pin > 0x8000U)
			{
				/* If the pin number is bigger than 15 */
				/* Pin number isn't valid */
				InitAdress_Error = P_E_INVALID_PIN;
				assert(Parallel_Struct.Adress[Adress_variadic_i].Pin <= 15U);
				va_end(ap);				/* End the variable list */
				break;					/* Exit the loop */
			}
			Parallel_Struct.Adress[Adress_variadic_i].used = true;
		}

		/* Check if an error was encountered within the loop */
		if (InitAdress_Error == P_NO_ERROR)
		{
			/* End the variadic list and clean memory */
			va_end(ap);
			Parallel_Struct.ready |= P_ADRESS_READY;
		}
		else							/* InitAdress_Error = P_NO_ERROR */
		{								/* There was an error in the loop */
			/* Reset all values in the adress structure */
			Parallel_ResetAdress();
		}
	}
        else if (num == 0)
        {
          Parallel_Struct.ready |= P_ADRESS_READY;
        }
	else								/* (num > 8) || (num < 0) */
	{
		/* number of argument invalid */
		InitAdress_Error = P_E_INVALID_NUM;
		assert((num <= 8) && (num > 0));
	}
	
	return (InitAdress_Error);
}


/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      Parallel_InitChipSelect                                               
* P_ERROR_T Parallel_InitChipSelect(size_t num, ...)
* Author : Pascal-Emmanuel Lachance  & Charles Lévesque-Matte
* Date : 2019-04-25
* 
* This function is a variadic function, meaning it can take a variable number of parameters.
* The first parameter of the function is size_t num, and indicates the number of pins to configure
* Every other parameter must come in pair of a GPIO_TypeDef pointer (such as GPIOE) and a pin number (such as GPIO_PIN_10).
* For every parameter passed to the function, it places it in the Parallel_Struct structure, within
* the chip select array.
* The chip select array within the Parallel_Struct struct is mapped as such:
*		GPIO_TypeDef* Port
*		uint32_t Pin
*		bool used
*		bool state* 
* 
* Called by : none
* Function calls : none
*
* Parameters : 
*			- size_t num : number of pins to configure
*			- GPIO_TypeDef* Port0 : First port to map
*			- uint32_t Pin0 : First pin to map
*			- GPIO_TypeDef* Port1 : Second port to map
*			- uint32_t Pin1 : Second pin to map
*			...
*
* Returns: P_ERROR_T InitChipSelect_Error :
*			- P_E_INVALID_NUM : Number of arguments is invalid
*			- P_E_INVALID_PORT : Port is invalid (not a GPIO_TypeDef pointer?)
*			- P_E_INVALID_PIN : Pin is invalid
*			
* Example call : 
* Parallel_InitChipSelect(0x03U, GPIOB, GPIO_PIN_11, GPIOB, GPIO_PIN_12, GPIOB, GPIO_PIN_13);
*			
* This function is noncompliant with MISRA-C:2004 (Rule 14.6 & 16.1), specific deviations have been made.
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/* MISRA-C : 2004 prohibits the usage of variadic functions (Rule 16.1)
 * We're creating a specific deviation for this function, Parallel_InitChipSelect, that takes as input
 * a variable amount of parameters.
 * This is done in order to make the librairy easier to use and more flexible.
 * We're checking every value after casting them to the proper type, and making sure all are valid before use.
 * Any doubt of invalidity instantly aborts the input process.
 * In the case of an error during data acquisition, all values are reset and an error flag is raised
 * In the case of an error during data acquisition, function va_end is called to stop the acquisition and free memory */
/* MISRA-C : 2004 prohibits the multiple break statement within a loop (Rule 14.6)
 * We're creating a specific deviation for this function, Parallel_InitChipSelect, that can exit the data acquisition loop
 * if an error arises either while receiving the port or the pin.
 * This is done in order to immediatly handle an error in the loop if it arises, and is more secure than not having two break statements.
 * In the case of an error during data acquisition, all values are reset and an error flag is raised
 * In the case of an error during data acquisition, function va_end is called to stop the acquisition and free memory */
P_ERROR_T Parallel_InitChipSelect(size_t num, ...)
{
	P_ERROR_T InitChipSelect_Error = P_NO_ERROR;
	va_list ap;
	/* Check if number of argument is correct */
	if ((num <= 6) && (num > 0))
	{
		uint8_t ChipSelect_variadic_i;
		/* Initialize va_list for num number of argument */
		va_start(ap, num);
		/* Read every argument passed */
		for (ChipSelect_variadic_i = 0x00; ChipSelect_variadic_i < num; ChipSelect_variadic_i++)
		{
			/* Retrieve Port */
			Parallel_Struct.ChipSelect[ChipSelect_variadic_i].Port = (GPIO_TypeDef*) va_arg(ap, GPIO_TypeDef*);
			/* Immediatly check if port is valid */
			if (Parallel_Struct.ChipSelect[ChipSelect_variadic_i].Port == NULL)
			{
				/* Port isn't valid! */
				InitChipSelect_Error = P_E_INVALID_PORT;
				assert(Parallel_Struct.ChipSelect[ChipSelect_variadic_i].Port != NULL);
				assert(Parallel_Struct.ChipSelect[ChipSelect_variadic_i].Port != 0);
				va_end(ap); /* End the variable list */
				break;					/* Exit the loop */
			}
			
			/* Retrieve Pin */
			Parallel_Struct.ChipSelect[ChipSelect_variadic_i].Pin = (uint32_t) va_arg(ap, uint32_t);
			/* Immediatly check if pin is valid */
			if (Parallel_Struct.ChipSelect[ChipSelect_variadic_i].Pin > 0x8000U)
			{
				/* If the pin number is bigger than 15 or smaller than 0 */
				/* Pin number isn't valid */
				InitChipSelect_Error = P_E_INVALID_PIN;
				assert(Parallel_Struct.ChipSelect[ChipSelect_variadic_i].Pin <= 15U);
				va_end(ap); /* End the variable list */
				break;					/* Exit the loop */
			}
            Parallel_Struct.ChipSelect[ChipSelect_variadic_i].used = true;
		}

		/* Check if an error was encountered within the loop */
		if (InitChipSelect_Error == P_NO_ERROR)
		{
			/* End the variadic list and clean memory */
			va_end(ap);
			Parallel_Struct.ready |= P_CS_READY;
		}
		else							/* InitChipSelect_Error != P_NO_ERROR */
		{
			/* There was an error in the loop */
			/* Reset all values in the chip select structure */
			Parallel_ResetChipSelect();
		}
	}
	else if (num == 0)
	{									/* We don't want to initialize a chip select */
		Parallel_Struct.ready |= P_CS_READY;
	}
	else								/* (num > 8) || (num < 0) */
	{									/* number of argument invalid */
		InitChipSelect_Error = P_E_INVALID_NUM;
		assert((num <= 8) && (num > 0));
	}
	
	return (InitChipSelect_Error);
}

P_ERROR_T Parallel_InitReadWrite(GPIO_TypeDef* ReadPort, uint32_t ReadPin, GPIO_TypeDef* WritePort, uint32_t WritePin)
{
	P_ERROR_T InitRW_Error = P_NO_ERROR;
	if (ReadPort != NULL)
	{
		if (ReadPin <= 0x8000U)
		{
			Parallel_Struct.Read.Port = ReadPort;
			Parallel_Struct.Read.Pin = ReadPin;
			Parallel_Struct.Read.used = true;
		}
		else							/* ReadPin > 0x8000U */
		{								/* Pin number is invalid */
			InitRW_Error = P_E_INVALID_PIN;
			assert(ReadPin <= 15U);
		}
	}
	
	if (WritePort != NULL)
	{
		if (WritePin <= 0x8000U)
		{
			Parallel_Struct.Write.Port = WritePort;
			Parallel_Struct.Write.Pin = WritePin;
			Parallel_Struct.Write.used = true;
		}
		else							/* ReadPin > 0x8000U */
		{								/* Pin number is invalid */
			InitRW_Error = P_E_INVALID_PIN;
			assert(WritePin <= 15U);
		}
	}
	
	
	return (InitRW_Error);
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      Parallel_Init                                               
* P_ERROR_T Parallel_Init(void)
* Author : Pascal-Emmanuel Lachance  & Charles Lévesque-Matte
* Date : 2019-04-25
* 
* This function must be called after Parallel_InitData, Parallel_InitAdress & Parallel_InitChipSelect
* Initialises the parallel module using HAL functions for GPIO initialisation.
* Pulls all the pins initialized at a low state.
* 
* Called by : none
* Function calls : Parallel_WriteIO()
*
* Parameters : none
*
* Returns: P_ERROR_T P_Init_Error:
*			- P_E_DATABUS : The databus hasn't been initialised (Use Parallel_InitData)
*			- P_E_ADRESSBUS : The adress bus hasn't been initialized (use Parallel_InitAdress)
*			- P_E_CSBUS : The chip select bus hasn't been initialized (use Parallel-InitChipSelect)
*			- P_E_NOBUS : None of the bus has been initialized
*			
* Example call : Parallel_Init();
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
P_ERROR_T Parallel_Init(void)
{
	P_ERROR_T P_Init_Error = P_NO_ERROR;
	if (Parallel_Struct.ready == ((P_DATA_READY) | (P_ADRESS_READY) | (P_CS_READY)))
	{
		/* HAL Initialization structure for the GPIOs */
		GPIO_InitTypeDef GPIO_InitStruct = { 0 };
		uint8_t P_Init_i;
	
		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOE_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOH_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
	
		/* Configure GPIO pin Output Level for Data bus */
		for (P_Init_i = 0; P_Init_i < (sizeof(Parallel_Struct.Data) / sizeof(GPIO_T)); P_Init_i++)
		{
			if (Parallel_Struct.Data[P_Init_i].used == true)
			{
				/* Put pin in low state */
				Parallel_WriteIO(&Parallel_Struct.Data[P_Init_i], PIN_LOW);
			}
		}
	
		/* Configure GPIO pin Output Level for Adress bus */
		for (P_Init_i = 0; P_Init_i < (sizeof(Parallel_Struct.Adress) / sizeof(GPIO_T)); P_Init_i++)
		{
			if (Parallel_Struct.Adress[P_Init_i].used == true)
			{
				/* Put pin in low state */
				Parallel_WriteIO(&Parallel_Struct.Adress[P_Init_i], PIN_LOW);
			}
		}
	
		/* Configure GPIO pin Output Level for Chip Selects */
		for (P_Init_i = 0; P_Init_i < (sizeof(Parallel_Struct.ChipSelect) / sizeof(GPIO_T)); P_Init_i++)
		{
			if (Parallel_Struct.ChipSelect[P_Init_i].used == true)
			{
				/* Put pin in low state */
				Parallel_WriteIO(&Parallel_Struct.ChipSelect[P_Init_i], PIN_LOW);
			}
		}
		
		/* Configure GPIO pin Output Level for Read and Write signals */
		/* Read and write signals are inverted logic signals */
		if (Parallel_Struct.Read.used == true)
		{
			/* Put pin in high state */
			Parallel_WriteIO(&Parallel_Struct.Read, !PIN_LOW);
		}
		if (Parallel_Struct.Write.used == true)
		{
			/* Put pin in high state */
			Parallel_WriteIO(&Parallel_Struct.Write, !PIN_LOW);
		}
	
		/* Configure Data bus GPIO pins */
		for (P_Init_i = 0; P_Init_i < (sizeof(Parallel_Struct.Data) / sizeof(GPIO_T)); P_Init_i++)
		{
			if (Parallel_Struct.Data[P_Init_i].used == true)
			{
				GPIO_InitStruct.Pin = Parallel_Struct.Data[P_Init_i].Pin;
				GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
				GPIO_InitStruct.Pull = GPIO_NOPULL;
				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
				HAL_GPIO_Init(Parallel_Struct.Data[P_Init_i].Port, &GPIO_InitStruct);
			}
		}
	
		/* Configure Adress bus GPIO pins */
		for (P_Init_i = 0; P_Init_i < (sizeof(Parallel_Struct.Adress) / sizeof(GPIO_T)); P_Init_i++)
		{
			if (Parallel_Struct.Adress[P_Init_i].used == true)
			{
				GPIO_InitStruct.Pin = Parallel_Struct.Adress[P_Init_i].Pin;
				GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
				GPIO_InitStruct.Pull = GPIO_NOPULL;
				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
				HAL_GPIO_Init(Parallel_Struct.Adress[P_Init_i].Port, &GPIO_InitStruct);
			}
		}
	
		/* Configure Chip Select bus GPIO pins */
		for (P_Init_i = 0; P_Init_i < (sizeof(Parallel_Struct.ChipSelect) / sizeof(GPIO_T)); P_Init_i++)
		{
			if (Parallel_Struct.ChipSelect[P_Init_i].used == true)
			{
				GPIO_InitStruct.Pin = Parallel_Struct.ChipSelect[P_Init_i].Pin;
				GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
				GPIO_InitStruct.Pull = GPIO_NOPULL;
				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
				HAL_GPIO_Init(Parallel_Struct.ChipSelect[P_Init_i].Port, &GPIO_InitStruct);
			}
		}
		
		/* Configure Read and Write signals GPIO pins */
		if (Parallel_Struct.Read.used == true)
		{
			GPIO_InitStruct.Pin = Parallel_Struct.Read.Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
			HAL_GPIO_Init(Parallel_Struct.Read.Port, &GPIO_InitStruct);
		}
		if (Parallel_Struct.Write.used == true)
		{
			GPIO_InitStruct.Pin = Parallel_Struct.Write.Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
			HAL_GPIO_Init(Parallel_Struct.Write.Port, &GPIO_InitStruct);
		}
		
		Parallel_Struct.ready = P_ALL_READY;
	}
	else								/* Parallel_Struct.ready != ((P_DATA_READY) | (P_ADRESS_READY) | (P_CS_READY)) */
	{									/* One of the bus hasn't been intialized properly before this function call */
		/* The functions Parallel_InitData, Parallel_InitAdress and Parallel_InitChipSelect need to be called before this function */
		if (Parallel_Struct.ready == 0x00)
		{								/* None of the bus has been initiaized */
			P_Init_Error = P_E_NOBUS;
		}
		if ((Parallel_Struct.ready & P_DATA_READY) != P_DATA_READY)
		{
			/* The data bus hasn't been initialized */
			P_Init_Error = P_E_DATABUS;
		}
		if ((Parallel_Struct.ready & P_ADRESS_READY) != P_ADRESS_READY)
		{
			/* The adress bus hasn't been initialized */
			P_Init_Error = P_E_ADRESSBUS;
		}
		if ((Parallel_Struct.ready & P_CS_READY) != P_CS_READY)
		{
			P_Init_Error = P_E_CSBUS;
		}
	}
	
	return (P_Init_Error);
}

/* --------------------------------------------------------------------------------------------- */
/* Low-level functions definitions */


/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      Parallel_WriteIO
* static void Parallel_WriteIO(GPIO_T* GPIO, bool state)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-25
* 
* Places a specific pin to the specified state.
* 
* Called by :
*			- Parallel_Write()
*			- Parallel_CS()
* Function calls : none
*			
* Parameters : GPIO_T* GPIO, bool state
*			- GPIO pointer to a pin in the Parallel_Struct structure.
*			- state : state to drive the chosen pin to.
* Returns: none
* 
* Example call : Parallel_WriteIO(&Parallel_Struct.Data[0], 1U);
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static void Parallel_WriteIO(GPIO_T* GPIO, bool state)
{
//  unsigned long i;
//  for (i = 0; i < 600; i++)
//  {
//    ;
//  }
    if (GPIO->state != state)
	{
		GPIO->state = state;
		HAL_GPIO_WritePin(GPIO->Port, GPIO->Pin, (GPIO_PinState) state);
	}
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      Parallel_CS
* P_ERROR_T Parallel_CS(uint8_t ChipSelect, bool state)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-25
* 
* Places a specific chip select to a chosen state and puts all other chip selects low
* Parallel_Init() must be called before this function is called in order to intialized the bus.
* 
* Called by : Parallel_Write()
* Function calls : 
*			- Parallel_WriteIO()
*			
* Parameters : uint8_t ChipSelect, bool state
*			- Chip select : Single chip select to activate
*			- state : state to drive the chosen chipselect pin to
* Returns: P_ERROR_T CS_Error
*			- P_E_INVALID_CS : The chip select chosen is not valid (refers to an innexisting CS?)
*			
* Example call : Parallel_CS(CS2, 1U);
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
P_ERROR_T Parallel_CS(P_CS_T ChipSelect, bool state)
{
	P_ERROR_T CS_Error = P_NO_ERROR;
	uint8_t CSWrite_i;
	/* Check if chosen Chip Select is valid */
	if (ChipSelect < ((sizeof(Parallel_Struct.ChipSelect)) / (sizeof(GPIO_T))))
	{
		/* Check if chosen chip select is initialized */
		if (Parallel_Struct.ChipSelect[ChipSelect].used == true)
		{
			for (CSWrite_i = 0x00U; CSWrite_i < (sizeof(Parallel_Struct.ChipSelect) / sizeof(GPIO_T)); CSWrite_i++)
			{
				if (Parallel_Struct.ChipSelect[CSWrite_i].used == true)
				{
					(CSWrite_i == ChipSelect) ? Parallel_WriteIO(&Parallel_Struct.ChipSelect[CSWrite_i], state) : Parallel_WriteIO(&Parallel_Struct.ChipSelect[CSWrite_i], 0U);
				}
				else					/* We've reached bits that aren't used */
				{
					break;				/* Exit loop */
				}
			}
		}
		else							/* Parallel_Struct.ChipSelect[ChipSelect].used != true */
		{
			/* Chosen Chip Select isn't available */
			CS_Error = P_E_INVALID_CS;
		}
	}
	else if(ChipSelect == NOCS)			/* Clearing all chip selects */
	{
		for (CSWrite_i = 0x00U; CSWrite_i < (sizeof(Parallel_Struct.ChipSelect) / sizeof(GPIO_T)); CSWrite_i++)
		{
			Parallel_WriteIO(&Parallel_Struct.ChipSelect[CSWrite_i], 0x00U);
		}
	}
	else								/* ChipSelect >= ((sizeof(Parallel_Struct.ChipSelect)) / (sizeof(GPIO_T))) */
	{									/* Chosen Chip Select doesn't exist */
		CS_Error = P_E_INVALID_CS;
	}
	
	return (CS_Error);
}

/* --------------------------------------------------------------------------------------------- */
/* Mid-level functions definitions */

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      Parallel_Write
* P_ERROR_T Parallel_Write(uint8_t Data, uint16_t Adress, uint8_t ChipSelect)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-25
* 
* Write Data and Adress to a specified Chip Select on the parallel bus.
* Parallel_Init() must be called before this function is called in order to intialized the bus.
* 
* Called by : none
* Function calls : 
*			- Parallel_WriteIO()
*			- Parallel_CS()
*			
* Parameters : uint8_t Data, uint16_t Adress, uint8_t ChipSelect
*			- Data : Data to write on the data bus
*			- Adress : Adress to send to data to
*			- Chip select : Single chip select to activate
* Returns: P_ERROR_T ParallelWrite_Error
*			- P_E_NOTREADY : The structure hasn't been initialized properly.
*			- P_E_INVALID_CS : The chip select chosen is not valid (refers to an innexisting CS?)
*			
* Example call : Parallel_Write(0x55, 0x01, CS2);
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
P_ERROR_T Parallel_Write(uint8_t Data, uint16_t Adress, P_CS_T ChipSelect)
{
	P_ERROR_T ParallelWrite_Error = P_NO_ERROR;
	
	/* Check if module is ready */
	if (Parallel_Struct.ready == 0xFFU)
	{
		uint8_t ParallelWrite_i;
		/* Activate the write signal and disable the read signal */
		/* These signals have inverted logic */
		if (Parallel_Struct.Read.used == true)
		{
			Parallel_WriteIO(&Parallel_Struct.Read, !PIN_LOW);
		}
		if (Parallel_Struct.Write.used == true)
		{
			Parallel_WriteIO(&Parallel_Struct.Write, !PIN_HIGH);
		}
		
		/* Write Data on the data bus */
		for (ParallelWrite_i = 0x00U; ParallelWrite_i < (sizeof(Parallel_Struct.Data) / sizeof(GPIO_T)); ParallelWrite_i++)
		{
			if (Parallel_Struct.Data[ParallelWrite_i].used == true)
			{
				Parallel_WriteIO(&Parallel_Struct.Data[ParallelWrite_i], ((Data >> ParallelWrite_i) & 0x01U));
			}
			else						/* We've reached bits that aren't used */
			{
				break;					/* Exit loop */
			}
		}
		
		/* Write Adress on the adress bus */
		for (ParallelWrite_i = 0x00U; ParallelWrite_i < (sizeof(Parallel_Struct.Adress) / sizeof(GPIO_T)); ParallelWrite_i++)
		{
			if (Parallel_Struct.Adress[ParallelWrite_i].used == true)
			{
				Parallel_WriteIO(&Parallel_Struct.Adress[ParallelWrite_i], ((Adress >> ParallelWrite_i) & 0x01U));
			}
			else						/* We've reached bits that aren't used */
			{
				break;					/* Exit loop */
			}
		}
		
		/* Drive the chip select pin high and disable other chip select pins */
		ParallelWrite_Error = Parallel_CS(ChipSelect, 1U);
	}
	else								/* Parallel_Struct.ready != 0xFF */
	{									/* The module hasn't been initialized properly */
		ParallelWrite_Error = P_E_NOTREADY;
	}
	
	return (ParallelWrite_Error);
}
/* --------------------------------------------------------------------------------------------- */
/* Private function definition */

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      Parallel_ResetData
* static void Parallel_ResetData(void)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-25
* 
* Reset the Data array within the Parallel_Struct structure.
* 
* Called by : Parallel_InitData()
* Function calls : none
*			
* Parameters : none
* Returns: none
*			
* Example call : Parallel_ResetData();
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static void Parallel_ResetData(void)
{
	uint8_t DataReset_i;
	for (DataReset_i = 0; DataReset_i < sizeof(Parallel_Struct.Data); DataReset_i++)
	{
		Parallel_Struct.Data[DataReset_i].Port = NULL;
		Parallel_Struct.Data[DataReset_i].Pin = 0x00U;
		Parallel_Struct.Data[DataReset_i].used = false;
		Parallel_Struct.Data[DataReset_i].state = false;
	}
}


/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      Parallel_ResetAdress
* static void Parallel_ResetAdress(void)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-25
* 
* Reset the Adress array within the Parallel_Struct structure.
* 
* Called by : Parallel_InitAdress()
* Function calls : none
*			
* Parameters : none
* Returns: none
*			
* Example call : Parallel_ResetAdress();
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static void Parallel_ResetAdress(void)
{
	uint8_t AdressReset_i;
	for (AdressReset_i = 0; AdressReset_i < sizeof(Parallel_Struct.Data); AdressReset_i++)
	{
		Parallel_Struct.Adress[AdressReset_i].Port = NULL;
		Parallel_Struct.Adress[AdressReset_i].Pin = 0x00U;
		Parallel_Struct.Adress[AdressReset_i].used = false;
		Parallel_Struct.Adress[AdressReset_i].state = false;
	}
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
/*                                      Parallel_ResetChipSelect
* static void Parallel_ResetChipSelect(void)
* Author : Pascal-Emmanuel Lachance & Charles Lévesque-Matte
* Date : 2019-04-25
* 
* Reset the Chip Select array within the Parallel_Struct structure.
* 
* Called by : Parallel_InitChipSelect()
* Function calls : none
*			
* Parameters : none
* Returns: none
*			
* Example call : Parallel_ResetChipSelect();
* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */
static void Parallel_ResetChipSelect(void)
{
	uint8_t CSReset_i;
	for (CSReset_i = 0; CSReset_i < sizeof(Parallel_Struct.Data); CSReset_i++)
	{
		Parallel_Struct.ChipSelect[CSReset_i].Port = NULL;
		Parallel_Struct.ChipSelect[CSReset_i].Pin = 0x00U;
		Parallel_Struct.ChipSelect[CSReset_i].used = false;
		Parallel_Struct.ChipSelect[CSReset_i].state = false;
	}
}