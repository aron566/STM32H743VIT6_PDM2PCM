/**
 *  @file LED_Port.h
 *
 *  @date 2021-01-07
 *
 *  @author aron566
 *
 *  @brief led操作接口
 *  
 *  @version V1.0
 */
#ifndef LED_PORT_H
#define LED_PORT_H
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

/*打开TP LED*/
void LED_Port_Set_TP_Open(void);
/*关闭TP LED*/
void LED_Port_Set_TP_Close(void);
/*闪烁LED1*/
void LED_Toggle_LED1(void);
  
#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
