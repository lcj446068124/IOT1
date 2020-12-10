///******************************************************************************
// *
// *               MSP432P401
// *             -----------------
// *            |                 |
// *            |                 |
// *            |                 |
// *       RST -|     P1.3/UCA0TXD|----> PC 
// *            |                 |
// *            |                 |
// *            |     P1.2/UCA0RXD|<---- PC
// *            |                 |
// *
// *******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "tick.h"
#include "myUart.h"
#include "buffer.h"
#include "ATCommand.h"
#include "cJSON.h"
#include "sign_api.h"

#define MaxCommandLength 100

const char* wifi_ssid 		=	"wangjiacai";
const char* wifi_pwd 			= "www.202058";
const char* ProductKey		= "a1za4jJ21X0";
const char* ProductSecret = "UDhiZmCv3WIv0mY3";
const char* DeviceName 		= "led0";
const char* DeviceSecret 	= "b35ec9fc8de7d6a40b2780f23bb90915";
char ATCommandBuffer[MaxCommandLength];

//打开微库 Options->Target->Code Generation->Use MicroLIB
int fputc(int ch, FILE *f)
{
	MAP_UART_transmitData(EUSCI_A0_BASE, ch & 0xFF);
	return ch;
}

int main(void)
{  
    /* 停用开门狗 */
    MAP_WDT_A_holdTimer();
    
    //![Simple FPU Config]   
    MAP_FPU_enableModule();/*启用FPU加快DCO频率计算，注：DCO是内部数字控制振荡器，默认是3M频率*/    
    MAP_CS_setDCOFrequency(4000000);/* 设置DCO频率为指定频率，此处DCO=4M*/
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);/*设置MCLK（主时钟，可用于CPU主频等），MCLK=DCO频率/时钟分频系数，此处MCLK=DCO=4M*/
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);/*设置HSMCLK（子系统主时钟），HSMCLK=DCO频率/时钟分频系数，此处HSMCLK=DCO/4=1M*/
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);/*设置SMCLK（低速子系统主时钟，可用TimerA频率），SMCLK=DCO频率/时钟分频系数，此处SMCLK=DCO/4=1M*/    
    //![Simple FPU Config]
	
		ms_ticker_init();
	  MAP_SysTick_enableModule();
    MAP_SysTick_enableInterrupt();//systick config
		//LED Config
		MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
		MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);	
	
		myUartInit();
	
		iotx_dev_meta_info_t *meta = (iotx_dev_meta_info_t *) malloc(sizeof(iotx_dev_meta_info_t));
    strcpy(meta->device_name, DeviceName);
    strcpy(meta->device_secret, DeviceSecret);
    strcpy(meta->product_key, ProductKey);
    strcpy(meta->product_secret, ProductSecret);
		iotx_sign_mqtt_t signout;
		if(IOT_Sign_MQTT(IOTX_CLOUD_REGION_SHANGHAI,meta,&signout) != 0){
			printf("pwd generate error!!!\r\n");
			while(1);
		}else{
			free(meta);
			meta = NULL;
		}
		
		delay_ms(2000);//等待3080系统初始化时间
		do{
			execAT("AT+UARTE=OFF\r");//关闭指令回显
			execAT("AT+MQTTCLOSE\r");																														//if presents error,just let it go!
			execAT("AT+WEVENT=OFF\r");
			execAT("AT+MQTTEVENT=OFF\r");																												//mqtt close turn on
			AT_generate_wifi_connect_command(ATCommandBuffer,MaxCommandLength,wifi_ssid,wifi_pwd);
			execAT(ATCommandBuffer);
			AT_generate_MQTTAUTH_command(ATCommandBuffer,MaxCommandLength,&signout);
			execAT(ATCommandBuffer);
			//execAT("AT+MQTTAUTH=light0&a1mCXgajcO4,8F04564BBFEC5C07BA5D842891BA0CA13F04CC33\r");//close reset
			AT_generate_MQTTSOCK_command(ATCommandBuffer,MaxCommandLength,&signout);
			execAT(ATCommandBuffer);
			//execAT("AT+MQTTSOCK=a1mCXgajcO4.iot-as-mqtt.cn-shanghai.aliyuncs.com,1883\r");			//close reset
			execAT("AT+MQTTCAVERIFY=OFF,OFF\r");
			execAT("AT+MQTTSSL=OFF\r");
			AT_generate_MQTTCID_command(ATCommandBuffer,MaxCommandLength,&signout);
			execAT(ATCommandBuffer);
			//execAT("AT+MQTTCID=a1mCXgajcO4.light0|securemode=3\\,signmethod=hmacsha1|\r");			//close reset
			execAT("AT+MQTTKEEPALIVE=30\r");
			execAT("AT+MQTTRECONN=ON\r");
			execAT("AT+MQTTAUTOSTART=ON\r");																											//close reset
			AT_generate_MQTTSUB_command(ATCommandBuffer,MaxCommandLength,"0",ProductKey,DeviceName);
		}while(!execAT("AT+MQTTSTART\r") || !execAT(ATCommandBuffer));
		
		AT_generate_MQTTPUB_command(ATCommandBuffer,MaxCommandLength,ProductKey,DeviceName);
		while(!execAT(ATCommandBuffer));
		
		AT_Send_message("{\"id\":1605187527200,\"params\":{\"PowerSwitch\":1},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}");										//物模型属性上报
		
    while(1){
			delay_ms(1000);
			//traverseBuffer(&myBuffer);
			char* Sub_Json_ptr = AT_Get_SUB_In_Json(&myBuffer);
			if(Sub_Json_ptr != NULL){
				cJSON* praseDataPtr = cJSON_Parse(Sub_Json_ptr);
				cJSON* params = cJSON_GetObjectItemCaseSensitive(praseDataPtr, "params");
				if (cJSON_IsObject(params))
				{
						cJSON *PowerSwitch = cJSON_GetObjectItemCaseSensitive(params, "PowerSwitch");
						if(cJSON_IsNumber(PowerSwitch)){
							int choice = PowerSwitch->valueint;
							if(choice == 0){
								printf("turn off the light \r\n");
								MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
								printf("Return statu to aliIOT\r\n");
								AT_Send_message("{\"id\":1605187527200,\"params\":{\"PowerSwitch\":0},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}");
							}else if(choice == 1){
								printf("turn on the light \r\n");
								MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
								printf("Return statu to aliIOT\r\n");
								AT_Send_message("{\"id\":1605187527200,\"params\":{\"PowerSwitch\":1},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}");
							}
						}
				}
				cJSON_Delete(praseDataPtr);
			}
		}
		
}



