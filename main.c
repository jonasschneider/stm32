// STM32_TIM1
// toggle LED on PC9 using software delay
// toggle LED on PC8 using timer 1 with ISR reload
// Default 8 MHz internal clock

#include "stm32f4_discovery.h"
#include <stm32f407xx.h>

#define PRESCALE        64
#define PERIOD          250 // microseconds - 1/4 second delay
#define TCLKS           ((F_CPU/PRESCALE*PERIOD)/1000)

void TIM1_UP_TIM10_IRQHandler(void)
{
  TIM1->CNT += (65536UL-TCLKS); // reload
  BSP_LED_Toggle(LED6);
  TIM1->SR = ~TIM_SR_UIF;       // clear update int flag (write 0)
}

void delay(volatile uint32_t d)
{
  while (d-- != 0)
    ;
}


// NVIC (interrupt controller) registers
#define MY_NVIC 0xE000E100
#define MY_NVIC_ISER0 *((uint32_t *)MY_NVIC+0x00)

#define NVIC_ISER0_TIM1_UP_TIM10 (1<<25)


int main(void)
{
	HAL_Init();

	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);

  RCC->CFGR = 0;                      // HSI, 8 MHz,

  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // enable Timer1

  TIM1->PSC = PRESCALE - 1;
  TIM1->DIER = TIM_DIER_UIE;          // enable update interrupt
  TIM1->CNT += (65536UL-TCLKS);       // initial load
  TIM1->CR1 = TIM_CR1_CEN;            // enable timer

  //NVIC_EnableIRQ
  //NVIC->ISER[0] = (1 << TIM1_UP_TIM16_IRQn);  // enable TIM1 int in NVIC
  MY_NVIC_ISER0 |= NVIC_ISER0_TIM1_UP_TIM10;
  //MY_NVIC_ISER0 = ~0;

  while (1)
  {
    BSP_LED_Toggle(LED5);
    delay(800000);
  }
}


/*
#include "stm32f4_discovery.h"

#include "main.h"

// TIM7 register base address
#define MY_TIM7 			0x40001400
#define MY_TIM7_CR1		*((uint32_t *)MY_TIM7+0x00)
#define MY_TIM7_DIER	*((uint32_t *)MY_TIM7+0x0c)
#define MY_TIM7_SR		*((uint32_t *)MY_TIM7+0x10)

#define TIM67_CR1_CEN 1
#define TIM67_DIER_UIE 1
#define TIM67_SR_UIF 1

#include <stm32f407xx.h>

// NVIC (interrupt controller) registers
#define MY_NVIC 0xE000E100
#define MY_NVIC_ISER0 *((uint32_t *)MY_NVIC+0x00)
#define MY_NVIC_ISER1 *((uint32_t *)MY_NVIC+0x04)

#define NVIC_ISER1_EIRQ55_TIM7 (1<<(55-32))

void timery() {
	// enable the counter
	MY_TIM7_CR1 |= TIM67_CR1_CEN;

	// enable interrupt
	MY_TIM7_DIER |= TIM67_DIER_UIE;

	// enable the interrupt in the global lulz table
	MY_NVIC_ISER1 |= NVIC_ISER1_EIRQ55_TIM7;
}

void TIM7_IRQHandler() {
	BSP_LED_Toggle(LED6);

	// clear interrupt flag
	MY_TIM7_SR &= ~TIM67_SR_UIF;
}

int main(void) {

	HAL_Init();

	SystemClock_Config();

	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);

	timery();

	int i = 0;

	while (1) {
		if(i==0)
		BSP_LED_Toggle(LED3);
		else if (i==1)
		BSP_LED_Toggle(LED4);
		else if (i==2)
		BSP_LED_Toggle(LED5);

		HAL_Delay(200);

		i = (i+1)%4;
	}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  while (1)
  {
  }
}
#endif
*/
