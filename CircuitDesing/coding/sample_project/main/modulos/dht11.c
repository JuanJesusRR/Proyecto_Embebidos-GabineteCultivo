#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <dht.h>
float hum;
float temp;
dht_sensor_type_t sen =0;



void dht11(void *pvParameters){
/*DHT11_init(GPIO_NUM_17);

    while(1) {
        printf("Temperature is %d \n", DHT11_read().temperature);
        printf("Humidity is %d\n", DHT11_read().humidity);
        printf("Status code is %d\n", DHT11_read().status);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }*/
    while(1){
        
  dht_read_float_data(sen,GPIO_NUM_17,&hum , &temp);
  //printf("Humidity: %.1f%% Temp: %.1fC\n", hum, temp);
  vTaskDelay(pdMS_TO_TICKS(500));}
  
}
