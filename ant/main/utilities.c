//
// Created by nicked on 10/7/22.
//
#include <stddef.h>
#include <string.h>
#include <esp_http_server.h>
#include <esp_log.h>
#include <cJSON.h>
#include "utilities.h"


static char *
uri_decode (const char *src)
{
  int i = 0;
  size_t size = 0;
  size_t len = 0;
  char *dec = NULL;
  char tmp[3];
  char ch = 0;

  // chars len
  len = strlen (src);

  // alloc
  dec = (char *) malloc (len + 1);

#define push(c) (dec[size++] = c)

  // decode
  while (len--)
    {
      ch = src[i++];

      // if prefix `%' then read byte and decode
      if ('%' == ch)
	{
	  tmp[0] = src[i++];
	  tmp[1] = src[i++];
	  tmp[2] = '\0';
	  push (strtol (tmp, NULL, 16));
	}
      else
	{
	  push (ch);
	}
    }

  dec[size] = '\0';

#undef push

  return dec;
}

cJSON *
paramParser (httpd_req_t * req, const uriParam * list)
{
  static char buffer[500];
  strcpy (buffer, req->uri);

  cJSON *root = cJSON_CreateObject ();
  cJSON *returns = cJSON_CreateArray ();
  if (strchr (buffer, '?') == 0)
    {
      int cnt = 0;

      while (list[cnt].fun)
	{
	  cJSON_AddItemToArray (returns, cJSON_CreateString (list[cnt].name));
	  cnt++;
	}

      cJSON_AddItemToObject (root, strrchr (buffer, '/') + 1, returns);
      return root;

    }
  char *token = strtok (buffer, "?");

  printf ("buffer {%s} token[%d] {%s} ", buffer, 0,
	  token == NULL ? "none" : token);


  /* walk through the token tokens look for the end of a param */
  while ((token = strtok (NULL, "&")) != NULL)
    {
      int cnt = 0;
      printf (" token[%d] {%s} \n", cnt, token == NULL ? "none" : token);
      while (list[cnt].fun)
	{

	  // find the equal sign "=" seperating the name from the value
	  // noting that the value could be empty but the equal sign will
	  // still be there.
	  // if the param string is found strcpm will show us the
	  // point where the two string differ. i.e. the end equal sign
	  int len = strlen (list[cnt].name);
	  int equChar = strncmp (token, list[cnt].name, len);


	  // ok we have found it
	  if (equChar == 0)
	    {
	      // just checking that the function is implemented
	      if (list[cnt].fun != NULL)
		{
		  // make back into a json string
		  char *val = uri_decode (token);
		  // what ever it is we are supposed to do



		  cJSON *data = (*list[cnt].fun) (val + len + 1, req);



		  cJSON_AddItemToArray (returns, data);
		  // clean up the malloc created in uri_decode
		  free (val);
		  // look no more
		  break;
		}
	    }
	  cnt++;
	}
    }
  cJSON_AddItemToObject (root, strrchr (buffer, '/') + 1, returns);
  return root;
}
