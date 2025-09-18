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


typedef struct{
  char buf[255];
  char response[100];
  uint16_t len;//текущий символ в буфере для записи
  uint32_t varification;
  uint32_t currentFrame;//Текущий кадр
  uint32_t currentValue;//Значение датчика
  uint32_t currentSensorNum;//номер датчика
  uint32_t delay;
  uint8_t sendNewFrame;//Готовность для отправки нового кадра
  uint8_t sendNewData;//готовность записать в структуру новый кадр
  uint32_t timeWaitAnswer;//Время ожидания ответа
  uint32_t counterError;
  UART_HandleTypeDef *huart;
  
}UsartDriver_TypeDef;


void initUsartDriver(UsartDriver_TypeDef *driver,UART_HandleTypeDef *huart);
void setValue(UsartDriver_TypeDef *driver,MassValue_TypeDef massValue);
void reciveUsartSumbol(UsartDriver_TypeDef *driver,uint8_t rxByte);
uint32_t isReady(UsartDriver_TypeDef *driver);
void driverProcess(UsartDriver_TypeDef *driver);
void sendMassFrame(int currentFrame, int currentSensor, int currentValue,int delay,UART_HandleTypeDef *huart);
#endif // SKC_USART_MASS_DRIVER