#ifndef _ATCOMMAND_H_
#define _ATCOMMAND_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include "myUart.h"
#include "buffer.h"
#include "tick.h"
#include <string.h>
#include "cJSON.h"
#include "sign_api.h"
bool execAT(char* command);
bool sendCommand(char* command);
bool ATCheckSetResponse(Buffer *b);
bool AT_Send_message(char* message);
char* AT_Get_SUB_In_Json(Buffer *b);
bool AT_generate_wifi_connect_command(char* ATCommandBuffer,int len,const char* ssid,const char* pwd);
bool AT_generate_MQTTAUTH_command(char* ATCommandBuffer,int len,iotx_sign_mqtt_t* signout);
bool AT_generate_MQTTSOCK_command(char* ATCommandBuffer,int len,iotx_sign_mqtt_t* signout);
bool AT_generate_MQTTCID_command(char* ATCommandBuffer,int len,iotx_sign_mqtt_t* signout);

bool AT_generate_MQTTSUB_command(char* ATCommandBuffer,int len,char* subId,const char* ProductKey,const char* DeviceName);
bool checkWifiConnection(char* command);
bool AT_generate_MQTTPUB_command(char* ATCommandBuffer,int len,const char* ProductKey,const char* DeviceName);

#endif


