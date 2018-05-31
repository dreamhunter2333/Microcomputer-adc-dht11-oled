#include "stm32f10x.h"
#include "delay.h"
#include "dht11.h"
#include "usart.h"
#include "includes.h"
#include "OLED.h"



DHT11_Data_TypeDef DHT11_Data;
/**************************************************************************************
 * ��  �� : GPIO/USART1��ʼ������
 * ��  �� : ��
 * ����ֵ : ��
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
	GPIO_DeInit(GPIOB);	 //������GPIOA�Ĵ�������Ϊȱʡֵ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_DeInit(GPIOA);	 //������GPIOA�Ĵ�������Ϊȱʡֵ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB , GPIO_Pin_9);   //��ʼ״̬��Ϩ��ָʾ��LED1
}

/**************************************************************************************
 * ��  �� : ������ʾʵʱ��ʪ��
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void DHT11_SCAN(void)
{
	if( Read_DHT11(&DHT11_Data)==SUCCESS)
	{ int e,f,g,h;
		printf("\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d ��RH ���¶�Ϊ %d.%d�� \r\n",\
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
		//printf("\r\n ʪ��:%d,�¶�:%d \r\n" ,DHT11_Data.humi_int,DHT11_Data.temp_int);
	}
	else
	{
		printf("Read DHT11 ERROR!\r\n");
	}
}

/**************************************************************************************
 * ��  �� : MAIN����
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
int main(void)
{
	SystemInit();			//����ϵͳʱ��72MHZ
	GPIO_Configuration();
	USART1_Init();    //��ʼ������TIM 
  USART2_Init();	
  ADC1_Init();
	DHT11_GPIO_Config();   // ��ʼ����ʪ�ȴ�����PB1���ų�ʼʱΪ�������
	LCD_Init();
		
	GPIO_ResetBits(GPIOB , GPIO_Pin_9);
	delay_ms(500);	
	printf("\r\n **��ʼadת��**\r\n");
  while(1)
  {
		GPIO_SetBits(GPIOB , GPIO_Pin_9); 
    printf("\r\n *********************\r\n");	
		AD_Start(); 
    DHT11_SCAN();  //ʵʱ��ʾ��ʪ��
    USART2_Tx_Puts();
 

		printf("\r\n *****����һ�β���*****\r\n"); 
		delay_ms(1500);
		LCD_Print(0, 0, "co2Ũ�ȣ�",TYPE16X16,TYPE8X16);
		LCD_Print(0, 16, "�¶�Ϊ��",TYPE16X16,TYPE16X16);
		LCD_Print(96, 16, "��",TYPE16X16,TYPE16X16);
		LCD_Print(0, 32, "ʪ��Ϊ��",TYPE16X16,TYPE6X8);
		LCD_Print(96, 32, "%RH",TYPE16X16,TYPE8X16);
		LCD_Print(0, 48, "����ǿ�ȣ�",TYPE16X16,TYPE6X8);
		
	} 		
}
