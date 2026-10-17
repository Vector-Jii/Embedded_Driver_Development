//Output the HSI on PA8 pin on STM32

#include <stdint.h>
#include <stdio.h>


#define RCC_base_address 0x40023800
#define GPIOA_base_address 0x40020000

#define RCC_CFGR_REG_OFFSET 0x08

#define RCC_CFGR_REG_ADDR (RCC_base_address + RCC_CFGR_REG_OFFSET)

int main(void)
{
	uint32_t *pRCC_CFGR_REG = (uint32_t*)RCC_CFGR_REG_ADDR;

	// 1. Configure RCC_CFGR MCO1 bit to select HSI as clock source

	/*
	 * 0x3: Represents the binary value 11 (two consecutive bits set to 1).
	 * << 21: Shifts those two bits 21 positions to the left, targeting bits 21 and 22 of the register
	 * ~: Inverts the bits, creating a mask where bits 21 and 22 are 0 and all other bits are 1
	 * &=: Performs a bitwise AND operation, which clears bits 21 and 22 to 0 while leaving the rest of the register completely unchanged.
	 */
	*pRCC_CFGR_REG &= ~(0x3 << 21);

	//2. Configure PA8 to AF0 mode to behave MCO1 signal

	// a. Enable the peripheral clock for GPIOA peripheral
	uint32_t *pRCC_AHB1_ENR = (uint32_t*)(RCC_base_address + 0X30);  //RCC_AHB1ENR
	*pRCC_AHB1_ENR |= (1 << 0);  //  IO port A clock enabled

	// b. Configure the mode of GPIO pin 8 as alternate function mode
	uint32_t *pGPIOAmodeREG = (uint32_t*)(GPIOA_base_address);  // GPIOx_MODER
	*pGPIOAmodeREG &= ~(0x3 << 16); //Clear   First make 00 at 16th and 17th bit
	*pGPIOAmodeREG |= (0x2 << 16);  // set    10 at 16th and 17th bit for PA8

	//c. Configure the alternate function register to set the mode 0 for PA8
	uint32_t *pGPIOAAltFunHighReg = (uint32_t*)(GPIOA_base_address + 0x24);
	 *pGPIOAAltFunHighReg &= (0xf << 0);   // (GPIOx_AFRH)




	for(;;);
}
