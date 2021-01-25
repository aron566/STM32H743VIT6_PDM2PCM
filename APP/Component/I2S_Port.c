/**
 *  @file I2S_Port.c
 *
 *  @date 2021-01-15
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 
 *
 *  @details 1、
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "I2S_Port.h"
#include "main.h"
#include "usbd_audio.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
#define ENABLE_I2S_PERIPHERAL 1
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
extern I2S_HandleTypeDef hi2s1;
extern volatile PDM2PCM_BUF_Typedef_t Pdm2Pcm_ChannelBuf[MIC_CHANNEL_NUM];

extern volatile int16_t g_UACRingBuf[UAC_BUFFER_SIZE];
extern volatile uint16_t g_UACWriteIndex;
extern volatile uint16_t g_UACReadIndex;
/** Private variables --------------------------------------------------------*/

/** Private function prototypes ----------------------------------------------*/

/** Private user code --------------------------------------------------------*/

  /** Private application code -------------------------------------------------*/
/*******************************************************************************
*
*       Static code
*
********************************************************************************
*/

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*
*       Public code
*
********************************************************************************
*/
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) 
{

}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) 
{

}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s) 
{

}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s) 
{

}

/**
  ******************************************************************
  * @brief   I2S接口启动
  * @param   [in]None
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2021-01-24
  ******************************************************************
  */
void I2S_Port_Start(void)
{
//  if()
//  {
//    
//  }
}

/**
  ******************************************************************
  * @brief   I2S接口初始化
  * @param   [in]None
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2021-01-24
  ******************************************************************
  */
void I2S_Port_Init(void)
{
#if ENABLE_I2S_PERIPHERAL
  HAL_I2S_Receive_DMA(&hi2s1, (uint16_t *)Pdm2Pcm_ChannelBuf[0].PDM_RX_Buf, PDM_TWO_SAMPLE_NUM);
#endif
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
