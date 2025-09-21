#include "skc_usart_mass_driver.h"



void reciveUsartSumbol(UsartDriver_TypeDef *driver,uint8_t rxByte){
   driver->buf[driver->len++] = rxByte;
  
   if (rxByte == '\n' || driver->len >= sizeof(driver->buf)) {
            driver->buf[driver->len] = 0; // null-terminator для строки
            driver->len = 0;

            char *token;
            memset(driver->response, 0, sizeof(driver->response));
            token = strtok(driver->buf,".");
            
              if(strcmp(token,"START")==0){
                token = strtok(NULL, ".");
                int value = atoi(token);
                if(value>=0){
                   SEND_FRAME; 
                   driver->currentFrame = value+1;   
                   sprintf(driver->response, "VorG.OK.%i.\n",driver->currentFrame);
                   driver->varification=1;
                   driver->sendNewFrame=1;
                   driver->sendNewData=1;
                   HAL_UART_Transmit_IT(driver->huart,  (uint8_t *)driver->response, strlen(driver->response));
                }else {
                   //ОШИБКА НЕ ВЕРНЫЙ currentFrame
                }                  
              }
              else if(strcmp(token,"CONFIRM")==0){// Подтверждение получения кадра
                 token = strtok(NULL, ".");
                 int value = atoi(token);
                 if(value == 0 ) {
                   SEND_FRAME;
                   sendMassFrame( driver->currentFrame,driver->currentSensorNum,driver->currentValue,driver->delay,driver->huart);//Ошибка кадра (Необходимо повторно отправить) 
                 }
                 else {
                    driver->sendNewFrame=1;
                    driver->sendNewData=1;
                    driver->currentFrame++;
                 }
              }
              driver->timeWaitAnswer=0;
              driver->counterError=0;
              memset(driver->buf, 0, sizeof(driver->buf));
   }
}


void driverProcess(UsartDriver_TypeDef *driver){
  
  if(driver->sendNewFrame==1 && driver->counterError<=ERROR_COUNTER_STOP){//Проверка завершился ли предыдущий успешно
    if(driver->sendNewData==0){//Проверка есть ли новые данные для отправления
      if(driver->timeWaitAnswer == 0 || driver->timeWaitAnswer >=1000)
      sendMassFrame(driver->currentFrame,driver->currentSensorNum,driver->currentValue,driver->currentValue,driver->huart);
      driver->sendNewFrame=0;
      driver->timeWaitAnswer=1;
    }
    
  }
  if(driver->timeWaitAnswer>=0xFFFFFFF0) driver->timeWaitAnswer=0;
  
}

uint32_t isReady(UsartDriver_TypeDef *driver){ 
  if(driver->varification==1){
      return driver->sendNewFrame;
  }else return 0;

}

void setValue(UsartDriver_TypeDef *driver,MassValue_TypeDef massValue){
  driver->currentSensorNum = massValue.sensor;
  driver->currentValue = massValue.mass;
  driver->delay = massValue.delay;
  driver->sendNewData=0;
}

void sendMassFrame(int currentFrame, int currentSensor, int currentValue,int delay,UART_HandleTypeDef *huart){
  char response[50]={0};
  sprintf(response, "VorG.%i.%i.%i.%i.\n",currentFrame,currentSensor,currentValue,delay);  
  HAL_UART_Transmit_IT(huart, (const uint8_t *) response, strlen(response)); 
  
  
}

void initUsartDriver(UsartDriver_TypeDef *driver,UART_HandleTypeDef *huart){
   driver->varification=0;
   driver->sendNewFrame=0;
   driver->sendNewData=0;
   driver->huart=huart;
}