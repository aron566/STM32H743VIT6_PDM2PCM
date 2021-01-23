/**
 *  @file USB_Audio_Port.c
 *
 *  @date 2021-01-08
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief USB音频操作接口
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
#include "USB_Audio_Port.h"
#include "usbd_audio_if.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/

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

/**
  ******************************************************************
  * @brief   音频输出设备初始化
  * @param   [in]AudioFreq 频率
  * @param   [in]Volume 音量 
  * @param   [in]options 保留
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_Init(uint32_t AudioFreq, uint32_t Volume, uint32_t options)
{
  UNUSED(AudioFreq);
  UNUSED(Volume);
  UNUSED(options);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备反初始化
  * @param   [in]options 保留
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_DeInit(uint32_t options)
{
  UNUSED(options);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备功能控制
  * @param   [in]pbuf 控制数据
  * @param   [in]size 控制数据大小 
  * @param   [in]cmd 功能
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_Command(uint8_t* pbuf, uint32_t size, uint8_t cmd)
{
//  AUDIO_CMD_START = 1,
//  AUDIO_CMD_PLAY,
//  AUDIO_CMD_STOP,

  UNUSED(pbuf);
  UNUSED(size);
  UNUSED(cmd);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备音量控制
  * @param   [in]vol 0-100音量等级
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_VolumeCtl(uint8_t vol)
{
  UNUSED(vol);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备消音控制
  * @param   [in]cmd 功能
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_MuteCtl(uint8_t cmd)
{
  UNUSED(cmd);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备周期控制操作
  * @param   [in]cmd 功能
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_PeriodicTC(uint8_t *pbuf, uint32_t size, uint8_t cmd)
{
  UNUSED(pbuf);
  UNUSED(size);
  UNUSED(cmd);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备状态获取
  * @param   [in]cmd 功能
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_GetState(void)
{
  return USBD_OK;
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
