/**
 *  @file PDM_Datacov.h
 *
 *  @date 2021-01-19
 *
 *  @author aron566
 *
 *  @brief 
 *  
 *  @version V1.0
 */
#ifndef PDM_DATA_COV_H
#define PDM_DATA_COV_H
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< need definition of uint8_t */
#include <stddef.h> /**< need definition of NULL    */
#include <stdbool.h>/**< need definition of BOOL    */
#include <stdio.h>  /**< if need printf             */
#include <stdlib.h>
#include <string.h>
#include <limits.h> /**< need variable max value    */
/** Private includes ---------------------------------------------------------*/

/** Private defines ----------------------------------------------------------*/

/** Exported typedefines -----------------------------------------------------*/
/** 
* @brief   Microphone internal structure definition  
*/ 
typedef struct
{      
  uint32_t MicChannels;       /*!< Specifies the number of channels */

  uint32_t PdmBufferSize;     /*!< Specifies the size of the PDM double buffer for 1 microphone and 1 ms in bytes*/
  
  uint32_t Sampling_Freq;     /*!< Specifies the desired sampling frequency */
  
  uint32_t DecimationFactor;  /*!< Specifies the PDM to PCM decimation factor */
  
  uint16_t * PDM_Data;	      /*!< Takes track of the external PDM data buffer as passed by the user in the start function*/

}PDM2PCM_HandlerTypeDef;

/**
* @}
*/  
/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*PDM流转换PCM数据流接口*/
uint8_t PDM_To_PCM_Stream(uint16_t *PDMBuf, uint16_t *PCMBuf);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
