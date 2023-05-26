//
// Created by nicked on 10/7/22.
//

#ifndef ANT_I2C_DRIVER_H
#define ANT_I2C_DRIVER_H

cJSON *i2cDump(uint8_t chip_addr, uint8_t size);
cJSON *i2cScan();
cJSON *i2cGet(uint8_t chip_addr, uint8_t data_addr, uint8_t len,int delay,uint8_t and,uint8_t test);
cJSON *i2cGetRaw(uint8_t chip_addr, uint8_t data_addr, uint8_t len);
cJSON *i2cSet(uint8_t chip_addr, uint8_t reg, cJSON *data);
cJSON *i2cInfo ();


#endif //ANT_I2C_DRIVER_H
