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
