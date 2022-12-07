#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/timers.h"
#include "driver/adc.h"
static const char *TAG = "Main";
TimerHandle_t xTimers;
int interval2 = 500;
int timer_ID2 = 1;
int ADC_VALUE2 = 0;
int ADC_VALUE3 =0;

int getLight(){
    return ADC_VALUE3;
}

int getSoil_Humidity(){
    int soil_humid = ADC_VALUE2;
    return soil_humid;
}
void vTimerCallback2(TimerHandle_t pxTimer)
{
    ADC_VALUE2 = adc1_get_raw(ADC1_CHANNEL_7); // pin 33
    //ESP_LOGI(TAG, "The ADC value is:%u", ADC_VALUE2);
    ADC_VALUE3 = adc1_get_raw(ADC1_CHANNEL_5);
}


esp_err_t set_timer2(void)
{
    ESP_LOGI(TAG, "timer init configuration");
    xTimers = xTimerCreate("Timer",                 // Just a text name, not used by the kernel.
                           pdMS_TO_TICKS(interval2), // The timer period in ticks.
                           pdTRUE,                  // The timers will auto-reload themselves when they expire.
                           (void *)timer_ID2,        // Assign each timer a unique id equal to its array index.
                           vTimerCallback2           // Each timer calls the same callback when it expires.
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


esp_err_t set_ADC2(void)
{
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_6);//el segundo parametro ayuda a ajustar el Vref y por ende la resolucion 
    adc1_config_width(ADC_WIDTH_BIT_12);
    return ESP_OK;
}
esp_err_t set_Luz(void){
    adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_6);//el segundo parametro ayuda a ajustar el Vref y por ende la resolucion 
    adc1_config_width(ADC_WIDTH_BIT_12);
     return ESP_OK;
}