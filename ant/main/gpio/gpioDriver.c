
#include <stdio.h>
#include "argtable3/argtable3.h"
#include "driver/i2c.h"
#include "esp_myconsole.h"
#include "esp_log.h"
#include "cJSON.h"
#include <driver/gpio.h>
#include "freertos/semphr.h"


static SemaphoreHandle_t SemaphoreHandle = NULL;
cJSON *
gpioGet (uint8_t pin)
{

  if (SemaphoreHandle == NULL)
    SemaphoreHandle = xSemaphoreCreateMutex ();
  xSemaphoreTake (SemaphoreHandle, portMAX_DELAY);

 
  cJSON *response = cJSON_CreateObject ();
  cJSON_AddNumberToObject (response, "pin", gpio_get_level(pin));

  xSemaphoreGive (SemaphoreHandle);
  return response;
}
