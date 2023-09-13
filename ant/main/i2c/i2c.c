/*************************************************************
// Created by nicked on 10/7/22.
// the i2c_patch_handler is exposed the rest it internal
// it uses paramParser from utilities to do the function dispaching
//
**************************************************************/

#include <esp_http_server.h>
#include <esp_system.h>
#include <cJSON.h>
#include "utilities.h"
#include "rest_server.h"
#include "i2cDriver.h"
#include "i2c.h"

#include "esp_err.h"
#include "esp_log.h"

static const char *TAG = "i2c";
// these are the function
static const char infoString[] = "info";
static const char scanString[] = "scan";
static const char getString[] = "get";
static const char setString[] = "set";


// these are the varous parm
static const char addressString[] = "address";
static const char indexString[] = "index";
static const char lengthString[] = "length";
static const char dataString[] = "data";
static const char optionsString[] = "options";


/**
 * @brief      {  get the i2c port info}
 *
 * @param      parm         The parameter
 * @param[in]  httpd_req_t  The httpd request
 *
 * @return     a json string from the actual function or an indication of the error
 */
static cJSON *infoFun(char *parm, UNUSED httpd_req_t

*req) {
    cJSON *response = cJSON_CreateObject();

    cJSON_AddItemToObject(response, infoString, i2cInfo());

    return response;
}


/**
 * @brief      Scans a fun.
 *
 * @param      parm         The parameter
 * @param[in]  httpd_req_t  The httpd request
 *
 * @return     a json string from the actual function or an indication of the error
 */
static cJSON *scanFun(char *parm, UNUSED httpd_req_t *req) {
    cJSON *response = cJSON_CreateObject();

    cJSON_AddItemToObject(response, scanString, i2cScan());

    return response;
}

//
//static esp_err_t optionsFunction(cJSON *options, int address) {
//
//    static char *paramString = "wait";
//    int wait = 1000;
//    cJSON *waitJSON = cJSON_GetObjectItemCaseSensitive(options, paramString);
//    if (waitJSON && cJSON_IsNumber(waitJSON)) {
//        wait = waitJSON->valueint;
//
//    }
//
//    int index = -1;
//    static char *indexString = "index";
//    cJSON *indexJSON = cJSON_GetObjectItemCaseSensitive(options, indexString);
//    if ((indexJSON && cJSON_IsNumber(indexJSON))) {
//        index = indexJSON->valueint;
//
//    }
//
//    int mask = -1;
//    static char *maskString = "mask";
//    cJSON *maskJSON = cJSON_GetObjectItemCaseSensitive(options, maskString);
//    if ((maskJSON && cJSON_IsNumber(maskJSON))) {
//        mask = maskJSON->valueint;
//
//    }
//    int testValue = -1;
//    static char *testValueString = "Value";
//    cJSON *testValueJSON = cJSON_GetObjectItemCaseSensitive(options, testValueString);
//    if ((testValueJSON && cJSON_IsNumber(testValueJSON))) {
//        testValue = testValueJSON->valueint;
//
//    }
//    printf("options %s \n", cJSON_Print(options));
//    if (index == -1 || mask == -1 || testValue == -1) {
//        if (index != -1 || mask != -1 || testValue != -1) {
//            // we have an error
//        }
//        printf("wait is %d\n\n", wait);
//    } else {
//        printf("wait is %d, index= %d, mask = %d, value = %d\n", wait, index, mask, testValue);
//    }
//
//    return ESP_OK;
//
//}

/**
 * @brief      Gets the fun.
 *
 * @param      parm         The parameter
 * @param[in]  httpd_req_t  The httpd request
 *
 * @return     a json string from the actual function or an indication of the error
 */
static cJSON *getFun(char *parm, UNUSED httpd_req_t *req) {
    const char *funName = getString;

    cJSON *response = cJSON_CreateObject();

    printf("%s param %s => %s\n", funName, parm, cJSON_Print(cJSON_Parse(parm)));

    char *paramString = "Parse Error";
    cJSON *monitor_json = cJSON_Parse(parm);

    if (monitor_json) {

//if we do infact have params then check the type of each on
// we care about

        paramString = addressString;
        cJSON *address = cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
        if (!(address && cJSON_IsNumber(address))) {
            goto error;
        }

        paramString = indexString;
        cJSON *index = cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
        if (!(index && cJSON_IsNumber(index))) {
            goto error;
        }

        paramString = lengthString;
        cJSON *length = cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
        if (!(length && cJSON_IsNumber(length))) {
            goto error;
        }

        uint8_t and = 0;
        uint8_t test = 0;
        int delay = -1;
        uint8_t length_int = length->valueint;
        paramString = optionsString;
        cJSON *options = cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
        if (options) {
            delay = 300;
            cJSON *andItem = cJSON_GetObjectItemCaseSensitive(options, "and");
            if (!(andItem && cJSON_IsNumber(andItem))) {

                goto error;
            }
            cJSON *testItem = cJSON_GetObjectItemCaseSensitive(options, "test");
            if (!(testItem && cJSON_IsNumber(testItem))) {
                goto error;
            }
            test = testItem->valueint;

            and = andItem->valueint;
            cJSON *delayItem = cJSON_GetObjectItemCaseSensitive(options, "delay");
            if (delayItem && cJSON_IsNumber(delayItem)) {
                delay = delayItem->valueint;
            }
            printf("options {\"and\":%d,\"test\":%d,\"delay\":%d }\n", and, test, delay);
            length_int = 1;
        }

// everthing looks good call the get
        printf("{ \"%s\": %d, \"%s\": %d , \"%s\": %d } \n\n\n", addressString, (unsigned int) (address->valueint),
               indexString, (unsigned int) (index->valueint), lengthString, (unsigned int) (length->valueint));

        cJSON_AddItemToObject(response, funName,
                              i2cGet((uint8_t) (address->valueint), (uint8_t) (index->valueint), length_int, delay, and,
                                     test));
        return response;
    }

    error:                // we don't want to be here
    cJSON_AddStringToObject(response, "error", paramString);
    return response;
}


/**
 * @brief      Sets the fun.
 *
 * @param      parm  The parameter
 * @param      req   The new value
 *
 * @return     a json string from the actual function or an indication of the error
 */
static cJSON *setFun(char *parm, httpd_req_t *req) {
    const char *funName = setString;

    char *paramString = "Parse ";
// get the html data and put it in a temporary buffer
//////////////////////////////////////////////////////////
// this is much like the one about it could be folded
//////////////////////////////////////////////////////////
    cJSON *response = cJSON_CreateObject();
    printf("%s param %s => %s\n", funName, parm, cJSON_Print(cJSON_Parse(parm)));

    cJSON *monitor_json = cJSON_Parse(parm);
    if (monitor_json) {
        paramString = addressString;
        cJSON *address = cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
        if (!(address && cJSON_IsNumber(address))) {

            goto error;
        }

        paramString = indexString;
        cJSON *index = cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
        if (!(index && cJSON_IsNumber(index))) {

            goto error;
        }

        ///////////////////////////////////////////////////////////////
// a little majic here check the param of a data para (array) if not
        // then look for the data in the body.
        // Short or chained command can be sent as a stream of params
        // longer ones are in the body
        paramString = dataString;
        cJSON *data = cJSON_GetObjectItemCaseSensitive(monitor_json, dataString);
        if (!data) {
            // we do not have a {"data" : [1,2,3]} thing in the param string
            // look in the body
            size_t recv_size = req->content_len;
            char *tmp = malloc(recv_size);
            int ret = httpd_req_recv(req, tmp, recv_size);
            // parse it and delete the buffer
            printf("data %d %s\n\n", strlen(tmp), tmp);

            data = cJSON_GetObjectItemCaseSensitive(cJSON_Parse(tmp), dataString);
            free(tmp);
        }

        if (!(data && cJSON_IsArray(data))) {
            goto error;
        }

        printf("{ \"%s\": %d, \"%s\": %d  data: %s } \n\n\n", addressString, (unsigned int) (address->valueint),
               indexString, (unsigned int) (index->valueint), cJSON_Print(data));

        cJSON_AddItemToObject(response, funName,
                              i2cSet((uint8_t) (address->valueint), (uint8_t) (index->valueint), data));
        return response;

    }
    error:
    cJSON_AddStringToObject(response, "error", paramString);
    return response;
}


/**
 * @brief      the i2c function are patches here we take the link /i2c?***=xx had distribute it
 *
 * @param      req   The request
 *
 * @return     The esp error.
 */
esp_err_t i2c_patch_handler(httpd_req_t *req) {

    static const uriParam configurationParams[] = {{.name = scanString, .fun = scanFun},
                                                   {.name = getString, .fun = getFun},
                                                   {.name = setString, .fun = setFun},
                                                   {.name = infoString, .fun = infoFun},
                                                   {.name = NULL, .fun = NULL},};
    cors_header(req);
    cJSON *response = paramParser(req, configurationParams);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, cJSON_Print(response));
    cJSON_Delete(response);
    return ESP_OK;
}
