#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"
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
char string[30];
// sensor variables
int Soil_humidity = 10;
int Temp = 20;
int Air_humidity = 25;
int tank_level = 50;
bool Ligth_level = true;
// Buttoms variables
int selected_buttonSW = 0;
int ADC_Buttoms_VALUE = 0;

// Screen variables
int actual_line = 1;
bool first_frame = true;
int lower_limit = 2;

// funtions for buttons
esp_err_t set_ADC(void);
esp_err_t Read_Button(void);
esp_err_t define_button_action(void);
// settings variables:
int Authomatic_soil_humidity = 60;
int Authomatic_air_humidity = 40;
int Authomatic_tank_level = 60;
int Authomatic_temp = 15;
// Manual settings
int Manual_soil_humidity = 20;
int Manual_air_humidity = 47;
int Manual_Temp = 10;

/// states
char actual_state = '0';
char initial_state = '0';
char principal_menu = '1';
char actual_variables_state = '2';
char configuration_settings = '3';
char Automatic_settings = '4';
char Manual_settings = '5';
// char Temperature_settings = '6';
// char Humidity_settings = '7';

// Principal timer variables
int interval = 300;
int timer_ID = 1;
TimerHandle_t xTimers;

esp_err_t set_timer(void);
esp_err_t FSM(void);
esp_err_t Select_line(void);
esp_err_t Enter(void);
esp_err_t Go_to_back(void);
esp_err_t configure_manual_settings(void);

void setTemp(float temperature)
{
	Temp = temperature;
}
void setHum(float humid){
	Air_humidity = humid;
}

void vTimerCallback(TimerHandle_t pxTimer)
{
	ADC_Buttoms_VALUE = adc1_get_raw(ADC1_CHANNEL_6); // pin 34
	Read_Button();
	// define_button_action();
	FSM();
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

esp_err_t FSM(void)
{

	switch (actual_state)
	{

	case '0': // initial_state

		ssd1306_init(&dev, 128, 32);
		ESP_LOGI(tag, "Panel is 128x32");
		ssd1306_clear_screen(&dev, true);
		ssd1306_display_text(&dev, 1, " Hello,welcome!!", 16, true);
		ESP_LOGW(tag, "estoy en 0");
		vTaskDelay(5000 / portTICK_PERIOD_MS);
		ssd1306_clear_screen(&dev, false);
		actual_state = principal_menu;
		break;

	case '1': //  principal_menu

		if (first_frame == true)
		{

			ssd1306_contrast(&dev, 0xff);
			ssd1306_display_text(&dev, 0, "Principal Menu", 16, false);
			ssd1306_display_text(&dev, 1, " Actual state", 16, false);
			ssd1306_display_text(&dev, actual_line, "*", 1, false);
			ssd1306_display_text(&dev, 2, " system settings", 16, false);
			ssd1306_display_text(&dev, 3, "            Next", 16, false);
			first_frame = false;
		}
		else
		{

			ssd1306_display_text(&dev, actual_line, "*", 1, false);
		}

		//ESP_LOGW(tag, "estoy en 1");

		break;

	case '2': //  actual_variables_state

		ssd1306_contrast(&dev, 0xff);
		ssd1306_display_text(&dev, 0, "Actual variables", 16, false);
		sprintf(&string, "Temp:%02d", Temp);
		ssd1306_display_text(&dev, 1, string, strlen(string), false);
		sprintf(&string, "Air humidity:%02d", Air_humidity);
		ssd1306_display_text(&dev, 2, string, strlen(string), false);
		sprintf(&string, "Tank level:%02d", tank_level);
		ssd1306_display_text(&dev, 3, string, strlen(string), false);
		sprintf(&string, "Soil humidity:%02d", Soil_humidity);
		ssd1306_display_text(&dev, 4, string, strlen(string), false);
		sprintf(&string, "Ligth level:%02d", Ligth_level);
		ssd1306_display_text(&dev, 5, string, strlen(string), false);
		ssd1306_display_text(&dev, 7, "Back", 4, false);
		//ESP_LOGW(tag, "estoy en 2");
		break;
	case '3': //  configuration_settings

		if (first_frame == true)
		{

			ssd1306_contrast(&dev, 0xff);
			ssd1306_display_text(&dev, 0, "Select mode:", 12, false);
			ssd1306_display_text(&dev, 1, " Automatic", 10, false);
			ssd1306_display_text(&dev, actual_line, "*", 1, false);
			ssd1306_display_text(&dev, 2, " Manual", 7, false);
			ssd1306_display_text(&dev, 3, "Back        Next", 16, false);
			first_frame = false;
		}

		else
		{

			ssd1306_display_text(&dev, actual_line, "*", 1, false);
		}
		//ESP_LOGW(tag, "estoy en 3");
		break;

	case '4': // Automatic_settings

		ssd1306_contrast(&dev, 0xff);
		ssd1306_display_text(&dev, 0, "Auto settings", 13, false);
		sprintf(&string, "Soil humidity:%02d", Authomatic_soil_humidity);
		ssd1306_display_text(&dev, 1, string, strlen(string), false);
		sprintf(&string, "Air humidity:%02d", Authomatic_air_humidity);
		ssd1306_display_text(&dev, 2, string, strlen(string), false);
		sprintf(&string, "Tank level:%02d", Authomatic_tank_level);
		ssd1306_display_text(&dev, 3, string, strlen(string), false);
		sprintf(&string, "Temp:%02d", Authomatic_temp);
		ssd1306_display_text(&dev, 4, string, strlen(string), false);
		ssd1306_display_text(&dev, 7, "Back", 4, false);
		ESP_LOGW(tag, "estoy en 4");

		break;

	case '5': // Manual_settings
		if (first_frame == true)
		{
			ssd1306_contrast(&dev, 0xff);
			ssd1306_display_text(&dev, 0, "Manual settings", 16, false);
			sprintf(&string, " Soil H2O:%02d", Manual_soil_humidity);
			ssd1306_display_text(&dev, 1, string, strlen(string), false);
			ssd1306_display_text(&dev, actual_line, "*", 1, false);
			sprintf(&string, " Air H2O:%02d", Manual_air_humidity);
			ssd1306_display_text(&dev, 2, string, strlen(string), false);
			sprintf(&string, " Temp:%02d", Manual_Temp);
			ssd1306_display_text(&dev, 3, string, strlen(string), false);
			ssd1306_display_text(&dev, 4, "Back", 4, false);
			first_frame = false;
		}

		else
		{
			//ESP_LOGW(tag, "estoy en 5");
			ssd1306_display_text(&dev, actual_line, "*", 1, false);
			configure_manual_settings();
		}

		break;

	default:
		//ESP_LOGW(tag, "estoy en default");
		actual_state = initial_state;
		break;
	}

	return ESP_OK;
}

esp_err_t set_ADC(void)
{
	adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11); // el segundo parametro ayuda a ajustar el Vref y por ende la resolucion
	adc1_config_width(ADC_WIDTH_BIT_12);
	return ESP_OK;
}
esp_err_t Read_Button(void)
{
	if (ADC_Buttoms_VALUE == 4095)
	{
		selected_buttonSW = 0; // any sw selected
	}

	// selected_buttonSW=number SW selected
	else if (ADC_Buttoms_VALUE == 0)
	{
		selected_buttonSW = 1;
		ESP_LOGI(tag, "you have selected the SW%u", selected_buttonSW);
		Go_to_back();
	}
	else if (ADC_Buttoms_VALUE >= 370 && ADC_Buttoms_VALUE <= 430)
	{
		selected_buttonSW = 2;
		ESP_LOGI(tag, "you have selected the SW%u", selected_buttonSW);
		Select_line();
	}
	else if (ADC_Buttoms_VALUE >= 1130 && ADC_Buttoms_VALUE <= 1170)
	{
		selected_buttonSW = 3;
		ESP_LOGI(tag, "you have selected the SW%u", selected_buttonSW);
		Select_line();
	}
	else if (ADC_Buttoms_VALUE >= 1830 && ADC_Buttoms_VALUE <= 1895)
	{
		selected_buttonSW = 4;
		ESP_LOGI(tag, "you have selected the SW%u", selected_buttonSW);
		Enter();
	}
	else if (ADC_Buttoms_VALUE >= 2800 && ADC_Buttoms_VALUE <= 2880)
	{
		selected_buttonSW = 5;
		ESP_LOGI(tag, "you have selected the SW%u", selected_buttonSW);
	}
	// return selected_buttonSW;
	return ESP_OK;
}
esp_err_t define_button_action(void)
{

	switch (selected_buttonSW)
	{
	case 0:
		ESP_LOGW(tag, "%u", ADC_Buttoms_VALUE);
		ESP_LOGE(tag, "you haven't selected any SW");
		break;

	case 1:
		ESP_LOGW(tag, "%u", ADC_Buttoms_VALUE);
		ESP_LOGI(tag, "you have selected the SW%u", selected_buttonSW);
		break;

	case 2:
		ESP_LOGW(tag, "%u", ADC_Buttoms_VALUE);
		ESP_LOGI(tag, "you have selected the SW%u", selected_buttonSW);
		// Select_line();
		break;

	case 3:
		ESP_LOGW(tag, "%u", ADC_Buttoms_VALUE);
		ESP_LOGI(tag, "you have selected the SW%u", selected_buttonSW);
		// Select_line();
		break;

	case 4:
		ESP_LOGW(tag, "%u", ADC_Buttoms_VALUE);
		ESP_LOGI(tag, "you have selected the SW%u", selected_buttonSW);
		// Select_line();
		break;

	case 5:
		ESP_LOGW(tag, "%u", ADC_Buttoms_VALUE);
		ESP_LOGI(tag, "you have selected the SW%u", selected_buttonSW);
		break;

	default:
		ESP_LOGW(tag, "%u", ADC_Buttoms_VALUE);
		selected_buttonSW = 0;
		break;
	}
	return ESP_OK;
}

esp_err_t Select_line(void)
{
	switch (selected_buttonSW)
	{

	case 3:

		if (actual_line == lower_limit)
		{
			ssd1306_display_text(&dev, actual_line, " ", 1, false);
			actual_line = 1;
			// vTaskDelay(5000 / portTICK_PERIOD_MS);
		}

		else
		{
			ssd1306_display_text(&dev, actual_line, " ", 1, false);
			actual_line += 1;
		}

		break;

	case 2:

		if (actual_line == 1)
		{
			ssd1306_display_text(&dev, actual_line, " ", 1, false);
			actual_line = lower_limit;
			// vTaskDelay(5000 / portTICK_PERIOD_MS);
		}

		else
		{
			ssd1306_display_text(&dev, actual_line, " ", 1, false);
			actual_line -= 1;
		}

		break;

	default:;
	}

	return ESP_OK;
}

esp_err_t Enter(void)
{
	switch (actual_state)
	{
	case '1':

		switch (actual_line)
		{
		case 1:
			actual_line = 1;
			first_frame = true;
			lower_limit = 2;
			actual_state = actual_variables_state;
			ssd1306_init(&dev, 128, 64);
			ssd1306_clear_screen(&dev, false);
			break;

		case 2:
			actual_line = 1;
			first_frame = true;
			lower_limit = 2;
			actual_state = configuration_settings;
			ssd1306_init(&dev, 128, 32);
			ssd1306_clear_screen(&dev, false);

			break;

		default:
			actual_state = principal_menu;
			break;
		}

		break;

	case '3':

		switch (actual_line)
		{
		case 1:
			actual_line = 1;
			first_frame = true;
			actual_state = Automatic_settings;
			lower_limit = 2;
			ssd1306_init(&dev, 128, 64);
			ssd1306_clear_screen(&dev, false);
			break;

		case 2:
			actual_line = 1;
			first_frame = true;
			actual_state = Manual_settings;
			lower_limit = 3;
			ssd1306_init(&dev, 128, 32);
			ssd1306_clear_screen(&dev, false);

			break;

		default:
			actual_state = principal_menu;
			break;
		}

		break;

	default:;
	}

	return ESP_OK;
}

esp_err_t Go_to_back(void)
{
	switch (actual_state)
	{
	case '2':
		actual_line = 1;
		first_frame = true;
		actual_state = principal_menu;
		ssd1306_init(&dev, 128, 32);
		ssd1306_clear_screen(&dev, false);
		break;

	case '3':
		actual_line = 1;
		first_frame = true;
		lower_limit = 2;
		actual_state = principal_menu;
		ssd1306_init(&dev, 128, 32);
		ssd1306_clear_screen(&dev, false);
		break;

	case '4':
		actual_line = 1;
		first_frame = true;
		lower_limit = 2;
		actual_state = configuration_settings;
		ssd1306_init(&dev, 128, 32);
		ssd1306_clear_screen(&dev, false);
		break;

	case '5':
		actual_line = 1;
		first_frame = true;
		lower_limit = 2;
		actual_state = configuration_settings;
		ssd1306_init(&dev, 128, 32);
		ssd1306_clear_screen(&dev, false);
		break;

	default:;
	}
	return ESP_OK;
}

esp_err_t configure_manual_settings(void)
{
	switch (actual_line)
	{
	case 1:
		if (selected_buttonSW == 4)
		{
			if (Manual_soil_humidity >= 100)
			{
				Manual_soil_humidity = 30;
				ssd1306_clear_line(&dev, actual_line, false);
				sprintf(&string, " Soil H2O:%02d", Manual_soil_humidity);
				ssd1306_display_text(&dev, actual_line, string, strlen(string), false);
			}
			else
			{
				Manual_soil_humidity += 5;
				sprintf(&string, " Soil H2O:%02d", Manual_soil_humidity);
				ssd1306_display_text(&dev, actual_line, string, strlen(string), false);
			}
		}

		break;

	case 2:
		if (selected_buttonSW == 4)
		{
			if (Manual_air_humidity >= 100)
			{
				Manual_air_humidity = 30;
				ssd1306_clear_line(&dev, actual_line, false);
				sprintf(&string, " Air H2O:%02d", Manual_air_humidity);
				ssd1306_display_text(&dev, actual_line, string, strlen(string), false);
			}
			else
			{
				Manual_air_humidity += 5;
				sprintf(&string, " Air H2O:%02d", Manual_air_humidity);
				ssd1306_display_text(&dev, actual_line, string, strlen(string), false);
			}
		}

		break;

	case 3:
		if (selected_buttonSW == 4)
		{
			if (Manual_Temp >= 35)
			{
				Manual_Temp = 15;
				ssd1306_clear_line(&dev, actual_line, false);
				sprintf(&string, " Temp:%02d", Manual_Temp);
				ssd1306_display_text(&dev, actual_line, string, strlen(string), false);
			}
			else
			{
				Manual_Temp += 1;
				sprintf(&string, " Temp:%02d", Manual_Temp);
				ssd1306_display_text(&dev, actual_line, string, strlen(string), false);
			}
		}

		break;

	default:;
	}

	return ESP_OK;
}
