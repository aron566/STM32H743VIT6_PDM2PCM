/**
 *  @file Timer_Port.c
 *
 *  @date 2021-01-01
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 定时器接口
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
#include "Timer_Port.h"
#include "main.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/

/** Private constants --------------------------------------------------------*/
extern volatile PDM2PCM_BUF_Typedef_t Pdm2Pcm_ChannelBuf[MIC_CHANNEL_NUM];
extern volatile uint8_t Rec_Full_Flag;
static uint32_t cunt= 0;
static uint16_t temp = 0;
static uint8_t fq = 0;
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
static uint32_t Timer_Port_TimeMS = 0;
static uint32_t Timer_Port_TimeSec = 0;
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
  * @brief   定时器中断回调
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-13
  ******************************************************************
  */
void Timer_Port_IRQHandler(TIM_HandleTypeDef *htimx)
{
  Timer_Port_TimeMS++;
  if(Timer_Port_TimeMS == 1000)
  {
    Timer_Port_TimeMS = 0;
    Timer_Port_TimeSec++;
  }
//  HAL_GPIO_WritePin(CLOCK_OUT_GPIO_Port, CLOCK_OUT_Pin, GPIO_PIN_SET);
//  HAL_Delay(1);
//  HAL_GPIO_WritePin(CLOCK_OUT_GPIO_Port, CLOCK_OUT_Pin, GPIO_PIN_RESET);
//  
//  GPIO_PinState bitstatus = HAL_GPIO_ReadPin(DATA_IN_GPIO_Port, DATA_IN_Pin);
//  
//  if(fq == 16)
//  {
//    fq = 0;
//    /*存入数据*/
//    Pdm2Pcm_ChannelBuf[AUDIO_MIC_CHANNEL].PDM_RX_Buf[cunt++] = temp;
//    temp = 0;
//    if(cunt>=PDM_ONE_SAMPLE_NUM)
//    {
//      cunt = 0;
//      Rec_Full_Flag = 1;
//    }
//  }
//  else
//  {
//    temp = (uint8_t)bitstatus;
//    temp <<= 1;
//  }
//  fq++;
}

/**
  ******************************************************************
  * @brief   初始化定时器
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-13
  ******************************************************************
  */
void Timer_Port_Init(void)
{
  HAL_TIM_Base_Start_IT(&htim3);
}

/**
  ******************************************************************
  * @brief   获取当前运行累计时间
  * @param   [in]time_base 单位
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-13
  ******************************************************************
  */
uint32_t Timer_Port_Get_Current_Time(TIMER_TIME_UNIT_Typedef_t time_unit)
{
  
  return (time_unit == TIMER_MS)?Timer_Port_TimeMS:Timer_Port_TimeSec;
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
