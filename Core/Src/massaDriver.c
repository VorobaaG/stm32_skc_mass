#include "massaDriver.h"
#include <stdio.h>
#include <string.h>



void addMass(MassValue_TypeDef massValue,MassDriver_TypeDef *massDriver){
     massDriver->currentLen++;
     if(massDriver->currentLen>=256) massDriver->currentLen=0;
     massDriver->massValue[massDriver->currentLen] = massValue;
}

int isEmptyBank(MassDriver_TypeDef *massDriver){
    return (massDriver->currentLen == massDriver->sendLen);  
}


MassValue_TypeDef getMassAndNext(MassDriver_TypeDef *massDriver){  
       massDriver->sendLen++;
       if(massDriver->sendLen>=256) massDriver->sendLen=0;
       return  massDriver->massValue[massDriver->sendLen];   
}

void initMass(MassDriver_TypeDef *massDriver,RNG_HandleTypeDef *hrng){
    massDriver->hrng=hrng;
    for(int i =0;i<8;i++){
        massDriver->currentMass[i].sensor=i;
        massDriver->currentMass[i].mass=getValueSensor(massDriver,10000);
        massDriver->massValue[i].sensor=massDriver->currentMass[i].sensor;
        massDriver->massValue[i].mass=massDriver->currentMass[i].mass;
        massDriver->currentLen++;
    }
}

void addFrame(Frame_TypeDef *frame, int mass[8],uint32_t *frameNum,uint32_t *number){
   frame->frameNum=(*frameNum);
   for(int i=0;i<8;i++){
    frame->massValue[i]= mass[i];
   }
   frame->crc = 0xff;
   (*number)++;
   if((*number)>=256) (*number)=0;
   
   (*frameNum)++;
}


int getValueSensor(MassDriver_TypeDef *massDriver,int numCrat){
   int number =0;
   uint32_t num;
   HAL_RNG_GenerateRandomNumber(massDriver->hrng, &num);
   if(((int)num) <0) number = (int)(num*(-1));
   else  number = (int)num;
   number=number%numCrat;
   return number;
}
