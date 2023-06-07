#ifndef TIME_H
#define TIME_H

extern uint32_t sysclock_frequency; /*!< This is global variable. Read this variable to check what
                                     * is current sysclock frequency. If you change sysclock or
                                     * sysclock frequency, you shoudl change this variable to
                                     * appropriate value */
/**
 *
 */
typedef enum
{
  PLLMul_18 = 0,
  PLLMul_3,
  PLLMul_4,
  PLLMul_5,
  PLLMul_6,
  PLLMul_7,
  PLLMul_8,
  PLLMul_9,
  PLLMul_10,
  PLLMul_11,
  PLLMul_12,
  PLLMul_13,
  PLLMul_14,
  PLLMul_6p5,
  PLLMul_15,
  PLLMul_16
} PLLMul;

#define HSE_CLOCK_FREQUENCY 8000000 /*< High speed external clock frequency is 8MHz */
#define HSI_CLOCK_FREQUENCY 8000000 /*< High speed internal clock frequency is 8MHz */

/**
 * @fn wait
 *
 * @param ms time to wait in miliseconds
 */

void wait_ms(uint32_t ms);

/**
 * @fn wait
 *
 * @param us time to wait in micro seconds
 */

void wait_us(uint32_t us);

/**
 * @fn system_pll_clock_init
 *
 * @brief Used for configuring system clock. This function configures and initialize PLL. After
 *        PLL lock is detectd, PLL is set as system clock. As input refclock to PLL, HSE clock
 *        is used. PLL output frequency can be manipulated by setting appropriate multiplication
 *        factor. In other words, PLL can synthesize only limited set of frequencies, definied by
 *        refclock frequency and multiplication factor. For list of available multiplication
 * factors consult with PllMul enum.
 *
 * @param mul: PLL multiplication factor. By setting this value you can change PLL clock output
 *             frequency.
 */
void system_pll_clock_init(PLLMul mul);

#endif
