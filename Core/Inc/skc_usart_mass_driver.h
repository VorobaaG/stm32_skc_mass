#ifndef SKC_USART_MASS_DRIVER
#define SKC_USART_MASS_DRIVER

#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_uart.h"
#include "string.h"      
#include "stdio.h"
#include "stdlib.h"
#include "massaDriver.h"

#define TIME_RESEND_FRAME 1000 //1s
#define ERROR_COUNTER_STOP 10
#define SEND_FRAME  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
#define GET_FRAME   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);


typedef enum{
  WAIT,
  INIT,
  SEND_NEXT,
  REPEAT    
}stateUsart_TypeEnum;

typedef struct{
  char buf[255];
  char response[150];
  uint16_t len;//текущий символ в буфере для записи
  //uint32_t varification;   ?????????
  uint32_t timeWaitNewSymbol;
  Frame_TypeDef *frame[255];
  uint32_t numberSendFrame;   //Номер текущего frame
  uint32_t currentFrame;//Уникалный номер каждого кадра
  stateUsart_TypeEnum state;
  int valueStatusFrame;
  UART_HandleTypeDef *huart;   
}UsartDriver_TypeDef;



void initUsartDriver(UsartDriver_TypeDef *driver,UART_HandleTypeDef *huart);
void setValue(UsartDriver_TypeDef *driver,MassValue_TypeDef massValue);
void reciveUsartSumbol(UsartDriver_TypeDef *driver,uint8_t rxByte);
uint32_t isReady(UsartDriver_TypeDef *driver);
void driverProcess(UsartDriver_TypeDef *driver);
void sendMassFrame(UsartDriver_TypeDef *driver,int sendFrame);


void sendFrame(UsartDriver_TypeDef *driver);

#endif // SKC_USART_MASS_DRIVER