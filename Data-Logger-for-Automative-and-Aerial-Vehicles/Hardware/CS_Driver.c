/**
 * @file    :   CS_Driver.c
 * @brief   :   An abstraction for CS_Driver functions
 *
 *              This source file provides CS_Driver function which is used to
 *              initialize the Clock source for the MSP432
 *
 * @author  :   Alejandro Martinez
 *
 * @tools   :   Code Composer Studio
 *
 * @link    :   This code has been leveraged from the github of Alejandro Martinez
 *              https://github.com/amartinezacosta/MSP432-SD_Card
*/

#include "CS_Driver.h"

void CS_Init(void)
{
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
}


