#include "stm32f10x.h"
#include "delay.h"
#include "dht11.h"
#include "usart.h"
#include "includes.h"
#include "OLED.h"



DHT11_Data_TypeDef DHT11_Data;
/**************************************************************************************
 * 描  述 : GPIO/USART1初始化配置
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE); 						 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	//OLED 
	GPIO_DeInit(GPIOB);	 //将外设GPIOA寄存器重设为缺省值
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_DeInit(GPIOA);	 //将外设GPIOA寄存器重设为缺省值
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB , GPIO_Pin_9);   //初始状态，熄灭指示灯LED1
}

/**************************************************************************************
 * 描  述 : 串口显示实时温湿度
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void DHT11_SCAN(void)
{
	if( Read_DHT11(&DHT11_Data)==SUCCESS)
	{ int e,f,g,h;
		printf("\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d ％RH ，温度为 %d.%d℃ \r\n",\
		DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
		e=(int)(DHT11_Data.temp_int/10);
		f=(int)(DHT11_Data.temp_int-e*10);
		g=(int)(DHT11_Data.humi_int/10);
		h=(int)(DHT11_Data.humi_int-g*10);
		switch(e)
	{
		case 0: LCD_Print(72, 16, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(72, 16, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(72, 16, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(72, 16, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(72, 16, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(72, 16, "5",TYPE16X16,TYPE8X16);
		break;
		case 6: LCD_Print(72, 16, "6",TYPE16X16,TYPE8X16);
		break;
		case 7: LCD_Print(72, 16, "7",TYPE16X16,TYPE8X16);
		break;
		case 8: LCD_Print(72, 16, "8",TYPE16X16,TYPE8X16);
		break;
		case 9: LCD_Print(72, 16, "9",TYPE16X16,TYPE8X16);
		break;
	}
			switch(f)
	{
		case 0: LCD_Print(80, 32, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(80, 32, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(80, 32, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(80, 32, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(80, 32, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(80, 32, "5",TYPE16X16,TYPE8X16);
		break;
		case 6: LCD_Print(80, 32, "6",TYPE16X16,TYPE8X16);
		break;
		case 7: LCD_Print(80, 32, "7",TYPE16X16,TYPE8X16);
		break;
		case 8: LCD_Print(80, 32, "8",TYPE16X16,TYPE8X16);
		break;
		case 9: LCD_Print(80, 32, "9",TYPE16X16,TYPE8X16);
		break;
	}
			switch(g)
	{
		case 0: LCD_Print(72, 32, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(72, 32, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(72, 32, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(72, 32, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(72, 32, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(72, 32, "5",TYPE16X16,TYPE8X16);
		break;
		case 6: LCD_Print(72, 32, "6",TYPE16X16,TYPE8X16);
		break;
		case 7: LCD_Print(72, 32, "7",TYPE16X16,TYPE8X16);
		break;
		case 8: LCD_Print(72, 32, "8",TYPE16X16,TYPE8X16);
		break;
		case 9: LCD_Print(72, 32, "9",TYPE16X16,TYPE8X16);
		break;
	}
		switch(h)
	{
		case 0: LCD_Print(80, 16, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(80, 16, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(80, 16, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(80, 16, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(80, 16, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(80, 16, "5",TYPE16X16,TYPE8X16);
		break;
		case 6: LCD_Print(80, 16, "6",TYPE16X16,TYPE8X16);
		break;
		case 7: LCD_Print(80, 16, "7",TYPE16X16,TYPE8X16);
		break;
		case 8: LCD_Print(80, 16, "8",TYPE16X16,TYPE8X16);
		break;
		case 9: LCD_Print(80, 16, "9",TYPE16X16,TYPE8X16);
		break;
	}
		//printf("\r\n 湿度:%d,温度:%d \r\n" ,DHT11_Data.humi_int,DHT11_Data.temp_int);
	}
	else
	{
		printf("Read DHT11 ERROR!\r\n");
	}
}

/**************************************************************************************
 * 描  述 : MAIN函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int main(void)
{
	SystemInit();			//设置系统时钟72MHZ
	GPIO_Configuration();
	USART1_Init();    //初始化配置TIM 
  USART2_Init();	
  ADC1_Init();
	DHT11_GPIO_Config();   // 初始化温湿度传感器PB1引脚初始时为推挽输出
	LCD_Init();
		
	GPIO_ResetBits(GPIOB , GPIO_Pin_9);
	delay_ms(500);	
	printf("\r\n **开始ad转换**\r\n");
  while(1)
  {
		GPIO_SetBits(GPIOB , GPIO_Pin_9); 
    printf("\r\n *********************\r\n");	
		AD_Start(); 
    DHT11_SCAN();  //实时显示温湿度
    USART2_Tx_Puts();
 

		printf("\r\n *****结束一次采样*****\r\n"); 
		delay_ms(1500);
		LCD_Print(0, 0, "co2浓度：",TYPE16X16,TYPE8X16);
		LCD_Print(0, 16, "温度为：",TYPE16X16,TYPE16X16);
		LCD_Print(96, 16, "℃",TYPE16X16,TYPE16X16);
		LCD_Print(0, 32, "湿度为：",TYPE16X16,TYPE6X8);
		LCD_Print(96, 32, "%RH",TYPE16X16,TYPE8X16);
		LCD_Print(0, 48, "光照强度：",TYPE16X16,TYPE6X8);
		
	} 		
}
