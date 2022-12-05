#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>
#include <esp_err.h>
#include "driver/gpio.h"
#include "ssd1306.h"
#include "font8x8_basic.h"
#include "freertos/timers.h"
#include "driver/adc.h"
#include "pantalla/pantalla.h"
/* Librerias que  Se refieren a los distintos sensores y un blink de ejemplo*/
#include "modulos/ultrasonido.h"
#include "modulos/blink.h"
#include "modulos/soil_humidity.h"
#include "modulos/dht11.h"
/* Librerias  referentes a los actuadores*/
#include"actuadores/vent.h"

#include "pantalla/pantalla.h"

#define tag "SSD1306"
// variable for the screen
// variable for the screen
SSD1306_t dev;
char string[30];
#define CONFIG_MOSI_GPIO 21
#define CONFIG_SCLK_GPIO 22
#define CONFIG_CS_GPIO 25
#define CONFIG_DC_GPIO 15
//#define CONFIG_RESET_GPIO 14
#define CONFIG_SPI3_HOST 1

void app_main()
{   
    spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
	set_timer();
	set_ADC();
    
    /*En este se inicializan los parametros del PWM*/
    // setDuty(1000);
    // set_PWM();
    // set_timer();

    // xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 3, NULL);
    // xTaskCreate(dht11,"dht11",configMINIMAL_STACK_SIZE*3,NULL, 5, NULL);
    
    // while (1)
    // {
    //     printf("Temperatura %fC \n",getTemp());
    //     vTaskDelay(200);
    // }
    
    
    // /*xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 3, NULL);
    // xTaskCreate(blink,"blink",configMINIMAL_STACK_SIZE * 3, NULL, 4, NULL);
    // xTaskCreate(soil_humidity,"soil_humidity",configMINIMAL_STACK_SIZE*3,NULL,5,NULL);*/
    // //xTaskCreate(dht11,"dht11",configMINIMAL_STACK_SIZE*3,NULL, 5, NULL);
    // /*while (1)
    // {
    //     printf("Temperatura %fC \n",temp);
    //     vTaskDelay(100);
    //     if(temp>22){
    //         setDuty(900);
    //         vTaskDelay(200);
    //     }else{
    //         setDuty(0);
            
    //     }
    
    
    // }*/
    
    //     //printf("Temperatura %.1f%%",temp);


}

