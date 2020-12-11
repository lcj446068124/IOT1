#include "exti.h"

extern char shutdown_wifi;

void EXTIX_Init(void)
{
		MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);  //设置P1.1（s1按键）为输入模式并拉高
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);               //清除P1.1中断 
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);                  //使能GPIO1.1中断
   
		MAP_Interrupt_setPriority(INT_PORT1, 0x20);
    MAP_Interrupt_enableInterrupt(INT_PORT1);                           //使能PORT1中断
    
		MAP_Interrupt_enableMaster();  //开启中断总开关  
}

void PORT1_IRQHandler(void)
{
    uint32_t status;
    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0); //P2.0输出高电平，LED2中红灯亮
    if(status & GPIO_PIN1) //判断P1.1按键（S1键）是否按压下
    {      
			shutdown_wifi = 1;
    }
}
