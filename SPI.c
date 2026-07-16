/******************************************************
 * STM32F401RE SPI FULL DUPLEX DEMO
 *
 * SPI1 : MASTER
 * SPI2 : SLAVE
 *
 * Master TX : HELLO
 * Slave  TX : MAVEN
 *
 * After Communication
 * MasterRxBuffer = MAVEN
 * SlaveRxBuffer  = HELLO
 ******************************************************/

#include <stdint.h>

/**************** RCC ****************/
#define RCC_BASE        0x40023800UL

#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x44))

/**************** GPIOA ****************/
#define GPIOA_BASE      0x40020000UL

#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL      (*(volatile uint32_t *)(GPIOA_BASE + 0x20))

/**************** GPIOB ****************/
#define GPIOB_BASE      0x40020400UL

#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_AFRH      (*(volatile uint32_t *)(GPIOB_BASE + 0x24))

/**************** SPI1 ****************/
#define SPI1_BASE       0x40013000UL

#define SPI1_CR1        (*(volatile uint32_t *)(SPI1_BASE + 0x00))
#define SPI1_SR         (*(volatile uint32_t *)(SPI1_BASE + 0x08))
#define SPI1_DR         (*(volatile uint32_t *)(SPI1_BASE + 0x0C))

/**************** SPI2 ****************/
#define SPI2_BASE       0x40003800UL

#define SPI2_CR1        (*(volatile uint32_t *)(SPI2_BASE + 0x00))
#define SPI2_SR         (*(volatile uint32_t *)(SPI2_BASE + 0x08))
#define SPI2_DR         (*(volatile uint32_t *)(SPI2_BASE + 0x0C))

/**************** Function Prototypes ****************/
void SPI1_Master_Init(void);
void SPI2_Slave_Init(void);

/**************** Buffers ****************/
char MasterTxBuffer[] = "HELLO";
char SlaveTxBuffer[]  = "MAVEN";

char MasterRxBuffer[20] = {0};
char SlaveRxBuffer[20]  = {0};

/**************** MAIN ****************/
int main(void)
{
    int i = 0;

    SPI1_Master_Init();
    SPI2_Slave_Init();

    while(MasterTxBuffer[i] != '\0')
    {
        /* Wait until TX buffer is empty */
        while(!(SPI1_SR & (1 << 1)));

        /* Load slave data */
        SPI2_DR = SlaveTxBuffer[i];

        /* Master transmits */
        SPI1_DR = MasterTxBuffer[i];

        /* Wait until Master receives */
        while(!(SPI1_SR & (1 << 0)));

        /* Wait until Slave receives */
        while(!(SPI2_SR & (1 << 0)));

        /* Read received data */
        MasterRxBuffer[i] = SPI1_DR;
        SlaveRxBuffer[i]  = SPI2_DR;

        i++;
    }

    while(1);
}

/**************** SPI1 MASTER INIT ****************/
void SPI1_Master_Init(void)
{
    /* Enable GPIOA Clock */
    RCC_AHB1ENR |= (1 << 0);

    /* Enable SPI1 Clock */
    RCC_APB2ENR |= (1 << 12);

    /* PA5 PA6 PA7 -> Alternate Function */
    GPIOA_MODER &= ~(0x3F << 10);
    GPIOA_MODER |=  (0x2A << 10);

    /* AF5 */
    GPIOA_AFRL &= ~(0xFFF << 20);
    GPIOA_AFRL |=  (5 << 20);
    GPIOA_AFRL |=  (5 << 24);
    GPIOA_AFRL |=  (5 << 28);

    /* Master Mode */
    SPI1_CR1 |= (1 << 2);

    /* Baud Rate = fPCLK/16 */
    SPI1_CR1 |= (3 << 3);

    /* Software Slave Management */
    SPI1_CR1 |= (1 << 9);
    SPI1_CR1 |= (1 << 8);

    /* Enable SPI */
    SPI1_CR1 |= (1 << 6);
}

/**************** SPI2 SLAVE INIT ****************/
void SPI2_Slave_Init(void)
{
    /* Enable GPIOB Clock */
    RCC_AHB1ENR |= (1 << 1);

    /* Enable SPI2 Clock */
    RCC_APB1ENR |= (1 << 14);

    /* PB13 PB14 PB15 -> Alternate Function */
    GPIOB_MODER &= ~(0x3F << 26);
    GPIOB_MODER |=  (0x2A << 26);

    /* AF5 */
    GPIOB_AFRH &= ~(0xFFF << 20);
    GPIOB_AFRH |=  (5 << 20);
    GPIOB_AFRH |=  (5 << 24);
    GPIOB_AFRH |=  (5 << 28);

    /* Slave Mode */
    SPI2_CR1 = 0;

    /* Enable SPI2 */
    SPI2_CR1 |= (1 << 6);
}
