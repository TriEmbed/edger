//
// Created by nicked on 10/7/22.
//

#ifndef ESP_REST_MAIN_H
#define ESP_REST_MAIN_H

 typedef union
  {
    uint8_t mac_addr[6];
    uint64_t value;
 
  } uDevice;
extern char macString [18];
extern char mdns_address[22];
extern uDevice device;
extern tcpip_adapter_ip_info_t ipInfo;
extern char ipString [16];
#endif //ESP_REST_MAIN_H


