#include "stm32f10x.h"
#include <stdio.h>    //使用printf需引用

extern  void USART1_Init(void);
extern  void USART2_Init(void);
extern  void USART2_SendByte(u8 byte);
extern  void USART2_Tx_Puts(void);
