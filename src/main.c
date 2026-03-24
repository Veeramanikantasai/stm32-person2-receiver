#include "stm32l4xx.h"

void delay(volatile uint32_t dly);
void uart2_init(void);
void uart2_send_char(char c);
void uart2_send_string(const char *s);
void led_init(void);

int main(void)
{
    uart2_init();
    led_init();

    while (1)
    {
        uart2_send_string("=== PERSON 2 READY ===\r\n");
        uart2_send_string("Received: Hello\r\n");
        uart2_send_string("Transmitted: how are you?\r\n\r\n");

        // TOGGLE LED (BEST METHOD)
        GPIOB->ODR ^= (1 << 3);

        delay(2000000);
    }
}

void led_init(void)
{
    RCC->AHB2ENR |= (1 << 1);   // GPIOB enable

    // PB3 OUTPUT MODE
    GPIOB->MODER &= ~(3 << (2 * 3));
    GPIOB->MODER |=  (1 << (2 * 3));
}

void uart2_init(void)
{
    RCC->AHB2ENR  |= (1 << 0);
    RCC->APB1ENR1 |= (1 << 17);

    GPIOA->MODER &= ~(3 << (2 * 2));
    GPIOA->MODER |=  (2 << (2 * 2));

    GPIOA->AFR[0] &= ~(0xF << (4 * 2));
    GPIOA->AFR[0] |=  (7 << (4 * 2));

    USART2->CR1 = 0;
    USART2->BRR = 4000000 / 9600;

    USART2->CR1 |= (1 << 3);
    USART2->CR1 |= (1 << 0);
}

void uart2_send_char(char c)
{
    while ((USART2->ISR & (1 << 7)) == 0);
    USART2->TDR = c;
}

void uart2_send_string(const char *s)
{
    while (*s)
    {
        uart2_send_char(*s++);
    }
}

void delay(volatile uint32_t dly)
{
    while (dly--);
}