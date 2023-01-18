//
// Created by nicked on 10/7/22.
//

#ifndef ANT_UTILITIES_H
#define ANT_UTILITIES_H

#include "esp_vfs.h"

typedef struct uriParam {
    char *name;

    cJSON *(*fun)(char *, httpd_req_t *req);
} uriParam;

cJSON *paramParser(httpd_req_t *req, const uriParam *list);

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)
#define SCRATCH_BUFSIZE (10240)
typedef struct rest_server_context {
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;


#define UNUSED __attribute__((unused))
#endif //ANT_UTILITIES_H
