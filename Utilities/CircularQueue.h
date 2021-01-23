/**
 *  @file CircularQueue.h
 *
 *  @date 2020/6/25
 *
 *  @author aron566
 *
 *  @brief 缓冲区大小需为2的n次方(最高位为1其余为0)
 *  
 *  @version v1.3
 */
#ifndef CIRCULARQUEUE_H_
#define CIRCULARQUEUE_H_
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
/** Private includes ---------------------------------------------------------*/
/** Private defines ----------------------------------------------------------*/
    
/** Exported typedefines -----------------------------------------------------*/
/** 数据结构体*/
typedef struct
{
	union {
		uint8_t 	*data8Buffer;		/**< for 8bit buffer*/
		uint16_t 	*data16Buffer;	/**< for 16bit buffer*/
		uint32_t 	*data32Buffer;	/**< for 32bit buffer*/
		uint64_t 	*data64Buffer;	/**< for 64bit buffer*/
	}Buffer;
	uint32_t size;
	uint32_t entrance;
	uint32_t exit;
	bool is_malloc;
}CQ_handleTypeDef;

/*缓冲区大小*/
typedef enum
{
	CQ_BUF_128B = 1 << 7,
	CQ_BUF_256B = 1 << 8,
	CQ_BUF_512B = 1 << 9,
	CQ_BUF_1KB 	= 1 << (10 * 1), ///< 1*10
	CQ_BUF_2KB 	= 1 << 11,
	CQ_BUF_4KB 	= 1 << 12,
	CQ_BUF_8KB 	= 1 << 13,
	CQ_BUF_1MB 	= 1 << (10 * 2), ///< 2*10
	CQ_BUF_1GB 	= 1 << (10 * 3), ///< 3*10
    // CQ_BUF_1TB 	= 1 << (10 * 4), ///< 4*10
    // CQ_BUF_1PB 	= 1 << (10 * 5), ///< 5*10
}CQ_BUF_SIZE_ENUM_TypeDef;
/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
#define GET_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define IS_POWER_OF_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*============================= Common ================================*/
bool CQ_isEmpty(CQ_handleTypeDef *CircularQueue);
bool CQ_isFull(CQ_handleTypeDef *CircularQueue);
void CQ_emptyData(CQ_handleTypeDef*CircularQueue);
uint32_t CQ_getLength(CQ_handleTypeDef *CircularQueue);
/*手动缩减缓冲区长度--用作：错误帧偏移-正确帧读取后剔除*/
void CQ_ManualOffsetInc(CQ_handleTypeDef *CircularQueue, uint32_t len);

/*跳转到指定有效数据，用于快速丢弃无效头部数据*/
uint32_t CQ_skipInvaildU8Header(CQ_handleTypeDef *cb, uint8_t header_data);
/*modbus 高位在前-低位在后存储*/
uint32_t CQ_skipInvaildModbusU16Header(CQ_handleTypeDef *cb, uint16_t header_data);
uint32_t CQ_skipInvaildModbusU32Header(CQ_handleTypeDef *cb, uint32_t header_data);
/*低位在前-高位在后*/
uint32_t CQ_skipInvaildU32Header(CQ_handleTypeDef *cb, uint32_t header_data);
uint32_t CQ_skipInvaildU16Header(CQ_handleTypeDef *cb, uint16_t header_data);
/*删除一个由cb_xxcreate创建的缓冲区*/
void cb_delete(CQ_handleTypeDef *CircularQueue);

/*===========================8 Bit Option==============================*/
bool CQ_init(CQ_handleTypeDef *CircularQueue, uint8_t *memAdd, uint32_t len);
/*分配一个缓冲区并进行初始化--替代--CQ_init*/
CQ_handleTypeDef *cb_create(uint32_t buffsize);
uint32_t CQ_getData(CQ_handleTypeDef *CircularQueue, uint8_t *targetBuf, uint32_t len);
uint32_t CQ_putData(CQ_handleTypeDef *CircularQueue, const uint8_t *sourceBuf, uint32_t len);

/*修改后的加入数据操作--数据长度作为帧头先存入缓冲区*/
uint32_t DQ_putData(CQ_handleTypeDef *CircularQueue, const uint8_t *sourceBuf, uint32_t len);
/*修改后的取数据操作--会直接读取帧长度信息，依据当前一包长度加入缓冲区*/
uint32_t DQ_getData(CQ_handleTypeDef *CircularQueue, uint8_t *targetBuf);

/*修改后的获取数据操作--数据读取后不会减小缓冲区长度，需手动减小,目的为了分步取出完整数据*/
uint32_t CQ_ManualGetData(CQ_handleTypeDef *CircularQueue, uint8_t *targetBuf, uint32_t len);
/*修改后的获取数据操作--读取指定偏移的数据，不会减小缓冲区长度,目的为了验证数据，判断帧头等*/
uint8_t CQ_ManualGet_Offset_Data(CQ_handleTypeDef *CircularQueue, uint32_t index);

/*===========================16 Bit Option==============================*/
/*16bit环形缓冲区初始化*/
bool CQ_16_init(CQ_handleTypeDef *CircularQueue, uint16_t *memAdd, uint32_t len);
/*分配一个缓冲区并进行初始化--替代--CQ_16_init*/
CQ_handleTypeDef *cb_16create(uint32_t buffsize);
/*加入16bit类型数据*/
uint32_t CQ_16putData(CQ_handleTypeDef *CircularQueue, const uint16_t * sourceBuf, uint32_t len);
/*取出16bit类型数据*/
uint32_t CQ_16getData(CQ_handleTypeDef *CircularQueue, uint16_t *targetBuf, uint32_t len);

/*===========================32 Bit Option==============================*/
/*32bit环形缓冲区初始化*/
bool CQ_32_init(CQ_handleTypeDef *CircularQueue, uint32_t *memAdd, uint32_t len);
/*分配一个缓冲区并进行初始化--替代--CQ_32_init*/
CQ_handleTypeDef *cb_32create(uint32_t buffsize);
/*加入32bit类型数据*/
uint32_t CQ_32putData(CQ_handleTypeDef *CircularQueue, const uint32_t * sourceBuf, uint32_t len);
/*取出32bit类型数据*/
uint32_t CQ_32getData(CQ_handleTypeDef *CircularQueue, uint32_t *targetBuf, uint32_t len);

#ifdef __cplusplus ///<end extern c
}
#endif
#endif
/******************************** End of file *********************************/
