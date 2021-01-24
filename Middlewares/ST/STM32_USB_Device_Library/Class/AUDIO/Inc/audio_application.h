/**
******************************************************************************
* @file    audio_application.h 
* @author  Central Labs
* @version V 0.1
* @date    7-May-2015
* @brief   Header for audio_application.c module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_APPLICATION_H
#define __AUDIO_APPLICATION_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/** @addtogroup X_NUCLEO_CCA02M1_Applications
  * @{
  */ 

/** @addtogroup Microphones_Acquisition
  * @{
  */

/** @defgroup AUDIO_APPLICATION 
  * @{
  */



 /** @defgroup AUDIO_APPLICATION_Private_Types 
  * @{
  */  


 /** 
  * @brief Sampling Frequency enumerator  
  */ 
typedef enum
{
    SAMPLING_FREQ_16000 = 16000,
    SAMPLING_FREQ_32000 = 32000,
    SAMPLING_FREQ_48000 = 48000 
      
}OUTPUT_FREQ_TypeDef;

 /** 
  * @brief Channels input number enumerator  
  */ 


  /**
  * @}
  */ 

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** @defgroup AUDIO_APPLICATION_Exported_Defines 
 * @{
 */

#define AUDIO_CHANNELS 					    2
#define AUDIO_SAMPLING_FREQUENCY 		16000

#define PCM_TEMP_BUFF_SIZE  AUDIO_SAMPLING_FREQUENCY/1000*1


#if defined(STM32L053xx) || defined(STM32F072xB)
#if (AUDIO_CHANNELS > 2)
#error "Acquisition of more than 2 microphone is not yet supported for STM32L0 and STM32F0"
#elif (AUDIO_SAMPLING_FREQUENCY > 32000)
#error "Acquisition of sampling frequencies above 32 KHz is not yet supported for STM32L0 and STM32F0"
#elif (AUDIO_SAMPLING_FREQUENCY == 32000 && AUDIO_CHANNELS == 2)
#error "These settings are not yet supported for STM32L0 and STM32F0"
#endif
#endif


/**
 * @}
 */
/* Exported functions ------------------------------------------------------- */

void Error_Handler(void);
void AudioProcess(void);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */



#endif /* __AUDIO_APPLICATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
