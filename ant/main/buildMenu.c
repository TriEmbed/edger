#include "esp_http_server.h"
#include "esp_system.h"
#include "cJSON.h"

cJSON *devices;
extern char ip_str[17];
// https://stackoverflow.com/questions/14521108/dynamically-load-js-inside-js
//https://stackoverflow.com/questions/9413737/how-to-append-script-script-in-javascript
//

esp_err_t cors_header (httpd_req_t * req);
extern char ip_str[17];
// here we create a default menu item, the children parameter sets the type,
// other parameter can be set directly on the return object
static cJSON *
// other parameter can be set directly on the return object
menuItem (const char *text, const char *icon, bool hidden,
	  const char *resource, cJSON * permissions, const char *to,
	  cJSON * children)
{
  static char buf[128];

  cJSON *item = cJSON_CreateObject ();
  sprintf (buf, "http://%s%s", ip_str, to);
  cJSON_AddStringToObject (item, "path", buf);
  cJSON_AddStringToObject (item, "text", text);
  cJSON_AddStringToObject (item, "icon", icon);
  cJSON_AddItemToObject (item, "hidden",
			 hidden ? cJSON_CreateTrue () : cJSON_CreateFalse ());
  cJSON_AddStringToObject (item, "to", to);

  cJSON_AddItemToObject (item, "permissions",
			 permissions ==
			 NULL ? cJSON_CreateArray () : permissions);
  cJSON_AddStringToObject (item, "type", children == NULL ? "VIEW" : "MENU");

  cJSON_AddStringToObject (item, "resource", resource);
  if (children)
    cJSON_AddItemToObject (item, "children", children);


  return item;
}




static void
createADT7420Menus (cJSON * array)
{

  cJSON *ADT7420 = cJSON_CreateArray ();

  cJSON_AddItemToArray (ADT7420, menuItem ("Read",	// text
					   "F",	//icon
					   false,	// hidden
					   "ADT7420/file/readFile",	// resource,
					   NULL,	// permission
					   "/api/system/v1/ADT7420/read"	// to
					   , NULL));



  cJSON_AddItemToArray (array, menuItem ("ADT7420",	// text
					 "R",	//icon
					 false,	// hidden
					 "Reneses",	// resource,
					 NULL,	// permission
					 "/api/system/v1/write"	// to
					 , ADT7420));

}


static void
createHSPPD43aMenus (cJSON * array)
{
  cJSON *HSPPD43A = cJSON_CreateArray ();
  cJSON_AddItemToArray (HSPPD43A, menuItem ("Read",	// text
					    "F",	//icon
					    false,	// hidden
					    "ADT7420/file/readFile",	// resource,
					    NULL,	// permission
					    "/api/system/v1/HSPPD43A/read"	// to
					    , NULL));



  cJSON_AddItemToArray (array, menuItem ("HSPPD43A",	// text
					 "R",	//icon
					 false,	// hidden
					 "HSPPD43A",	// resource,
					 NULL,	// permission
					 "/api/system/v1/write"	// to
					 , HSPPD43A));

}

static void
createAD9833Menus (cJSON * array)
{

  cJSON *AD9833 = cJSON_CreateArray ();
  cJSON_AddItemToArray (AD9833, menuItem ("Read",	// text
					  "F",	//icon
					  false,	// hidden
					  "ADT7420/file/readFile",	// resource,
					  NULL,	// permission
					  "/api/system/v1/AD9833/read"	// to
					  , NULL));



  cJSON_AddItemToArray (array, menuItem ("AD9833",	// text
					 "R",	//icon
					 false,	// hidden
					 "AD9833",	// resource,
					 NULL,	// permission
					 "/api/system/v1/write"	// to
					 , AD9833));

}



static void
createRenesasMenus (cJSON * array)
{

  cJSON *Renesas = cJSON_CreateArray ();

  cJSON_AddItemToArray (Renesas, menuItem ("Read File",	// text
					   "F",	//icon
					   false,	// hidden
					   "Renesas/file/readFile",	// resource,
					   NULL,	// permission
					   "/api/system/v1/renesas/read"	// to
					   , NULL));

  cJSON_AddItemToArray (Renesas, menuItem ("BuzyBox",	// text
					   "O",	//icon
					   false,	// hidden
					   "Renesas/buzybox",	// resource,
					   NULL,	// permission
					   "/api/system/v1/rensas/buzybox"	// to
					   , NULL));

  cJSON_AddItemToArray (array, menuItem ("Renesas",	// text
					 "R",	//icon
					 false,	// hidden
					 "Reneses",	// resource,
					 NULL,	// permission
					 "/api/system/v1/write"	// to
					 , Renesas));

}

/*
 *I2CM 
 */
static void
createI2CMenus (cJSON * array)
{
  cJSON *i2c = cJSON_CreateArray ();
  cJSON_AddItemToArray (i2c, menuItem ("scan",	// text
				       "S",	//icon
				       false,	// hidden
				       "esp/scani2c",	// resource,
				       NULL,	// permission
				       "/api/v1/i2c"	// to
				       , NULL));

  cJSON_AddItemToArray (i2c, menuItem ("edit",	// text
				       " R",	//icon
				       false,	// hidden
				       "i2c/editi2c",	// resource,
				       NULL,	// permission
				       "/api/system/v1/edit"	// to
				       , NULL));



  // cJSON *endPoints = cJSON_CreateArray ();
  // createADT7420Menus (endPoints);
  // createAD9833Menus (endPoints);
  // createADT7420Menus (endPoints);
  // cJSON_AddItemToArray (i2c, menuItem ("endPoints",  // text
  //                     "   W",  //icon
  //                     false,   // hidden
  //                     "esp/write",     // resource,
  //                     NULL,    // permission
  //                     "/api/system/v1/write"   // to
  //                     , endPoints));


  cJSON_AddItemToArray (array, menuItem ("i2c",	// text
					 "   I",	//icon
					 false,	// hidden
					 "esp/info",	// resource,
					 NULL,	// permission
					 "/api/system/v1/info"	// to
					 , i2c));
}


static void
createGPIOMenus (cJSON * array)
{
  cJSON *gpio = cJSON_CreateArray ();


  cJSON_AddItemToArray (gpio, menuItem ("blinky",	// text
					"   I",	//icon
					false,	// hidden
					"esp/info",	// resource,
					NULL,	// permission
					"/api/system/v1/gpio"	// to
					, NULL));

  cJSON_AddItemToArray (gpio, menuItem ("Lights",	// text
					"   I",	//icon
					false,	// hidden
					"Light",	// resource,
					NULL,	// permission
					"/Light"	// to
					, NULL));

  cJSON_AddItemToArray (array, menuItem ("gpio",	// text
					 "   I",	//icon
					 false,	// hidden
					 "esp/blinker",	// resource,
					 NULL,	// permission
					 "/api/system/v1/blinker"	// to
					 , gpio));
}




cJSON *
createMenus ()
{

  cJSON *array = cJSON_CreateArray ();

  createI2CMenus (array);
  createGPIOMenus (array);
    createRenesasMenus( array);
  return array;
}
