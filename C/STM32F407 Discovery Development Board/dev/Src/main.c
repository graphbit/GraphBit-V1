#define MISRAC_DISABLE _Pragma ("diag_suppress= \
    Pm001,Pm002,Pm003,Pm004,Pm005,Pm006,Pm007,Pm008,Pm009,Pm010,Pm011,\
    Pm012,Pm013,Pm014,Pm015,Pm016,Pm017,Pm018,Pm019,Pm020,Pm021,Pm022,\
    Pm023,Pm024,Pm025,Pm026,Pm027,Pm028,Pm029,Pm030,Pm031,Pm032,Pm033,\
    Pm034,Pm035,Pm036,Pm037,Pm038,Pm039,Pm040,Pm041,Pm042,Pm043,Pm044,\
    Pm045,Pm046,Pm047,Pm048,Pm049,Pm050,Pm051,Pm052,Pm053,Pm054,Pm055,\
    Pm056,Pm057,Pm058,Pm059,Pm060,Pm061,Pm062,Pm063,Pm064,Pm065,Pm066,\
    Pm067,Pm068,Pm069,Pm070,Pm071,Pm072,Pm073,Pm074,Pm075,Pm076,Pm077,\
    Pm078,Pm079,Pm080,Pm081,Pm082,Pm083,Pm084,Pm085,Pm086,Pm087,Pm088,\
    Pm089,Pm090,Pm091,Pm092,Pm093,Pm094,Pm095,Pm096,Pm097,Pm098,Pm099,\
    Pm100,Pm101,Pm102,Pm103,Pm104,Pm105,Pm106,Pm107,Pm108,Pm109,Pm110,\
    Pm111,Pm112,Pm113,Pm114,Pm115,Pm116,Pm117,Pm118,Pm119,Pm120,Pm121,\
    Pm122,Pm123,Pm124,Pm125,Pm126,Pm127,Pm128,Pm129,Pm130,Pm131,Pm132,\
    Pm133,Pm134,Pm135,Pm136,Pm137,Pm138,Pm139,Pm140,Pm141,Pm142,Pm143,\
    Pm144,Pm145,Pm146,Pm147,Pm148,Pm149,Pm150,Pm151,Pm152,Pm153,Pm154,\
    Pm155")

      #define MISRAC_ENABLE _Pragma ("diag_default= \
    Pm001,Pm002,Pm003,Pm004,Pm005,Pm006,Pm007,Pm008,Pm009,Pm010,Pm011,\
    Pm012,Pm013,Pm014,Pm015,Pm016,Pm017,Pm018,Pm019,Pm020,Pm021,Pm022,\
    Pm023,Pm024,Pm025,Pm026,Pm027,Pm028,Pm029,Pm030,Pm031,Pm032,Pm033,\
    Pm034,Pm035,Pm036,Pm037,Pm038,Pm039,Pm040,Pm041,Pm042,Pm043,Pm044,\
    Pm045,Pm046,Pm047,Pm048,Pm049,Pm050,Pm051,Pm052,Pm053,Pm054,Pm055,\
    Pm056,Pm057,Pm058,Pm059,Pm060,Pm061,Pm062,Pm063,Pm064,Pm065,Pm066,\
    Pm067,Pm068,Pm069,Pm070,Pm071,Pm072,Pm073,Pm074,Pm075,Pm076,Pm077,\
    Pm078,Pm079,Pm080,Pm081,Pm082,Pm083,Pm084,Pm085,Pm086,Pm087,Pm088,\
    Pm089,Pm090,Pm091,Pm092,Pm093,Pm094,Pm095,Pm096,Pm097,Pm098,Pm099,\
    Pm100,Pm101,Pm102,Pm103,Pm104,Pm105,Pm106,Pm107,Pm108,Pm109,Pm110,\
    Pm111,Pm112,Pm113,Pm114,Pm115,Pm116,Pm117,Pm118,Pm119,Pm120,Pm121,\
    Pm122,Pm123,Pm124,Pm125,Pm126,Pm127,Pm128,Pm129,Pm130,Pm131,Pm132,\
    Pm133,Pm134,Pm135,Pm136,Pm137,Pm138,Pm139,Pm140,Pm141,Pm142,Pm143,\
    Pm144,Pm145,Pm146,Pm147,Pm148,Pm149,Pm150,Pm151,Pm152,Pm153,Pm154,\
    Pm155")

MISRAC_DISABLE
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stdio.h"
MISRAC_ENABLE
#include "LCD.h"
#include "Parallel.h"
#include "Chat1.h"
#include "Chat2.h"
#include "Chat3.h"
#include "Chat4.h"
#include "Chat5.h"
#include "Chat6.h"
#include "Tunnel.h"
#include "StarWars.h"
#include "StarWarsText.h"
#include "Beer.h"
#include "Beer2.h"
#include "Chat1v2.c"
#include "GraphBit.h"
#include "MISRA.h"
	
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);

void fnChatStill(void);
void fnChatText(void);
void fnMISRA(void);
void fnTunnel(void);
void fnCopyPasta(void);
void fnStarWars(void);
void fnGraphBit(void);
void fnBeer(void);
void fnTSO(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
#define SIZEOF_FNARRAY  8
static uint8_t clear[4800];
int main(void)
{
  /* USER CODE BEGIN 1 */
  void (*fnArray[SIZEOF_FNARRAY])(void);
  
  fnArray[0] = fnGraphBit;
  fnArray[1] = fnMISRA;
  fnArray[2] = fnChatStill;
  fnArray[3] = fnChatText;
  fnArray[4] = fnTunnel;
  fnArray[5] = fnCopyPasta;
  fnArray[6] = fnStarWars;
  fnArray[7] = fnBeer;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  LCD_Init_Parallel(&htim2);
//LCD_Init_I2C(&hi2c1, &htim2);
//LCD_Init_UART(&huart2, &htim2);
  memset (clear, 0x00, sizeof(clear));
        
  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
 
//  memset(table, 0xF0, sizeof(table));

   LCD_Write(LCD_GRAPH, clear, sizeof(clear));
   HAL_Delay(1000);
//   LCD_Write(LCD_GRAPH, Beer2, sizeof(Beer2));
//   LCD_Write(LCD_GRAPH, &StarWarsText[1][0], (sizeof(StarWarsText)) / NOMBRE_STARWARSTEXT);
//   LCD_Write(LCD_GRAPH, &StarWarsLogo[0], sizeof(StarWarsLogo));
   uint8_t button = 0;
   uint8_t oldButton = 0;
   int i = 0;
   
#define COUNT 2000
  while (1)
  {
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
    button = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin == 1);
    if (oldButton != button)
    {
      oldButton = button;
      if (button == 1)
      {
        i++;
        HAL_Delay(100);
        if (i == SIZEOF_FNARRAY)
        {
          break;
        }
      }
    }
    fnArray[i]();
  }
  while(1);
  /* USER CODE END 3 */

}

///////////////////////////////////////////////////////////////////////////////

void fnChatStill(void)
{
  static bool written = false;
  if (written == false)
  {
        LCD_Write(LCD_GRAPH, Chat1v2, sizeof(Chat1v2));
        written = true;
  }
}

void fnChatText(void)
{
  static char charArray[] = "This is a cat!";
  static char chatArray[] = "Meow";
  HAL_Delay(200);
  LCD_Write(LCD_GRAPH, &Chat1[0], sizeof(Chat1));
  HAL_Delay(5);
  LCD_WriteText(chatArray, strlen(chatArray), 0x01U, 0x05U);
  HAL_Delay(5);
  LCD_WriteText(charArray, strlen(charArray), 15U, 0x00U);
  HAL_Delay(200);
  LCD_Write(LCD_GRAPH, &Chat2[0], sizeof(Chat2));
  HAL_Delay(5);
  LCD_WriteText(chatArray, strlen(chatArray), 0x01U, 0x05U);
  HAL_Delay(5);
  LCD_WriteText(charArray, strlen(charArray), 15U, 0x00U);
  HAL_Delay(200);
  LCD_Write(LCD_GRAPH, &chat3[0], sizeof(chat3));
  HAL_Delay(5);
  LCD_WriteText(chatArray, strlen(chatArray), 0x01U, 0x05U);
  HAL_Delay(5);
  LCD_WriteText(charArray, strlen(charArray), 15U, 0x00U);
  HAL_Delay(200);
  LCD_Write(LCD_GRAPH, &chat4[0], sizeof(chat4));
  HAL_Delay(5);
  LCD_WriteText(chatArray, strlen(chatArray), 0x01U, 0x05U);
  HAL_Delay(5);
  LCD_WriteText(charArray, strlen(charArray), 15U, 0x00U);
  HAL_Delay(200);
  LCD_Write(LCD_GRAPH, &chat5[0], sizeof(chat5));
  HAL_Delay(5);
  LCD_WriteText(chatArray, strlen(chatArray), 0x01U, 0x05U);
  HAL_Delay(5);
  LCD_WriteText(charArray, strlen(charArray), 15U, 0x00U);
  HAL_Delay(200);
  LCD_Write(LCD_GRAPH, &chat6[0], sizeof(chat6));
  HAL_Delay(5);
  LCD_WriteText(chatArray, strlen(chatArray), 0x01U, 0x05U);
  HAL_Delay(5);
  LCD_WriteText(charArray, strlen(charArray), 15U, 0x00U);
}

void fnTunnel(void)
{
  static int i = 0;
  LCD_Write(LCD_GRAPH, &Tunnel[i][0], (sizeof(Tunnel)/ NOMBREIMAGE_TUNNEL));
  HAL_Delay(250);
  i++;
  if (i >= NOMBREIMAGE_TUNNEL) i = 0;
}

void fnCopyPasta(void)
{
  static char copyPasta[] = " The FitnessGram Pacer Test is a multistage aerobic capacity test that gets more difficult as it continues.\nThe 20 meter pacer test will begin in 30 seconds. Line up at the start.\nThe running speed starts slowly, but gets faster each minute after you hear this signal.\n[beep] A single lap should be completed each time you hear this sound. [ding]\nRemember to run in a straight line, and run as long as possible. The second time you fail to complete a lap before the sound, your test is over.";
  static bool written = false;
  if (written == false)
  {
    written = true;
    LCD_Write(LCD_GRAPH, clear, sizeof(clear));
    HAL_Delay(100);
    LCD_WriteText(copyPasta, strlen(copyPasta), 0x01U, 0x00U);
  }
}

void fnStarWars(void)
{
  static int i = 0;
  LCD_Write(LCD_GRAPH, &StarWars[i][0], (sizeof(StarWars)/ NOMBREIMAGE_STARS));
  HAL_Delay(250);
  i++;
  if (i >= NOMBREIMAGE_STARS) i = 0;
}

void fnMISRA(void)
{
  static bool written = false;
  if (written == false)
  {
        LCD_Write(LCD_GRAPH, MISRA, sizeof(MISRA));
        written = true;
  }
}

void fnGraphBit(void)
{
  static bool written = false;
  if (written == false)
  {
        LCD_Write(LCD_GRAPH, GraphBit, sizeof(GraphBit));
        written = true;
  }
}

void fnBeer(void)
{
  static bool written = false;
  static char retraite[] = "BONNE\nRETRAITE\nDANIEL!";
  if (written == false)
  {
        LCD_Write(LCD_GRAPH, Beer2, sizeof(Beer2));
        written = true;
        LCD_WriteText(retraite, strlen(retraite), 0x01U, 0x15U);
  }
}

///////////////////////////////////////////////////////////////////////////////

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchronization(&htim2, &sSlaveConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 19200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PC3   ------> I2S2_SD
     PA4   ------> I2S3_WS
     PB10   ------> I2S2_CK
     PC7   ------> I2S3_MCK
     PC10   ------> I2S3_CK
     PC12   ------> I2S3_SD
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, SPI_CS_0_Pin|CS_I2C_SPI_Pin|SPI_CS_2_Pin|SPI_CS_3_Pin 
                          |SPI_CS_4_Pin|PX5_Pin|INT2309_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, OTG_FS_PowerSwitchOn_Pin|PX7_Pin|PX6_Pin|PX4_Pin 
                          |PX3_Pin|PX2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RD_Pin|A4_Pin|CS0_Pin|CS1_Pin 
                          |CS2_Pin|CS3_Pin|CONVERT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, A0_Pin|A1_Pin|A2_Pin|A3_Pin 
                          |LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin 
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PX1_Pin|PX0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SPI_CS_0_Pin CS_I2C_SPI_Pin SPI_CS_2_Pin SPI_CS_3_Pin 
                           SPI_CS_4_Pin PX5_Pin INT2309_Pin */
  GPIO_InitStruct.Pin = SPI_CS_0_Pin|CS_I2C_SPI_Pin|SPI_CS_2_Pin|SPI_CS_3_Pin 
                          |SPI_CS_4_Pin|PX5_Pin|INT2309_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : EOC2544_Pin PY0_Pin */
  GPIO_InitStruct.Pin = EOC2544_Pin|PY0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_PowerSwitchOn_Pin PX7_Pin PX6_Pin PX4_Pin 
                           PX3_Pin PX2_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin|PX7_Pin|PX6_Pin|PX4_Pin 
                          |PX3_Pin|PX2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PY7_Pin */
  GPIO_InitStruct.Pin = PY7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PY7_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : I2S3_WS_Pin */
  GPIO_InitStruct.Pin = I2S3_WS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(I2S3_WS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RD_Pin A4_Pin CS0_Pin CS1_Pin 
                           CS2_Pin CS3_Pin CONVERT_Pin */
  GPIO_InitStruct.Pin = RD_Pin|A4_Pin|CS0_Pin|CS1_Pin 
                          |CS2_Pin|CS3_Pin|CONVERT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BOOT1_Pin PY5_Pin PY6_Pin INT8574_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin|PY5_Pin|PY6_Pin|INT8574_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : D0_Pin D1_Pin D2_Pin D3_Pin 
                           D4_Pin D5_Pin D6_Pin D7_Pin */
  GPIO_InitStruct.Pin = D0_Pin|D1_Pin|D2_Pin|D3_Pin 
                          |D4_Pin|D5_Pin|D6_Pin|D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : A0_Pin A1_Pin A2_Pin A3_Pin 
                           LD4_Pin LD3_Pin LD5_Pin LD6_Pin 
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = A0_Pin|A1_Pin|A2_Pin|A3_Pin 
                          |LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin 
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_MCK_Pin I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PX1_Pin PX0_Pin */
  GPIO_InitStruct.Pin = PX1_Pin|PX0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PY1_Pin PY2_Pin OTG_FS_OverCurrent_Pin PY3_Pin 
                           PY4_Pin */
  GPIO_InitStruct.Pin = PY1_Pin|PY2_Pin|OTG_FS_OverCurrent_Pin|PY3_Pin 
                          |PY4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : INT_KEY_Pin */
  GPIO_InitStruct.Pin = INT_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INT_KEY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
