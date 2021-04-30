/**
 *  @file DAC_Port.c
 *
 *  @date 2021-01-15
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief DAC�����ӿ�
 *
 *  @details 1��
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "DAC_Port.h"
#include "dac.h"
#include "main.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
#define DAC_IN_VAL_MAX      4095U/**< ��󾫶�12λ*/
#define DAC_VOL_MAX         3.3
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
extern DAC_HandleTypeDef hdac1;
/** Private variables --------------------------------------------------------*/
static float u16TOu12_Factor = 0.0078;
/** Private function prototypes ----------------------------------------------*/
static inline int16_t int16CovU12(int16_t data);
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
  * @brief   INT16תΪU12 DAC��ֵ
  * @param   [in]data
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-15
  ******************************************************************
  */
static inline int16_t int16CovU12(int16_t data)
{
  float temp = u16TOu12_Factor*data;
  return (data <= 0)?0:(int16_t)temp;
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
  * @brief   DAC��ʼ��
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-15
  ******************************************************************
  */
void DAC_Port_Init(void)
{
  u16TOu12_Factor = (float)DAC_IN_VAL_MAX/SHRT_MAX;
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
}

/**
  ******************************************************************
  * @brief   DAC���������ֵ����ϵ��
  * @param   [in]limit_val ��ֵ
  * @return  ��DACϵ��.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-15
  ******************************************************************
  */
float DAC_Port_Set_OutMax(int16_t limit_val)
{
  return u16TOu12_Factor = (float)DAC_IN_VAL_MAX/limit_val;
}

/**
  ******************************************************************
  * @brief   DAC���
  * @param   [in]pData ��ת��������
  * @param   [in]Length ������Ŀ->����DMA���õĴ��䵥λ
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-15
  ******************************************************************
  */
void DAC_Port_Set_Vol(int16_t *pData, uint32_t Length)
{
  /*ת������*/
  for(uint32_t i = 0; i < Length; i++)
  {
    pData[i] = int16CovU12(pData[i]);
  }
  HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t *)pData, Length, DAC_ALIGN_12B_R);
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
