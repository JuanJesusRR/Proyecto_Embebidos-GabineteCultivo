#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define ventout 18
#define humidificator 12
#define ventin 19

esp_err_t init_ventout(void){

    gpio_reset_pin(ventout);
    gpio_set_direction(ventout, GPIO_MODE_OUTPUT);

    return ESP_OK;
}
esp_err_t ventoutControl(bool i){

    gpio_set_level(ventout, i);
    return ESP_OK;
}

esp_err_t init_humid(void){

    gpio_reset_pin(humidificator);
    gpio_set_direction(humidificator, GPIO_MODE_OUTPUT);

    return ESP_OK;
}
esp_err_t humidControl(bool i){

    gpio_set_level(humidificator, i);
    return ESP_OK;
}

esp_err_t init_ventin(void){

    gpio_reset_pin(ventin);
    gpio_set_direction(ventin, GPIO_MODE_OUTPUT);

    return ESP_OK;
}
esp_err_t ventinControl(bool i){

    gpio_set_level(ventin, i);
    return ESP_OK;
}