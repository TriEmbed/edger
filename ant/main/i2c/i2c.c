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


/**
 * @brief      {  get the i2c port info}
 *
 * @param      parm         The parameter
 * @param[in]  httpd_req_t  The httpd request
 *
 * @return     a json string from the actual function or an indication of the error
 */
static cJSON *
infoFun(char *parm, UNUSED httpd_req_t* req)
{
    cJSON *response = cJSON_CreateObject();

    cJSON_AddItemToObject (response, infoString, i2cInfo());

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
static cJSON *
scanFun(char *parm, UNUSED httpd_req_t

* req)
{
cJSON *response = cJSON_CreateObject();

cJSON_AddItemToObject (response, scanString, i2cScan()
);

return
response;
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
getFun(char *parm, UNUSED httpd_req_t

* req)
{
const char *funName = getString;

cJSON *response = cJSON_CreateObject();

printf ("%s param %s => %s\n", funName, parm,

cJSON_Print (cJSON_Parse(parm))

);

char *paramString = "Parse Error";
cJSON *monitor_json = cJSON_Parse(parm);

if (monitor_json)
{

//if we do infact have params then check the type of each on
// we care about
paramString = addressString;
cJSON *address =
        cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
if (!(address &&
cJSON_IsNumber (address)
))
{
goto
error;
}

paramString = indexString;
cJSON *index =
        cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
if (!(index &&
cJSON_IsNumber (index)
))
{
goto
error;
}

paramString = lengthString;
cJSON *length =
        cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
if (!(length &&
cJSON_IsNumber (length)
))
{
goto
error;
}

// everthing looks good call the get
printf ("{ \"%s\": %d, \"%s\": %d , \"%s\": %d } \n\n\n",
addressString, (unsigned int) (address->valuedouble),
indexString, (unsigned int) (index->valuedouble),
lengthString, (unsigned int) (length->valuedouble));
cJSON_AddItemToObject (response, funName,
        i2cGet((unsigned char)(address
->valuedouble),
(unsigned char) (index->valuedouble),
(unsigned char) (length->valuedouble)));
return
response;
}

error:                // we don't want to be here
cJSON_AddStringToObject (response,
"error", paramString);
return
response;
}


/**
 * @brief      Sets the fun.
 *
 * @param      parm  The parameter
 * @param      req   The new value
 *
 * @return     a json string from the actual function or an indication of the error
 */
static cJSON *
setFun(char *parm, httpd_req_t *req) {
    const char *funName = setString;

    char *paramString = "Parse ";
// get the html data and put it in a temporary buffer 
//////////////////////////////////////////////////////////
// this is much like the one about it could be folded 
//////////////////////////////////////////////////////////
    cJSON *response = cJSON_CreateObject();
    printf("%s param %s => %s\n", funName, parm,
           cJSON_Print(cJSON_Parse(parm)));

    cJSON *monitor_json = cJSON_Parse(parm);
    if (monitor_json) {
        paramString = addressString;
        cJSON *address =
                cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
        if (!(address && cJSON_IsNumber(address))) {

            goto error;
        }

        paramString = indexString;
        cJSON *index =
                cJSON_GetObjectItemCaseSensitive(monitor_json, paramString);
        if (!(index && cJSON_IsNumber(index))) {

            goto error;
        }

        ///////////////////////////////////////////////////////////////
// a little majic here check the param of a data para (array) if not
        // then look for the data in the body.
        // Short or chained command can be sent as a stream of params
        // longer ones are in the body
        paramString = dataString;
        cJSON *data =
                cJSON_GetObjectItemCaseSensitive(monitor_json, dataString);
        if (!data) {
            // we do not have a {"data" : [1,2,3]} thing in the param string
            // look in the body
            size_t recv_size = req->content_len;
            char *tmp = malloc(recv_size);
            int ret = httpd_req_recv(req, tmp, recv_size);
            // parse it and delete the buffer
            printf("data %d %s\n\n",strlen(tmp),tmp);

            data =
                    cJSON_GetObjectItemCaseSensitive(cJSON_Parse(tmp), dataString);
            free(tmp);
        }

        if (!(data && cJSON_IsArray(data))) {
            goto error;
        }

        printf("{ \"%s\": %d, \"%s\": %d  data: %s } \n\n\n",
               addressString, (unsigned int) (address->valuedouble),
               indexString, (unsigned int) (index->valuedouble),
               cJSON_Print(data));

        cJSON_AddItemToObject(response, funName,
                              i2cSet((unsigned
                                     char) (address->valuedouble),
                                     (unsigned char) (index->valuedouble),
                                     data));
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
esp_err_t
i2c_patch_handler(httpd_req_t *req) {

    static const uriParam configurationParams[] = {
            {.name = scanString, .fun = scanFun},
            {.name = getString, .fun = getFun},
            {.name = setString, .fun = setFun},
            {.name = infoString, .fun = infoFun},
            {.name = NULL, .fun = NULL},
    };
    cors_header(req);
    cJSON *response = paramParser(req, configurationParams);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, cJSON_Print(response));
    cJSON_Delete(response);
    return ESP_OK;
}
