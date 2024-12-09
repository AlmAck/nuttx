/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>

#include "arm_internal.h"
#include "hardware/da1470x_pwmled.h"
#include "da1470x_pwmled.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_led_set_duty_cycle
 *
 * Description:
 *   Set PWM duty cycle for a specific LED
 *
 ****************************************************************************/

static int da1470x_led_set_duty_cycle(const struct da1470x_led_dev_s *dev,
                                  HW_LED_ID led_id,
                                  const hw_led_pwm_duty_cycle_t *duty_cycle)
{
  uint32_t pwm_conf_reg_offset;
  uint32_t reg_val;

  DEBUGASSERT(dev && duty_cycle && led_id < DA1470X_LED_MAX);

  /* Validate duty cycle parameters */

  if (duty_cycle->hw_led_pwm_start > (PWMLED_START_CYCLE_MASK >> PWMLED_START_CYCLE_POS) ||
      duty_cycle->hw_led_pwm_end > (PWMLED_STOP_CYCLE_MASK >> PWMLED_STOP_CYCLE_POS))
  {
      return -EINVAL;
  }

  /* Calculate the register offset for the specific LED */

  pwm_conf_reg_offset = DA1470X_PWMLED_LED1_PWM_CONF + (led_id * PWMLED_PWM_CONF_OFFSET_INTERVAL);

  /* Read the current register value */

  reg_val = getreg32(pwm_conf_reg_offset);

  /* Modify the START_CYCLE field */

  reg_val &= ~PWMLED_START_CYCLE_MASK;
  reg_val |= (duty_cycle->hw_led_pwm_start << PWMLED_START_CYCLE_POS) &
              PWMLED_START_CYCLE_MASK;

  /* Modify the STOP_CYCLE field */

  reg_val &= ~PWMLED_STOP_CYCLE_MASK;
  reg_val |= (duty_cycle->hw_led_pwm_end << PWMLED_STOP_CYCLE_POS) &
              PWMLED_STOP_CYCLE_MASK;

  /* Write back the modified register value */

  putreg32(reg_val, pwm_conf_reg_offset);

  return 0;
}

/****************************************************************************
 * Name: da1470x_led_set_load_sel
 *
 * Description:
 *   Set Set load select for a specific LED
 *
 ****************************************************************************/

static int da1470x_led_set_load_sel(const struct da1470x_led_dev_s *dev,
                                HW_LED_ID led_id, uint8_t load_sel)
{
  uint8_t pos;
  uint32_t load_sel_reg;

  DEBUGASSERT(dev && led_id < DA1470X_LED_MAX);

  // if (load_sel > (LED_LOAD_SEL_MASK >> LED_LOAD_SEL_POS))
  // {
  //     return -EINVAL;
  // }

  /* Calculate the bit position for the specific LED */

  pos = led_id * PWMLED_LED_LOAD_OFFSET_INTERVAL;

  /* Read the current LOAD_SEL register value */

  load_sel_reg = getreg32(DA1470X_PWMLED_LED_LOAD_SEL);

  /* Clear the existing LOAD_SEL bits for the LED */

  load_sel_reg &= ~(PWMLED_LED1_LOAD_SEL_MASK << pos);

  /* Set the new LOAD_SEL value */

  load_sel_reg |= ((uint32_t)(load_sel & 0x3) << pos);

  /* Write back the modified LOAD_SEL register */

  putreg32(load_sel_reg, DA1470X_PWMLED_LED_LOAD_SEL);

  return 0;
}

/****************************************************************************
 * Name: da1470x_led_get_duty_cycle
 *
 * Description:
 *   Get PWM duty cycle for a specific LED
 *
 ****************************************************************************/

static void da1470x_led_get_duty_cycle(const struct da1470x_led_dev_s *dev,
                                  HW_LED_ID led_id,
                                  hw_led_pwm_duty_cycle_t *duty_cycle)
{
  uint32_t pwm_conf_reg_offset;
  uint32_t reg_val;

  DEBUGASSERT(dev && duty_cycle && led_id < DA1470X_LED_MAX);

  /* Calculate the register offset for the specific LED */

  pwm_conf_reg_offset = DA1470X_PWMLED_LED1_PWM_CONF + (led_id * PWMLED_PWM_CONF_OFFSET_INTERVAL);

  /* Read the register value */

  reg_val = getreg32(pwm_conf_reg_offset);

  /* Extract the START_CYCLE and STOP_CYCLE fields */

  duty_cycle->hw_led_pwm_start = (reg_val & PWMLED_START_CYCLE_MASK) >> PWMLED_START_CYCLE_POS;
  duty_cycle->hw_led_pwm_end   = (reg_val & PWMLED_STOP_CYCLE_MASK) >> PWMLED_STOP_CYCLE_POS;
}

/****************************************************************************
 * Name: da1470x_led_get_load_sel
 *
 * Description:
 *   Get load select for a specific LED
 *
 ****************************************************************************/

static void da1470x_led_get_load_sel(const struct da1470x_led_dev_s *dev,
                                HW_LED_ID led_id, uint8_t *load_sel)
{
    uint8_t pos;
    uint32_t load_sel_reg;

    DEBUGASSERT(dev && load_sel && led_id < DA1470X_LED_MAX);

    /* Calculate the bit position for the specific LED */

    pos = led_id * 2;  /* Assuming each LED uses 2 bits for LOAD_SEL */

    /* Read the current LOAD_SEL register value */

    load_sel_reg = getreg32(DA1470X_PWMLED_LED_LOAD_SEL);

    /* Extract the LOAD_SEL value for the specific LED */

    *load_sel = (load_sel_reg >> pos) & 0x3;
}

/****************************************************************************
 * Name: da1470x_led_set_current_trim
 *
 * Description:
 *   Set current trim for a specific LED
 *
 ****************************************************************************/

static int da1470x_led_set_current_trim(const struct da1470x_led_dev_s *dev,
                                    HW_LED_ID led_id, uint32_t trim)
{
  uint32_t reg_val;

  DEBUGASSERT(dev && led_id < DA1470X_LED_MAX);

  // if (trim > (LED_CURR_TRIM_MASK >> LED_CURR_TRIM_POS))
  //   {
  //     return -EINVAL;
  //   }


  /* Read the current CURRENT_TRIM register value */

  reg_val = getreg32(DA1470X_PWMLED_LED_CURR_TRIM);

  /* Modify the trim value based on LED ID */

  switch (led_id)
    {
      case HW_LED_ID_LED_1:
        reg_val &= ~(PWMLED_LED1_CURR_TRIM_MASK);
        reg_val |= ((trim & 0xff) << PWMLED_LED1_CURR_TRIM_POS);
        break;

      case HW_LED_ID_LED_2:
        reg_val &= ~(PWMLED_LED2_CURR_TRIM_MASK);
        reg_val |= ((trim & 0xff) << PWMLED_LED2_CURR_TRIM_POS);
        break;

      case HW_LED_ID_LED_3:
        reg_val &= ~(PWMLED_LED3_CURR_TRIM_MASK);
        reg_val |= ((trim & 0xff) << PWMLED_LED3_CURR_TRIM_POS);
        break;

      default:
        return -EINVAL;
    }

  /* Write back the modified CURRENT_TRIM register */

  putreg32(reg_val, DA1470X_PWMLED_LED_CURR_TRIM);

  return 0;
}

/****************************************************************************
 * Name: da1470x_led_set_frequency
 *
 * Description:
 *   Set PWM frequency
 *
 ****************************************************************************/

static int da1470x_led_set_frequency(const struct da1470x_led_dev_s *dev,
                                 uint32_t freq)
{
  uint32_t freq_reg;
  uint32_t max_div;
  uint32_t max_per;
  uint32_t div;
  uint32_t per;

  DEBUGASSERT(dev);

  if (freq < 31 || freq > 7800)
    {
      return -EINVAL;
    }

  /* Read the current FREQUENCY register value */

  freq_reg = getreg32(DA1470X_PWMLED_LEDS_FREQUENCY);

  /* Extract the prescale and period masks */

  max_div = ((freq_reg & PWMLED_PWM_LEDS_PRESCALE_MASK) >> PWMLED_PWM_LEDS_PRESCALE_POS) + 1;
  max_per = ((freq_reg & PWMLED_PWM_LEDS_PERIOD_MASK) >> PWMLED_PWM_LEDS_PERIOD_POS) + 1;

  /* Calculate the prescale divider */

  div = 1 + (HW_LED_CLK_CYCLES / (max_per * freq));
  if (div > max_div)
    {
      div = max_div;
    }

  /* Calculate the period */

  per = HW_LED_CLK_CYCLES / (div * freq);
  if (per > max_per)
    {
      per = max_per;
    }

  /* Update the FREQUENCY register */

  freq_reg &= ~(PWMLED_PWM_LEDS_PRESCALE_MASK | PWMLED_PWM_LEDS_PERIOD_MASK);
  freq_reg |= ((div - 1) << PWMLED_PWM_LEDS_PRESCALE_POS) & PWMLED_PWM_LEDS_PRESCALE_MASK;
  freq_reg |= ((per - 1) << PWMLED_PWM_LEDS_PERIOD_POS) & PWMLED_PWM_LEDS_PERIOD_MASK;

  putreg32(freq_reg, DA1470X_PWMLED_LEDS_FREQUENCY);

  return 0;
}

/****************************************************************************
 * Name: da1470x_led_init
 *
 * Description:
 *   Initialize the LED driver
 *
 ****************************************************************************/

static void da1470x_led_init(const struct da1470x_led_dev_s *dev,
                        const struct hw_led_config *conf)
{
  int ret;

  DEBUGASSERT(dev && conf);

  ret = da1470x_set_frequency(dev, conf->leds_pwm_frequency);
  if (ret < 0)
    {
      return ret;
    }

  for (int i = 0; i < DA1470X_LED_MAX; i++)
    {
      ret = da1470x_set_duty_cycle_pct_off(dev, i,
                                           conf->leds_pwm_duty_cycle[i],
                                           conf->leds_pwm_start_cycle[i]);
      if (ret < 0)
        {
          return ret;
        }
    }
}

/****************************************************************************
 * Name: da1470x_set_duty_cycle_pct_off
 *
 * Description:
 *   Set PWM duty cycle percentage and offset for a specific LED
 *
 ****************************************************************************/

static int da1470x_set_duty_cycle_pct_off(const struct da1470x_led_dev_s *dev,
                                         HW_LED_ID led_id,
                                         uint32_t dc, uint32_t off)
{
  DEBUGASSERT(dev && led_id < DA1470X_LED_MAX);

  if (dc > 10000 || off > 10000)
    {
      return -EINVAL;
    }

  hw_led_pwm_duty_cycle_t duty_cycle;
  uint32_t period = 0;

  /* Read the current period from FREQUENCY register */

  uint32_t freq_reg = getreg32(DA1470X_PWMLED_LEDS_FREQUENCY);
  uint32_t per = ((freq_reg & PWMLED_PWM_LEDS_PERIOD_MASK) >> PWMLED_PWM_LEDS_PERIOD_POS) + 1;
  period = per - 1;

  if (dc == 10000)
    {
      duty_cycle.hw_led_pwm_start = 0;
      duty_cycle.hw_led_pwm_end   = 0;
    }
  else
    {
      uint32_t offset = (period * off) / 10000;
      /* uint32_t won't be overflowed here, do not use floating points round */
      uint32_t duration = (1 + period) * dc;
      duration = (duration + 5000) / 10000;

      if (duration == 0 && dc != 0)
        {
          duration = 1;
        }

      duty_cycle.hw_led_pwm_start = (uint16_t)offset;
      duty_cycle.hw_led_pwm_end   = (uint16_t)((offset + duration) % (period + 1));
    }

  return da1470x_set_duty_cycle(dev, led_id, &duty_cycle);
}

/****************************************************************************
 * Name: da1470x_set_duty_cycle_pct_off
 *
 * Description:
 *   Initalize the DA1470x LED driver
 *
 ****************************************************************************/

int da1470x_led_initialize(const struct hw_led_config *conf)
{
  const struct da1470x_led_dev_s *dev = &g_da1470x_led_dev;
  int ret;

  DEBUGASSERT(conf != NULL);

  /* Initialize the driver */

  ret = da1470x_led_init(dev, conf);
  if (ret < 0)
    {
      return ret;
    }

  /* Register the character device */

  ret = register_driver(DA1470X_LED_DEVPATH, &g_da1470x_led_fops,
                        0666, dev);
  if (ret < 0)
    {
      lldbg("Failed to register %s: %d\n", DA1470X_LED_DEVPATH, ret);
      return ret;
    }

  return 0;
}