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
  static DMA_HandleTypeDef dmaRxHandle;
  static DMA_HandleTypeDef dmaTxHandle;
  GPIO_InitTypeDef GPIO_Init_Structure;
//   SD_HandleTypeDef *hsd = &uSdHandle;
  
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

  /* SD Card detect pin configuration */
//   GPIO_Init_Structure.Mode      = GPIO_MODE_INPUT;
//   GPIO_Init_Structure.Pull      = GPIO_PULLUP;
//   GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
//   GPIO_Init_Structure.Pin       = SD_DETECT_PIN;
  //HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_Init_Structure);
# if 0
  /* NVIC configuration for SDIO interrupts */
  HAL_NVIC_SetPriority(SDIO_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(SDIO_IRQn);
    
  /* Configure DMA Rx parameters */
  dmaRxHandle.Init.Channel             = SD_DMAx_Rx_CHANNEL;
  dmaRxHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  dmaRxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  dmaRxHandle.Init.MemInc              = DMA_MINC_ENABLE;
  dmaRxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  dmaRxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  dmaRxHandle.Init.Mode                = DMA_PFCTRL;
  dmaRxHandle.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  dmaRxHandle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  dmaRxHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  dmaRxHandle.Init.MemBurst            = DMA_MBURST_INC4;
  dmaRxHandle.Init.PeriphBurst         = DMA_PBURST_INC4;
  
  dmaRxHandle.Instance = SD_DMAx_Rx_STREAM;
  
  /* Associate the DMA handle */
  // __HAL_LINKDMA(hsd, hdmarx, dmaRxHandle);
  
  /* Deinitialize the stream for new transfer */
  HAL_DMA_DeInit(&dmaRxHandle);
  
  /* Configure the DMA stream */
  HAL_DMA_Init(&dmaRxHandle);
  
  /* Configure DMA Tx parameters */
  dmaTxHandle.Init.Channel             = SD_DMAx_Tx_CHANNEL;
  dmaTxHandle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  dmaTxHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  dmaTxHandle.Init.MemInc              = DMA_MINC_ENABLE;
  dmaTxHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  dmaTxHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  dmaTxHandle.Init.Mode                = DMA_PFCTRL;
  dmaTxHandle.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  dmaTxHandle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  dmaTxHandle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  dmaTxHandle.Init.MemBurst            = DMA_MBURST_INC4;
  dmaTxHandle.Init.PeriphBurst         = DMA_PBURST_INC4;
  
  dmaTxHandle.Instance = SD_DMAx_Tx_STREAM;
  
  /* Associate the DMA handle */
//   __HAL_LINKDMA(hsd, hdmatx, dmaTxHandle);
  
  /* Deinitialize the stream for new transfer */
  HAL_DMA_DeInit(&dmaTxHandle);
  
  /* Configure the DMA stream */
  HAL_DMA_Init(&dmaTxHandle); 
  
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(SD_DMAx_Rx_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(SD_DMAx_Rx_IRQn);
  
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(SD_DMAx_Tx_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(SD_DMAx_Tx_IRQn);
  #endif
}