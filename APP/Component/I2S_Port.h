/**
 *  @file I2S_Port.h
 *
 *  @date 2021-01-21
 *
 *  @author aron566
 *
 *  @brief 
 *  
 *  @version V1.0
 */
#ifndef I2S_PORT_H
#define I2S_PORT_H
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

/*I2S接口初始化*/
void I2S_Port_Init(void);
/*I2S接口启动*/
void I2S_Port_Start(void);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
