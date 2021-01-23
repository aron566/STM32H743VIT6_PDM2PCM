/**
 *  @file DAC_Port.h
 *
 *  @date 2021-01-15
 *
 *  @author aron566
 *
 *  @brief DAC操作接口
 *  
 *  @version V1.0
 */
#ifndef DAC_PORT_H
#define DAC_PORT_H
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
#include <stdint.h> /**< nedd definition of uint8_t */
#include <stddef.h> /**< need definition of NULL    */
#include <stdbool.h>/**< need definition of BOOL    */
#include <stdio.h>  /**< if need printf             */
#include <stdlib.h>
#include <string.h>
#include <limits.h> /**< need variable max value    */
/** Private includes ---------------------------------------------------------*/

/** Private defines ----------------------------------------------------------*/

/** Exported typedefines -----------------------------------------------------*/

/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*DAC初始化*/
void DAC_Port_Init(void);
/*DAC设置输出阈值*/
float DAC_Port_Set_OutMax(int16_t limit_val);
/*DAC输出*/
void DAC_Port_Set_Vol(int16_t *pData, uint32_t Length);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
