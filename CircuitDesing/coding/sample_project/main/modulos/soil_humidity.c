#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_PIN  GPIO_NUM_35

void soil_humidity(void *pvParameters){
    
       
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    while(1) {       
        if (gpio_get_level(BUTTON_PIN) == 0) {  // If button is pressed
            
            printf("Humnedad OK\n");
        } else {
        
            printf("Sin humedad\n");
                    
        }
        vTaskDelay(3000); // Add 1 tick delay (10 ms) so that current task does not starve idle task and trigger watchdog timer
    }
}