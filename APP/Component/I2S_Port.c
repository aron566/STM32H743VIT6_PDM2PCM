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
#include "USB_Audio_Port.h"
#include "usbd_audio_if.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
extern I2S_HandleTypeDef hi2s1;
extern uint16_t PDM_Buffer[AUDIO_CHANNELS*AUDIO_SAMPLING_FREQUENCY/1000*64/8];
extern volatile PDM2PCM_BUF_Typedef_t Pdm2Pcm_ChannelBuf[MIC_CHANNEL_NUM];
/** Private variables --------------------------------------------------------*/
static uint16_t PCM_Buffer[AUDIO_CHANNELS*AUDIO_SAMPLING_FREQUENCY/1000];
/** Private function prototypes ----------------------------------------------*/

/** Private user code --------------------------------------------------------*/

  /** Private application code -------------------------------------------------*/
/*******************************************************************************
*
*       Static code
*
********************************************************************************
*/
/**
* @brief  User function that is called when 1 ms of PDM data is available.
* 		  In this application only PDM to PCM conversion and USB streaming
*                  is performed.
* 		  User can add his own code here to perform some DSP or audio analysis.
* @param  none
* @retval None
*/
static void AudioProcess(void)
{
  USB_Audio_IN_PDMToPCM((uint16_t * )PDM_Buffer,PCM_Buffer);
  Send_Audio_to_USB((int16_t *)PCM_Buffer, AUDIO_SAMPLING_FREQUENCY/1000*AUDIO_CHANNELS);
}

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*
*       Public code
*
********************************************************************************
*/
/**
  ******************************************************************
  * @brief   I2S半发送完成回调
  * @param   [in]hi2s I2S句柄
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2021-01-24
  ******************************************************************
  */
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) 
{
  UNUSED(hi2s);
}

/**
  ******************************************************************
  * @brief   I2S发送完成回调
  * @param   [in]hi2s I2S句柄
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2021-01-24
  ******************************************************************
  */
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) 
{
  UNUSED(hi2s);
}

/**
  ******************************************************************
  * @brief   I2S半接收完成回调
  * @param   [in]hi2s I2S句柄
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2021-01-24
  ******************************************************************
  */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s) 
{

}

/**
  ******************************************************************
  * @brief   I2S接收完成回调
  * @param   [in]hi2s I2S句柄
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2021-01-24
  ******************************************************************
  */
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s) 
{

}

/**
* @brief  Half Transfer user callback, called by BSP functions.
* @param  None
* @retval None
*/
void USB_Audio_IN_HalfTransfer_CallBack(void)
{
  AudioProcess();
}

/**
* @brief  Transfer Complete user callback, called by BSP functions.
* @param  None
* @retval None
*/
void USB_Audio_IN_TransferComplete_CallBack(void)
{
  AudioProcess();
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
  for(;;)
  {
    /*TODO*/
  }
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
  HAL_I2S_Receive_DMA(&hi2s1, (uint16_t *)Pdm2Pcm_ChannelBuf[0].PDM_RX_Buf, PDM_TWO_SAMPLE_NUM);
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
