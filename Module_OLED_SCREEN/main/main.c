#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ssd1306.h"
#include "font8x8_basic.h"
#include "freertos/timers.h"
#include "driver/adc.h"
#define tag "SSD1306"
// variable for the screen
SSD1306_t dev;
char string[20];
// sensor variables
int floor_humidity = 10;
int Temp = 20;
int Air_humidity = 25;
int tank_level = 50;
bool Ligth_level = true;

/// states
char actual_state = '0';
char initial_state = '0';
char principal_menu = '1';
char actual_variables_state = '2';
char configuration_settings = '3';
char automatic_settings = '4';
char Manual_settings = '5';
char Temperature_settings = '6';
char Humidity_settings = '7';
// Principal timer variables
int interval = 6000;
int timer_ID = 1;
TimerHandle_t xTimers;
// functions
esp_err_t set_timer(void);

void app_main(void)
{
	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
	set_timer();
}

void vTimerCallback(TimerHandle_t pxTimer)
{
	// ADC_VALUE = adc1_get_raw(ADC1_CHANNEL_4); // pin 32
	// Read_Button();
	// define_button_action();

	switch (actual_state)
	{

	case '0': // initial_state

		ssd1306_init(&dev, 128, 32);
		ESP_LOGI(tag, "Panel is 128x32");
		ssd1306_clear_screen(&dev, true);
		ssd1306_display_text(&dev, 1, " Hello,welcome!!", 16, true);
		ESP_LOGW(tag, "estoy en 0");
		actual_state = principal_menu;
		break;

	case '1': //  principal_menu

		ssd1306_clear_screen(&dev, false);
		ssd1306_contrast(&dev, 0xff);
		ssd1306_display_text(&dev, 0, "Principal Menu", 16, false);
		ssd1306_display_text(&dev, 1, "Actual variables", 16, false);
		ssd1306_display_text(&dev, 2, "system settings", 16, false);
		ESP_LOGW(tag, "estoy en 1");
		vTaskDelay(5000 / portTICK_PERIOD_MS);
		actual_state = actual_variables_state;
		break;

	case '2': //  actual_variables_state

		ssd1306_init(&dev, 128, 64);
		ssd1306_clear_screen(&dev, false);
		ssd1306_contrast(&dev, 0xff);
		sprintf(&string, "Temp:%02d", Temp);
		ssd1306_display_text(&dev, 0, string, strlen(string), false);
		sprintf(&string, "Air humidity:%02d", Air_humidity);
		ssd1306_display_text(&dev, 1, string, strlen(string), false);
		sprintf(&string, "Tank level:%02d", tank_level);
		ssd1306_display_text(&dev, 2, string, strlen(string), false);
		sprintf(&string, ":%02d", floor_humidity);
		ssd1306_display_text(&dev, 3, "Floor humidity", 14, false);
		ssd1306_display_text(&dev, 4, string, strlen(string), false);
		sprintf(&string, "Ligth level:%02d", Ligth_level);
		ssd1306_display_text(&dev, 5, string, strlen(string), false);
		// ssd1306_display_text(&dev, 5, string, strlen(string), false);
		ESP_LOGW(tag, "estoy en 2");
		vTaskDelay(5000 / portTICK_PERIOD_MS);
		// ouede regresar al esatdo anerior y de ahi a configuration_settings o quedarse aca
		actual_state = configuration_settings;
		break;
	case '3': //  principal_menu

		ssd1306_init(&dev, 128,32);
		ssd1306_clear_screen(&dev, false);
		ssd1306_contrast(&dev, 0xff);
		ssd1306_display_text(&dev, 0, "Temperature", 11, false);
		ssd1306_display_text(&dev, 3, "Floor humidity", 16, false);
		ESP_LOGW(tag, "estoy en 3");
		vTaskDelay(5000 / portTICK_PERIOD_MS);
		actual_state = '5';
		break;

	default:
		ESP_LOGW(tag, "estoy en default");
		actual_state = initial_state;
		break;
	}
}

esp_err_t set_timer(void)
{
	ESP_LOGI(tag, "timer init configuration");
	xTimers = xTimerCreate("Timer",					// Just a text name, not used by the kernel.
						   pdMS_TO_TICKS(interval), // The timer period in ticks.
						   pdTRUE,					// The timers will auto-reload themselves when they expire.
						   (void *)timer_ID,		// Assign each timer a unique id equal to its array index.
						   vTimerCallback			// Each timer calls the same callback when it expires.
	);

	if (xTimers == NULL)
	{
		ESP_LOGW(tag, "The timer was not created.");
	}
	else
	{
		// Start the timer.  No block time is specified, and even if one was
		// it would be ignored because the scheduler has not yet been
		// started.
		if (xTimerStart(xTimers, 0) != pdPASS)
		{
			ESP_LOGE(tag, "The timer could not be set into the Active state.");
		}
	}

	return ESP_OK;
}
