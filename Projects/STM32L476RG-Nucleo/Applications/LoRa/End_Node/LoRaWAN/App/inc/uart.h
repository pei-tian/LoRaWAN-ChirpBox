#include "hw.h"


#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA

#define USART3_TX_Pin GPIO_PIN_4
#define USART3_TX_GPIO_Port GPIOC
#define USART3_RX_Pin GPIO_PIN_5
#define USART3_RX_GPIO_Port GPIOC


void MX_USART2_UART_Init(void);


#define LED_GPIO_Port GPIOC
#define LED1 GPIO_PIN_8
#define LED2 GPIO_PIN_6


void MX_GPIO_Init(void);

void			gpi_led_on(int id);
void			gpi_led_off(int id);

#define LED_TX		LED1
#define LED_RX		LED2


