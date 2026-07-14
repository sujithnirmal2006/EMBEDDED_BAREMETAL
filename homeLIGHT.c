#include <stdint.h>

#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)

#define GPIOA_MODER (*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR   (*(volatile uint32_t *)0x40020014)

#define GPIOC_MODER (*(volatile uint32_t *)0x40020800)
#define GPIOC_IDR   (*(volatile uint32_t *)0x40020810)

int main(void)
{
    RCC_AHB1ENR |= (1 << 0);
    RCC_AHB1ENR |= (1 << 2);
    GPIOA_MODER &= ~(3 << (10));
    GPIOA_MODER |=  (1 << (10));
    GPIOC_MODER &= ~(3 << (26));

    while(1)
    {
        if(!(GPIOC_IDR & (1 << 13)))
        {
            GPIOA_ODR ^= (1 << 5);
            while(!(GPIOC_IDR & (1 << 13)));

        }
    }
}
