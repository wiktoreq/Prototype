#include "PwmDriver.h"

#include "Pins.h"
#include "driver/ledc.h"

namespace
{
constexpr uint32_t PWM_FREQUENCY_HZ = 20000;
constexpr uint8_t INITIAL_PWM_DUTY = 255;

void configureTimer()
{
    ledc_timer_config_t timer = {};
    timer.speed_mode = LEDC_LOW_SPEED_MODE;
    timer.duty_resolution = LEDC_TIMER_8_BIT;
    timer.timer_num = LEDC_TIMER_0;
    timer.freq_hz = PWM_FREQUENCY_HZ;
    timer.clk_cfg = LEDC_AUTO_CLK;

    ledc_timer_config(&timer);
}

void configureChannel(ledc_channel_t channelNumber, uint8_t outputPin)
{
    ledc_channel_config_t channel = {};
    channel.speed_mode = LEDC_LOW_SPEED_MODE;
    channel.channel = channelNumber;
    channel.intr_type = LEDC_INTR_DISABLE;
    channel.timer_sel = LEDC_TIMER_0;
    channel.duty = INITIAL_PWM_DUTY;
    channel.hpoint = 0;
    channel.gpio_num = outputPin;

    ledc_channel_config(&channel);
}

void writeChannelDuty(ledc_channel_t channel, uint8_t duty)
{
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}
}

void PwmDriver::configure()
{
    configureTimer();
    configureChannel(LEDC_CHANNEL_0, ACTUATOR_PWM_24V);
    configureChannel(LEDC_CHANNEL_1, ACTUATOR_PWM_12V);
    configureChannel(LEDC_CHANNEL_2, LED12V_PWM);
}

uint8_t PwmDriver::percentToDuty(uint8_t percent)
{
    if (percent >= 100) {
        return 255;
    }

    uint32_t scaledDuty = static_cast<uint32_t>(percent) * 255;
    scaledDuty = scaledDuty / 100;
    return static_cast<uint8_t>(scaledDuty);
}

void PwmDriver::updateDuty(uint8_t pin, uint8_t duty)
{
    if (pin == ACTUATOR_PWM_24V) {
        writeChannelDuty(LEDC_CHANNEL_0, duty);
        return;
    }

    if (pin == ACTUATOR_PWM_12V) {
        writeChannelDuty(LEDC_CHANNEL_1, duty);
        return;
    }

    if (pin == LED12V_PWM) {
        writeChannelDuty(LEDC_CHANNEL_2, duty);
    }
}
