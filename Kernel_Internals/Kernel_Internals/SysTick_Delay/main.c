#include "stm32f4xx.h"                  // Device header


#define LED (1U<<5)
#define RED (1U<<4)

#define LED_BIT (1U<<10)
#define RED_BIT (1U<<8)

#define GPIOA_CLOCK (1U<<0)

volatile uint32_t tick;
volatile uint32_t _tick;

void SysTick_Handler(void)
{
	++tick;
}

uint32_t getTick(void)
{
	__disable_irq();
	_tick = tick;
	__enable_irq();
	
	return _tick;
}

void delayS(uint32_t s)
{
	s *= 100;
	uint32_t temp = getTick();
		while((getTick()-temp)<s)
		{
		}
}

int	main(void)
{
	RCC->AHB1ENR |= GPIOA_CLOCK;
	
	GPIOA->MODER |= LED_BIT;
	GPIOA->MODER |= RED_BIT;
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/100);
	__enable_irq();
	
	while(1)
	{
		GPIOA->ODR |= LED;
		GPIOA->ODR &= ~RED;
		delayS(1);
		GPIOA->ODR |= RED;
		GPIOA->ODR &= ~LED;
		delayS(1);
	}
}
