

#include "board.h"


#define DEBUG_USARTx                           USART2
#define DEBUG_USARTx_CLK_ENABLE()              __USART2_CLK_ENABLE();
// #define DEBUG_USARTx_DMAx_CLK_ENABLE()         __DMA2_CLK_ENABLE()
#define DEBUG_USARTx_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define DEBUG_USARTx_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define DEBUG_USARTx_FORCE_RESET()             __USART1_FORCE_RESET()
#define DEBUG_USARTx_RELEASE_RESET()           __USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define DEBUG_USARTx_TX_PIN                    GPIO_PIN_2
#define DEBUG_USARTx_TX_GPIO_PORT              GPIOA
#define DEBUG_USARTx_TX_AF                     GPIO_AF7_USART2
#define DEBUG_USARTx_RX_PIN                    GPIO_PIN_3
#define DEBUG_USARTx_RX_GPIO_PORT              GPIOA
#define DEBUG_USARTx_RX_AF                     GPIO_AF7_USART2

static UART_HandleTypeDef uartHandle;

static void gpio_init(void);

void debug_uart_init() {

    gpio_init();

	uartHandle.Instance        = DEBUG_USARTx;
	uartHandle.Init.BaudRate   = 230400;//230400;115200
	uartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	uartHandle.Init.StopBits   = UART_STOPBITS_1;
	uartHandle.Init.Parity     = UART_PARITY_NONE;
	uartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	uartHandle.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_UART_Init(&(uartHandle)) != HAL_OK){

    }
}

void debug_uart_send(const uint8_t *data, uint32_t len) {
    HAL_UART_Transmit(&uartHandle, (uint8_t *)data, len, 100);
}

static void gpio_init(void) {
    GPIO_InitTypeDef  GPIO_InitStruct;

    DEBUG_USARTx_CLK_ENABLE();
    DEBUG_USARTx_RX_GPIO_CLK_ENABLE();
    DEBUG_USARTx_TX_GPIO_CLK_ENABLE();

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = DEBUG_USARTx_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = DEBUG_USARTx_TX_AF;
    
    HAL_GPIO_Init(DEBUG_USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
        
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = DEBUG_USARTx_RX_PIN;
    GPIO_InitStruct.Alternate = DEBUG_USARTx_RX_AF;
        
    HAL_GPIO_Init(DEBUG_USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

}

// overwrite printf fd
#include  <errno.h>
#include  <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO

int _write(int file, char *data, int len)
{
   if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
   {
      errno = EBADF;
      return -1;
   }

   // arbitrary timeout 1000
   HAL_StatusTypeDef status =
      HAL_UART_Transmit(&uartHandle, (uint8_t*)data, len, 1000);

   // return # of bytes written - as best we can tell
   return (status == HAL_OK ? len : 0);
}