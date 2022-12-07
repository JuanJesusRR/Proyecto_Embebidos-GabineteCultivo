#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>
#include <esp_err.h>
#include <math.h>
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
    xTaskCreate(dht11,"dht11",configMINIMAL_STACK_SIZE*3,NULL, 5, NULL);
    xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 3, NULL);
    spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
	set_timer();
	set_ADC();
    set_timer2();
    set_ADC2();
    setDuty(1000);
    vTaskDelay(200);
    void waterControl();
    void HumidityControl();
    init_humid();
    init_ventout();
    init_ventin();
    init_bomba();
    bombaControl(0);
    vTaskDelay(200);
    float dis;
    float light;
    //xTaskCreate(blink,"blink",configMINIMAL_STACK_SIZE*3,NULL,5, NULL);
    while (1)
    {   
        setHum(getHum());
        setTemp(getTemp());
        setSoilhumidity(getSoil_Humidity()*100/4095);
        vTaskDelay(200);
        dis = getDis();
        dis = 0.089-dis;
        dis = dis*100 *100/5;
        dis =round(dis);
        setTankLevel(dis);
        light =getLight()*100/4095;
        printf(" Nivel: %0.1f%%",dis);
        printf(" Humedad %0.1f%%",getHum());
        printf(" Luz %0.1f",light);
        setLight(light);
        printf("Humedad suelo %d \n",getSoil_Humidity()*100/4095);
        
        waterControl();
        //HumidityControl();
        vTaskDelay(100);
    }
    



}

void waterControl(){
    if( 2000>=getSoil_Humidity()){
        printf("Encender Bomba %d \n",getSoil_Humidity());
        bombaControl(1);
        vTaskDelay(1000);
        bombaControl(0);
        printf("Apagar bomba \n");
    }else{
        bombaControl(0);
        //printf("Humedad OK\n");
        vTaskDelay(200);
    }
}

void HumidityControl(){
    if(getHum()>=80){
            //setGpio(18);
            //set_PWM();
            ventoutControl(1);
            ventinControl(1);
            humidControl(0);
            printf("Prendiendo Ventilador \n");
            vTaskDelay(100);
        }else if (getHum()<=77)
        {
            //setGpio(12);
           // set_PWM();
           ventoutControl(0);
           ventinControl(0);
           humidControl(1);
            printf("Prendiendo humidificador\n");
            vTaskDelay(100);
        }
}

