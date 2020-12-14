#include "myUart.h"
void myUartInit(){
//		eUSCI_UART_Config uartConfig =
//		{
//						EUSCI_A_UART_CLOCKSOURCE_SMCLK,          //选用SMCLK（1M）时钟源
//						6,                                       // BRDIV = 6 ，clockPrescalar时钟分频系数 
//						8,                                       // UCxBRF = 8  firstModReg （BRDIV、UCxBRF、 UCxBRS和SMCLK，用于设置串口波特率）
//						17,                                      // UCxBRS = 17 secondModReg
//						EUSCI_A_UART_NO_PARITY,                  // 校验位None
//						EUSCI_A_UART_LSB_FIRST,                  // 低位优先，小端模式
//						EUSCI_A_UART_ONE_STOP_BIT,               // 停止位1位
//						EUSCI_A_UART_MODE,                       // UART mode
//						EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // 设置为过采样，该数值为1
//		};
	
		eUSCI_UART_Config uartConfig =
		{
						EUSCI_A_UART_CLOCKSOURCE_SMCLK,          //选用SMCLK（1M）时钟源
						8,                                       // BRDIV = 8 ，clockPrescalar时钟分频系数 
						0,                                       // UCxBRF = 0  firstModReg （BRDIV、UCxBRF、 UCxBRS和SMCLK，用于设置串口波特率）
						214,                                      // UCxBRS = 214 secondModReg
						EUSCI_A_UART_NO_PARITY,                  // 校验位None
						EUSCI_A_UART_LSB_FIRST,                  // 低位优先，小端模式
						EUSCI_A_UART_ONE_STOP_BIT,               // 停止位1位
						EUSCI_A_UART_MODE,                       // UART mode
						EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION 
		};
	
		/* 选用P1.2和P1.3使用UART模式 */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    
    /* 使用uartConfig配置UART_A0 */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* 使能UART_A0 */
    MAP_UART_enableModule(EUSCI_A0_BASE);
		
		/* 选用P3.2和P3.3使用UART模式 */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    
    /* 使用uartConfig配置UART_A2 */
    MAP_UART_initModule(EUSCI_A2_BASE, &uartConfig);

    /* 使能UART_A2 */
    MAP_UART_enableModule(EUSCI_A2_BASE);
		
		MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT); /*使能UART_A2串口接收中断*/ 
    MAP_Interrupt_enableInterrupt(INT_EUSCIA2); /*使能UART_A2中断*/  
    MAP_Interrupt_enableMaster();  /*使能中断总开关*/   
		
		BufferInit(&myBuffer);				//初始化UART_A2接收缓存
}

void myEUSCIA0Init(){
	
	eUSCI_UART_Config uartConfig =
		{
						EUSCI_A_UART_CLOCKSOURCE_SMCLK,          //选用SMCLK（1M）时钟源
						8,                                       // BRDIV = 8 ，clockPrescalar时钟分频系数 
						0,                                       // UCxBRF = 0  firstModReg （BRDIV、UCxBRF、 UCxBRS和SMCLK，用于设置串口波特率）
						214,                                      // UCxBRS = 214 secondModReg
						EUSCI_A_UART_NO_PARITY,                  // 校验位None
						EUSCI_A_UART_LSB_FIRST,                  // 低位优先，小端模式
						EUSCI_A_UART_ONE_STOP_BIT,               // 停止位1位
						EUSCI_A_UART_MODE,                       // UART mode
						EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION 
		};
	
		/* 选用P1.2和P1.3使用UART模式 */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    
    /* 使用uartConfig配置UART_A0 */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* 使能UART_A0 */
    MAP_UART_enableModule(EUSCI_A0_BASE);

}

void Usart_SendByte(uint32_t moduleInstance, uint_fast8_t transmitData){
	UART_transmitData(moduleInstance, transmitData);
}

void Usart_SendString(uint32_t moduleInstance, char* str){
	uint32_t k = 0;
	while (*(str + k)!='\0'){
		Usart_SendByte( moduleInstance, *(str + k) );
		k++;
	}
}
/*UART_A2中断函数*/
void EUSCIA2_IRQHandler(void)
{
		uint8_t val;    
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE); /*将UART_A2中断标志位的值赋值给status*/
    MAP_UART_clearInterruptFlag(EUSCI_A2_BASE, status);/*清空UART_A2中断标志位*/
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) /*判断是否为UART_A2接受中断*/
    {      
			
			val = UART_receiveData(EUSCI_A2_BASE);
			EnBuffer(&myBuffer,val);
			
			//UART_transmitData(EUSCI_A0_BASE, val);		
    }   
}

//        sprintf(tmp,"%c",MAP_UART_receiveData(EUSCI_A2_BASE));/*将从UART_A0中接收到的单个字节以十六进制打印至tmp中*/
//        /*将tmp字符串通过UART_A0逐个字符发出*/
//        for (i=0;i<strlen(tmp);i++) 
//            MAP_UART_transmitData(EUSCI_A0_BASE, tmp[i]); 

