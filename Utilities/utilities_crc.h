/**
 *  @file utilities_crc.h
 *
 *  @date 2021-01-06
 *
 *  @author aron566
 *
 *  @brief crc计算
 *  
 *  @version V1.0
 */
#ifndef UTILITIES_H
#define UTILITIES_H
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

/*查表计算CRC16*/
uint16_t get_crc(uint8_t *puchMsg, uint16_t usDataLen);
/*查表计算CRC16并对比数据包中的CRC，返回结果，数据包含crc计算内容+crc结果，否则可能指针越界访问*/                                                                              
bool get_crc_result(uint8_t *puchMsg, uint16_t usDataLen);

/*计算CRC*/
uint16_t modbus_crc_return(uint8_t *data, uint16_t data_len);
/*查表法计算modbus CRC16*/
uint16_t modbus_crc_return_with_table(uint8_t *data, uint16_t data_len);
/*计算CRC16并对比数据包中的CRC，返回结果，数据包含crc计算内容+crc结果，否则可能指针越界访问*/ 
bool modbus_get_crc_result(uint8_t *msg, uint16_t len);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
