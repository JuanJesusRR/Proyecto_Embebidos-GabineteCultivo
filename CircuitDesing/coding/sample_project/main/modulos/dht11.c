#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <dht.h>
float hum;
float temp;
dht_sensor_type_t sen =0;
/*Funciones Para obtener el valor de las variables medidas por el sensor*/
float getHum(void){
  return hum;
}
float getTemp(void){
  return temp;
}

/*Tarea que se encargara de hacer uso del sensor para medicion con la funcion dht_read_float_data que debe devoler un ESP_OK en caso 
 de que todo se ecuentre funcionando de manera correcta, si no, debe devoler 0 en los valores y una alerta*/
void dht11(void *pvParameters){
    while(1){
        
  esp_err_t res = dht_read_float_data(sen,GPIO_NUM_17,&hum , &temp);
  if (res != ESP_OK){
    temp=0;
    hum=0;
    printf("El sensor no se encuentra debidamente conectado");
    vTaskDelay(pdMS_TO_TICKS(500));
  }else{ 
  //printf("Humidity: %.1f%% Temp: %.1fC\n", hum, temp);
  vTaskDelay(pdMS_TO_TICKS(500));}
  }
}
