//Where is the LED connected
//-Pin? Green LED = 7, Blue LED = 7, Red LED = 9
//Port? Green LED = C,Blue LED = B, Red LED = A
//LED1: Green LED -> NOT Correct: Pin PA5 (SB120 ON, SB118 OFF)
//Short Circuit->ON, Else OFF, show on the back of the board
//As it actually SB120OFF, SB118ON -> Pin PC7 is CORRECT Pin to drive the LED1
//Voltage 1V8

//How do you access preferred Ports&Pins, turn it on/off?
//1. Locate the address of Port A/B/C
//    a. look at the memory map of a one time micro-controller
//               -> Go to Data Sheet, search memory map
//               -> Else, check reference manual
//    b. get the starting address of the corresponding bus (connect to GPIO peripheral)

#define PERIPH_BASE 0x40000000UL //UL: unsigned long
#define AHB2_OFFSET 0X02020000UL
#define AHB2_PERIPH_BASE PERIPH_BASE + AHB2_OFFSET

#define GPIOA_OFFSET 0
#define GPIOA_BASE AHB2_PERIPH_BASE + GPIOA_OFFSET

#define GPIOB_OFFSET 0x400UL
#define GPIOB_BASE AHB2_PERIPH_BASE + GPIOB_OFFSET

#define GPIOC_OFFSET 0x800UL
#define GPIOC_BASE AHB2_PERIPH_BASE + GPIOC_OFFSET

/*
 * 2. Create required Registers
 *     a. Find Address of RCC
 *     			***RCC peripheral is used to control the internal peripherals, and RESET&CLOCK signal
 *     b. Find Address of AHB2ENR
 *     		-> Find its offset to RCC_BASE
 *     		-> Find the exact bit location to enable the clock of corresponding GPIO A/B/C
 *     c. Find Address of the Mode registers
 */
#define RCC_BASE 0x40021000UL

#define AHB2EN_R_OFFSET 0x04CUL
#define AHB2EN_R_BASE (*(volatile unsigned int *)(RCC_BASE + AHB2EN_R_OFFSET)) //first type cast it to a pointer, then deference the pointer

#define GPIOA_CLK_EN (1U<<0) //Set bit0 to 1 for the 32bits
#define GPIOB_CLK_EN (1U<<1) //Set bit1 to 1 for the 32bits
#define GPIOC_CLK_EN (1U<<2) //Set bit2 to 1 for the 32bits

//Configure mode/direction registers, search MODER in reference manual
//configure PC7, PB7, PA9 as output pins
#define MODER_OFFSET 0x00UL

#define GPIOA_MODE_R (*(volatile unsigned int *)(GPIOA_BASE + MODER_OFFSET))
#define GPIOB_MODE_R (*(volatile unsigned int *)(GPIOB_BASE + MODER_OFFSET))
#define GPIOC_MODE_R (*(volatile unsigned int *)(GPIOC_BASE + MODER_OFFSET))

#define USER_LED1_MODER (1U<<14) //Green
#define USER_LED2_MODER	(1U<<14) //Blue
#define USER_LED3_MODER (1U<<18) //Red

//configure output data register
#define ODR_OFFSET 0X14UL
#define GPIOA_OD_R (*(volatile unsigned int *)(GPIOA_BASE + ODR_OFFSET))
#define GPIOB_OD_R (*(volatile unsigned int *)(GPIOB_BASE + ODR_OFFSET))
#define GPIOC_OD_R (*(volatile unsigned int *)(GPIOC_BASE + ODR_OFFSET))

#define USER_LED1 (1U<<7)
#define USER_LED2 (1U<<7)
#define USER_LED3 (1U<<9)

/*
 * 3. Configuring Pins Using the register we created
 */

/*
 * 4. Creating Registers from Structure Members
 */
int main(void){
	/*Enable clock access to Ports A&B&C*/

	AHB2EN_R_BASE |= (GPIOA_CLK_EN | GPIOB_CLK_EN | GPIOC_CLK_EN);

	/*Configure LED pins as output pins*/
	GPIOA_MODE_R |= USER_LED3_MODER;
	GPIOA_MODE_R &= 0b11111111111101111111111111111111;
	GPIOB_MODE_R |= USER_LED2_MODER;
	GPIOB_MODE_R &= 0b11111111111111110111111111111111;
	GPIOC_MODE_R |= USER_LED1_MODER;
	GPIOC_MODE_R &= 0b11111111111111110111111111111111;
	while(1){
		/*Operate Leds */
		//1. turn on all LEDs
//		GPIOA_OD_R |= USER_LED3;
//		GPIOB_OD_R |= USER_LED2;
//		GPIOC_OD_R |= USER_LED1;
		//2. toggle LEDs
		GPIOA_OD_R ^= USER_LED3;
		GPIOB_OD_R ^= USER_LED2;
		GPIOC_OD_R ^= USER_LED1;
		for(int i = 0; i < 100000; i++){}
	}
}

