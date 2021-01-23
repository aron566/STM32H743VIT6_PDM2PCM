/**
 *  @file LED_Port.c
 *
 *  @date 2021-01-01
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief led操作接口
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
#include "LED_Port.h"
#include "main.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
#define TEST_GPIO_TOGGLE HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin)
#define TEST_GPIO_HIGH   HAL_GPIO_WritePin(TP2_GPIO_Port,TP2_Pin,GPIO_PIN_SET)
#define TEST_GPIO_LOW    HAL_GPIO_WritePin(TP2_GPIO_Port,TP2_Pin,GPIO_PIN_RESET)
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
/**
  ******************************************************************
  * @brief   
  * @param   [in]None
  * @return  TRUE  成功
  * @return  FALSE 失败
  * @author  aron566
  * @version V1.0
  * @date    2021-01-01
  ******************************************************************
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
  * @brief   打开TP LED
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-07
  ******************************************************************
  */
void LED_Port_Set_TP_Open(void)
{
  TEST_GPIO_HIGH;
}

/**
  ******************************************************************
  * @brief   关闭TP LED
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-07
  ******************************************************************
  */
void LED_Port_Set_TP_Close(void)
{
  TEST_GPIO_LOW;
}

/**
  ******************************************************************
  * @brief   闪烁LED1
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-07
  ******************************************************************
  */
void LED_Toggle_LED1(void)
{
  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
