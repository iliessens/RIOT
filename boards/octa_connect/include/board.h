
#ifndef OCTA_CONNECT_H
#define OCTA_CONNECT_H

void board_init(void);

#if defined(CPU_MODEL_STM32L496ZG)
#define LED0_PORT           GPIOC
#define LED0_PIN            GPIO_PIN(PORT_C, 7)
#define LED0_MASK           (1 << 7)
#else
#define LED0_PORT           GPIOB
#define LED0_PIN            GPIO_PIN(PORT_B, 0)
#define LED0_MASK           (1 << 0)
#endif
#define LED0_ON             (LED0_PORT->BSRR = LED0_MASK)
#define LED0_OFF            (LED0_PORT->BSRR = (LED0_MASK << 16))
#define LED0_TOGGLE         (LED0_PORT->ODR  ^= LED0_MASK)

#define LED1_PIN            GPIO_PIN(PORT_B, 7)
#define LED1_MASK           (1 << 7)
#define LED1_ON             (GPIOB->BSRR = LED1_MASK)
#define LED1_OFF            (GPIOB->BSRR = (LED1_MASK << 16))
#define LED1_TOGGLE         (GPIOB->ODR  ^= LED1_MASK)

#define LED2_PIN            GPIO_PIN(PORT_B, 14)
#define LED2_MASK           (1 << 14)
#define LED2_ON             (GPIOB->BSRR = LED2_MASK)
#define LED2_OFF            (GPIOB->BSRR = (LED2_MASK << 16))
#define LED2_TOGGLE         (GPIOB->ODR  ^= LED2_MASK)

/* the Nucleo144 boards always use LED0, as there is no dual use of its pin */
#define AUTO_INIT_LED0
/** @} */

/**
 * @brief   User button
 * @{
 */
#define BTN0_PIN            GPIO_PIN(PORT_C, 13)
#define BTN0_MODE           GPIO_IN_PD

#endif
