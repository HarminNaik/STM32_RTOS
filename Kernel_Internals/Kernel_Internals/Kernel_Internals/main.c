#include "stm32f4xx.h"                  // Device header


#define LED (1U<<5)
#define RED (1U<<4)

#define RED_BIT (1U<<8)
#define LED_BIT (1U<<10)

#define GPIOA_CLOCK (1U<<0)

volatile uint32_t tick;
volatile uint32_t _tick;

void delayS(uint32_t s);
void GPIOAInit(void);
void ledOn(void);
void ledOff(void);
void redOn(void);
void redOff(void);

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

//GPIO INIT
void GPIOAInit(void)
{
	RCC->AHB1ENR |= GPIOA_CLOCK;
	GPIOA->MODER |= LED_BIT | RED_BIT;
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/100);
	__enable_irq();
	
}

int led_main(void)
{
	while(1)
	{
		ledOn();
		delayS(1);
		ledOff();
		delayS(1);
	}
}

int red_main(void)
{
	while(1)
	{
		redOn();
		delayS(1);
		redOff();
		delayS(1);
	}
}


int	main(void)
{
	uint32_t volatile start = 0U;
	GPIOAInit();
	
	if (start)
	{
		led_main();
	}
	
	else
	{
		red_main();
	}
	
	while(1)
	{
	}
}


//Led Initialization
void ledOn(void)
{
	GPIOA->ODR |= LED;
}

void ledOff(void)
{
	GPIOA->ODR = 00;
}


//Red Led initialization
void redOn(void)
{
	GPIOA->ODR |= RED;
}

void redOff(void)
{
	GPIOA->ODR = 00;
}
