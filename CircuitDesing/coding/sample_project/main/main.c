#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>
#include <esp_err.h>
#include "driver/gpio.h"
/* Librerias que  Se refieren a los distintos sensores y un blink de ejemplo*/
#include "modulos/ultrasonido.h"
#include "modulos/blink.h"
#include "modulos/soil_humidity.h"
#include "modulos/dht11.h"
/* Librerias  referentes a los actuadores*/
#include"actuadores/vent.h"




void app_main()
{   /*En este se inicializan los parametros del PWM*/
    setDuty(1000);
    set_PWM();
    set_timer();
    xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 3, NULL);
    xTaskCreate(dht11,"dht11",configMINIMAL_STACK_SIZE*3,NULL, 5, NULL);
    while (1)
    {
        printf("Temperatura %fC \n",getTemp());
        vTaskDelay(200);
    }
    
    
    /*xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 3, NULL);
    xTaskCreate(blink,"blink",configMINIMAL_STACK_SIZE * 3, NULL, 4, NULL);
    xTaskCreate(soil_humidity,"soil_humidity",configMINIMAL_STACK_SIZE*3,NULL,5,NULL);*/
    //xTaskCreate(dht11,"dht11",configMINIMAL_STACK_SIZE*3,NULL, 5, NULL);
    /*while (1)
    {
        printf("Temperatura %fC \n",temp);
        vTaskDelay(100);
        if(temp>22){
            setDuty(900);
            vTaskDelay(200);
        }else{
            setDuty(0);
            
        }
    
    
    }*/
    
        //printf("Temperatura %.1f%%",temp);


}

