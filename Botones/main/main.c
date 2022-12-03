#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/timers.h"
#include "driver/adc.h"

static const char *TAG = "Main";
TimerHandle_t xTimers;
int selected_buttonSW = 0;
int interval = 20;
int timer_ID = 1;
int ADC_VALUE = 0;
//Functions
esp_err_t set_timer(void);
esp_err_t set_ADC(void);
esp_err_t Read_Button(void);
esp_err_t define_button_action(void);

void app_main(void)
{
    set_ADC();
    set_timer();
}
//en VTimercallback se lee el pin, en este caso el 32 ya que es el chanel 4 del ADC1
void vTimerCallback(TimerHandle_t pxTimer)
{
    ADC_VALUE = adc1_get_raw(ADC1_CHANNEL_6); // pin 34
    Read_Button();
    define_button_action();
     
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

esp_err_t set_ADC(void)
{
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11); // el segundo parametro ayuda a ajustar el Vref y por ende la resolucion
    adc1_config_width(ADC_WIDTH_BIT_12);
    return ESP_OK;
}

 esp_err_t Read_Button(void)
{
    if (ADC_VALUE == 4095)
    {
        selected_buttonSW = 0; // any sw selected
    }

    // selected_buttonSW=number SW selected
    else if (ADC_VALUE == 0)
    {
        selected_buttonSW = 1;
    }
    else if (ADC_VALUE >= 425 && ADC_VALUE <= 440)
    {
        selected_buttonSW = 2;
    }
    else if (ADC_VALUE >= 1175 && ADC_VALUE <= 1190)
    {
        selected_buttonSW = 3;
    }
    else if (ADC_VALUE >= 1880 && ADC_VALUE <= 1895)
    {
        selected_buttonSW = 4;
    }
    else if (ADC_VALUE >= 2825 && ADC_VALUE <= 2840)
    {
        selected_buttonSW = 5;
    }
    //return selected_buttonSW;
    return ESP_OK;
}

esp_err_t define_button_action(void)
{
    switch (selected_buttonSW)
    {
    case 0:
        ESP_LOGW(TAG, "%u", ADC_VALUE);
        ESP_LOGE(TAG, "you haven't selected any SW");
        break;

    case 1:
        ESP_LOGW(TAG, "%u", ADC_VALUE);
        ESP_LOGI(TAG, "you have selected the SW%u", selected_buttonSW);
        break;

    case 2:
        ESP_LOGW(TAG, "%u", ADC_VALUE);
        ESP_LOGI(TAG, "you have selected the SW%u", selected_buttonSW);
        break;

    case 3:
        ESP_LOGW(TAG, "%u", ADC_VALUE);
        ESP_LOGI(TAG, "you have selected the SW%u", selected_buttonSW);
        break;

    case 4:
        ESP_LOGW(TAG, "%u", ADC_VALUE);
        ESP_LOGI(TAG, "you have selected the SW%u", selected_buttonSW);
        break;

    case 5:
        ESP_LOGW(TAG, "%u", ADC_VALUE);
        ESP_LOGI(TAG, "you have selected the SW%u", selected_buttonSW);
        break;

    default:
        ESP_LOGW(TAG, "%u", ADC_VALUE);
        selected_buttonSW = 0;
        break;
    }
     return ESP_OK;
}