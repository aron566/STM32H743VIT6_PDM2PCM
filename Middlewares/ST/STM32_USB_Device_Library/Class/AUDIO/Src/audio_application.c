/**
******************************************************************************
* @file    audio_application.c 
* @author  Central Labs
* @version V 1.0.0
* @date    07-May-2015
* @brief   Audio input application. 
*******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
********************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "audio_application.h"
#include "usbd_audio_if.h"
/** @addtogroup X_NUCLEO_CCA02M1_Applications
* @{
*/ 

/** @addtogroup Microphones_Acquisition
* @{
*/

/** @defgroup AUDIO_APPLICATION 
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/** @defgroup AUDIO_APPLICATION_Exported_Variables 
* @{
*/
uint16_t PDM_Buffer[AUDIO_CHANNELS*AUDIO_SAMPLING_FREQUENCY/1000*64/8];
/**
* @}
*/

/** @defgroup AUDIO_APPLICATION_Private_Variables 
* @{
*/
/* Private variables ---------------------------------------------------------*/
static uint16_t PCM_Buffer[AUDIO_CHANNELS*AUDIO_SAMPLING_FREQUENCY/1000];
/**
* @}
*/

/** @defgroup AUDIO_APPLICATION_Exported_Function 
* @{
*/

/**
* @brief  Half Transfer user callback, called by BSP functions.
* @param  None
* @retval None
*/
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  AudioProcess();
}

/**
* @brief  Transfer Complete user callback, called by BSP functions.
* @param  None
* @retval None
*/
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  AudioProcess();
}

/**
* @brief  User function that is called when 1 ms of PDM data is available.
* 		  In this application only PDM to PCM conversion and USB streaming
*                  is performed.
* 		  User can add his own code here to perform some DSP or audio analysis.
* @param  none
* @retval None
*/

void AudioProcess(void)
{
  USB_Audio_IN_PDMToPCM((uint16_t * )PDM_Buffer,PCM_Buffer);
  Send_Audio_to_USB((int16_t *)PCM_Buffer, AUDIO_SAMPLING_FREQUENCY/1000*AUDIO_CHANNELS);

}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
