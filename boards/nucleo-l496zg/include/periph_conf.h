/*
 * Copyright (C) 2017 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_nucleo-l496zg
 * @{
 *
 * @file
 * @brief       Peripheral MCU configuration for the nucleo144-l496 board
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#include "periph_cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Clock system configuration
 * @{
 */
/* 0: no external high speed crystal available
 * else: actual crystal frequency [in Hz] */
#define CLOCK_HSE           (0)

#ifndef CLOCK_LSE
/* 0: no external low speed crystal available,
 * 1: external crystal available (always 32.768kHz) */
#define CLOCK_LSE           (1)
#endif

/* 0: enable MSI only if HSE isn't available
 * 1: always enable MSI (e.g. if USB or RNG is used)*/
#define CLOCK_MSI_ENABLE    (1)

#ifndef CLOCK_MSI_LSE_PLL
/* 0: disable Hardware auto calibration with LSE
 * 1: enable Hardware auto calibration with LSE (PLL-mode)
 * Same as with CLOCK_LSE above this defaults to 0 because LSE is
 * mandatory for MSI/LSE-trimming to work */
#define CLOCK_MSI_LSE_PLL   (0)
#endif

/* give the target core clock (HCLK) frequency [in Hz], maximum: 80MHz */
#define CLOCK_CORECLOCK     (80000000U)
/* PLL configuration: make sure your values are legit!
 *
 * compute by: CORECLOCK = (((PLL_IN / M) * N) / R)
 * with:
 * PLL_IN:  input clock, HSE or MSI @ 48MHz
 * M:       pre-divider,  allowed range: [1:8]
 * N:       multiplier,   allowed range: [8:86]
 * R:       post-divider, allowed range: [2,4,6,8]
 *
 * Also the following constraints need to be met:
 * (PLL_IN / M)     -> [4MHz:16MHz]
 * (PLL_IN / M) * N -> [64MHz:344MHz]
 * CORECLOCK        -> 80MHz MAX!
 */
#define CLOCK_PLL_M         (6)
#define CLOCK_PLL_N         (20)
#define CLOCK_PLL_R         (2)
/* peripheral clock setup */
#define CLOCK_AHB_DIV       RCC_CFGR_HPRE_DIV1
#define CLOCK_AHB           (CLOCK_CORECLOCK / 1)
#define CLOCK_APB1_DIV      RCC_CFGR_PPRE1_DIV4
#define CLOCK_APB1          (CLOCK_CORECLOCK / 4)
#define CLOCK_APB2_DIV      RCC_CFGR_PPRE2_DIV2
#define CLOCK_APB2          (CLOCK_CORECLOCK / 2)
/** @} */


/**
 * @name    Timer configuration
 * @{
 */
static const timer_conf_t timer_config[] = {
    {
        .dev      = TIM2,
        .max      = 0xffffffff,
        .rcc_mask = RCC_APB1ENR1_TIM2EN,
        .bus      = APB1,
        .irqn     = TIM2_IRQn
    },
    {
        .dev      = TIM5,
        .max      = 0xffffffff,
        .rcc_mask = RCC_APB1ENR1_TIM5EN,
        .bus      = APB1,
        .irqn     = TIM5_IRQn
    },
};

#define TIMER_0_ISR         isr_tim2
#define TIMER_1_ISR         isr_tim5

#define TIMER_NUMOF         (sizeof(timer_config) / sizeof(timer_config[0]))
/** @} */

/**
 * @name    UART configuration
 * @{
 */
static const uart_conf_t uart_config[] = {
    {
        .dev        = USART3,
        .rcc_mask   = RCC_APB1ENR1_USART3EN,
        .rx_pin     = GPIO_PIN(PORT_D, 9),
        .tx_pin     = GPIO_PIN(PORT_D, 8),
        .rx_af      = GPIO_AF7,
        .tx_af      = GPIO_AF7,
        .bus        = APB1,
        .irqn       = USART3_IRQn,
#ifdef UART_USE_DMA
        .dma_stream = 5,
        .dma_chan   = 4
#endif
    },
    {
        .dev        = USART2,
        .rcc_mask   = RCC_APB1ENR1_USART2EN,
        .rx_pin     = GPIO_PIN(PORT_D, 6),
        .tx_pin     = GPIO_PIN(PORT_D, 5),
        .rx_af      = GPIO_AF7,
        .tx_af      = GPIO_AF7,
        .bus        = APB1,
        .irqn       = USART2_IRQn,
#ifdef UART_USE_DMA
        .dma_stream = 5,
        .dma_chan   = 4
#endif
    }
};

#define UART_0_ISR          (isr_usart3)
#define UART_0_DMA_ISR      (isr_dma1_stream5)

#define UART_1_ISR          (isr_usart2)
#define UART_1_DMA_ISR      (isr_dma1_stream5)

#define UART_NUMOF          (sizeof(uart_config) / sizeof(uart_config[0]))
/** @} */

/**
 * @name    PWM configuration
 * @{
 */
static const pwm_conf_t pwm_config[] = {
    {
        .dev      = TIM1,
        .rcc_mask = RCC_APB2ENR_TIM1EN,
        .chan     = { { .pin = GPIO_PIN(PORT_E,  9) /* D6 */, .cc_chan = 0},
                      { .pin = GPIO_PIN(PORT_E, 11) /* D5 */, .cc_chan = 1},
                      { .pin = GPIO_PIN(PORT_E, 13) /* D3 */, .cc_chan = 2},
                      { .pin = GPIO_UNDEF,                    .cc_chan = 0} },
        .af       = GPIO_AF1,
        .bus      = APB2
    },
    {
        .dev      = TIM4,
        .rcc_mask = RCC_APB1ENR1_TIM4EN,
        .chan     = { { .pin = GPIO_PIN(PORT_D, 15) /* D9 */, .cc_chan = 3},
                      { .pin = GPIO_UNDEF,                    .cc_chan = 0},
                      { .pin = GPIO_UNDEF,                    .cc_chan = 0},
                      { .pin = GPIO_UNDEF,                    .cc_chan = 0} },
        .af       = GPIO_AF2,
        .bus      = APB1
    },
};

#define PWM_NUMOF           (sizeof(pwm_config) / sizeof(pwm_config[0]))
/** @} */

/**
 * @name    SPI configuration
 *
 * @note    The spi_divtable is auto-generated from
 *          `cpu/stm32_common/dist/spi_divtable/spi_divtable.c`
 * @{
 */
static const uint8_t spi_divtable[2][5] = {
    {       /* for APB1 @ 20000000Hz */
        7,  /* -> 78125Hz */
        5,  /* -> 312500Hz */
        3,  /* -> 1250000Hz */
        1,  /* -> 5000000Hz */
        0   /* -> 10000000Hz */
    },
    {       /* for APB2 @ 40000000Hz */
        7,  /* -> 156250Hz */
        6,  /* -> 312500Hz */
        4,  /* -> 1250000Hz */
        2,  /* -> 5000000Hz */
        1   /* -> 10000000Hz */
    }
};

static const spi_conf_t spi_config[] = {
    {
        .dev      = SPI1,
        .mosi_pin = GPIO_PIN(PORT_A, 7),
        .miso_pin = GPIO_PIN(PORT_A, 6),
        .sclk_pin = GPIO_PIN(PORT_A, 5),
        .cs_pin   = GPIO_UNDEF,
        .af       = GPIO_AF5,
        .rccmask  = RCC_APB2ENR_SPI1EN,
        .apbbus   = APB2
    }
};

#define SPI_NUMOF           (sizeof(spi_config) / sizeof(spi_config[0]))
/** @} */

/**
 * @name    ADC configuration
 * @{
 */
#define ADC_NUMOF           (0)
/** @} */

/**
 * @name    RTT configuration
 *
 * On the STM32Lx platforms, we always utilize the LPTIM1.
 * @{
 */
#define RTT_NUMOF           (1)
#define RTT_FREQUENCY       (1024U)             /* 32768 / 2^n */
#define RTT_MAX_VALUE       (0x0000ffff)        /* 16-bit timer */
/** @} */

/**
 * @name   RTC configuration
 * @{
 */
#define RTC_NUMOF           (1)
/** @} */


#define I2C_0_EN            1
#define I2C_1_EN            1
#define I2C_2_EN            1
#define I2C_NUMOF           (I2C_0_EN + I2C_1_EN + I2C_2_EN)
#define I2C_IRQ_PRIO        1
#define I2C_APBCLK          (CLOCK_APB1)

/* I2C 0 device configuration */
#define I2C_0_DEV           I2C1
#define I2C_0_CLKEN()       (periph_clk_en(APB1, RCC_APB1ENR1_I2C1EN))
#define I2C_0_CLKDIS()      (periph_clk_dis(APB1, RCC_APB1ENR1_I2C1EN))
#define I2C_0_EVT_IRQ       I2C1_IRQn
#define I2C_0_EVT_ISR       isr_i2c1
/* I2C 0 pin configuration */
#define I2C_0_SCL_PORT      PORT_B
#define I2C_0_SCL_PIN       8
#define I2C_0_SCL_AF        4
#define I2C_0_SCL_CLKEN()   (periph_clk_en(AHB1, RCC_AHB2ENR_GPIOBEN))
#define I2C_0_SDA_PORT      PORT_B
#define I2C_0_SDA_PIN       9
#define I2C_0_SDA_AF        4
#define I2C_0_SDA_CLKEN()   (periph_clk_en(AHB1, RCC_AHB2ENR_GPIOBEN))

/* I2C 1 device configuration */
#define I2C_1_DEV           I2C2
#define I2C_1_CLKEN()       (periph_clk_en(APB1, RCC_APB1ENR1_I2C2EN))
#define I2C_1_CLKDIS()      (periph_clk_dis(APB1, RCC_APB1ENR1_I2C2EN))
#define I2C_1_EVT_IRQ       I2C2_IRQn
#define I2C_1_EVT_ISR       isr_i2c2
/* I2C 1 pin configuration */
#define I2C_1_SCL_PORT      PORT_F
#define I2C_1_SCL_PIN       1
#define I2C_1_SCL_AF        4
#define I2C_1_SCL_CLKEN()   (periph_clk_en(AHB1, RCC_AHB2ENR_GPIOFEN))
#define I2C_1_SDA_PORT      PORT_F
#define I2C_1_SDA_PIN       0
#define I2C_1_SDA_AF        4
#define I2C_1_SDA_CLKEN()   (periph_clk_en(AHB1, RCC_AHB2ENR_GPIOFEN))

/* I2C 2 device configuration */
#define I2C_2_DEV           I2C3
#define I2C_2_CLKEN()       (periph_clk_en(APB1, RCC_APB1ENR1_I2C3EN))
#define I2C_2_CLKDIS()      (periph_clk_dis(APB1, RCC_APB1ENR1_I2C3EN))
#define I2C_2_EVT_IRQ       I2C3_IRQn
#define I2C_2_EVT_ISR       isr_i2c3
/* I2C 2 pin configuration */
#define I2C_2_SCL_PORT      PORT_C
#define I2C_2_SCL_PIN       0
#define I2C_2_SCL_AF        4
#define I2C_2_SCL_CLKEN()   (periph_clk_en(AHB1, RCC_AHB2ENR_GPIOCEN))
#define I2C_2_SDA_PORT      PORT_C
#define I2C_2_SDA_PIN       1
#define I2C_2_SDA_AF        4
#define I2C_2_SDA_CLKEN()   (periph_clk_en(AHB1, RCC_AHB2ENR_GPIOCEN))

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H */
/** @} */
