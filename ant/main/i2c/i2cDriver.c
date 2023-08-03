/* cmd_i2ctools.c

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "argtable3/argtable3.h"
#include "driver/i2c.h"
#include "edger_console.h"
#include "esp_log.h"
#include "cJSON.h"
#include "nvs_flash.h"
#include "nvs.h"

#define I2C_MASTER_TX_BUF_DISABLE 0	/*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0	/*!< I2C master doesn't need buffer */
#define WRITE_BIT I2C_MASTER_WRITE	/*!< I2C master write */
#define READ_BIT I2C_MASTER_READ	/*!< I2C master read */
#define ACK_CHECK_EN 0x1	/*!< I2C master will check ack from slave */
#define ACK_CHECK_DIS 0x0	/*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0		/*!< I2C ack value */
#define NACK_VAL 0x1		/*!< I2C nack value */

static const char *TAG = "cmd_i2ctools";


#include "freertos/semphr.h"

//Create semphore
static SemaphoreHandle_t SemaphoreHandle = NULL;
static cJSON *response;


#ifndef CONFIG_I2C_MASTER_SCL

#if CONFIG_IDF_TARGET_ESP32S3
uint8_t i2c_gpio_sda = 1;
uint8_t i2c_gpio_scl = 0;
#elif CONFIG_IDF_TARGET_ESP32C3
uint8_t i2c_gpio_sda = 1;
uint8_t i2c_gpio_scl = 0;

#else
uint8_t i2c_gpio_sda = 18;
uint8_t i2c_gpio_scl = 19;
#endif

#else
uint8_t i2c_gpio_scl = (gpio_num_t) CONFIG_I2C_MASTER_SCL;
uint8_t i2c_gpio_sda = (gpio_num_t) CONFIG_I2C_MASTER_SDA;
#endif


static uint32_t i2c_frequency = 100000;
static i2c_port_t i2c_port = I2C_NUM_0;


/*
ls> i2cconfig --sda=0 --scl=1
i2c-tools> i2cdetect
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
i2c-tools> i2cconfig --sda=1 --scl=0
i2c-tools> i2cdetect
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00: -- -- -- -- -- -- -- -- 08 09 0a 0b -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
*/
i2c_config_t conf;

static esp_err_t
i2c_master_driver_initialize (void)
{

  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = i2c_gpio_sda;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_io_num = i2c_gpio_scl;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = i2c_frequency;


  return i2c_param_config (i2c_port, &conf);
}

cJSON *
i2cInfo ()
{
    i2c_master_driver_initialize ();
  cJSON *response = cJSON_CreateObject ();
  //////////////////////////////////////////////


  cJSON_AddStringToObject(response, "mode",
			   conf.mode == I2C_MODE_MASTER ? "MASTER" : "SLAVE");
  cJSON_AddNumberToObject (response, "sda io num", conf.sda_io_num);
  cJSON_AddNumberToObject (response, "sda pullup en ", conf.sda_pullup_en);
  cJSON_AddNumberToObject (response, "scl io num", conf.scl_io_num);
  cJSON_AddNumberToObject (response, "scl pullup en", conf.scl_pullup_en);
  cJSON_AddNumberToObject (response, "master clk speed",
			   conf.master.clk_speed);

  return response;
}



cJSON *
i2cScan ()
{
  if (SemaphoreHandle == NULL)
    SemaphoreHandle = xSemaphoreCreateMutex ();

  xSemaphoreTake (SemaphoreHandle, portMAX_DELAY);
  i2c_driver_install (i2c_port, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE,
		      I2C_MASTER_TX_BUF_DISABLE, 0);
  i2c_master_driver_initialize ();

  int address;
  response = cJSON_CreateArray ();
  for (int i = 0; i < 128; i += 4)
    {

      for (int j = 0; j < 4; j++)
	{
	  address = i + j;
	  i2c_cmd_handle_t cmd = i2c_cmd_link_create ();
	  i2c_master_start (cmd);
	  i2c_master_write_byte (cmd, (address << 1) | WRITE_BIT,
				 ACK_CHECK_EN);
	  i2c_master_stop (cmd);
	  esp_err_t ret =
	    i2c_master_cmd_begin (i2c_port, cmd, 50 / portTICK_PERIOD_MS);
	  i2c_cmd_link_delete (cmd);
	  if (ret == ESP_OK)
	    {

	      cJSON *x_json = cJSON_CreateNumber (address);
	      cJSON_AddItemToArray (response, x_json);
	    }
	  else if (ret == ESP_ERR_TIMEOUT)
	    {

	      cJSON *x_json = cJSON_CreateNumber (-address);
	      cJSON_AddItemToArray (response, x_json);
	    }
	}
    }

  i2c_driver_delete (i2c_port);
  xSemaphoreGive (SemaphoreHandle);
  return response;
}


cJSON *
i2cGet (uint8_t chip_addr, uint8_t data_addr, uint8_t len)
{

  if (SemaphoreHandle == NULL)
    SemaphoreHandle = xSemaphoreCreateMutex ();
  xSemaphoreTake (SemaphoreHandle, portMAX_DELAY);


  response = cJSON_CreateArray ();
  // Check data length: "-l" option
  //  = 1;
  // if (i2cget_args.data_length->count) {
  //   len = i2cget_args.data_length->ival[0];
  // }
  uint8_t *data = malloc (len);

  i2c_driver_install (i2c_port, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE,
          I2C_MASTER_TX_BUF_DISABLE, 0);

  i2c_master_driver_initialize ();


  i2c_cmd_handle_t cmd = i2c_cmd_link_create ();
  i2c_master_start (cmd);
  if (data_addr != -1)
    {
      i2c_master_write_byte (cmd, chip_addr << 1 | WRITE_BIT, ACK_CHECK_EN);
      i2c_master_write_byte (cmd, data_addr, ACK_CHECK_EN);
      i2c_master_start (cmd);
    }
  i2c_master_write_byte (cmd, chip_addr << 1 | READ_BIT, ACK_CHECK_EN);
  if (len > 1)
    {
      i2c_master_read (cmd, data, len - 1, ACK_VAL);
    }
  i2c_master_read_byte (cmd, data + len - 1, NACK_VAL);
  i2c_master_stop (cmd);
  esp_err_t ret =
    i2c_master_cmd_begin (i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete (cmd);

  if (ret == ESP_OK)
    {
      for (int i = 0; i < len; i++)
  {
    cJSON *x_json = cJSON_CreateNumber (data[i]);
    cJSON_AddItemToArray (response, x_json);
#ifdef CONFIG_ANT_VERBOSE
    printf ("0x%02x ", data[i]);
    if ((i + 1) % 16 == 0)
      {
        printf ("\r\n");
      }
#endif
  }
#ifdef CONFIG_ANT_VERBOSE
      if (len % 16)
  {
    printf ("\r\n");
  }
#endif
    }
  else if (ret == ESP_ERR_TIMEOUT)
    {
      ESP_LOGW (TAG, "Bus is busy");
    }
  else
    {
      ESP_LOGW (TAG, "Read failed");
    }
  free (data);
  i2c_driver_delete (i2c_port);

  xSemaphoreGive (SemaphoreHandle);
  return response;
}


cJSON *
i2cSet (uint8_t chip_addr, uint8_t index, cJSON * data)
{

  //if (SemaphoreHandle == NULL)
   // SemaphoreHandle = xSemaphoreCreateMutex ();
  //xSemaphoreTake (SemaphoreHandle, portMAX_DELAY);


  int len = cJSON_GetArraySize (data);
  printf("Lenght %d\n\n",len);
  i2c_driver_install (i2c_port, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE,
		      I2C_MASTER_TX_BUF_DISABLE, 0);
  i2c_master_driver_initialize ();
  i2c_cmd_handle_t cmd = i2c_cmd_link_create ();
  i2c_master_start (cmd);
  ///////////////////////////////////////////
ESP_ERROR_CHECK (  i2c_master_write_byte (cmd, chip_addr << 1 | WRITE_BIT, ACK_CHECK_EN));
  printf ("chip address  %d %u\n", chip_addr,index);
  // if (i2cset_args.register_address->count) {
  ESP_ERROR_CHECK (i2c_master_write_byte (cmd, index, ACK_CHECK_EN));
  // }
  for (int i = 0; i < len; i++)
    {
      cJSON *value = cJSON_GetArrayItem (data, i);
ESP_ERROR_CHECK (      i2c_master_write_byte (cmd, value->valueint, ACK_CHECK_EN));
    }
ESP_ERROR_CHECK (  i2c_master_stop (cmd));
  esp_err_t ret =
    i2c_master_cmd_begin (i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete (cmd);
  if (ret == ESP_OK)
    {

      response = cJSON_CreateString ("Write OK");
      ESP_LOGI (TAG, "Write OK");
    }
  else if (ret == ESP_ERR_TIMEOUT)
    {

        response = cJSON_CreateString ("Bus is busy");
      ESP_LOGW (TAG, "Bus is busy");
    }
  else
    {
      response = cJSON_CreateString ("Write Failed");
      ESP_LOGW (TAG, "Write Failed");
    }
  i2c_driver_delete (i2c_port);
  //xSemaphoreGive (SemaphoreHandle);
  return response;
}


cJSON *
i2cDump (uint8_t chip_addr, uint8_t size)
{

  if (SemaphoreHandle == NULL)
    SemaphoreHandle = xSemaphoreCreateMutex ();
  xSemaphoreTake (SemaphoreHandle, portMAX_DELAY);
  response = NULL;		// necessary for error handling

#ifdef CONFIG_ANT_VERBOSE
  ESP_LOGW (TAG, "int chip_addr=%d, int size=%d", chip_addr, size);
#endif
  response = cJSON_CreateArray ();

  i2c_driver_install (i2c_port, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE,
		      I2C_MASTER_TX_BUF_DISABLE, 0);
  i2c_master_driver_initialize ();
  uint8_t data_addr;
  uint8_t data[4];
  int32_t block[16];
#ifdef CONFIG_ANT_VERBOSE
  printf ("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f"
	  "    0123456789abcdef\r\n");
#endif
  for (int i = 0; i < 0x100; i += 16)
    {
#ifdef CONFIG_ANT_VERBOSE
      printf ("%02x: ", i);
#endif
      for (int j = 0; j < 16; j += size)
	{
#ifdef CONFIG_ANT_VERBOSE
	  fflush (stdout);
#endif
	  data_addr = i + j;
	  i2c_cmd_handle_t cmd = i2c_cmd_link_create ();
	  i2c_master_start (cmd);
	  i2c_master_write_byte (cmd, chip_addr << 1 | WRITE_BIT,
				 ACK_CHECK_EN);
	  i2c_master_write_byte (cmd, data_addr, ACK_CHECK_EN);
	  i2c_master_start (cmd);
	  i2c_master_write_byte (cmd, chip_addr << 1 | READ_BIT,
				 ACK_CHECK_EN);
	  if (size > 1)
	    {
	      i2c_master_read (cmd, data, size - 1, ACK_VAL);
	    }
	  i2c_master_read_byte (cmd, data + size - 1, NACK_VAL);
	  i2c_master_stop (cmd);
	  esp_err_t ret =
	    i2c_master_cmd_begin (i2c_port, cmd, 50 / portTICK_PERIOD_MS);
	  i2c_cmd_link_delete (cmd);
	  if (ret == ESP_OK)
	    {
	      for (int k = 0; k < size; k++)
		{
#ifdef CONFIG_ANT_VERBOSE
		  printf ("%02x ", data[k]);
#endif
		  block[j + k] = data[k];
		  cJSON *x_json = cJSON_CreateNumber (data[k]);
		  cJSON_AddItemToArray (response, x_json);
		}
	    }
	  else
	    {

#ifdef CONFIG_ANT_VERBOSE
	      for (int k = 0; k < size; k++)
		{
		  printf ("XX ");
		  block[j + k] = -1;
		}
#endif
	    }
	}
#ifdef CONFIG_ANT_VERBOSE
      printf ("   ");
      for (int k = 0; k < 16; k++)
	{
	  if (block[k] < 0)
	    {
	      printf ("X");
	    }
	  if ((block[k] & 0xff) == 0x00 || (block[k] & 0xff) == 0xff)
	    {
	      printf (".");
	    }
	  else if ((block[k] & 0xff) < 32 || (block[k] & 0xff) >= 127)
	    {
	      printf ("?");
	    }
	  else
	    {
	      printf ("%c", block[k] & 0xff);
	    }
	}
      printf ("\r\n");
#endif
    }

  i2c_driver_delete (i2c_port);


  xSemaphoreGive (SemaphoreHandle);
  return response;
// err:
//   if (response != NULL) {
//     cJSON_Delete (response);
//   }
//   response = cJSON_CreateString ("Dump Failed");
//   return response;

}
