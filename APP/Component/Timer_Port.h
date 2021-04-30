/**
 *  @file Timer_Port.h
 *
 *  @date 2021-01-13
 *
 *  @author aron566
 *
 *  @brief ��ʱ���ӿ�
 *  
 *  @version V1.0
 */
#ifndef TIMER_PORT_H
#define TIMER_PORT_H
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< nedd definition of uint8_t */
#include <stddef.h> /**< need definition of NULL    */
//#include <stdbool.h>/**< need definition of BOOL    */
#include <stdio.h>  /**< if need printf             */
#include <stdlib.h>
#include <string.h>
#include <limits.h> /**< need variable max value    */
/** Private includes ---------------------------------------------------------*/
#include "tim.h"
/** Private defines ----------------------------------------------------------*/

/** Exported typedefines -----------------------------------------------------*/
/*��ʱ��ʱ�䵥λ*/
typedef enum
{
  TIMER_SEC = 0,
  TIMER_MS
}TIMER_TIME_UNIT_Typedef_t;
/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*��ʼ����ʱ��*/
void Timer_Port_Init(void);
/*��ȡ����ʱ��*/
uint32_t Timer_Port_Get_Current_Time(TIMER_TIME_UNIT_Typedef_t time_unit);
/*��ʱ���жϻص�*/
void Timer_Port_IRQHandler(TIM_HandleTypeDef *htimx);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
