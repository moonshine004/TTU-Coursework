/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : FreeRTOS + TouchGFX WAV audio player
  ******************************************************************************
  *
  * Final Project
  * Author: Amna Malik
  * R#: R11827604
  *
  * Project Summary:
  * This program makes the STM32F429 act as a WAV audio player. WAV files are
  * scanned from the microSD card, shown in a TouchGFX scroll list, and played
  * through DAC channel 2 using DMA. The GUI and UART CLI send commands through
  * a FreeRTOS queue so playback control stays separated from the screen code.
  *
  * Main features:
  * - SD card WAV scan
  * - TouchGFX song list and playback screen
  * - DAC + DMA audio streaming
  * - Pause, resume, stop, next, previous, rewind, fast forward
  * - Shuffle mode with no repeat until all songs play
  * - UART debug / CLI commands
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "app_touchgfx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

// LCD driver used by the STM32F429 discovery display
#include "Components/ili9341/ili9341.h"

// standard C helpers used for strings, debug text, and CLI parsing
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/*
 * Parsed WAV file information.
 * These values come from the WAV header and are used to set up
 * the timer rate, track length, and buffer math.
 */
typedef struct {
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    uint32_t dataSize;
    uint32_t dataStart;
} wavifo_t;

/*
 * Audio command message sent through the FreeRTOS queue.
 * GUI buttons and UART commands both use this format.
 */
typedef struct {
    uint8_t  cmd;
    int32_t  param;
    char     filename[64];
} AudioCmd_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// SDRAM settings used by the external frame buffer memory
#define REFRESH_COUNT           ((uint32_t)1386)
#define SDRAM_TIMEOUT           ((uint32_t)0xFFFF)

/**
  * @brief  FMC SDRAM Mode definition register defines
  */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

// peripheral timeout values
#define I2C3_TIMEOUT_MAX                    0x3000
#define SPI5_TIMEOUT_MAX                    0x1000

// audio buffer sizes
#define AUDIOBUFLEN   1024
#define HALFBUFLEN    (AUDIOBUFLEN/2)
// CLI/file limits
#define CMDMAXLEN     128
#define FREADBUFLEN   4096
#define MAX_SONGS     32

/* Audio command codes */
#define cmdplay    1
#define cmdpauz    2
#define cmdresume  3
#define cmdfwd     4
#define cmdrew     5
#define cmdlst     6

/* NEW command codes for final project */
#define cmdstop    7
#define cmdnext    8
#define cmdprev    9
#define cmdshufl   10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac2;

DMA2D_HandleTypeDef hdma2d;

I2C_HandleTypeDef hi2c3;

LTDC_HandleTypeDef hltdc;

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio;

SPI_HandleTypeDef hspi5;

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart1;

SDRAM_HandleTypeDef hsdram1;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Definitions for GUI task */
osThreadId_t GUI_TaskHandle;
const osThreadAttr_t GUI_Task_attributes = {
  .name = "GUI_Task",
  .stack_size = 8192 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Definitions for audio playback task */
osThreadId_t audiotskHandle;
const osThreadAttr_t audiotsk_attributes = {
  .name = "audiotsk",
  .stack_size = 1536 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Definitions for SD monitor task */
osThreadId_t sdmttskHandle;
const osThreadAttr_t sdmttsk_attributes = {
  .name = "sdmttsk",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* UART RX queue */
osMessageQueueId_t rxQHandle;
const osMessageQueueAttr_t rxQ_attributes = {
  .name = "rxQ"
};
/* UART transmit semaphore */
osSemaphoreId_t uartHandle;
const osSemaphoreAttr_t uart_attributes = {
  .name = "uart"
};
/* DMA first-half buffer semaphore */
osSemaphoreId_t dma1Handle;
const osSemaphoreAttr_t dma1_attributes = {
  .name = "dma1"
};
/* DMA second-half buffer semaphore */
osSemaphoreId_t dma2Handle;
const osSemaphoreAttr_t dma2_attributes = {
  .name = "dma2"
};
/* SD card semaphore */
osSemaphoreId_t sdHandle;
const osSemaphoreAttr_t sd_attributes = {
  .name = "sd"
};
/* USER CODE BEGIN PV */
// board revision flag used for touch input setup
uint8_t isRevD = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CRC_Init(void);
static void MX_I2C3_Init(void);
static void MX_SPI5_Init(void);
static void MX_FMC_Init(void);
static void MX_LTDC_Init(void);
static void MX_DMA2D_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM10_Init(void);
void StartDefaultTask(void *argument);
extern void TouchGFX_Task(void *argument);
void StartTask03(void *argument);
void StartTask04(void *argument);

/* USER CODE BEGIN PFP */
static void BSP_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command);



static uint8_t            I2C3_ReadData(uint8_t Addr, uint8_t Reg);
static void               I2C3_WriteData(uint8_t Addr, uint8_t Reg, uint8_t Value);
static uint8_t            I2C3_ReadBuffer(uint8_t Addr, uint8_t Reg, uint8_t *pBuffer, uint16_t Length);

/* SPIx bus function */
static void               SPI5_Write(uint16_t Value);
static uint32_t           SPI5_Read(uint8_t ReadSize);
static void               SPI5_Error(void);

/* Link function for LCD peripheral */
void                      LCD_IO_Init(void);
void                      LCD_IO_WriteData(uint16_t RegValue);
void                      LCD_IO_WriteReg(uint8_t Reg);
uint32_t                  LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);
void                      LCD_Delay(uint32_t delay);

/* IOExpander IO functions */
void                      IOE_Init(void);
void                      IOE_ITConfig(void);
void                      IOE_Delay(uint32_t Delay);
void                      IOE_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t                   IOE_Read(uint8_t Addr, uint8_t Reg);
uint16_t                  IOE_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *pBuffer, uint16_t Length);


/* -------------------- AUDIO PLAYBACK STATE -------------------- */

// audio DMA buffers are static so they do not use task stack space
static uint16_t audioBuf[AUDIOBUFLEN];
static uint8_t  freadBuf[FREADBUFLEN];

// current WAV file and parsed header info
static FIL       wavF;
static wavifo_t  wavifo;
static volatile uint8_t  playing = 0;
static volatile uint8_t  paused  = 0;
static volatile uint8_t  fileopn = 0;
static uint32_t  datpos  = 0;
static volatile uint8_t   mounted = 0;

/* -------------------- UART CLI STATE -------------------- */

// UART receive state
static uint8_t  rxByte;
static char     cmdLine[CMDMAXLEN];
static uint8_t  cmdIdx = 0;


/* -------------------- AUDIO COMMAND QUEUE -------------------- */

osMessageQueueId_t audioCmdQ;

/* -------------------- SONG LIST / GUI STATE -------------------- */

char songNames[MAX_SONGS][64];
volatile uint8_t  numSongs      = 0;
volatile uint8_t  fileListReady = 0;
volatile uint32_t curTimeSec    = 0;
volatile uint32_t totTimeSec    = 0;
char curSongName[64];
volatile uint8_t  sdRemoved     = 0;
volatile uint8_t  shuffleOn     = 0;
volatile uint8_t  isPlaying     = 0;
volatile uint8_t  isPaused      = 0;


static int16_t  curSongIdx = -1;
static int16_t  shuffleOrder[MAX_SONGS];
static uint8_t  shufflePos = 0;


static void    stpplay(void);
static void    scnsd(void);
static void    shflst(void);
static int16_t getnxt(void);
static int16_t getprv(void);
/* USER CODE END PFP */


/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static LCD_DrvTypeDef* LcdDrv;

uint32_t I2c3Timeout = I2C3_TIMEOUT_MAX;
uint32_t Spi5Timeout = SPI5_TIMEOUT_MAX;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  // reset peripherals and initialize HAL
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  // configure system clock
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  // initialize board peripherals
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CRC_Init();
  MX_I2C3_Init();
  MX_SPI5_Init();
  MX_FMC_Init();
  MX_LTDC_Init();
  MX_DMA2D_Init();
  MX_SDIO_SD_Init();
  MX_USART1_UART_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_TIM10_Init();
  MX_FATFS_Init();
  MX_TouchGFX_Init();

  // TouchGFX setup before the scheduler starts
  MX_TouchGFX_PreOSInit();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  // start RTOS setup
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  // UART print semaphore
  uartHandle = osSemaphoreNew(1, 1, &uart_attributes);

  // DMA semaphores used by DAC half/full callbacks
  dma1Handle = osSemaphoreNew(1, 1, &dma1_attributes);
  dma2Handle = osSemaphoreNew(1, 1, &dma2_attributes);

  // SD semaphore kept for SD-related access
  sdHandle = osSemaphoreNew(1, 1, &sd_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  // start DMA semaphores empty so callbacks control refills
  osSemaphoreAcquire(dma1Handle, 0);
  osSemaphoreAcquire(dma2Handle, 0);
  /* USER CODE END RTOS_SEMAPHORES */


  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  // UART receive queue
  rxQHandle = osMessageQueueNew (16, sizeof(uint16_t), &rxQ_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  // audio command queue used by GUI and CLI controls
  audioCmdQ = osMessageQueueNew(8, sizeof(AudioCmd_t), NULL);
  /* USER CODE END RTOS_QUEUES */

  // default background task
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  // TouchGFX GUI task
  GUI_TaskHandle = osThreadNew(TouchGFX_Task, NULL, &GUI_Task_attributes);

  // WAV playback task
  audiotskHandle = osThreadNew(StartTask03, NULL, &audiotsk_attributes);

  // SD card monitor task
  sdmttskHandle = osThreadNew(StartTask04, NULL, &sdmttsk_attributes);


  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
  HAL_UART_Transmit(&huart1, (uint8_t*)"PRE-SCHED\r\n", 11, 1000);
    if (audiotskHandle == NULL) {
        HAL_UART_Transmit(&huart1, (uint8_t*)"AUD=NULL\r\n", 10, 1000);
    } else {
        HAL_UART_Transmit(&huart1, (uint8_t*)"AUD=OK\r\n", 8, 1000);
    }

    /* Start scheduler */
    osKernelStart();
  /* Start scheduler */


  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_DISABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 9;
  hltdc.Init.VerticalSync = 1;
  hltdc.Init.AccumulatedHBP = 29;
  hltdc.Init.AccumulatedVBP = 3;
  hltdc.Init.AccumulatedActiveW = 269;
  hltdc.Init.AccumulatedActiveH = 323;
  hltdc.Init.TotalWidth = 279;
  hltdc.Init.TotalHeigh = 327;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 240;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 320;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 240;
  pLayerCfg.ImageHeight = 320;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */
    /*Select the device */
  LcdDrv = &ili9341_drv;
  LcdDrv->Init();

  LcdDrv->DisplayOn();
  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 40;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}

/**
  * @brief SPI5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI5_Init(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  /* SPI5 parameter configuration*/
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI5_Init 2 */
  // Check if the board has the old or new revision of the gyroscope
  // This tells if the board is revision D or newer
  // It is used to handle the touch input correctly
  const uint8_t READ_ID_CMD = 0x8F; // 0b10001111 = set read bit and register address of WHO_AM_I
  uint8_t pdata = 0;
  HAL_GPIO_WritePin(SPI5_NCS_GPIO_Port, SPI5_NCS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi5, &READ_ID_CMD, 1, 1000);
  HAL_SPI_Receive(&hspi5, &pdata, 1, 1000);
  HAL_GPIO_WritePin(SPI5_NCS_GPIO_Port, SPI5_NCS_Pin, GPIO_PIN_SET);
  if (pdata == 0xD3) // 0b11010011
  {
    isRevD = 1;
  }
  /* USER CODE END SPI5_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 0;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */
  htim6.Init.Prescaler = 0;
  htim6.Init.Period    = (HAL_RCC_GetPCLK1Freq() * 2 / 44100) - 1;
  HAL_TIM_Base_Init(&htim6);

  TIM_MasterConfigTypeDef mc6 = {0};
  mc6.MasterOutputTrigger = TIM_TRGO_UPDATE;
  mc6.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim6, &mc6);
  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 17999;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 9999;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 3;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  FMC_SDRAM_CommandTypeDef command;

  /* Program the SDRAM external device */
  BSP_SDRAM_Initialization_Sequence(&hsdram1, &command);
  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, VSYNC_FREQ_Pin|RENDER_TIME_Pin|FRAME_RATE_Pin|MCU_ACTIVE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI5_NCS_GPIO_Port, SPI5_NCS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pins : VSYNC_FREQ_Pin RENDER_TIME_Pin FRAME_RATE_Pin MCU_ACTIVE_Pin */
  GPIO_InitStruct.Pin = VSYNC_FREQ_Pin|RENDER_TIME_Pin|FRAME_RATE_Pin|MCU_ACTIVE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI5_NCS_Pin */
  GPIO_InitStruct.Pin = SPI5_NCS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI5_NCS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : sddet_Pin */
  GPIO_InitStruct.Pin = sddet_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(sddet_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* -------------------- BOARD / LCD SUPPORT FUNCTIONS -------------------- */
/**
  * @brief  Perform the SDRAM external memory initialization sequence
  * @param  hsdram: SDRAM handle
  * @param  Command: Pointer to SDRAM command structure
  * @retval None
  */
static void BSP_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
 __IO uint32_t tmpmrd =0;

  /* Step 1:  Configure a clock configuration enable command */
  Command->CommandMode             = FMC_SDRAM_CMD_CLK_ENABLE;
  Command->CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  Command->AutoRefreshNumber       = 1;
  Command->ModeRegisterDefinition  = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 2: Insert 100 us minimum delay */
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);

  /* Step 3: Configure a PALL (precharge all) command */
  Command->CommandMode             = FMC_SDRAM_CMD_PALL;
  Command->CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  Command->AutoRefreshNumber       = 1;
  Command->ModeRegisterDefinition  = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 4: Configure an Auto Refresh command */
  Command->CommandMode             = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command->CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  Command->AutoRefreshNumber       = 4;
  Command->ModeRegisterDefinition  = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 5: Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                     SDRAM_MODEREG_CAS_LATENCY_3           |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  Command->CommandMode             = FMC_SDRAM_CMD_LOAD_MODE;
  Command->CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  Command->AutoRefreshNumber       = 1;
  Command->ModeRegisterDefinition  = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT);
}

/**
  * @brief  IOE Low Level Initialization.
  */
void IOE_Init(void)
{
  //Dummy function called when initializing to stmpe811 to setup the i2c.
  //This is done with cubmx and is therefore not done here.
}

/**
  * @brief  IOE Low Level Interrupt configuration.
  */
void IOE_ITConfig(void)
{
  //Dummy function called when initializing to stmpe811 to setup interrupt for the i2c.
  //The interrupt is not used in our case, therefore nothing is done here.
}

/**
  * @brief  IOE Writes single data operation.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address
  * @param  Value: Data to be written
  */
void IOE_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  I2C3_WriteData(Addr, Reg, Value);
}

/**
  * @brief  IOE Reads single data.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address
  * @retval The read data
  */
uint8_t IOE_Read(uint8_t Addr, uint8_t Reg)
{
  return I2C3_ReadData(Addr, Reg);
}

/**
  * @brief  IOE Reads multiple data.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address
  * @param  pBuffer: pointer to data buffer
  * @param  Length: length of the data
  * @retval 0 if no problems to read multiple data
  */
uint16_t IOE_ReadMultiple(uint8_t Addr, uint8_t Reg, uint8_t *pBuffer, uint16_t Length)
{
 return I2C3_ReadBuffer(Addr, Reg, pBuffer, Length);
}

/**
  * @brief  IOE Delay.
  * @param  Delay in ms
  */
void IOE_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/**
  * @brief  Writes a value in a register of the device through BUS.
  * @param  Addr: Device address on BUS Bus.
  * @param  Reg: The target register address to write
  * @param  Value: The target register value to be written
  */
static void I2C3_WriteData(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Write(&hi2c3, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, I2c3Timeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initialize the BUS */
    //I2Cx_Error();
  }
}

/**
  * @brief  Reads a register of the device through BUS.
  * @param  Addr: Device address on BUS Bus.
  * @param  Reg: The target register address to write
  * @retval Data read at register address
  */
static uint8_t I2C3_ReadData(uint8_t Addr, uint8_t Reg)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t value = 0;

  status = HAL_I2C_Mem_Read(&hi2c3, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &value, 1, I2c3Timeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initialize the BUS */
    //I2Cx_Error();

  }
  return value;
}

/**
  * @brief  Reads multiple data on the BUS.
  * @param  Addr: I2C Address
  * @param  Reg: Reg Address
  * @param  pBuffer: pointer to read data buffer
  * @param  Length: length of the data
  * @retval 0 if no problems to read multiple data
  */
static uint8_t I2C3_ReadBuffer(uint8_t Addr, uint8_t Reg, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Read(&hi2c3, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, pBuffer, Length, I2c3Timeout);

  /* Check the communication status */
  if(status == HAL_OK)
  {
    return 0;
  }
  else
  {
    /* Re-Initialize the BUS */
    //I2Cx_Error();

    return 1;
  }
}

/**
  * @brief  Reads 4 bytes from device.
  * @param  ReadSize: Number of bytes to read (max 4 bytes)
  * @retval Value read on the SPI
  */
static uint32_t SPI5_Read(uint8_t ReadSize)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t readvalue;

  status = HAL_SPI_Receive(&hspi5, (uint8_t*) &readvalue, ReadSize, Spi5Timeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initialize the BUS */
    SPI5_Error();
  }

  return readvalue;
}

/**
  * @brief  Writes a byte to device.
  * @param  Value: value to be written
  */
static void SPI5_Write(uint16_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&hspi5, (uint8_t*) &Value, 1, Spi5Timeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initialize the BUS */
    SPI5_Error();
  }
}

/**
  * @brief  SPI5 error treatment function.
  */
static void SPI5_Error(void)
{
  /* De-initialize the SPI communication BUS */
  //HAL_SPI_DeInit(&SpiHandle);

  /* Re- Initialize the SPI communication BUS */
  //SPIx_Init();
}

void LCD_IO_Init(void)
{
  /* Set or Reset the control line */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
}

/**
  * @brief  Writes register value.
  */
void LCD_IO_WriteData(uint16_t RegValue)
{
  /* Set WRX to send data */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);

  /* Reset LCD control line(/CS) and Send data */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
  SPI5_Write(RegValue);

  /* Deselect: Chip Select high */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
}

/**
  * @brief  Writes register address.
  */
void LCD_IO_WriteReg(uint8_t Reg)
{
  /* Reset WRX to send command */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);

  /* Reset LCD control line(/CS) and Send command */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
  SPI5_Write(Reg);

  /* Deselect: Chip Select high */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
}

/**
  * @brief  Reads register value.
  * @param  RegValue Address of the register to read
  * @param  ReadSize Number of bytes to read
  * @retval Content of the register value
  */
uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize)
{
  uint32_t readvalue = 0;

  /* Select: Chip Select low */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);

  /* Reset WRX to send command */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);

  SPI5_Write(RegValue);

  readvalue = SPI5_Read(ReadSize);

  /* Set WRX to send data */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);

  /* Deselect: Chip Select high */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

  return readvalue;
}

/**
  * @brief  Wait for loop in ms.
  * @param  Delay in ms.
  */
void LCD_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}




/* -------------------- UART / DEBUG HELPERS -------------------- */

// UART print helper protected by a semaphore
static void txstr(const char *s)
{
    osSemaphoreAcquire(uartHandle, osWaitForever);
    HAL_UART_Transmit(&huart1, (uint8_t *)s, strlen(s), HAL_MAX_DELAY);
    osSemaphoreRelease(uartHandle);
}

void gui_dbg(const char *s)
{
    txstr(s);
}

/* -------------------- WAV FILE HELPERS -------------------- */

// reads WAV header info
static int parsewav(FIL *f, wavifo_t *info)
{
    uint8_t hdr[44];
    UINT br;

    f_lseek(f, 0);
    f_read(f, hdr, 44, &br);
    if (br < 44) return -1;

    // check wav tags
    if (hdr[0]!='R'||hdr[1]!='I'||hdr[2]!='F'||hdr[3]!='F') return -1;
    if (hdr[8]!='W'||hdr[9]!='A'||hdr[10]!='V'||hdr[11]!='E') return -1;

    // get wav format info
    info->audioFormat   = hdr[20] | (hdr[21]<<8);
    info->numChannels   = hdr[22] | (hdr[23]<<8);
    info->sampleRate    = hdr[24]|(hdr[25]<<8)|(hdr[26]<<16)|(hdr[27]<<24);
    info->byteRate      = hdr[28]|(hdr[29]<<8)|(hdr[30]<<16)|(hdr[31]<<24);
    info->blockAlign    = hdr[32] | (hdr[33]<<8);
    info->bitsPerSample = hdr[34] | (hdr[35]<<8);

    // find data chunk
    uint32_t pos = 36;
    while (pos < 500) {
        uint8_t chk[8];
        f_lseek(f, pos);
        f_read(f, chk, 8, &br);
        if (br < 8) return -1;
        if (chk[0]=='d'&&chk[1]=='a'&&chk[2]=='t'&&chk[3]=='a') {
            info->dataSize  = chk[4]|(chk[5]<<8)|(chk[6]<<16)|(chk[7]<<24);
            info->dataStart = pos + 8;
            f_lseek(f, info->dataStart);
            return 0;
        }
        uint32_t clen = chk[4]|(chk[5]<<8)|(chk[6]<<16)|(chk[7]<<24);
        pos += 8 + clen;
    }
    return -1;
}

// fills one section of the DAC buffer from the active WAV file
static void fillbuffa(uint16_t *buf, uint32_t len)
{
    UINT br;
    uint32_t bps   = wavifo.bitsPerSample / 8;
    uint32_t frame = bps * wavifo.numChannels;
    uint32_t need  = len * frame;

    // don't read past the song
    uint32_t remain = wavifo.dataSize - datpos;
    if (need > remain) need = remain;
    if (need > FREADBUFLEN) need = FREADBUFLEN;

    f_read(&wavF, freadBuf, need, &br);
    datpos += br;

    uint32_t frames = br / frame;

    for (uint32_t i = 0; i < frames; i++) {
        int32_t sample;
        if (bps == 2) {
            // 16 bit audio
            int16_t *src = (int16_t *)&freadBuf[i * frame];
            sample = src[0];
            sample = (sample + 32768) >> 4;
        } else {
            // 8 bit audio
            sample = freadBuf[i * frame];
            sample = sample << 4;
        }
        buf[i] = (uint16_t)sample;
    }

    // fill the rest with silence
    for (uint32_t i = frames; i < len; i++) {
        buf[i] = 2048;
    }
}

/* -------------------- DMA AND UART CALLBACKS -------------------- */

// first half of DMA buffer finished
void HAL_DACEx_ConvHalfCpltCallbackCh2(DAC_HandleTypeDef *hdac_ptr)
{
    osSemaphoreRelease(dma1Handle);
}

// second half of DMA buffer finished
void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef *hdac_ptr)
{
    osSemaphoreRelease(dma2Handle);
}

// UART byte received, then pushed into the CLI queue
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        uint16_t ch = rxByte;
        osMessageQueuePut(rxQHandle, &ch, 0, 0);
        HAL_UART_Receive_IT(&huart1, &rxByte, 1);
    }
}

/* -------------------- PLAYER STATE HELPERS -------------------- */

// stops playback and resets audio state
static void stpplay(void)
{
    HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_2);
    HAL_TIM_Base_Stop(&htim6);
    if (fileopn) {
        f_close(&wavF);
        fileopn = 0;
    }
    playing   = 0;   paused   = 0;
    isPlaying = 0;   isPaused = 0;
    datpos    = 0;   curTimeSec = 0;
}

// scans SD card root folder for WAV files
static void scnsd(void)
{
    DIR dir;
    FILINFO fno;
    FRESULT fr;
    char msg[100];

    numSongs = 0;
    fileListReady = 0;

    fr = f_opendir(&dir, "/");

    if (fr != FR_OK)
    {
        snprintf(msg, sizeof(msg), "SD scan failed: %d\r\n", fr);
        txstr(msg);
        return;
    }

    txstr("Refreshing WAV list...\r\n");

    while (numSongs < MAX_SONGS)
    {
        fr = f_readdir(&dir, &fno);

        if (fr != FR_OK)
        {
            snprintf(msg, sizeof(msg), "SD read failed: %d\r\n", fr);
            txstr(msg);
            break;
        }

        if (fno.fname[0] == '\0')
        {
            break;
        }

        if (fno.fattrib & (AM_HID | AM_SYS | AM_DIR))
        {
            continue;
        }

        if (fno.fname[0] == '.')
        {
            continue;
        }

        size_t len = strlen(fno.fname);

        if (len < 5)
        {
            continue;
        }

        char *ext = &fno.fname[len - 4];

        if (ext[0] != '.')
        {
            continue;
        }

        if ((ext[1] != 'w' && ext[1] != 'W') ||
            (ext[2] != 'a' && ext[2] != 'A') ||
            (ext[3] != 'v' && ext[3] != 'V'))
        {
            continue;
        }

        strncpy(songNames[numSongs], fno.fname, 63);
        songNames[numSongs][63] = '\0';

        snprintf(msg, sizeof(msg),
                 "WAV %d loaded - %.60s\r\n",
                 numSongs,
                 songNames[numSongs]);
        txstr(msg);

        numSongs++;
    }

    f_closedir(&dir);

    snprintf(msg, sizeof(msg),
             "WAV files found - %d total\r\n",
             numSongs);
    txstr(msg);

    fileListReady = 1;
}

// shuffles song order
static void shflst(void)
{
    for (uint8_t i = 0; i < numSongs; i++)
        shuffleOrder[i] = i;

    for (int i = numSongs - 1; i > 0; i--) {
        int j = (int)((HAL_GetTick() * (uint32_t)(i + 1) * 1103515245u + 12345u) % (uint32_t)(i + 1));
        int16_t tmp      = shuffleOrder[i];
        shuffleOrder[i]  = shuffleOrder[j];
        shuffleOrder[j]  = tmp;
    }

    /* move current song to position 0 so it counts as "already played" */
    if (curSongIdx >= 0) {
        for (uint8_t i = 0; i < numSongs; i++) {
            if (shuffleOrder[i] == curSongIdx) {
                shuffleOrder[i] = shuffleOrder[0];
                shuffleOrder[0] = curSongIdx;
                break;
            }
        }
    }
    shufflePos = 0;
}

// gets next song index
static int16_t getnxt(void)
{
    if (numSongs == 0) return -1;
    if (shuffleOn) {
        shufflePos++;
        if (shufflePos >= numSongs) {
            shflst();
            shufflePos = 0;
        }
        return shuffleOrder[shufflePos];
    }
    return (curSongIdx + 1) % numSongs;
}

// gets previous song index
static int16_t getprv(void)
{
    if (numSongs == 0) return -1;
    if (shuffleOn) {
        if (shufflePos == 0) shufflePos = numSongs;
        shufflePos--;
        return shuffleOrder[shufflePos];
    }
    return (curSongIdx - 1 + numSongs) % numSongs;
}


#define SD_PRESENT_LEVEL GPIO_PIN_SET

static uint8_t sdcard_present(void)
{
    return HAL_GPIO_ReadPin(sddet_GPIO_Port, sddet_Pin) == SD_PRESENT_LEVEL;
}
/* USER CODE END 4 */

/* -------------------- FREERTOS TASKS -------------------- */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    // start UART interrupt receive for CLI input
    HAL_UART_Receive_IT(&huart1, &rxByte, 1);

    txstr("\r\n=== WAV Player Ready ===\r\n> ");

    for (;;) {
        uint16_t ch;
        if (osMessageQueueGet(rxQHandle, &ch, NULL, osWaitForever) != osOK)
            continue;

        char c = (char)ch;
        HAL_UART_Transmit(&huart1, (uint8_t *)&c, 1, HAL_MAX_DELAY);

        if (c == '\r' || c == '\n') {
            // command finished, parse the line
            cmdLine[cmdIdx] = '\0';
            txstr("\r\n");

            if (cmdIdx > 0) {
                AudioCmd_t ac = {0};

                if (strcmp(cmdLine, "/list") == 0) {
                    ac.cmd = cmdlst;
                    osMessageQueuePut(audioCmdQ, &ac, 0, 0);
                }
                else if (strncmp(cmdLine, "/play ", 6) == 0) {
                    ac.cmd   = cmdplay;
                    ac.param = atoi(&cmdLine[6]);
                    osMessageQueuePut(audioCmdQ, &ac, 0, 0);
                }
                else if (strcmp(cmdLine, "/pause") == 0) {
                    ac.cmd = cmdpauz;
                    osMessageQueuePut(audioCmdQ, &ac, 0, 0);
                }
                else if (strcmp(cmdLine, "/resume") == 0) {
                    ac.cmd = cmdresume;
                    osMessageQueuePut(audioCmdQ, &ac, 0, 0);
                }
                else if (strcmp(cmdLine, "/fwd") == 0) {
                    ac.cmd = cmdfwd;  ac.param = 15;
                    osMessageQueuePut(audioCmdQ, &ac, 0, 0);
                }
                else if (strcmp(cmdLine, "/rew") == 0) {
                    ac.cmd = cmdrew;  ac.param = 15;
                    osMessageQueuePut(audioCmdQ, &ac, 0, 0);
                }
                // extra cli commands
                else if (strcmp(cmdLine, "/stop") == 0) {
                    ac.cmd = cmdstop;
                    osMessageQueuePut(audioCmdQ, &ac, 0, 0);
                }
                else if (strcmp(cmdLine, "/next") == 0) {
                    ac.cmd = cmdnext;
                    osMessageQueuePut(audioCmdQ, &ac, 0, 0);
                }
                else if (strcmp(cmdLine, "/prev") == 0) {
                    ac.cmd = cmdprev;
                    osMessageQueuePut(audioCmdQ, &ac, 0, 0);
                }
                else if (strcmp(cmdLine, "/shuffle") == 0) {
                    ac.cmd = cmdshufl;
                    osMessageQueuePut(audioCmdQ, &ac, 0, 0);
                }
                else {
                    txstr("unknown cmd\r\n");
                }
            }
            cmdIdx = 0;
            txstr("> ");
        }
        else if (c == 127 || c == '\b') {
            if (cmdIdx > 0) cmdIdx--;
        }
        else {
            if (cmdIdx < CMDMAXLEN - 1)
                cmdLine[cmdIdx++] = c;
        }
    }
}

  /* USER CODE END 5 */


/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the audiotsk thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
    AudioCmd_t rx;
    osStatus_t qs;

    // let sd task mount first
    osDelay(500);

    // start timer for song time
    HAL_TIM_Base_Start_IT(&htim10);

    txstr("audio task ready\r\n");

    // main loop
    for (;;) {

        // poll commands while playing so audio keeps filling
        // block while idle so task can wait for a command
        if (playing && !paused)
            qs = osMessageQueueGet(audioCmdQ, &rx, NULL, 0);
        else
            qs = osMessageQueueGet(audioCmdQ, &rx, NULL, osWaitForever);

        // handle command if we got one
        if (qs == osOK) {

            switch (rx.cmd) {

            // play song by index
            case cmdplay: {
                int idx = rx.param;
                if (idx < 0 || idx >= numSongs) {
                    txstr("play: bad index\r\n");
                    break;
                }

                // stop current song first
                stpplay();

                curSongIdx = idx;
                strncpy(curSongName, songNames[idx], 63);
                curSongName[63] = '\0';

                // open wav file
                FRESULT fr = f_open(&wavF, songNames[idx], FA_READ);
                if (fr != FR_OK) {
                    char tmp[80];
                    snprintf(tmp, 80, "open fail: %d\r\n", fr);
                    txstr(tmp);
                    break;
                }
                fileopn = 1;

                // read wav header
                if (parsewav(&wavF, &wavifo) != 0) {
                    txstr("bad wav\r\n");
                    f_close(&wavF);
                    fileopn = 0;
                    break;
                }

                // go to audio data
                f_lseek(&wavF, wavifo.dataStart);

                // get song time
                totTimeSec = wavifo.dataSize / wavifo.byteRate;
                curTimeSec = 0;
                datpos     = 0;

                // set timer for wav sample rate
                uint32_t pclk = HAL_RCC_GetPCLK1Freq() * 2;
                __HAL_TIM_SET_AUTORELOAD(&htim6, (pclk / wavifo.sampleRate) - 1);

                // fill dma buffer first
                fillbuffa(audioBuf, AUDIOBUFLEN);

                // clear dma semaphores
                osSemaphoreAcquire(dma1Handle, 0);
                osSemaphoreAcquire(dma2Handle, 0);

                // start dac and timer
                HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2,
                    (uint32_t *)audioBuf, AUDIOBUFLEN, DAC_ALIGN_12B_R);
                HAL_TIM_Base_Start(&htim6);

                playing = 1;   paused = 0;
                isPlaying = 1; isPaused = 0;

                char msg[80];
                snprintf(msg, 80, "playing: %s\r\n", songNames[idx]);
                txstr(msg);
                break;
            }

            // pause song
            case cmdpauz:
                if (playing && !paused) {
                    HAL_TIM_Base_Stop(&htim6);
                    paused = 1;  isPaused = 1;
                    txstr("paused\r\n");
                }
                break;

            // resume song
            case cmdresume:
                if (playing && paused) {
                    HAL_TIM_Base_Start(&htim6);
                    paused = 0;  isPaused = 0;
                    txstr("resumed\r\n");
                }
                break;

            // skip forward
            case cmdfwd: {
                if (!playing || !fileopn) break;
                uint32_t skip = (uint32_t)rx.param * wavifo.byteRate;
                datpos += skip;
                if (datpos > wavifo.dataSize) datpos = wavifo.dataSize;
                datpos -= datpos % wavifo.blockAlign;
                f_lseek(&wavF, wavifo.dataStart + datpos);
                curTimeSec = datpos / wavifo.byteRate;
                txstr("fwd\r\n");
                break;
            }

            // rewind song
            case cmdrew: {
                if (!playing || !fileopn) break;
                uint32_t skip = (uint32_t)rx.param * wavifo.byteRate;
                if (datpos > skip) datpos -= skip;
                else datpos = 0;
                datpos -= datpos % wavifo.blockAlign;
                f_lseek(&wavF, wavifo.dataStart + datpos);
                curTimeSec = datpos / wavifo.byteRate;
                txstr("rew\r\n");
                break;
            }

            // list songs
            case cmdlst: {
                char buf[80];
                for (uint8_t i = 0; i < numSongs; i++) {
                    snprintf(buf, 80, " %d: %s\r\n", i, songNames[i]);
                    txstr(buf);
                }
                break;
            }

            // stop song
            case cmdstop:
                stpplay();
                txstr("stopped\r\n");
                break;

            // next song
            case cmdnext: {
                int16_t nxt = getnxt();
                if (nxt >= 0) {
                    stpplay();
                    AudioCmd_t pc = {0};
                    pc.cmd   = cmdplay;
                    pc.param = nxt;
                    osMessageQueuePut(audioCmdQ, &pc, 0, 0);
                }
                break;
            }

            // previous song
            case cmdprev: {
                int16_t prv = getprv();
                if (prv >= 0) {
                    stpplay();
                    AudioCmd_t pc = {0};
                    pc.cmd   = cmdplay;
                    pc.param = prv;
                    osMessageQueuePut(audioCmdQ, &pc, 0, 0);
                }
                break;
            }

            // toggle shuffle
            case cmdshufl:
                    shuffleOn = !shuffleOn;
                    if (shuffleOn) {
                    shflst();
                    txstr("shuffle ON\r\n");
                } else {
                    txstr("shuffle OFF\r\n");
                }
                break;

            } // end switch
        } // end command check

        // fill dma buffer while playing
        if (playing && !paused) {
            if (osSemaphoreAcquire(dma1Handle, 5) == osOK) {
                fillbuffa(audioBuf, HALFBUFLEN);
            }
            if (osSemaphoreAcquire(dma2Handle, 5) == osOK) {
                fillbuffa(audioBuf + HALFBUFLEN, HALFBUFLEN);
            }

            // go to next song when done
            if (datpos >= wavifo.dataSize) {
                stpplay();
                txstr("song ended\r\n");
                if (numSongs > 0) {
                    int16_t nxt = getnxt();
                    if (nxt >= 0) {
                        AudioCmd_t pc = {0};
                        pc.cmd   = cmdplay;
                        pc.param = nxt;
                        osMessageQueuePut(audioCmdQ, &pc, 0, 0);
                    }
                }
            }
        }

    } // loop forever
}
  /* USER CODE END StartTask03 */


/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the sdmttsk thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void *argument)
{
    uint8_t cardIn;

    osDelay(300);

    cardIn = sdcard_present();

    if (cardIn) {
        FRESULT fr;

        txstr("SD detect startup: inserted\r\n");

        mounted = 0;
        numSongs = 0;
        fileListReady = 0;
        sdRemoved = 1;

        fr = f_mount(&SDFatFS, SDPath, 1);

        if (fr == FR_OK) {
            mounted = 1;
            sdRemoved = 0;

            scnsd();

            txstr("SD mounted OK\r\n");
        } else {
            char msg[80];

            mounted = 0;
            numSongs = 0;
            fileListReady = 0;
            sdRemoved = 1;

            snprintf(msg, sizeof(msg), "SD mount fail: %d\r\n", fr);
            txstr(msg);
        }
    } else {
        txstr("SD detect startup: removed\r\n");

        mounted = 0;
        numSongs = 0;
        fileListReady = 0;
        sdRemoved = 1;
    }

    for (;;) {
        uint8_t nowIn = sdcard_present();

        // card removed
        if (cardIn && !nowIn) {
            stpplay();

            f_mount(NULL, SDPath, 0);
            HAL_SD_DeInit(&hsd);

            mounted       = 0;
            numSongs      = 0;
            fileListReady = 0;
            curSongIdx    = -1;
            sdRemoved     = 1;

            txstr("SD removed\r\n");
        }

        // card inserted
                if (!cardIn && nowIn) {
                    FRESULT fr;
                    uint8_t retries;

                    osDelay(800);

                    HAL_SD_DeInit(&hsd);
                    osDelay(100);

                    // retry so card can settle
                    for (retries = 0; retries < 5; retries++) {
                        if (HAL_SD_Init(&hsd) == HAL_OK) break;
                        HAL_SD_DeInit(&hsd);
                        osDelay(200);
                    }

                    osDelay(300);

                    fr = f_mount(&SDFatFS, SDPath, 1);

                    if (fr != FR_OK) {
                        // clean retry
                        f_mount(NULL, SDPath, 0);
                        HAL_SD_DeInit(&hsd);
                        osDelay(300);
                        HAL_SD_Init(&hsd);
                        osDelay(300);
                        fr = f_mount(&SDFatFS, SDPath, 1);
                    }

                    if (fr == FR_OK) {
                        mounted = 1;
                        sdRemoved = 0;

                        scnsd();

                        txstr("SD inserted, rescanned\r\n");
                    } else {
                        char msg[80];

                        mounted = 0;
                        numSongs = 0;
                        fileListReady = 0;
                        sdRemoved = 1;

                        snprintf(msg, sizeof(msg), "SD remount fail: %d\r\n", fr);
                        txstr(msg);
                    }
                }

        cardIn = nowIn;
        osDelay(200);
    }
}
/* USER CODE END StartTask04 */


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if (htim->Instance == TIM10) {

      // update playback time once per timer tick
      if (playing && !paused && wavifo.byteRate > 0) {
          curTimeSec = datpos / wavifo.byteRate;
      }
  }
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
