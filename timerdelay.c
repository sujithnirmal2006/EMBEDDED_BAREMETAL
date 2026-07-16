#include <stdint.h>

/**************** RCC ****************/
#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))

/**************** GPIOA ****************/
#define GPIOA_BASE      0x40020000UL
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

/**************** TIM2 ****************/
#define TIM2_BASE       0x40000000UL
#define TIM2_CR1        (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_SR         (*(volatile uint32_t *)(TIM2_BASE + 0x10))
#define TIM2_EGR        (*(volatile uint32_t *)(TIM2_BASE + 0x14))
#define TIM2_CNT        (*(volatile uint32_t *)(TIM2_BASE + 0x24))
#define TIM2_PSC        (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR        (*(volatile uint32_t *)(TIM2_BASE + 0x2C))

/************** Timer Initialization **************/
void TIM2_Init(void)
{
    /* Enable TIM2 Clock */
    RCC_APB1ENR |= (1 << 0);

    /* Timer Clock = 16 MHz
       Prescaler = 15999
       Timer Tick = 1 ms
    */
    TIM2_PSC = 15999;

    /* 500 ms delay */
    TIM2_ARR = 499;

    /* Load PSC and ARR */
    TIM2_EGR |= (1 << 0);
}

/************** Delay Function **************/
void delay_500ms(void)
{
    /* Reset Counter */
    TIM2_CNT = 0;

    /* Clear Update Flag */
    TIM2_SR &= ~(1 << 0);

    /* Start Timer */
    TIM2_CR1 |= (1 << 0);

    /* Wait until timer overflows */
    while(!(TIM2_SR & (1 << 0)));

    /* Stop Timer */
    TIM2_CR1 &= ~(1 << 0);

    /* Clear Update Flag */
    TIM2_SR &= ~(1 << 0);
}

/************** Main **************/
int main(void)
{
    /* Enable GPIOA Clock */
    RCC_AHB1ENR |= (1 << 0);

    /* PA5 Output */
    GPIOA_MODER &= ~(3 << (5 * 2));
    GPIOA_MODER |=  (1 << (5 * 2));

    /* Initialize Timer */
    TIM2_Init();

    while(1)
    {
        /* Toggle LED */
        GPIOA_ODR ^= (1 << 5);

        /* Wait 500 ms */
        delay_500ms();
    }
}
