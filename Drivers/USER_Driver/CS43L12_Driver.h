/**
 *  @file CS43L12_Driver.h
 *
 *  @date 2021-03-10
 *
 *  @author Copyright (c) 2021 aron566 <aron566@163.com>.
 *
 *  @brief CS43L12 DAC芯片驱动接口
 *  
 *  @version V1.0
 */
#ifndef CS43L12_DRIVER_H
#define CS43L12_DRIVER_H
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< need definition of uint8_t */
#include <stddef.h> /**< need definition of NULL    */
#include <stdbool.h>/**< need definition of BOOL    */
#include <stdio.h>  /**< if need printf             */
#include <stdlib.h>
#include <string.h>
#include <limits.h> /**< need variable max value    */
/** Private includes ---------------------------------------------------------*/

/** Private defines ----------------------------------------------------------*/

/** Exported typedefines -----------------------------------------------------*/
/*CS43L12声道编号*/
typedef enum
{
  CS43L12_DRIVER_LEFT_CH = 0,
  CS43L12_DRIVER_RIGHT_CH,
  CS43L12_DRIVER_LR_CH
}CS43L12_DRIVER_CH_Typedef_t;

/*CS43L12声道输出开关*/
typedef enum
{
  CS43L12_DRIVER_POWER_OFF = 0,
  CS43L12_DRIVER_POWER_ON
}CS43L12_DRIVER_POWER_Typedef_t;

/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*CS43L12 驱动初始化*/
void CS43L12_Driver_Init(void);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
