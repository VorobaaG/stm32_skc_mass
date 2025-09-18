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
   MassValue_TypeDef massValue[255];
   uint32_t currentLen;
   uint32_t sendLen;
   RNG_HandleTypeDef *hrng;
}MassDriver_TypeDef;
   


void addMass(MassValue_TypeDef massValue,MassDriver_TypeDef *massDriver);
int isEmptyBank(MassDriver_TypeDef *massDriver);
MassValue_TypeDef getMassAndNext(MassDriver_TypeDef *massDriver);
void initMass(MassDriver_TypeDef *massDriver,RNG_HandleTypeDef *hrng);
int getValueSensor(MassDriver_TypeDef *massDriver);
#endif // MASSA_DRIVER