#include "PwmDriver.h"

static ledc_channel_config_t actuator24VChannel = {};
static ledc_channel_config_t actuator12VChannel = {};
static ledc_channel_config_t led12VChannel = {};
static ledc_timer_config_t ledcTimer = {};

void PwmDriver::configure()
{
    ledcTimer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcTimer.duty_resolution = LEDC_TIMER_8_BIT;
    ledcTimer.timer_num = LEDC_TIMER_0;
    ledcTimer.freq_hz = 20000;
    ledcTimer.clk_cfg = LEDC_AUTO_CLK;

    ledc_timer_config(&ledcTimer);

    actuator24VChannel.speed_mode = LEDC_LOW_SPEED_MODE;
    actuator24VChannel.channel = LEDC_CHANNEL_0;
    actuator24VChannel.intr_type = LEDC_INTR_DISABLE;
    actuator24VChannel.timer_sel = LEDC_TIMER_0;
    actuator24VChannel.duty = 255;
    actuator24VChannel.hpoint = 0;
    actuator24VChannel.gpio_num = ACTUATOR_PWM_24V;

    ledc_channel_config(&actuator24VChannel);

    actuator12VChannel.speed_mode = LEDC_LOW_SPEED_MODE;
    actuator12VChannel.channel = LEDC_CHANNEL_1;
    actuator12VChannel.intr_type = LEDC_INTR_DISABLE;
    actuator12VChannel.timer_sel = LEDC_TIMER_0;
    actuator12VChannel.duty = 255;
    actuator12VChannel.hpoint = 0;
    actuator12VChannel.gpio_num = ACTUATOR_PWM_12V;

    ledc_channel_config(&actuator12VChannel);

    led12VChannel.speed_mode = LEDC_LOW_SPEED_MODE;
    led12VChannel.channel = LEDC_CHANNEL_2;
    led12VChannel.intr_type = LEDC_INTR_DISABLE;
    led12VChannel.timer_sel = LEDC_TIMER_0;
    led12VChannel.duty = 255;
    led12VChannel.hpoint = 0;
    led12VChannel.gpio_num = LED12V_PWM;

    ledc_channel_config(&led12VChannel);
}

void PwmDriver::updateDuty(uint8_t pin, uint8_t duty)
{
    switch (pin)
    {
    case ACTUATOR_PWM_24V:
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        break;
    case ACTUATOR_PWM_12V:
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
        break;
    case LED12V_PWM:
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);
        break;
    default:
        break;
    }
}
