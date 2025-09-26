#ifndef MASSA_DRIVER
#define MASSA_DRIVER

#include "stm32l0xx_hal.h"
#include "stdlib.h"

typedef struct{
   uint32_t mass;
   uint32_t sensor;
   uint32_t delay;  
}MassValue_TypeDef;
   
typedef struct{
   MassValue_TypeDef massValue[255];//Бувер для сохраненеия не отпрвленных масс
   MassValue_TypeDef currentMass[8];//текущее состояние датчиков 
   MassValue_TypeDef prevMass[8];//предыдущее значение
   uint32_t currentLen;
   uint32_t sendLen;
   RNG_HandleTypeDef *hrng;
}MassDriver_TypeDef;
   



typedef struct{
  int massValue[8];
  uint32_t frameNum;
  uint32_t crc;
}Frame_TypeDef;

void addMass(MassValue_TypeDef massValue,MassDriver_TypeDef *massDriver);
int isEmptyBank(MassDriver_TypeDef *massDriver);
MassValue_TypeDef getMassAndNext(MassDriver_TypeDef *massDriver);
void initMass(MassDriver_TypeDef *massDriver,RNG_HandleTypeDef *hrng);
int getValueSensor(MassDriver_TypeDef *massDriver,int numCrat);

void addFrame(Frame_TypeDef *frame, int mass[8],uint32_t *frameNum,uint32_t *number);
#endif // MASSA_DRIVER