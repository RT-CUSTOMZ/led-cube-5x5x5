/*
 * board_config.h
 *
 *  Created on: 07.05.2012
 *      Author: guelland
 */

#ifndef BOARD_CONFIG_H_
#define BOARD_CONFIG_H_

// #include "board_sd.h"

////////////////////////////////////////////////////////////////////

#define FRAMEBUFFER_SIZE_X				5
#define FRAMEBUFFER_SIZE_Y				5
#define FRAMEBUFFER_SIZE_Z				5

////////////////////////////////////////////////////////////////////

#define KEY_BUTTON_EXTI_IRQ_Handler				EXTI15_10_IRQHandler
#define KEY_BUTTON_POLARITY						GPIO_PIN_RESET

////////////////////////////////////////////////////////////////////

#define FRAMEBUFFER_COUNT			3

#define FRAMEBUFFER0_CLOCK()	__GPIOB_CLK_ENABLE()
#define FRAMEBUFFER0_PORT		GPIOB
#define FRAMEBUFFER0_MASK 		(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_11 | \
								GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | \
								GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | \
								GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | \
								GPIO_PIN_15 | GPIO_PIN_10 )

#define FRAMEBUFFER1_CLOCK()	__GPIOA_CLK_ENABLE()
#define FRAMEBUFFER1_PORT		GPIOA
#define FRAMEBUFFER1_MASK 		(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | \
								GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | \
								GPIO_PIN_15 )

#define FRAMEBUFFER2_CLOCK()	__GPIOC_CLK_ENABLE()
#define FRAMEBUFFER2_PORT		GPIOC
#define FRAMEBUFFER2_MASK 		(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | \
								GPIO_PIN_5 | GPIO_PIN_7)

////////////////////////////////////////////////////////////////////

#define FRAMEBUFFER_INVERT_OUTPUT

#define  PERIOD_VALUE       ((15*2) +1)  /* Period Value  */
#define  PULSE1_VALUE       (10*2)
#define  PULSE0_VALUE        (3*2)

#define TIMx                           TIM1
#define TIMx_CLK_ENABLE                __TIM1_CLK_ENABLE
#define DMAx_CLK_ENABLE                  __DMA2_CLK_ENABLE

/* Definition for TIMx's DMA */
#define TIMx_CC1_DMA_CHANNEL             DMA_CHANNEL_6
#define TIMx_CC1_DMA_STREAM              DMA2_Stream1

/* Definition for ADCx's NVIC */
#define TIMx_CC1_DMA_IRQn                DMA2_Stream1_IRQn
#define TIMx_CC1_DMA_IRQHandler          DMA2_Stream1_IRQHandler

/* Definition for TIMx's DMA */
#define TIMx_CC4_DMA_CHANNEL             DMA_CHANNEL_6
#define TIMx_CC4_DMA_STREAM              DMA2_Stream4

/* Definition for ADCx's NVIC */
#define TIMx_CC4_DMA_IRQn                DMA2_Stream4_IRQn
#define TIMx_CC4_DMA_IRQHandler          DMA2_Stream4_IRQHandler

/* Definition for TIMx's DMA */
#define TIMx_AAR_DMA_CHANNEL             DMA_CHANNEL_6
#define TIMx_AAR_DMA_STREAM              DMA2_Stream5

/* Definition for ADCx's NVIC */
#define TIMx_AAR_DMA_IRQn                DMA2_Stream5_IRQn
#define TIMx_AAR_DMA_IRQHandler          DMA2_Stream5_IRQHandler

////////////////////////////////////////////////////////////////////

#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __USART1_CLK_ENABLE();
#define USARTx_DMAx_CLK_ENABLE()         __DMA2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART1

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_CHANNEL             DMA_CHANNEL_4
#define USARTx_TX_DMA_STREAM              DMA2_Stream7
#define USARTx_RX_DMA_CHANNEL             DMA_CHANNEL_4
#define USARTx_RX_DMA_STREAM              DMA2_Stream2

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn                DMA2_Stream7_IRQn
#define USARTx_DMA_RX_IRQn                DMA2_Stream2_IRQn
#define USARTx_DMA_TX_IRQHandler          DMA2_Stream7_IRQHandler
#define USARTx_DMA_RX_IRQHandler          DMA2_Stream2_IRQHandler

#define LED2_PIN                                GPIO_PIN_5
#define LED2_GPIO_PORT                          GPIOA
#define LED2_GPIO_CLK_ENABLE()                  __GPIOA_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()                 __GPIOA_CLK_DISABLE()


static inline void debug_led_init(){
	  LED2_GPIO_CLK_ENABLE();
	  GPIO_InitTypeDef  GPIO_InitStruct;

	  /* Enable the GPIO_LED Clock */
	  LED2_GPIO_CLK_ENABLE();

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStruct.Pin = LED2_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	  HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
}

static inline void debug_led_toggle(){
	HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
}


#endif /* BOARD_CONFIG_H_ */
