#include "led.h"

void LED_Init(){
		
		//LED Config
		MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
		MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);	
		//红灯
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN0);    //设置P2.0为输出模式
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0);   //使LED2中红色灯初始为低电平（常暗）
    
    //绿灯
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN1);    //设置P2.1为输出模式
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1); //使LED2中绿色灯初始为低电平（常暗）
	
		//绿灯
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN2);    //设置P2.2为输出模式
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN2); //使LED2中蓝色灯初始为低电平（常暗）

}

void turnOffLed2(){
	 GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0);   //使LED2中红色灯初始为低电平（常暗）
	 GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1); //使LED2中绿色灯初始为低电平（常暗）
	 GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN2); //使LED2中蓝色灯初始为低电平（常暗）
}

void turnOffLed1(){
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);	
}

void turnOnLed2Red(){
	GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);
}

void toggleLed2Red(){
	GPIO_toggleOutputOnPin(GPIO_PORT_P2,GPIO_PIN0);
}

void turnOnLed2Green(){
	GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
}

void toggleLed2Green(){
	GPIO_toggleOutputOnPin(GPIO_PORT_P2,GPIO_PIN1);
}

void turnOnLed2Blue(){
	GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);
}

void toggleLed2Blue(){
	GPIO_toggleOutputOnPin(GPIO_PORT_P2,GPIO_PIN2);
}

void tunnOnLed1(){
	MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);	
}



void led2ShowRed(){
	turnOffLed2();
	turnOnLed2Red();
}

void led2ShowGreen(){
	turnOffLed2();
	turnOnLed2Green();
}

void led2ShowBlue(){
	turnOffLed2();
	turnOnLed2Blue();
}

void led2ShowGreenAndRed(){
	turnOffLed2();
	turnOnLed2Red();
	turnOnLed2Green();
}
/*

*/
void led2blink(char color,int n,int gab_ms){
	turnOffLed2();
	for(int i = 0;i < 2 * n;i++){
		if(color&Red)
			toggleLed2Red();
		if(color&Green)
			toggleLed2Green();
		if(color&Blue)
			toggleLed2Blue();
		delay_ms(gab_ms);
	}
	turnOffLed2();
}

