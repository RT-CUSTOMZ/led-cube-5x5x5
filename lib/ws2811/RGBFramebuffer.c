/*
 * RGBFramebuffer.c
 *
 *  Created on: 07.05.2012
 *      Author: guelland
 */

#include <string.h>

#include "RGBFramebuffer.h"
#include "stm32f4xx_hal.h"
#include "board.h"
#include "board_config.h"

#include <boost/preprocessor.hpp>

#define DECL_FRAMEBUFFER_CLOCK(z, n, text) FRAMEBUFFER ## n ## _CLOCK();
#define FRAMEBUFFER_CLOCKS() BOOST_PP_REPEAT(FRAMEBUFFER_COUNT, DECL_FRAMEBUFFER_CLOCK, 0)

#define DECL(z, n, text) FRAMEBUFFER ## text ## _MASK,
#define DECLDECL(z, n, text) BOOST_PP_REPEAT(LAYERS, DECL, text)
#define DECLBUFFER(z, n, text) \
	static const uint16_t buffer ## n ## _periode[] = {\
		BOOST_PP_REPEAT(PIXELSIZE, DECLDECL, n)\
		0,\
	};\
	\
	volatile uint16_t buffer ## n[sizeof(buffer ## n ## _periode) / sizeof(buffer ##n ## _periode[0])];

BOOST_PP_REPEAT(FRAMEBUFFER_COUNT, DECLBUFFER, FRAMEBUFFER1_MASK)

typedef struct {
	GPIO_TypeDef* io_port;
	uint32_t size;
	const uint16_t* periode;
	volatile uint16_t* buffer;
} Framebuffer;

static const Framebuffer framebuffers[] = {
#define DECL_FRAMEBUFFERS(z, n, text) \
		{\
		.io_port = FRAMEBUFFER ## n ## _PORT,\
		.size = sizeof(buffer ## n ## _periode) / sizeof(buffer ## n ## _periode[0]),\
		.periode = buffer ## n ## _periode,\
		.buffer = buffer ## n,\
		},
		BOOST_PP_REPEAT(FRAMEBUFFER_COUNT, DECL_FRAMEBUFFERS, 0)
};

static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
  /* Configure DMA Stream data length */
  hdma->Instance->NDTR = DataLength;

  /* Peripheral to Memory */
  if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)
  {
    /* Configure DMA Stream destination address */
    hdma->Instance->PAR = DstAddress;

    /* Configure DMA Stream source address */
    hdma->Instance->M0AR = SrcAddress;
  }
  /* Memory to Peripheral */
  else
  {
    /* Configure DMA Stream source address */
    hdma->Instance->PAR = SrcAddress;

    /* Configure DMA Stream destination address */
    hdma->Instance->M0AR = DstAddress;
  }
}

HAL_StatusTypeDef HAL_DMA_Start_NO_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{

   /* Check the parameters */
  assert_param(IS_DMA_BUFFER_SIZE(DataLength));

  /* Disable the peripheral */
  __HAL_DMA_DISABLE(hdma);

  /* Configure the source, destination address and the data length */
  DMA_SetConfig(hdma, SrcAddress, DstAddress, DataLength);

   /* Enable the Peripheral */
  __HAL_DMA_ENABLE(hdma);

  return HAL_OK;
}


//volatile uint8_t framebufferSelect = 0;

static TIM_HandleTypeDef    TimHandle;

void initFramebuffer(void) {

	//memset(buffer1,0,sizeof(buffer1)/sizeof(buffer1[0]));
	  /* Compute the prescaler value to have TIM3 counter clock equal to 21 MHz */
	  uint32_t uhPrescalerValue = (uint32_t) ((SystemCoreClock /2) / ((168 /2) * 1000000 /8) ) - 1;


	  /*##-1- Configure the TIM peripheral #######################################*/
	  /* -----------------------------------------------------------------------
	  TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles.

	    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1),
	    since APB1 prescaler is different from 1.
	      TIM3CLK = 2 * PCLK1
	      PCLK1 = HCLK / 4
	      => TIM3CLK = HCLK / 2 = SystemCoreClock /2

	    To get TIM3 counter clock at 21 MHz, the prescaler is computed as follows:
	       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
	       Prescaler = ((SystemCoreClock /2) /21 MHz) - 1

	    To get TIM3 output clock at 30 KHz, the period (ARR)) is computed as follows:
	       ARR = (TIM3 counter clock / TIM3 output clock) - 1
	           = 665

	    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
	    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
	    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
	    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%

	    Note:
	     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
	     Each time the core clock (HCLK) changes, user had to update SystemCoreClock
	     variable value. Otherwise, any configuration based on this variable will be incorrect.
	     This variable is updated in three ways:
	      1) by calling CMSIS function SystemCoreClockUpdate()
	      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
	      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
	  ----------------------------------------------------------------------- */

	  /* Initialize TIMx peripheral as follow:
	       + Prescaler = (SystemCoreClock/2)/21000000
	       + Period = 665
	       + ClockDivision = 0
	       + Counter direction = Up
	  */
	  memset(&TimHandle,0,sizeof(TimHandle));
	  TimHandle.Instance = TIMx;

	  TimHandle.Init.Prescaler = uhPrescalerValue;
	  TimHandle.Init.Period = PERIOD_VALUE;
//	  TimHandle.Init.RepetitionCounter = 3;
	  TimHandle.Init.ClockDivision = 0;
	  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	  if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
	  {
	    /* Initialization Error */
	    //Error_Handler();
	  }

	  {

		  static TIM_OC_InitTypeDef sConfig;
		  memset(&sConfig,0,sizeof(sConfig));
		  /*##-2- Configure the PWM channels #########################################*/
		  /* Common configuration for all channels */
		  sConfig.OCMode = TIM_OCMODE_TIMING;
		  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
		  sConfig.Pulse      = PULSE0_VALUE;//0;
		  //sConfig.OCFastMode = TIM_OCFAST_DISABLE;
		  if(HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
		  {
			/* Configuration Error */
			//Error_Handler();
		  }
	  }
	  {

		  static TIM_OC_InitTypeDef sConfig;
		  memset(&sConfig,0,sizeof(sConfig));
		  /*##-2- Configure the PWM channels #########################################*/
		  /* Common configuration for all channels */
		  sConfig.OCMode = TIM_OCMODE_TIMING;
		  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
		  sConfig.Pulse      = PULSE1_VALUE;//0;
		  //sConfig.OCFastMode = TIM_OCFAST_DISABLE;
		  if(HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4) != HAL_OK)
		  {
			/* Configuration Error */
			//Error_Handler();
		  }
	  }

	  blankFrameBuffer();
	  updateFramebuffer();

}

static void updateFramebufferPort(const Framebuffer * framebuffer)
{
	__HAL_TIM_DISABLE(&TimHandle);

	__HAL_DMA_CLEAR_FLAG((&TimHandle)->hdma[TIM_DMA_ID_CC1],
			__HAL_DMA_GET_HT_FLAG_INDEX((&TimHandle)->hdma[TIM_DMA_ID_CC1]));
	__HAL_DMA_CLEAR_FLAG((&TimHandle)->hdma[TIM_DMA_ID_CC1],
			__HAL_DMA_GET_TC_FLAG_INDEX((&TimHandle)->hdma[TIM_DMA_ID_CC1]));
	__HAL_DMA_CLEAR_FLAG((&TimHandle)->hdma[TIM_DMA_ID_CC1],
			__HAL_DMA_GET_FE_FLAG_INDEX((&TimHandle)->hdma[TIM_DMA_ID_CC1]));

	/*##-3- Start PWM signal generation in DMA mode ############################*/
	HAL_DMA_Start_NO_IT(
		(&TimHandle)->hdma[TIM_DMA_ID_CC1],
		(uint32_t) framebuffer->buffer,
#ifdef FRAMEBUFFER_INVERT_OUTPUT
		(uint32_t) & (framebuffer->io_port->BSRR),
#else
		(uint32_t) & (framebuffer->io_port->BSRR) + 2, //select BSRRH
#endif
 		framebuffer->size);

	HAL_DMA_Start_NO_IT(
		(&TimHandle)->hdma[TIM_DMA_ID_CC4],
		(uint32_t) framebuffer->periode,
#ifdef FRAMEBUFFER_INVERT_OUTPUT
		(uint32_t) & (framebuffer->io_port->BSRR),
#else
		(uint32_t) & (framebuffer->io_port->BSRR)  + 2, //select BSRRH
#endif
 		1);

	/* Enable the TIM Capture/Compare 2 DMA request */

	__HAL_TIM_ENABLE_DMA((&TimHandle), TIM_DMA_CC1);
	__HAL_TIM_ENABLE_DMA((&TimHandle), TIM_DMA_CC4);

	HAL_DMA_Start_NO_IT(
		(&TimHandle)->hdma[TIM_DMA_ID_UPDATE],
		(uint32_t) framebuffer->periode,
#ifdef FRAMEBUFFER_INVERT_OUTPUT
		(uint32_t) & (framebuffer->io_port->BSRR)  + 2, //select BSRRH
#else
		(uint32_t) & (framebuffer->io_port->BSRR),
#endif
 		framebuffer->size);

	/* Enable the TIM Update DMA request */
	__HAL_TIM_ENABLE_DMA((&TimHandle), TIM_DMA_UPDATE);

	__HAL_DMA_ENABLE_IT((&TimHandle)->hdma[TIM_DMA_ID_UPDATE], DMA_IT_TC);

	TimHandle.Instance->CNT = TimHandle.Instance->ARR - 1;
	//GPIO_PORT_CHANNEL2->BSRRL = GPIO_PIN_CHANNEL2;
	__HAL_TIM_ENABLE(&TimHandle);
}



void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIMx Peripheral clock enable */
	TIMx_CLK_ENABLE();
	DMAx_CLK_ENABLE();
	FRAMEBUFFER_CLOCKS();

	GPIO_InitTypeDef   GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	for(uint32_t i = 0; i < sizeof(framebuffers) / sizeof(framebuffers[0]); i++) {
		GPIO_InitStruct.Pin = framebuffers[i].periode[0];
		HAL_GPIO_Init(framebuffers[i].io_port, &GPIO_InitStruct);
	}

//	GPIO_InitStruct.Pin = GPIO_PIN_CHANNEL2;
//	HAL_GPIO_Init(GPIO_PORT_CHANNEL2, &GPIO_InitStruct);

	{
	static DMA_HandleTypeDef  hdma_tim;

	/* Set the parameters to be configured */
	hdma_tim.Init.Channel  = TIMx_CC1_DMA_CHANNEL;
	hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD ;
	hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD ;
	hdma_tim.Init.Mode = DMA_NORMAL;
	hdma_tim.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_tim.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_tim.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_tim.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_tim.Init.PeriphBurst = DMA_PBURST_SINGLE;

	/* Set hdma_tim instance */
	hdma_tim.Instance = TIMx_CC1_DMA_STREAM;

	/* Link hdma_tim to hdma[3] (channel3) */
	__HAL_LINKDMA(htim, hdma[TIM_DMA_ID_CC1], hdma_tim);

	/* Initialize TIMx DMA handle */
	HAL_DMA_Init(htim->hdma[TIM_DMA_ID_CC1]);

	/*##-2- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt */
	HAL_NVIC_SetPriority(TIMx_CC1_DMA_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIMx_CC1_DMA_IRQn);
	}
	{
	static DMA_HandleTypeDef  hdma_tim;

	/* Set the parameters to be configured */
	hdma_tim.Init.Channel  = TIMx_CC4_DMA_CHANNEL;
	hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD ;
	hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD ;
	hdma_tim.Init.Mode = DMA_CIRCULAR;//DMA_NORMAL;
	hdma_tim.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_tim.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_tim.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_tim.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_tim.Init.PeriphBurst = DMA_PBURST_SINGLE;

	/* Set hdma_tim instance */
	hdma_tim.Instance = TIMx_CC4_DMA_STREAM;

	/* Link hdma_tim to hdma[3] (channel3) */
	__HAL_LINKDMA(htim, hdma[TIM_DMA_ID_CC4], hdma_tim);

	/* Initialize TIMx DMA handle */
	HAL_DMA_Init(htim->hdma[TIM_DMA_ID_CC4]);

	/*##-2- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt */
	HAL_NVIC_SetPriority(TIMx_CC4_DMA_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIMx_CC4_DMA_IRQn);
	}
	{
	static DMA_HandleTypeDef  hdma_tim;

	/* Set the parameters to be configured */
	hdma_tim.Init.Channel  = TIMx_AAR_DMA_CHANNEL;
	hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD ;
	hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD ;
	hdma_tim.Init.Mode = DMA_NORMAL;
	hdma_tim.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_tim.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_tim.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_tim.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_tim.Init.PeriphBurst = DMA_PBURST_SINGLE;

	/* Set hdma_tim instance */
	hdma_tim.Instance = TIMx_AAR_DMA_STREAM;

	/* Link hdma_tim to hdma[3] (channel3) */
	__HAL_LINKDMA(htim, hdma[TIM_DMA_ID_UPDATE], hdma_tim);

	/* Initialize TIMx DMA handle */
	HAL_DMA_Init(htim->hdma[TIM_DMA_ID_UPDATE]);

	/*##-2- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt */
	HAL_NVIC_SetPriority(TIMx_AAR_DMA_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIMx_AAR_DMA_IRQn);
	}
}

void TIMx_CC1_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(TimHandle.hdma[TIM_DMA_ID_CC1]);
}

void TIMx_CC4_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(TimHandle.hdma[TIM_DMA_ID_CC4]);
}

static volatile frameBufferSelector = 0;

void updateFramebuffer(void);

//void TIMx_AAR_DMA_IRQHandler(void)
void __attribute__ ((section(".after_vectors")))
DMA2_Stream5_IRQHandler(void)
{
	__HAL_TIM_DISABLE(&TimHandle);
	HAL_DMA_IRQHandler(TimHandle.hdma[TIM_DMA_ID_UPDATE]);
	frameBufferSelector++;
	updateFramebuffer();
}

void updateFramebuffer(void)
{
	if(frameBufferSelector < ( sizeof(framebuffers) / sizeof(framebuffers[0]) ) )
		updateFramebufferPort(&(framebuffers[frameBufferSelector]));
	else
		frameBufferSelector = 0;
}

void tickFramebuffer(void) {
}

void blankFrameBuffer(void) {
	for(uint32_t i = 0; i < sizeof(framebuffers) / sizeof(framebuffers[0]); i++) {
		uint32_t size =  framebuffers[i].size * sizeof(framebuffers[i].buffer[0]);
		memcpy(framebuffers[i].buffer,framebuffers[i].periode, size);
	}
}
