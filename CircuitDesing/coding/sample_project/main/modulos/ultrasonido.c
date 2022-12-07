#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>
#include <esp_err.h>


#define TRIGGER_GPIO 23
#define ECHO_GPIO 16
#define MAX_DISTANCE_CM 500 // 5m max

float distance=0;
float getDis(){
    return distance;
}

void ultrasonic_test(void *pvParameters)
{
    ultrasonic_sensor_t sensor = {
        .trigger_pin = TRIGGER_GPIO,
        .echo_pin = ECHO_GPIO
    };

    ultrasonic_init(&sensor);

    while (true)
    {
        
        esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);
        if (res != ESP_OK)
        {
            //printf("Error %d: ", res);
            switch (res)
            {
                case ESP_ERR_ULTRASONIC_PING:
                    //printf("Cannot ping (device is in invalid state)\n");
                    distance=distance;
                    vTaskDelay(pdMS_TO_TICKS(500));
                    break;
                case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                    //printf("Ping timeout (no device found)\n");
                    distance=distance;
                    vTaskDelay(pdMS_TO_TICKS(500));
                    break;
                case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                    //printf("Echo timeout (i.e. distance too big)\n");
                    distance=distance;
                    vTaskDelay(pdMS_TO_TICKS(500));
                    break;
                default:
                    //printf("%s\n", esp_err_to_name(res));
                    distance=distance;
                    vTaskDelay(pdMS_TO_TICKS(500));
            }
        }
        else
           //printf("Distance: %0.04f m\n", distance);
            
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}