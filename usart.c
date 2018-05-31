#include "usart.h"
#include "stm32f10x.h"
#include "delay.h"
#include "OLED.h"
volatile u8 Flag=0;
volatile u16 Rx232buffer[9];
char i=0;

void USART1_Init(void)
{
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

	/* Enable the PWR/BKP Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE);  
	
	USART_DeInit(USART1);          //������USART1�Ĵ�������Ϊȱʡֵ
	USART_InitStructure.USART_BaudRate = 19200;     //���ô���1������Ϊ19200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //����һ��֡�д�������λ 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //���巢�͵�ֹͣλ��ĿΪ1
	USART_InitStructure.USART_Parity = USART_Parity_No;     //��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //���ͽ���ʹ��
	USART_Init(USART1, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;     //ʱ�ӵ͵�ƽ�
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;         //����SCLK������ʱ������ļ���Ϊ�͵�ƽ
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;   //ʱ�ӵ�һ�����ؽ������ݲ���
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //���һλ���ݵ�ʱ�����岻��SCLK���
  USART_ClockInit(USART1, &USART_ClockInitStructure); 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //ʹ��USART1�����ж�
	USART_Cmd(USART1, ENABLE);      //ʹ��USART1����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //NVIC_Group:NVIC���� 0~4 �ܹ�5��,���2λ��ռ
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************************
 * ��  �� : �ض���c�⺯��printf��USART1
 * ��  �� : 
 * ����ֵ : ��
 **************************************************************************************/
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/**************************************************************************************
 * ��  �� : �ض���c�⺯��scanf��USART1
 * ��  �� : 
 * ����ֵ : ��
 **************************************************************************************/
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

/**************************************************************************************
 * ��  �� : USART1ȫ���жϷ���
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void USART1_IRQHandler(void)
{   
  if(USART_GetITStatus(USART1,USART_IT_RXNE)!= RESET)  //�����ж�
  {  
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);  //���USART1���жϴ�����λ
  }  
}

/**************************************************************************************
 * ��  �� : ��ʼ��USART2������USART2�ж����ȼ�
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void USART2_Init(void)
{
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

	/* Enable the PWR/BKP Clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 , ENABLE);  
	
	USART_DeInit(USART2);          //������USART2�Ĵ�������Ϊȱʡֵ
	USART_InitStructure.USART_BaudRate = 9600;     //���ô���2������Ϊ19200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //����һ��֡�д�������λ 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //���巢�͵�ֹͣλ��ĿΪ1
	USART_InitStructure.USART_Parity = USART_Parity_No;     //��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //���ͽ���ʹ��
	USART_Init(USART2, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;     //ʱ�ӵ͵�ƽ�
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;         //����SCLK������ʱ������ļ���Ϊ�͵�ƽ
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;   //ʱ�ӵ�һ�����ؽ������ݲ���
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //���һλ���ݵ�ʱ�����岻��SCLK���
  USART_ClockInit(USART2, &USART_ClockInitStructure); 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //ʹ��USART2�����ж�
	USART_Cmd(USART2, ENABLE);      //ʹ��USART2����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //NVIC_Group:NVIC���� 0~4 �ܹ�5��,���2λ��ռ
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************************
 * ��  �� : USART2���ͺ���
 * ��  �� : �����ַ�
 * ����ֵ : ��
 **************************************************************************************/
void USART2_SendByte(u8 byte)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
	USART_SendData(USART2, byte);
}

/**************************************************************************************
 * ��  �� : USART2���յ����ݺ󴮿ڷ��ͳ�ȥ
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void USART2_Tx_Puts(void)
{ int co2,co1,co0,a,b,c,d;
	USART2_SendByte(0xFF);
	USART2_SendByte(0x01);
	USART2_SendByte(0x86);
	USART2_SendByte(0x00);
	USART2_SendByte(0x00);
	USART2_SendByte(0x00);
	USART2_SendByte(0x00);
	USART2_SendByte(0x00);
	USART2_SendByte(0x79);
	delay_ms(500);
  if(Flag)      //��������ͨ�����ڱ����յ�
	{ 
		co0=(int)(Rx232buffer[2]&0xFF);
		co1=(int)(Rx232buffer[3]&0xFF);
		co2=(int)(co0*256+co1);
		
//    USART2_SendByte(Rx232buffer[2]);  //�����ַ�
//		USART2_SendByte(Rx232buffer[3]);  //�����ַ�
//		USART2_SendByte(co2);  //�����ַ�
//    USART2_SendByte(0x0D);    //���ͻ��з�
 //   USART2_SendByte(0x0A);    //���ͻ��з�	
		Flag=0;     //������ձ�ʶ��
  }
	a=(int)(co2/1000);
	b=(int)((co2-a*1000)/100);
	c=(int)((co2-a*1000-b*100)/10);
	d=(int)(co2-a*1000-b*100-c*10);
	switch(a)
	{
		case 0: LCD_Print(64, 0, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(64, 0, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(64, 0, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(64, 0, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(64, 0, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(64, 0, "5",TYPE16X16,TYPE8X16);
		break;
	}
		switch(b)
	{
		case 0: LCD_Print(72, 0, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(72, 0, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(72, 0, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(72, 0, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(72, 0, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(72, 0, "5",TYPE16X16,TYPE8X16);
		break;
		case 6: LCD_Print(72, 0, "6",TYPE16X16,TYPE8X16);
		break;
		case 7: LCD_Print(72, 0, "7",TYPE16X16,TYPE8X16);
		break;
		case 8: LCD_Print(72, 0, "8",TYPE16X16,TYPE8X16);
		break;
		case 9: LCD_Print(72, 0, "9",TYPE16X16,TYPE8X16);
		break;
	}
		switch(c)
	{
		case 0: LCD_Print(80, 0, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(80, 0, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(80, 0, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(80, 0, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(80, 0, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(80, 0, "5",TYPE16X16,TYPE8X16);
		break;
		case 6: LCD_Print(80, 0, "6",TYPE16X16,TYPE8X16);
		break;
		case 7: LCD_Print(80, 0, "7",TYPE16X16,TYPE8X16);
		break;
		case 8: LCD_Print(80, 0, "8",TYPE16X16,TYPE8X16);
		break;
		case 9: LCD_Print(80, 0, "9",TYPE16X16,TYPE8X16);
		break;
	}
		switch(d)
	{
		case 0: LCD_Print(88, 0, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(88, 0, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(88, 0, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(88, 0, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(88, 0, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(88, 0, "5",TYPE16X16,TYPE8X16);
		break;
		case 6: LCD_Print(88, 0, "6",TYPE16X16,TYPE8X16);
		break;
		case 7: LCD_Print(88, 0, "7",TYPE16X16,TYPE8X16);
		break;
		case 8: LCD_Print(88, 0, "8",TYPE16X16,TYPE8X16);
		break;
		case 9: LCD_Print(88, 0, "9",TYPE16X16,TYPE8X16);
		break;
	}
	LCD_Print(96, 0, "ppm",TYPE16X16,TYPE8X16);
	printf("\r\nCO2Ũ��Ϊ:%dppm\r\n",co2);
}

/**************************************************************************************
 * ��  �� : USART2ȫ���жϷ���
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void USART2_IRQHandler(void)
{  
  if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)  //�����ж�
  {  
    Rx232buffer[i]= USART_ReceiveData(USART2);     //ͨ������USART2��������
		i++;
		if(i==9)
		{
		i=0;
		Flag=1;
		}                                     //���յ�����,���ձ�ʶ����Ч
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);  //���USART2���жϴ�����λ
  }  
}





