#include <string.h>
#include <fcntl.h>
#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "cJSON.h"

extern char ip_str[17];
esp_err_t cors_header (httpd_req_t * req);

// here we create a default menu item, the children parameter sets the type,
// other parameter can be set directly on the return object
static cJSON *
menuItem (const char *text, const char *icon, bool hidden,
	  const char *resource, cJSON * permissions, const char *to,
	  cJSON * children)
{
  char buf[48];

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

//cJSON *par =   cJSON_CreateObject ();
//cJSON_AddStringToObject(par ,"part" , "asd");
//cJSON_AddItemToObject(item ,"query",par);
  return item;
}


static cJSON *
createDialog (cJSON * array)
{
///============================================
  cJSON *subMenu = cJSON_CreateArray ();

  cJSON_AddItemToArray (subMenu,
			menuItem ("read", "read", false,
				  "http://192.168.100.100/api/v1/system/info",
				  NULL, "q1", NULL));


  cJSON_AddItemToArray (subMenu,
			menuItem ("write", "wire", false,
				  "http://192.168.100.100/api/v1/system/info",
				  NULL, "q2", NULL));


/*
* text: 'Project Management',
* icon: 'apps',
* hidden: false,
* to: '/project',
* permissions: [],
* type: 'MENU',
* resource: '', 
*/
  cJSON_AddItemToArray (array,
			menuItem ("dialog2", "dialog", false,
				  "http://192.168.100.100/api/v1/system/info",
				  NULL, "q3", subMenu));
  return array;
}


static cJSON *
createI2C (cJSON * array)
{
//************ create a sub menu*********
/*
*  text: 'item list',
*  hidden: false,
*  to: '/project/list',
*  permissions: [],
*  type: 'VIEW'*  resource: 'project/index',
*/
  cJSON *subMenu = cJSON_CreateArray ();

  cJSON_AddItemToArray (subMenu,
			menuItem ("scan", "scan", false,
				  "http://192.168.100.100/api/v1/system/info",
				  NULL, "q4", NULL));

/*
* text: 'Project Management',
* icon: 'apps',
* hidden: false,
* to: '/project',
* permissions: [],
* type: 'MENU',
* resource: '', 
*/
  cJSON_AddItemToArray (array,
			menuItem ("i2c", "i2c", false,
				  "http://192.168.100.100/api/v1/system/info",
				  NULL, "", subMenu));
  return array;
}

cJSON *
createMenus ()
{

  cJSON *array = cJSON_CreateArray ();

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

/*
* text: device,
* icon: 'Esp',
* hidden: false,
* to: '/eps',
* permissions: [],
* type: 'VIEW',
* resource: 'home/index',
*/


  // cJSON_AddItemToArray(array,
  //                      menuItem("info",// text
  //                               "   I",//icon
  //                               false,// hidden
  //                               "esp/info",// resource,
  //                               NULL,// permission
  //                               "/api/system/v1/info"// to
  //                              , NULL));


  cJSON *i2c = cJSON_CreateArray ();
  cJSON_AddItemToArray (i2c, menuItem ("scan",	// text
				       "S",	//icon
				       false,	// hidden
				       "esp/scan",	// resource,
				       NULL,	// permission
				       "/api/v1/i2c"	// to
				       , NULL));

  cJSON_AddItemToArray (i2c, menuItem ("read",	// text
				       "   R",	//icon
				       false,	// hidden
				       "esp/read",	// resource,
				       NULL,	// permission
				       "/api/system/v1/read"	// to
				       , NULL));

  cJSON_AddItemToArray (i2c, menuItem ("write",	// text
				       "   W",	//icon
				       false,	// hidden
				       "esp/write",	// resource,
				       NULL,	// permission
				       "/api/system/v1/write"	// to
				       , NULL));

//createI2C( array);
//createDialog( array);
  cJSON_AddItemToArray (array, menuItem ("i2c",	// text
					 "   I",	//icon
					 false,	// hidden
					 "esp/info",	// resource,
					 NULL,	// permission
					 "/api/system/v1/info"	// to
					 , i2c));

  cJSON *gpio = cJSON_CreateArray ();




  cJSON_AddItemToArray (gpio, menuItem ("blinky",	// text
					"   I",	//icon
					false,	// hidden
					"esp/info",	// resource,
					NULL,	// permission
					"/api/system/v1/gpio"	// to
					, NULL));

  cJSON_AddItemToArray (array, menuItem ("gpio",	// text
					 "   I",	//icon
					 false,	// hidden
					 "esp/blinker",	// resource,
					 NULL,	// permission
					 "/api/system/v1/blinker"	// to
					 , gpio));

  return array;
}
