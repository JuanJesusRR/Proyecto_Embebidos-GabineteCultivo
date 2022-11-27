#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>
#include <esp_err.h>
#include "modulos/ultrasonido.h"
#include "modulos/blink.h"






void app_main()
{
    xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    xTaskCreate(blink,"blink",configMINIMAL_STACK_SIZE * 3, NULL, 4, NULL);
}

