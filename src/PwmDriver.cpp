#include "PwmDriver.h"
#include "Pins.h"
#include "driver/ledc.h"

static ledc_channel_config_t ledc_channel_1 = {};
static ledc_channel_config_t ledc_channel_2 = {};
static ledc_timer_config_t ledc_timer = {};

void PwmDriver::configure()
{
    // 1. Configure the LEDC Timer for Motor Control
    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_timer.duty_resolution = LEDC_TIMER_8_BIT; // 0 to 255 speed steps
    ledc_timer.timer_num = LEDC_TIMER_0;
    ledc_timer.freq_hz = 20000;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;

    ledc_timer_config(&ledc_timer);

    // 2. Configure Channel 1 (Actuator 1 / Speed Signal A)
    ledc_channel_1.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel_1.channel = LEDC_CHANNEL_0;
    ledc_channel_1.intr_type = LEDC_INTR_DISABLE;
    ledc_channel_1.timer_sel = LEDC_TIMER_0;
    ledc_channel_1.duty = 0;                         // Start at 0% speed
    ledc_channel_1.hpoint = 0;
    ledc_channel_1.gpio_num = ACTUATOR_PWM_24V;        // Output to Driver PWM Pin A

    ledc_channel_config(&ledc_channel_1);

    // 3. Configure Channel 2 (Actuator 2 / Speed Signal B)
    ledc_channel_2.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel_2.channel = LEDC_CHANNEL_1;
    ledc_channel_2.intr_type = LEDC_INTR_DISABLE;
    ledc_channel_2.timer_sel = LEDC_TIMER_0;
    ledc_channel_2.duty = 0;                         // Start at 0% speed
    ledc_channel_2.hpoint = 0;
    ledc_channel_2.gpio_num = ACTUATOR_PWM_12V;        // Output to Driver PWM Pin B

    ledc_channel_config(&ledc_channel_2);
}

void PwmDriver::updateDuty(uint8_t pin_num, uint8_t duty)
{
    switch (pin_num)
    {
    case ACTUATOR_PWM_24V:
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        break;
    case ACTUATOR_PWM_12V:
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
        break;
    default:
        break;
    }
}
