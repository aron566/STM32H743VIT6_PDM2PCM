/**
 *  @file PDM_Datacov.c
 *
 *  @date 2021-01-19
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief PDM数据转换
 *
 *  @details 1、PDM为脉冲密度调制 脉冲的相对密度对应于模拟信号的幅度。
 *              大量的1s对应于高（正）幅度值，而大量的0s对应于低（负）幅度值，交替的1s和0s对应于幅度值0
 *           2、PCM为脉冲编码 在PCM信号中，具体的幅度值被编码为脉冲
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "PDM_Datacov.h"
#include "pdm2pcm.h"
/** Private typedef ----------------------------------------------------------*/
/*PDM转PCM返回值*/
typedef enum
{
  USER_PDM_FILTER_NO_ERROR              = 0x0000, ///< 无错误
  USER_PDM_FILTER_ENDIANNESS_ERROR      = 0x0001, ///< 不支持的字节序
  USER_PDM_FILTER_BIT_ORDER_ERROR       = 0x0002, ///< 不支持的位序
  USER_PDM_FILTER_CRC_LOCK_ERROR        = 0x0004, ///< 目标不是STM32
  USER_PDM_FILTER_DECIMATION_ERROR      = 0x0008, ///< 不支持的抽取因子
  USER_PDM_FILTER_INIT_ERROR            = 0x0010, ///< -
  USER_PDM_FILTER_CONFIG_ERROR          = 0x0020, ///< -
  USER_PDM_FILTER_GAIN_ERROR            = 0x0040, ///< 不支持的麦克风增益
  USER_PDM_FILTER_SAMPLES_NUMBER_ERROR  = 0x0080, ///< 不支持的样本数
}PDM2PCM_ERROR_CODE_Typedef_t;
/** Private macros -----------------------------------------------------------*/
#define ENABLE_DEBUG    1
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
#if ENABLE_DEBUG
/**
  ******************************************************************
  * @brief   PDM流转换PCM数据流返回码解析
  * @param   [in]code 返回码
  * @return  code
  * @author  aron566
  * @version V1.0
  * @date    2021-01-19
  ******************************************************************
  */
static uint8_t Decode_PDM2PCM_Return_Code(uint8_t code)
{
  if(code == 0)
  {
//    printf("pdm to pcm no error.\r\n");
    return code;
  }
  
  if(code & USER_PDM_FILTER_ENDIANNESS_ERROR)
  {
    printf("不支持的字节序.\r\n");
  }
  else if(code & USER_PDM_FILTER_BIT_ORDER_ERROR)
  {
    printf("不支持的位序.\r\n");
  }
  else if(code & USER_PDM_FILTER_CRC_LOCK_ERROR)
  {
    printf("目标不是STM32.\r\n");
  }
  else if(code & USER_PDM_FILTER_DECIMATION_ERROR)
  {
    printf("不支持的抽取因子.\r\n");
  }
  else if(code & USER_PDM_FILTER_GAIN_ERROR)
  {
    printf("不支持的麦克风增益.\r\n");
  }
  else if(code & USER_PDM_FILTER_SAMPLES_NUMBER_ERROR)
  {
    printf("不支持的样本数.\r\n");
  }
  return code;
}
#endif

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*
*       Public code
*
********************************************************************************
*/
/**
  ******************************************************************
  * @brief   PDM流转换PCM数据流接口
  * @param   [in]PDMBuf PDM数据流
  * @param   [out]PCMBuf PCM数据流存储区
  * @return  转换返回状态码
  * @author  aron566
  * @version V1.0
  * @date    2021-01-19
  ******************************************************************
  */
uint8_t PDM_To_PCM_Stream(uint16_t *PDMBuf, uint16_t *PCMBuf)
{
#if ENABLE_DEBUG
  return Decode_PDM2PCM_Return_Code(MX_PDM2PCM_Process(PDMBuf, PCMBuf));
#else
  return MX_PDM2PCM_Process(PDMBuf, PCMBuf);
#endif
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
