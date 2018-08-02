#include "stm32f4xx_hal.h"

/* DMA definitions for SD DMA transfer */
#define __DMAx_TxRx_CLK_ENABLE            __DMA2_CLK_ENABLE
#define SD_DMAx_Tx_CHANNEL                DMA_CHANNEL_4
#define SD_DMAx_Rx_CHANNEL                DMA_CHANNEL_4
#define SD_DMAx_Tx_STREAM                 DMA2_Stream6  
#define SD_DMAx_Rx_STREAM                 DMA2_Stream3  
#define SD_DMAx_Tx_IRQn                   DMA2_Stream6_IRQn
#define SD_DMAx_Rx_IRQn                   DMA2_Stream3_IRQn
#define SD_DMAx_Tx_IRQHandler             DMA2_Stream6_IRQHandler
#define SD_DMAx_Rx_IRQHandler             DMA2_Stream3_IRQHandler
#define SD_DetectIRQHandler()             HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13)


/**
  * @brief  Initializes the SD MSP.
  * @param  None
  * @retval None
  */
void SD_MspInit(void)
{
  GPIO_InitTypeDef GPIO_Init_Structure;
  
  /* Enable SDIO clock */
  __SDIO_CLK_ENABLE();
  
  /* Enable DMA2 clocks */
  __DMAx_TxRx_CLK_ENABLE();

  /* Enable GPIOs clock */
  __GPIOC_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
//   __SD_DETECT_GPIO_CLK_ENABLE();
  
  /* Common GPIO configuration */
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF12_SDIO;
  
  /* GPIOC configuration */
  GPIO_Init_Structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
   
  HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

  /* GPIOD configuration */
  GPIO_Init_Structure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);
}