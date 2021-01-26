/**
 *  @file SAI_Port.h
 *
 *  @date 2021-01-01
 *
 *  @author aron566
 *
 *  @brief sai操作接口-MIC
 *  
 *  @version V1.0
 */
#ifndef SAI_PORT_H
#define SAI_PORT_H
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< nedd definition of uint8_t */
#include <stddef.h> /**< need definition of NULL    */
#include <stdbool.h>/**< need definition of BOOL    */
#include <stdio.h>  /**< if need printf             */
#include <stdlib.h>
#include <string.h>
#include <limits.h> /**< need variable max value    */
/** Private includes ---------------------------------------------------------*/
#include "main.h"
/** Private defines ----------------------------------------------------------*/
#define MIC_CHANNEL_NUM     (8/2U)                /**< 通道总数*/
#define MONO_FRAME_SIZE     (160)                 /**< 单声道帧大小*/
#define STEREO_FRAME_SIZE   (MONO_FRAME_SIZE*2)   /**< 立体声帧大小*/
#define PCM_ONE_SAMPLE_NUM  STEREO_FRAME_SIZE     /**< 单次PCM转换帧大小*/
#define PCM_TWO_SAMPLE_NUM  (PCM_ONE_SAMPLE_NUM*2)/**< 两次PCM转换帧大小*/
#define PDM_ONE_SAMPLE_NUM  (PCM_ONE_SAMPLE_NUM*4)/**< 单次PDM采样帧大小*/
#define PDM_TWO_SAMPLE_NUM  (PDM_ONE_SAMPLE_NUM*2)/**< 两次PDM采样帧大小*/

/** Exported typedefines -----------------------------------------------------*/
/*PDM转PCM数据结构*/
typedef struct
{
  uint16_t PDM_RX_Buf[PDM_TWO_SAMPLE_NUM*2];/**< 双缓冲区*/
  uint16_t PDM_One_Sample_Buf[PDM_ONE_SAMPLE_NUM];/**< 分离通道数据*/
  int16_t  PCM_Buf[PCM_TWO_SAMPLE_NUM];
  int16_t  PCM_One_Sample_Buf[PCM_ONE_SAMPLE_NUM];
}PDM2PCM_BUF_Typedef_t;
/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/* Flag定义 */
extern volatile uint8_t SAI_Transmit_Complete_Flag;
extern volatile uint8_t SAI_Receive_Complete_Flag;
extern volatile uint8_t SAI_Can_Send_Data_Flag;
/** Exported functions prototypes --------------------------------------------*/

/*SAI操作初始化*/
void Sai_Port_Init(void);
/*SAI数据发送*/
void Sai_Port_Send_Data(uint8_t *data, uint16_t size);
/*SAI双缓冲发送*/
HAL_StatusTypeDef HAL_SAI_MultiMemTransmit_DMA(SAI_HandleTypeDef *hsai, uint8_t *pData, uint8_t *SecondMemAddress, uint16_t Size);
/*SAI双缓冲接收*/
HAL_StatusTypeDef HAL_SAI_MultiMemReceive_DMA(SAI_HandleTypeDef *hsai, uint8_t *pData, uint8_t *SecondMemAddress, uint16_t Size);

void Sai_ADC_Data_LoopBack(void);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
