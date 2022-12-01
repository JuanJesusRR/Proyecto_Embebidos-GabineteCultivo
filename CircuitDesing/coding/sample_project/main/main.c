#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>
#include <esp_err.h>
#include "driver/gpio.h"
#include "modulos/ultrasonido.h"
#include "modulos/blink.h"
#include "modulos/soil_humidity.h"
#include "modulos/dht11.h"

#include"actuadores/vent.h"




void app_main()
{   
    set_PWM();
    set_timer();

    /*xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 3, NULL);
    xTaskCreate(blink,"blink",configMINIMAL_STACK_SIZE * 3, NULL, 4, NULL);
    xTaskCreate(soil_humidity,"soil_humidity",configMINIMAL_STACK_SIZE*3,NULL,5,NULL);*/
    xTaskCreate(dht11,"dht11",configMINIMAL_STACK_SIZE*3,NULL, 5, NULL);
    while (1)
    {
        printf("Temperatura %fC \n",temp);/* code */
        vTaskDelay(100);
        if(temp>22){
            setDuty(900);
            vTaskDelay(200);
        }else{
            setDuty(0);
            
        }
    
    
    }
    
        printf("Temperatura %.1f%%",temp);

}

