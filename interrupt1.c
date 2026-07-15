#include <stdint.h>

#define RCC_BASE        0x40023800UL
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x44))

#define GPIOA_BASE      0x40020000UL
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

#define GPIOB_BASE      0x40020400UL
#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_ODR       (*(volatile uint32_t *)(GPIOB_BASE + 0x14))

#define GPIOC_BASE      0x40020800UL
#define GPIOC_MODER     (*(volatile uint32_t *)(GPIOC_BASE + 0x00))

#define SYSCFG_BASE     0x40013800UL
#define SYSCFG_EXTICR4  (*(volatile uint32_t *)(SYSCFG_BASE + 0x14))

#define EXTI_BASE       0x40013C00UL
#define EXTI_IMR        (*(volatile uint32_t *)(EXTI_BASE + 0x00))
#define EXTI_RTSR       (*(volatile uint32_t *)(EXTI_BASE + 0x08))   // rising edge (B1 is active-HIGH)
#define EXTI_PR         (*(volatile uint32_t *)(EXTI_BASE + 0x14))

#define NVIC_ISER1      (*(volatile uint32_t *)0xE000E104)

void delay(void)
{
    for(volatile uint32_t i=0; i<800000; i++);
}

/* Everything button-related lives HERE, outside main() */
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_PR & (1<<13))
    {
        GPIOB_ODR |= (1<<10);
        delay();
        GPIOB_ODR &= ~(1<<10);

        EXTI_PR |= (1<<13);   // clear pending bit
    }
}

int main(void)
{
    RCC_AHB1ENR |= (1<<0);
    RCC_AHB1ENR |= (1<<1);
    RCC_AHB1ENR |= (1<<2);
    RCC_APB2ENR |= (1<<14);

    GPIOA_MODER &= ~(3<<(8*2));
    GPIOA_MODER |=  (1<<(8*2));

    GPIOB_MODER &= ~(3<<(10*2));
    GPIOB_MODER |=  (1<<(10*2));

    GPIOC_MODER &= ~(3<<(13*2));

    SYSCFG_EXTICR4 &= ~(0xF<<4);
    SYSCFG_EXTICR4 |=  (2<<4);

    EXTI_RTSR |= (1<<13);   // rising edge — matches B1's active-HIGH wiring
    EXTI_IMR  |= (1<<13);

    NVIC_ISER1 |= (1<<8);

    /* main() only does its own job — no flag, no if-checks */
    while(1)
    {
        GPIOA_ODR ^= (1<<8);
        delay();
    }
}
