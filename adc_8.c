#include "includes.h"
#include "OLED.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

static int i=0;                             //定义全局变量i
#define M 8																  //定义M
u16 AD_Value[M]	;														//定义字符串AD_Value[8]用来存放8位电压值

void Delay(u32 count)
{
	u32 i=0;
	for(;i<count;i++);
}
/*
 * 函数名：ADC1_GPIO_Config
 * 描述  ：使能ADC1和DMA1的时钟，初始化PA
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1 , ENABLE );	  //使能ADC1通道时钟，各个管脚时钟


	
	/* Configure PC.01  as analog input */
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				// PC1,输入时不用设置速率
}


/* 函数名：ADC1_Mode_Config
 * 描述  ：配置ADC1的工作模式为MDA模式
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址

	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)AD_Value;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;		//内存作为数据传输的目的地
	
	DMA_InitStructure.DMA_BufferSize = 8;

	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址递增

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//数据宽度为16位

	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;		   //数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输

	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);


	
	/* ADC1 configuration */
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //扫描模式，扫描模式用于多通道采集
	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 8;	 	//要转换的通道数目M
	ADC_Init(ADC1, &ADC_InitStructure);
	

	/*配置ADC时钟，为PCLK2的8分频，即9Hz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*配置ADC1的通道11为55.	5个采样周期，序列为1 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
 * 函数名：ADC1_Init
 * 描述  ：无
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
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
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//等待传输完成否则第一位数据容易丢失       
		 
		for(i=1;i<2;i++)
			{
					value[i]=AD_Value[i];
				  Delay(500000);
					printf("\r\n 光敏电阻电压  \t AD值: %d\r\n", value[i]); 
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






