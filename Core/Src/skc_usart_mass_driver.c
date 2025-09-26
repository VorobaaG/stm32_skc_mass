#include "skc_usart_mass_driver.h"



void reciveUsartSumbol(UsartDriver_TypeDef *driver,uint8_t rxByte){
   driver->buf[driver->len++] = rxByte;
   driver->timeWaitNewSymbol=1;
   if (rxByte == '\n' || driver->len >= sizeof(driver->buf)) {
            driver->buf[driver->len] = 0; // null-terminator для строки
            driver->len = 0;

            char *token;
            memset(driver->response, 0, sizeof(driver->response));
            token = strtok(driver->buf,".");
            if(strcmp(token,"VorG")==0){     
              token = strtok(NULL,".");
              if(strcmp(token,"START")==0){
                token = strtok(NULL, ".");
                int value = atoi(token);
                if(value!=-1){
                  driver->valueStatusFrame = value;
                  driver->state = INIT;
                }else {
                   driver->state = WAIT;
                   driver->valueStatusFrame =0;
                   //ОШИБКА НЕ ВЕРНЫЙ currentFrame
                }                  
              }
              else if(strcmp(token,"NEXT")==0){// Отправить следующий кадр
                driver->state = SEND_NEXT;
                driver->valueStatusFrame = 0;
              }
              else if(strcmp(token,"REPEAT")==0){// Отправить определенный кадр
                 driver->state = REPEAT;
                 token = strtok(NULL, ".");
                 driver->valueStatusFrame = atoi(token);
              }
            }
              
     memset(driver->buf, 0, sizeof(driver->buf));
   }
}

void driverProcess(UsartDriver_TypeDef *driver){
      
  switch(driver->state){
    
  case WAIT: break;
  
  case INIT:
    if(driver->currentFrame < driver->valueStatusFrame) driver->currentFrame = driver->valueStatusFrame;         
    sprintf(driver->response, "VorG.OK.%i.\n",driver->currentFrame);
    sendFrame(driver);
    break;
    
  case SEND_NEXT: {
    int massValue[8]={0,1,2,3,4,5,6,7};
    addFrame(driver->frame[driver->numberSendFrame],massValue,&driver->currentFrame,&driver->numberSendFrame);
    sendMassFrame(driver,driver->numberSendFrame);   
    break;
  }
    
  case REPEAT:{
    for(int i = 0; i<255;i++){
      if(driver->frame[i]->frameNum == driver->valueStatusFrame) {
        sendMassFrame(driver,driver->frame[i]->frameNum);  
        return; 
      }
    }
    sprintf(driver->response, "VorG.NONE.\n");
    sendFrame(driver);
    break;    
  }
    
  default:
    
    break;
  }
    

  
}


void sendFrame(UsartDriver_TypeDef *driver){
    uint16_t size =  strlen(driver->response);
    SEND_FRAME;    
    HAL_UART_Transmit_IT(driver->huart,  (uint8_t *)driver->response, size);    
    
}



void sendMassFrame(UsartDriver_TypeDef *driver,int sendFrame){
    //VorG.SEND.[currentFrame].[0_mass].[1_mass]. ... [7_mass].[CRC].\n
    sprintf(driver->response, "VorG.SEND.%i.%i.%i.%i.%i.%i.%i.%i.%i.%i.\n",
    driver->frame[sendFrame]->frameNum,
    driver->frame[sendFrame]->massValue[0],
    driver->frame[sendFrame]->massValue[1],
    driver->frame[sendFrame]->massValue[2],
    driver->frame[sendFrame]->massValue[3],
    driver->frame[sendFrame]->massValue[4],
    driver->frame[sendFrame]->massValue[5],
    driver->frame[sendFrame]->massValue[6],
    driver->frame[sendFrame]->massValue[7],
    driver->frame[sendFrame]->crc);  
    
    uint16_t size =  strlen(driver->response);
    SEND_FRAME;    
    HAL_UART_Transmit_IT(driver->huart,  (uint8_t *)driver->response, size);           
  
}

void initUsartDriver(UsartDriver_TypeDef *driver,UART_HandleTypeDef *huart){
   driver->huart=huart;
}