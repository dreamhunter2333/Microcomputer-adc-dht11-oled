#include "includes.h"
#include "OLED.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

static int i=0;                             //����ȫ�ֱ���i
#define M 8																  //����M
u16 AD_Value[M]	;														//�����ַ���AD_Value[8]�������8λ��ѹֵ

void Delay(u32 count)
{
	u32 i=0;
	for(;i<count;i++);
}
/*
 * ��������ADC1_GPIO_Config
 * ����  ��ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PA
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1 , ENABLE );	  //ʹ��ADC1ͨ��ʱ�ӣ������ܽ�ʱ��


	
	/* Configure PC.01  as analog input */
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				// PC1,����ʱ������������
}


/* ��������ADC1_Mode_Config
 * ����  ������ADC1�Ĺ���ģʽΪMDAģʽ
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC��ַ

	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)AD_Value;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;		//�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	
	DMA_InitStructure.DMA_BufferSize = 8;

	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ����

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//���ݿ��Ϊ16λ

	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;		   //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//ѭ������

	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);


	
	/* ADC1 configuration */
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 8;	 	//Ҫת����ͨ����ĿM
	ADC_Init(ADC1, &ADC_InitStructure);
	

	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9Hz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*����ADC1��ͨ��11Ϊ55.	5���������ڣ�����Ϊ1 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
 * ��������ADC1_Init
 * ����  ����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}



void AD_Start(void)
{
		u16 value[M];
	char h,l,g,k;
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//�ȴ�������ɷ����һλ�������׶�ʧ       
		 
		for(i=1;i<2;i++)
			{
					value[i]=AD_Value[i];
				  Delay(500000);
					printf("\r\n ���������ѹ  \t ADֵ: %d\r\n", value[i]); 
	h=(int)(value[i]/1000);
	l=(int)((value[i]-h*1000)/100);
	g=(int)((value[i]-h*1000-l*100)/10);
	k=(int)(value[i]-h*1000-l*100-g*10);
	switch(h)
	{
		case 0: LCD_Print(72, 48, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(72, 48, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(72, 48, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(72, 48, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(72, 48, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(72, 48, "5",TYPE16X16,TYPE8X16);
		break;
	}
		switch(l)
	{
		case 0: LCD_Print(80, 48, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(80, 48, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(80, 48, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(80, 48, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(80, 48, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(80, 48, "5",TYPE16X16,TYPE8X16);
		break;
		case 6: LCD_Print(80, 48, "6",TYPE16X16,TYPE8X16);
		break;
		case 7: LCD_Print(80, 48, "7",TYPE16X16,TYPE8X16);
		break;
		case 8: LCD_Print(80, 48, "8",TYPE16X16,TYPE8X16);
		break;
		case 9: LCD_Print(80, 48, "9",TYPE16X16,TYPE8X16);
		break;
	}
		switch(g)
	{
		case 0: LCD_Print(88, 48, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(88, 48, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(88, 48, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(88, 48, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(88, 48, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(88, 48, "5",TYPE16X16,TYPE8X16);
		break;
		case 6: LCD_Print(88, 48, "6",TYPE16X16,TYPE8X16);
		break;
		case 7: LCD_Print(88, 48, "7",TYPE16X16,TYPE8X16);
		break;
		case 8: LCD_Print(88, 48, "8",TYPE16X16,TYPE8X16);
		break;
		case 9: LCD_Print(88, 48, "9",TYPE16X16,TYPE8X16);
		break;
	}
		switch(k)
	{
		case 0: LCD_Print(96, 48, "0",TYPE16X16,TYPE8X16);
		break;
		case 1: LCD_Print(96, 48, "1",TYPE16X16,TYPE8X16);
		break;
		case 2: LCD_Print(96, 48, "2",TYPE16X16,TYPE8X16);
		break;
		case 3: LCD_Print(96, 48, "3",TYPE16X16,TYPE8X16);
		break;
		case 4: LCD_Print(96, 48, "4",TYPE16X16,TYPE8X16);
		break;
		case 5: LCD_Print(96, 48, "5",TYPE16X16,TYPE8X16);
		break;
		case 6: LCD_Print(96, 48, "6",TYPE16X16,TYPE8X16);
		break;
		case 7: LCD_Print(96, 48, "7",TYPE16X16,TYPE8X16);
		break;
		case 8: LCD_Print(96, 48, "8",TYPE16X16,TYPE8X16);
		break;
		case 9: LCD_Print(96, 48, "9",TYPE16X16,TYPE8X16);
		break;
	}
			}
 


}






