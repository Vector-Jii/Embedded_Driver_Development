#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define RCC_base_address 0x40023800
#define GPIOA_base_address 0x40020000

#define RCC_CR_REG_OFFSET 0x00
#define RCC_CFGR_REG_OFFSET 0x08

#define RCC_CFGR_REG_ADDR (RCC_base_address + RCC_CFGR_REG_OFFSET)
#define RCC_CR_REG_ADDR (RCC_base_address + RCC_CR_REG_OFFSET)

int main(void)
{
    //1. Enable the HSE clock using HSEON bit (RCC_CR)
	uint32_t *pHSE_ON_REG = (uint32_t*)RCC_CR_REG_ADDR;
	*pHSE_ON_REG |= (1 << 16);

	//2. Wait until HSE clock from external crystal stabilizes
	while(!(*pHSE_ON_REG & (1 << 17)));

	//3. Switch the system clock to HSE (RCC_CFGR)
	uint32_t *pRccCfgrReg = (uint32_t*)RCC_CFGR_REG_ADDR;
	*pRccCfgrReg |= (1 << 0);

	//4. Do MCO1 settings for measurement
	*pRccCfgrReg |= (1 << 22);

	// Pre Scalar
	*pRccCfgrReg = (1 << 25);
	*pRccCfgrReg = (1 << 26);

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

void I2C1_EV_IRQHandler(void)
{

}
