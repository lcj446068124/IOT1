#include "ATCommand.h"

char tempBuffer[MAXBUFFERSIZE];										//取出缓冲区内所有数据


bool sendCommand(char* command){
	printf("Commond: %s",command);
	Usart_SendString(EUSCI_A2_BASE,command);
	delay_ms(3000);
	bool result = ATCheckSetResponse(&myBuffer);
	if(!result){
		printf("%s statu: ERROR\r\n",command);
		return false;
	}
	printf("%s statu: OK\r\n",command);
	return true;
}

bool ATCheckSetResponse(Buffer *b){
	int i = 0;
	while(!(BufferLength(b) >= 6 && b->data[(b->rear + MAXBUFFERSIZE - 2) % MAXBUFFERSIZE] == 0x0d && b->data[(b->rear + MAXBUFFERSIZE - 1) % MAXBUFFERSIZE] == 0x0a) && i < 500000){
		i++;
	}
	if(i == 500000){
		printf("Time Limit Exceed....\r\n");
		BufferInit(b);
		return false;
	}
	
	int ptr = 0;
	while(b->front != b->rear){
		tempBuffer[ptr++] = b->data[b->front];
		DeBuffer(b);
	}
	BufferInit(b);
	tempBuffer[ptr] = '\0';
	printf("Response:%s\r\nlen:%d\r\n",tempBuffer,ptr);
	for(i = 0;i + 1 < ptr;i++){
		if(tempBuffer[i] == 'O' && tempBuffer[i + 1] == 'K')
			return true;
	}
	return false;
//	if(tempBuffer[2] == 'O' && tempBuffer[3] == 'K'){
//		return true;
//	}else{
//		return false;
//	}
}

bool checkWifiConnection(char* command){
	for(int i = 0;i < 3;i++){
		printf("-------------------\r\n");
			if(sendCommand(command)){
				delay_ms(1000);
				int len = strlen(tempBuffer);
				for(int j = 0;j + 1 < len;j++){
					if(tempBuffer[j] == 'U' && tempBuffer[j + 1] =='P')
							return true;
				}
			}
			if(i < 2)
				printf("retry time(s) %d: \r\n",i + 1);
			if(i == 2)
				printf("*******fatal error:connecting wifi failed*******\r\n");
			delay_ms(500);
	}
	delay_ms(1000);
	return false;
}


bool execAT(char* command){
	for(int i = 0;i < 3;i++){
		printf("-------------------\r\n");
			if(sendCommand(command)){
				delay_ms(1000);
				return true;
			}
			if(i < 2)
				printf("retry time(s) %d: \r\n",i + 1);
			if(i == 2)
				printf("*******fatal error:%s exec failed*******\r\n",command);
			delay_ms(500);
	}
	delay_ms(1000);
	return false;
}

bool AT_Send_message(char* message){
		int i = 0;
		//char *ptr = tempBuffer;
		BufferInit(&myBuffer);
		memset(tempBuffer,0,sizeof(tempBuffer));
		strcpy(tempBuffer,"AT+MQTTSEND=");
		sprintf(tempBuffer + strlen(tempBuffer),"%d\r",strlen(message));
		printf("%s\r\n",tempBuffer);
		printf("%s\r\n",message);
		Usart_SendString(EUSCI_A2_BASE,tempBuffer);
		//Usart_SendString(EUSCI_A2_BASE,"AT+MQTTSEND=5\r");
		while(myBuffer.rear == myBuffer.front && i < 500000){
			i++;
		}
		if(myBuffer.data[myBuffer.front] != '>' || i == 500000){
			printf("Time Limit Exceed....\r\n");
			delay_ms(2000);
			BufferInit(&myBuffer);
			return false;
		}
		printf("sending data......\r\n");
		Usart_SendString(EUSCI_A2_BASE,message);
		printf("done!\r\n");
		return true;
}

char* AT_Get_SUB_In_Json(Buffer *b){
		int findJsonBegin = 0;
		int tempPtr = 0;
	bool findJson = false;
		 if(isEmpty(b)){
        return NULL;
    }
		//memset(tempBuffer,0,sizeof(tempBuffer));
    while(b->front != b->rear){
				if(b->data[b->front] == '{'){
					if(findJsonBegin == 0){
						memset(tempBuffer,0,sizeof(tempBuffer));
					}
					findJsonBegin++;
				}
				if(findJsonBegin && tempPtr < MAXBUFFERSIZE){
					tempBuffer[tempPtr++] = b->data[b->front];
				}
				if(findJsonBegin > 0 && b->data[b->front] == '}'){
					findJsonBegin--;
					if(findJsonBegin == 0){
						findJson = true;
						DeBuffer(b);
						break;
					}
				}
        DeBuffer(b);
    }
		if(!findJson)
			BufferInit(&myBuffer);
		if(tempPtr == 0 || findJsonBegin != 0 || tempPtr == MAXBUFFERSIZE){
			return NULL;
		}	
		return tempBuffer;
}

bool AT_generate_wifi_connect_command(char* ATCommandBuffer,int len,const char* ssid,const char* pwd){
	memset(ATCommandBuffer,0,len * sizeof(char));
	char head[] = "AT+WJAPN=";
	if(strlen(head) + strlen(ssid) + strlen(pwd) + 3 >= len)
			return false;
	memcpy(ATCommandBuffer, head, strlen(head));
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),ssid,strlen(ssid));
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),",",1);
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),pwd,strlen(pwd));
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),"\r",1);
	return true;
}

bool AT_generate_MQTTAUTH_command(char* ATCommandBuffer,int len,iotx_sign_mqtt_t* signout){
	memset(ATCommandBuffer,0,len * sizeof(char));
	char head[] = "AT+MQTTAUTH=";
	if(strlen(head) + strlen(signout->username) + strlen(signout->password) + 3 >= len)
			return false;
	memcpy(ATCommandBuffer, head, strlen(head));
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),signout->username,strlen(signout->username));
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),",",1);
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),signout->password,strlen(signout->password));
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),"\r",1);
	return true;
}

bool AT_generate_MQTTSOCK_command(char* ATCommandBuffer,int len,iotx_sign_mqtt_t* signout){
	memset(ATCommandBuffer,0,len * sizeof(char));
	char head[] = "AT+MQTTSOCK=";
	if(strlen(head) + strlen(signout->hostname) + 4 + 3 >= len)
			return false;
	memcpy(ATCommandBuffer, head, strlen(head));
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),signout->hostname,strlen(signout->hostname));
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),",",1);
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),"1883",4);
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),"\r",1);
	return true;
}

bool AT_generate_MQTTCID_command(char* ATCommandBuffer,int len,iotx_sign_mqtt_t* signout){
	memset(ATCommandBuffer,0,len * sizeof(char));
	char head[] = "AT+MQTTCID=";
	if(strlen(head) + strlen(signout->clientid) + 2 >= len)
			return false;
	memcpy(ATCommandBuffer, head, strlen(head));
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),signout->clientid,strlen(signout->clientid));
	memcpy(ATCommandBuffer + strlen(ATCommandBuffer),"\r",1);
	return true;
}

//memcpy(ATCommandBuffer,,strlen());
//"AT+MQTTSUB=0,/sys/a1mCXgajcO4/light0/thing/service/property/set,0\r"
bool AT_generate_MQTTSUB_command(char* ATCommandBuffer,int len,char* subId,const char* ProductKey,const char* DeviceName){
	memset(ATCommandBuffer,0,len * sizeof(char));
	char head[] = "AT+MQTTSUB=";
	char en[] = "/thing/service/property/set,0\r";
	if(strlen(head) + strlen(en) + strlen(subId) + strlen(ProductKey) + strlen(DeviceName) + 7>= len)
			return false;
	memcpy(ATCommandBuffer, head, strlen(head));
	memcpy(ATCommandBuffer+ strlen(ATCommandBuffer),subId,strlen(subId));
	memcpy(ATCommandBuffer+ strlen(ATCommandBuffer),",/sys/",strlen(",/sys/"));
	memcpy(ATCommandBuffer+ strlen(ATCommandBuffer),ProductKey,strlen(ProductKey));
	memcpy(ATCommandBuffer+ strlen(ATCommandBuffer),"/",strlen("/"));
	memcpy(ATCommandBuffer+ strlen(ATCommandBuffer),DeviceName,strlen(DeviceName));
	memcpy(ATCommandBuffer+ strlen(ATCommandBuffer),en,strlen(en));
	return true;
}
//"AT+MQTTPUB=/sys/a1mCXgajcO4/light0/thing/event/property/post,0\r"
bool AT_generate_MQTTPUB_command(char* ATCommandBuffer,int len,const char* ProductKey,const char* DeviceName){
	memset(ATCommandBuffer,0,len * sizeof(char));
	char head[] = "AT+MQTTPUB=";
	char en[] = "/thing/event/property/post,0\r";
	if(strlen(head) + strlen(en) + strlen(ProductKey) + strlen(DeviceName) + 6>= len)
			return false;
	memcpy(ATCommandBuffer, head, strlen(head));
	memcpy(ATCommandBuffer+ strlen(ATCommandBuffer),"/sys/",strlen("/sys/"));
	memcpy(ATCommandBuffer+ strlen(ATCommandBuffer),ProductKey,strlen(ProductKey));
	memcpy(ATCommandBuffer+ strlen(ATCommandBuffer),"/",strlen("/"));
	memcpy(ATCommandBuffer+ strlen(ATCommandBuffer),DeviceName,strlen(DeviceName));
	memcpy(ATCommandBuffer+ strlen(ATCommandBuffer),en,strlen(en));
	return true;

}


