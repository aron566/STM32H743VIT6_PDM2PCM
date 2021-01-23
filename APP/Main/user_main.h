/**
 *  @file user_mian.h
 *
 *  @date 2021-01-11
 *
 *  @author aron566
 *
 *  @brief 主线任务
 *  
 *  @version V1.0
 */
#ifndef USER_MAIN_H
#define USER_MAIN_H
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

void User_InterruptVectorTable_Move(void);
void User_Main_Task_Init(void);
void User_Main_Task_Process_Loop(void);
void User_Main_PlayTask_Process_Loop(void);
/*存储来自音频设备数据*/
void User_Audio_Channel_Data_Save(uint16_t *data, uint32_t len);


#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
