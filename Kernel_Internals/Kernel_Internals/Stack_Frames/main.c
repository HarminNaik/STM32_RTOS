#include "stm32f4xx.h"                  // Device header


#define LED (1U<<5)
#define RED (1U<<4)

#define RED_BIT (1U<<8)
#define LED_BIT (1U<<10)

#define GPIOA_CLOCK (1U<<0)

uint32_t red_stack[40];
uint32_t led_stack[40];

uint32_t *sp_red = &red_stack[40];
uint32_t *sp_led = &led_stack[40];

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
	GPIOAInit();
	
	// Initializing stack frame for LED thread
		*(--sp_led) = (1U<<24); 								    // XPSR sets the processor in thumb mode
		*(--sp_led) = (uint32_t)&led_main;					// PC gets the address of led_main, so control goes to led_main after exception is served
		*(--sp_led) = 0x0000000DU;									// LR, random value
		*(--sp_led) = 0x0000000EU;									// R12, random value
		*(--sp_led) = 0x0000000AU;									// R3, random value
		*(--sp_led) = 0x0000000DU;									// R2, random value
		*(--sp_led) = 0x0000000AU;									// R1, random value
		*(--sp_led) = 0x0000000FU;									// R0, random value
	
	// Initializing stack frame for RED thread
		*(--sp_red) = (1U<<24); 								    // XPSR sets the processor in thumb mode
		*(--sp_red) = (uint32_t)&red_main;					// PC gets the address of red_main, so control goes to red_main after exception is served
		*(--sp_red) = 0x0000000FU;									// LR, random value
		*(--sp_red) = 0x0000000AU;									// R12, random value
		*(--sp_red) = 0x0000000AU;									// R3, random value
		*(--sp_red) = 0x0000000FU;									// R2, random value
		*(--sp_red) = 0x0000000DU;									// R1, random value
		*(--sp_red) = 0x0000000AU;									// R0, random value

	
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
