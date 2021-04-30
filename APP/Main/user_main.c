/**
 *  @file user_main.c
 *
 *  @date 2021-01-11
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 主线任务
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
#include "user_main.h"
#include "main.h"

/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
/*判断音频数据是否小于最小检测数据量*/
#define AUDIO_DATA_SIZE_MIN   STEREO_FRAME_SIZE//MONO_FRAME_SIZE
#define AUDIO_DATA_SIZE_IS_LESS_AUDIO_DATA_MIN(size)  ((size < AUDIO_DATA_SIZE_MIN)? \
                                                      1:0)
/*将整形数据数组复制到浮点型数组中*/
#define INT16_ARRAY2FLOAT32(float_buf, i16_buf, size) do{ \
                                                          for(uint16_t index = 0; index < size; index++) \
                                                          { \
                                                            float_buf[index] = (float)i16_buf[index]; \
                                                          }}while(0)
/*将数据复制成双份到数组中*/
#define INT16_ARRAY_CROSS_JOIN(cross_buf, source_buf, cross_size) \
                                                      do{ \
                                                          uint16_t cnt = 0; \
                                                          for(uint16_t index = 0; index < cross_size; index +=2) \
                                                          { \
                                                            cross_buf[index+1] = cross_buf[index] = source_buf[cnt++]; \
                                                          }}while(0)
                                                          
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
  * @brief   播放音频任务处理
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-14
  ******************************************************************
  */
void User_Main_PlayTask_Process_Loop(void)
{

}

/**
  ******************************************************************
  * @brief   任务处理
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-11
  ******************************************************************
  */
void User_Main_Task_Process_Loop(void)
{
  for(;;)
  {
//    DFSDM_Port_Start();
    
    Sai_Port_Start();
  }
  /*never return*/
}

/**
  ******************************************************************
  * @brief   初始化各模块功能
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-11
  ******************************************************************
  */
void User_Main_Task_Init(void)
{
  /*串口操作初始化*/
  Uart_Port_Init();

  /*定时器初始化*/
  Timer_Port_Init();
  
  /*Sai操作初始化*/
  Sai_Port_Init();
  
  /*DFSDM初始化*/
  DFSDM_Port_Init();
  
  /*USB初始化*/
  UAC_Init();
  
  /*I2S初始化*/
  I2S_Port_Init();
  
  /*DAC初始化*/
  DAC_Port_Init();
  /*other initialization task code*/
}

/**
  ******************************************************************
  * @brief   中断向量表拷贝到D加速区
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-18
  ******************************************************************
  */
void User_InterruptVectorTable_Move(void)
{
  uint32_t *SouceAddr = (uint32_t *)FLASH_BANK1_BASE;
  uint32_t *DestAddr = (uint32_t *)D1_DTCMRAM_BASE;
  memcpy(DestAddr, SouceAddr, 0x400);
  /* 设置中断向量表到 DTCM 里面*/
  SCB->VTOR = D1_DTCMRAM_BASE;
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
