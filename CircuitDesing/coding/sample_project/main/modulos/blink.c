#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define led1 23
u_int8_t led_level =0;
esp_err_t init_led(void){

    gpio_reset_pin(led1);
    gpio_set_direction(led1, GPIO_MODE_OUTPUT);

    return ESP_OK;
}
esp_err_t blink_led(void){
    led_level =!led_level;
    gpio_set_level(led1, led_level);
    return ESP_OK;
}


void blink(void *pvParameters){
    init_led();
    while (true)
    {
        blink_led();
        vTaskDelay(200);
    }
    
}