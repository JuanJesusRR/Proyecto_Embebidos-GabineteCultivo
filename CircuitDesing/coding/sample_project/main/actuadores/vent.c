#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/timers.h"
#include "driver/ledc.h"
static const char *TAG = "Main";
//#define led1 32
//uint8_t led1_level = 0;
TimerHandle_t xTimers;
int interval = 1000;
int timer_ID = 1;
int duty = 0;
esp_err_t init_led(void);
esp_err_t blink_led1(void);
esp_err_t set_timer(void);
esp_err_t set_PWM(void);
esp_err_t set_PWM_duty_cycle(void);



void setDuty(int dutycycle){
    duty=dutycycle;
}
void vTimerCallback(TimerHandle_t pxTimer)
{
    
    if(duty>=1023)
    {
        duty=0;
    }
    set_PWM_duty_cycle();
}


esp_err_t set_timer(void)
{
    ESP_LOGI(TAG, "timer init configuration");
    xTimers = xTimerCreate("Timer",                 // Just a text name, not used by the kernel.
                           pdMS_TO_TICKS(interval), // The timer period in ticks.
                           pdTRUE,                  // The timers will auto-reload themselves when they expire.
                           (void *)timer_ID,        // Assign each timer a unique id equal to its array index.
                           vTimerCallback           // Each timer calls the same callback when it expires.
    );

    if (xTimers == NULL)
    {
        ESP_LOGW(TAG, "The timer was not created.");
    }
    else
    {
        // Start the timer.  No block time is specified, and even if one was
        // it would be ignored because the scheduler has not yet been
        // started.
        if (xTimerStart(xTimers, 0) != pdPASS)
        {
            ESP_LOGE(TAG, "The timer could not be set into the Active state.");
        }
    }

    return ESP_OK;
}

esp_err_t set_PWM(void)
{
    ledc_channel_config_t PWM_channel_conf = {0};
    PWM_channel_conf.gpio_num = 18;
    PWM_channel_conf.speed_mode = LEDC_HIGH_SPEED_MODE;
    PWM_channel_conf.channel = LEDC_CHANNEL_0;
    PWM_channel_conf.intr_type = LEDC_TIMER_0;
    PWM_channel_conf.duty = 0;
    ledc_channel_config(&PWM_channel_conf);

    ledc_timer_config_t timer_conf_PWM = {0};
    timer_conf_PWM.speed_mode = LEDC_HIGH_SPEED_MODE;
    timer_conf_PWM.duty_resolution = LEDC_TIMER_10_BIT;
    timer_conf_PWM.bit_num = LEDC_TIMER_10_BIT;
    timer_conf_PWM.timer_num = LEDC_TIMER_0;
    timer_conf_PWM.freq_hz = 1000;
    ledc_timer_config(&timer_conf_PWM);

    return ESP_OK;
}
esp_err_t set_PWM_duty_cycle(void)
{
    //ESP_LOGI(TAG, "The timer is counting, the dutty cycle actual is%u", (duty*100/1023));
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

    return ESP_OK;
}