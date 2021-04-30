/**
 *  @file Timer_Port.c
 *
 *  @date 2021-01-01
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 定时任务接口
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
//#include "Update_Port.h"
//#include "Utilities_Multi_Timer.h"
//#include "Utilities_Multi_Button.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
#define ENABLE_SYSTICK_COUNT  1/**< 是否启用Systick计数*/
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/

/** Private variables --------------------------------------------------------*/
static uint32_t Timer_Port_TimeMS  = 0;
static uint32_t Timer_Port_TimeSec = 0;
/** Private function prototypes ----------------------------------------------*/
static inline void Timer_Port_IRQHandler(void);
/** Private user code --------------------------------------------------------*/

/** Private application code -------------------------------------------------*/
/*******************************************************************************
*
*       Static code
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
static inline void Timer_Port_IRQHandler(void)
{
  Timer_Port_TimeMS++;
  if(Timer_Port_TimeMS == 1000)
  {
    Timer_Port_TimeMS = 0;
    Timer_Port_TimeSec++;
  }
//  timer_ticks(); ///<! 1ms ticks
}

/**
  ******************************************************************
  * @brief   定时1任务
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-02-25
  ******************************************************************
  */
static void timer1_callback(void)
{
  /*参数检测随存*/
//  Parameter_Port_Start();
}

/**
  ******************************************************************
  * @brief   延时任务
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-02-25
  ******************************************************************
  */
static void timer2_callback(void)
{
  /*更新接口初始化*/
//  Update_Port_Init();
}

/**
  ******************************************************************
  * @brief   定时3任务
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-04-19
  ******************************************************************
  */
static void timer3_callback(void)
{
  /*按键检测时基*/
//  button_ticks();
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
  * @brief   定时器周期时间到回调
  * @param   [in]htim 定时器句柄
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-02-25
  ******************************************************************
  */
#if !ENABLE_SYSTICK_COUNT
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM3)
  {
    Timer_Port_IRQHandler();
  }
}
#endif

/**
  ******************************************************************
  * @brief   滴答时钟回调
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-02-25
  ******************************************************************
  */
void HAL_SYSTICK_Callback(void)
{
#if ENABLE_SYSTICK_COUNT
  Timer_Port_IRQHandler();
#endif
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

/**
  ******************************************************************
  * @brief   定时器接口启动
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-13
  ******************************************************************
  */
void Timer_Port_Start(void)
{
//  timer_loop();
}

/**
  ******************************************************************
  * @brief   定时器初始化
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-04-19
  ******************************************************************
  */
void Timer_Port_Init(void)
{
//  /*初始化定时任务 3s loop*/
//	timer_init(&timer1, timer1_callback, 3000, 3000);
//	timer_start(&timer1);
//	
//  /*初始化延时任务 50ms delay*/
//	timer_init(&timer2, timer2_callback, 50, 0);
//	timer_start(&timer2);

//  /*初始化定时任务 5ms delay*/
//	timer_init(&timer3, timer3_callback, 1000, 5);
//	timer_start(&timer3);
}

/******************************** End of file *********************************/
