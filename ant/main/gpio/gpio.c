#include <string.h>

#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_log.h"


#include "stdatomic.h"

#include <cJSON.h>
#include "utilities.h"
#include "rest_server.h"
#include "gpio.h"
#include "gpioDriver.h"
static const char *REST_TAG = "esp-gpio";
/* esp_err_t gpio_config(const gpio_config_t *pGPIOConfig)
GPIO common configuration.

   Configure GPIO's Mode,pull-up,PullDown,IntrType
Parameters
pGPIOConfig – Pointer to GPIO configure struct

Returns
ESP_OK success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_reset_pin(gpio_num_t gpio_num)
Reset an gpio to default state (select gpio function, enable pullup and disable input and output).

Note

This function also configures the IOMUX for this pin to the GPIO function, and disconnects any other peripheral output configured via GPIO Matrix.

Parameters
gpio_num – GPIO number.

Returns
Always return ESP_OK.

esp_err_t gpio_set_intr_type(gpio_num_t gpio_num, gpio_int_type_t intr_type)
GPIO set interrupt trigger type.

Parameters
gpio_num – GPIO number. If you want to set the trigger type of e.g. of GPIO16, gpio_num should be GPIO_NUM_16 (16);

intr_type – Interrupt type, select from gpio_int_type_t

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_intr_enable(gpio_num_t gpio_num)
Enable GPIO module interrupt signal.

Note

ESP32: Please do not use the interrupt of GPIO36 and GPIO39 when using ADC or Wi-Fi and Bluetooth with sleep mode enabled. Please refer to the comments of adc1_get_raw. Please refer to Section 3.11 of ESP32 ECO and Workarounds for Bugs for the description of this issue.

Parameters
gpio_num – GPIO number. If you want to enable an interrupt on e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_intr_disable(gpio_num_t gpio_num)
Disable GPIO module interrupt signal.

Note

This function is allowed to be executed when Cache is disabled within ISR context, by enabling CONFIG_GPIO_CTRL_FUNC_IN_IRAM

Parameters
gpio_num – GPIO number. If you want to disable the interrupt of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);

Returns
ESP_OK success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level)
GPIO set output level.

Note

This function is allowed to be executed when Cache is disabled within ISR context, by enabling CONFIG_GPIO_CTRL_FUNC_IN_IRAM

Parameters
gpio_num – GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);

level – Output level. 0: low ; 1: high

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG GPIO number error

int gpio_get_level(gpio_num_t gpio_num)
GPIO get input level.

Warning

If the pad is not configured for input (or input and output) the returned value is always 0.

Parameters
gpio_num – GPIO number. If you want to get the logic level of e.g. pin GPIO16, gpio_num should be GPIO_NUM_16 (16);

Returns
0 the GPIO input level is 0

1 the GPIO input level is 1

esp_err_t gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode)
GPIO set direction.

Configure GPIO direction,such as output_only,input_only,output_and_input

Parameters
gpio_num – Configure GPIO pins number, it should be GPIO number. If you want to set direction of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);

mode – GPIO direction

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG GPIO error

esp_err_t gpio_set_pull_mode(gpio_num_t gpio_num, gpio_pull_mode_t pull)
Configure GPIO pull-up/pull-down resistors.

Note

ESP32: Only pins that support both input & output have integrated pull-up and pull-down resistors. Input-only GPIOs 34-39 do not.

Parameters
gpio_num – GPIO number. If you want to set pull up or down mode for e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);

pull – GPIO pull up/down mode.

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG : Parameter error

esp_err_t gpio_wakeup_enable(gpio_num_t gpio_num, gpio_int_type_t intr_type)
Enable GPIO wake-up function.

Parameters
gpio_num – GPIO number.

intr_type – GPIO wake-up type. Only GPIO_INTR_LOW_LEVEL or GPIO_INTR_HIGH_LEVEL can be used.

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_wakeup_disable(gpio_num_t gpio_num)
Disable GPIO wake-up function.

Parameters
gpio_num – GPIO number

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_isr_register(void (*fn)(void*), void *arg, int intr_alloc_flags, gpio_isr_handle_t *handle)
Register GPIO interrupt handler, the handler is an ISR. The handler will be attached to the same CPU core that this function is running on.

This ISR function is called whenever any GPIO interrupt occurs. See the alternative gpio_install_isr_service() and gpio_isr_handler_add() API in order to have the driver support per-GPIO ISRs.

To disable or remove the ISR, pass the returned handle to the interrupt allocation functions.

Parameters
fn – Interrupt handler function.

arg – Parameter for handler function

intr_alloc_flags – Flags used to allocate the interrupt. One or multiple (ORred) ESP_INTR_FLAG_* values. See esp_intr_alloc.h for more info.

handle – Pointer to return handle. If non-NULL, a handle for the interrupt will be returned here.

Returns
ESP_OK Success ;

ESP_ERR_INVALID_ARG GPIO error

ESP_ERR_NOT_FOUND No free interrupt found with the specified flags

esp_err_t gpio_pullup_en(gpio_num_t gpio_num)
Enable pull-up on GPIO.

Parameters
gpio_num – GPIO number

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_pullup_dis(gpio_num_t gpio_num)
Disable pull-up on GPIO.

Parameters
gpio_num – GPIO number

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_pulldown_en(gpio_num_t gpio_num)
Enable pull-down on GPIO.

Parameters
gpio_num – GPIO number

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_pulldown_dis(gpio_num_t gpio_num)
Disable pull-down on GPIO.

Parameters
gpio_num – GPIO number

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_install_isr_service(int intr_alloc_flags)
Install the GPIO driver’s ETS_GPIO_INTR_SOURCE ISR handler service, which allows per-pin GPIO interrupt handlers.

This function is incompatible with gpio_isr_register() - if that function is used, a single global ISR is registered for all GPIO interrupts. If this function is used, the ISR service provides a global GPIO ISR and individual pin handlers are registered via the gpio_isr_handler_add() function.

Parameters
intr_alloc_flags – Flags used to allocate the interrupt. One or multiple (ORred) ESP_INTR_FLAG_* values. See esp_intr_alloc.h for more info.

Returns
ESP_OK Success

ESP_ERR_NO_MEM No memory to install this service

ESP_ERR_INVALID_STATE ISR service already installed.

ESP_ERR_NOT_FOUND No free interrupt found with the specified flags

ESP_ERR_INVALID_ARG GPIO error

void gpio_uninstall_isr_service(void)
Uninstall the driver’s GPIO ISR service, freeing related resources.

esp_err_t gpio_isr_handler_add(gpio_num_t gpio_num, gpio_isr_t isr_handler, void *args)
Add ISR handler for the corresponding GPIO pin.

Call this function after using gpio_install_isr_service() to install the driver’s GPIO ISR handler service.

The pin ISR handlers no longer need to be declared with IRAM_ATTR, unless you pass the ESP_INTR_FLAG_IRAM flag when allocating the ISR in gpio_install_isr_service().

This ISR handler will be called from an ISR. So there is a stack size limit (configurable as “ISR stack size” in menuconfig). This limit is smaller compared to a global GPIO interrupt handler due to the additional level of indirection.

Parameters
gpio_num – GPIO number

isr_handler – ISR handler function for the corresponding GPIO number.

args – parameter for ISR handler.

Returns
ESP_OK Success

ESP_ERR_INVALID_STATE Wrong state, the ISR service has not been initialized.

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_isr_handler_remove(gpio_num_t gpio_num)
Remove ISR handler for the corresponding GPIO pin.

Parameters
gpio_num – GPIO number

Returns
ESP_OK Success

ESP_ERR_INVALID_STATE Wrong state, the ISR service has not been initialized.

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_set_drive_capability(gpio_num_t gpio_num, gpio_drive_cap_t strength)
Set GPIO pad drive capability.

Parameters
gpio_num – GPIO number, only support output GPIOs

strength – Drive capability of the pad

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_get_drive_capability(gpio_num_t gpio_num, gpio_drive_cap_t *strength)
Get GPIO pad drive capability.

Parameters
gpio_num – GPIO number, only support output GPIOs

strength – Pointer to accept drive capability of the pad

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG Parameter error

esp_err_t gpio_hold_en(gpio_num_t gpio_num)
Enable gpio pad hold function.

When the pin is set to hold, the state is latched at that moment and will not change no matter how the internal signals change or how the IO MUX/GPIO configuration is modified (including input enable, output enable, output value, function, and drive strength values). It can be used to retain the pin state through a core reset and system reset triggered by watchdog time-out or Deep-sleep events.

The gpio pad hold function works in both input and output modes, but must be output-capable gpios. If pad hold enabled: in output mode: the output level of the pad will be force locked and can not be changed. in input mode: input read value can still reflect the changes of the input signal.

The state of the digital gpio cannot be held during Deep-sleep, and it will resume to hold at its default pin state when the chip wakes up from Deep-sleep. If the digital gpio also needs to be held during Deep-sleep, gpio_deep_sleep_hold_en should also be called.

Power down or call gpio_hold_dis will disable this function.

Parameters
gpio_num – GPIO number, only support output-capable GPIOs

Returns
ESP_OK Success

ESP_ERR_NOT_SUPPORTED Not support pad hold function

esp_err_t gpio_hold_dis(gpio_num_t gpio_num)
Disable gpio pad hold function.

When the chip is woken up from Deep-sleep, the gpio will be set to the default mode, so, the gpio will output the default level if this function is called. If you don’t want the level changes, the gpio should be configured to a known state before this function is called. e.g. If you hold gpio18 high during Deep-sleep, after the chip is woken up and gpio_hold_dis is called, gpio18 will output low level(because gpio18 is input mode by default). If you don’t want this behavior, you should configure gpio18 as output mode and set it to hight level before calling gpio_hold_dis.

Parameters
gpio_num – GPIO number, only support output-capable GPIOs

Returns
ESP_OK Success

ESP_ERR_NOT_SUPPORTED Not support pad hold function

void gpio_deep_sleep_hold_en(void)
Enable all digital gpio pads hold function during Deep-sleep.

Enabling this feature makes all digital gpio pads be at the holding state during Deep-sleep. The state of each pad holds is its active configuration (not pad’s sleep configuration!).

Note that this pad hold feature only works when the chip is in Deep-sleep mode. When the chip is in active mode, the digital gpio state can be changed freely even you have called this function.

After this API is being called, the digital gpio Deep-sleep hold feature will work during every sleep process. You should call gpio_deep_sleep_hold_dis to disable this feature.

void gpio_deep_sleep_hold_dis(void)
Disable all digital gpio pads hold function during Deep-sleep.

void gpio_iomux_in(uint32_t gpio_num, uint32_t signal_idx)
Set pad input to a peripheral signal through the IOMUX.

Parameters
gpio_num – GPIO number of the pad.

signal_idx – Peripheral signal id to input. One of the *_IN_IDX signals in soc/gpio_sig_map.h.

void gpio_iomux_out(uint8_t gpio_num, int func, bool oen_inv)
Set peripheral output to an GPIO pad through the IOMUX.

Parameters
gpio_num – gpio_num GPIO number of the pad.

func – The function number of the peripheral pin to output pin. One of the FUNC_X_* of specified pin (X) in soc/io_mux_reg.h.

oen_inv – True if the output enable needs to be inverted, otherwise False.

esp_err_t gpio_sleep_sel_en(gpio_num_t gpio_num)
Enable SLP_SEL to change GPIO status automantically in lightsleep.

Parameters
gpio_num – GPIO number of the pad.

Returns
ESP_OK Success

esp_err_t gpio_sleep_sel_dis(gpio_num_t gpio_num)
Disable SLP_SEL to change GPIO status automantically in lightsleep.

Parameters
gpio_num – GPIO number of the pad.

Returns
ESP_OK Success

esp_err_t gpio_sleep_set_direction(gpio_num_t gpio_num, gpio_mode_t mode)
GPIO set direction at sleep.

Configure GPIO direction,such as output_only,input_only,output_and_input

Parameters
gpio_num – Configure GPIO pins number, it should be GPIO number. If you want to set direction of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);

mode – GPIO direction

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG GPIO error

esp_err_t gpio_sleep_set_pull_mode(gpio_num_t gpio_num, gpio_pull_mode_t pull)
Configure GPIO pull-up/pull-down resistors at sleep.

Note

ESP32: Only pins that support both input & output have integrated pull-up and pull-down resistors. Input-only GPIOs 34-39 do not.

Parameters
gpio_num – GPIO number. If you want to set pull up or down mode for e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);

pull – GPIO pull up/down mode.

Returns
ESP_OK Success

ESP_ERR_INVALID_ARG : Parameter error

Structures
struct gpio_config_t
Configuration parameters of GPIO pad for gpio_config function.

Public Members

uint64_t pin_bit_mask
GPIO pin: set with bit mask, each bit maps to a GPIO

gpio_mode_t mode
GPIO mode: set input/output mode

gpio_pullup_t pull_up_en
GPIO pull-up

gpio_pulldown_t pull_down_en
GPIO pull-down

gpio_int_type_t intr_type
GPIO interrupt type

Macros
GPIO_PIN_COUNT
GPIO_IS_VALID_GPIO(gpio_num)
Check whether it is a valid GPIO number.

GPIO_IS_VALID_OUTPUT_GPIO(gpio_num)
Check whether it can be a valid GPIO number of output mode.

GPIO_IS_VALID_DIGITAL_IO_PAD(gpio_num)
Check whether it can be a valid digital I/O pad.

Type Definitions
typedef intr_handle_t gpio_isr_handle_t
typedef void (*gpio_isr_t)(void *arg)
GPIO interrupt handler.

Param arg
User registered data

Header File
components/hal/include/hal/gpio_types.h

*/
 static const char getString[] = "get";
// static const char setString[] = "set";

// // these are the varous parm
static const char pinString[] = "pin";
// static const char indexString[] = "index";
// static const char lengthString[] = "length";
// static const char dataString[] = "data";

atomic_int blinker_duration_ms_atomic = 500;

/* Simple handler for blinker duration control */
static esp_err_t
blinker_duration_post_handler (httpd_req_t * req)
{
  int total_len = req->content_len;
  int cur_len = 0;
  char *buf = ((rest_server_context_t *) (req->user_ctx))->scratch;
  int received = 0;
  if (total_len >= SCRATCH_BUFSIZE)
    {
      /* Respond with 500 Internal Server Error */
      httpd_resp_send_err (req, HTTPD_500_INTERNAL_SERVER_ERROR,
			   "content too long");
      return ESP_FAIL;
    }
  while (cur_len < total_len)
    {
      received = httpd_req_recv (req, buf + cur_len, total_len);
      if (received <= 0)
	{
	  /* Respond with 500 Internal Server Error */
	  httpd_resp_send_err (req, HTTPD_500_INTERNAL_SERVER_ERROR,
			       "Failed to post control value");
	  return ESP_FAIL;
	}
      cur_len += received;
    }
  buf[total_len] = '\0';

  cJSON *root = cJSON_Parse (buf);
  blinker_duration_ms_atomic =
    cJSON_GetObjectItem (root, "duration_ms")->valueint;
  ESP_LOGI (REST_TAG, "Blinker control: duration_ms = %d",
	    blinker_duration_ms_atomic);
  cJSON_Delete (root);
  httpd_resp_sendstr (req, "Post control value successfully");
  return ESP_OK;
}

atomic_bool blinker_state_atomic = false;

/* Simple handler for blinker state control */
static esp_err_t
blinker_state_post_handler (httpd_req_t * req)
{
  int total_len = req->content_len;
  int cur_len = 0;
  char *buf = ((rest_server_context_t *) (req->user_ctx))->scratch;
  int received = 0;
  if (total_len >= SCRATCH_BUFSIZE)
    {
      /* Respond with 500 Internal Server Error */
      httpd_resp_send_err (req, HTTPD_500_INTERNAL_SERVER_ERROR,
			   "content too long");
      return ESP_FAIL;
    }
  while (cur_len < total_len)
    {
      received = httpd_req_recv (req, buf + cur_len, total_len);
      if (received <= 0)
	{
	  /* Respond with 500 Internal Server Error */
	  httpd_resp_send_err (req, HTTPD_500_INTERNAL_SERVER_ERROR,
			       "Failed to post control value");
	  return ESP_FAIL;
	}
      cur_len += received;
    }
  buf[total_len] = '\0';

  cJSON *root = cJSON_Parse (buf);
  blinker_state_atomic = cJSON_IsTrue (cJSON_GetObjectItem (root, "state"));
  ESP_LOGI (REST_TAG, "Blinker control: state = %d", blinker_state_atomic);
  cJSON_Delete (root);
  httpd_resp_sendstr (req, "Post control value successfully");
  return ESP_OK;
}


/**
 * @brief      Gets the fun.
 *
 * @param      parm         The parameter
 * @param[in]  httpd_req_t  The httpd request
 *
 * @return     a json string from the actual function or an indication of the error
 */
static cJSON *
getFun (char *parm, UNUSED httpd_req_t * req)
{
  const char *funName = getString;

  cJSON *response = cJSON_CreateObject ();

  printf ("%s param %s => %s\n", funName, parm,
	  cJSON_Print (cJSON_Parse (parm)));

  char *paramString = "Parse Error";
  cJSON *monitor_json = cJSON_Parse (parm);

  if (monitor_json)
    {

//if we do infact have params then check the type of each on
// we care about
      paramString = pinString;
      cJSON *address =
              cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
      if (!(address && cJSON_IsNumber(address))) {
          goto error;
      }



// everthing looks good call the get
      // printf ("{ \"%s\": %d, \"%s\": %d , \"%s\": %d } \n\n\n",
      //   pinString, (unsigned int) (address->valuedouble));
      cJSON_AddItemToObject(response, funName,
                            gpioGet((unsigned char) (address->valuedouble)));
      return response;
  }

error:				// we don't want to be here
  cJSON_AddStringToObject (response, "error", paramString);
  return response;
}


/**
 * @brief      the i2c function are patches here we take the link /i2c?***=xx had distribute it
 *
 * @param      req   The request
 *
 * @return     The esp error.
 */
esp_err_t
gpio_patch_handler (httpd_req_t * req)
{

  static const uriParam configurationParams[] = {
          //{.name = scanString, .fun = scanFun},rm
//            {.name = getString, .fun = getFun},
    {.name = getString,.fun = getFun},
//            {.name = infoString, .fun = infoFun},
    {.name = NULL,.fun = NULL},
  };
  cors_header (req);
  cJSON *response = paramParser (req, configurationParams);
  httpd_resp_set_type (req, "application/json");
  httpd_resp_sendstr (req, cJSON_Print (response));
  cJSON_Delete (response);
  return ESP_OK;
}
