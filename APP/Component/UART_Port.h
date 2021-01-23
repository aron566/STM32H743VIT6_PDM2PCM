/**
 *  @file UART_Port.h
 *
 *  @date 2021-01-04
 *
 *  @author aron566
 *
 *  @brief 串口操作接口
 *  
 *  @version V1.0
 */
#ifndef UART_PORT_H
#define UART_PORT_H
#ifdef __cplusplus //use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/*库接口*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
/** Private includes ---------------------------------------------------------*/
/*外部接口*/
#include "stm32H7xx_hal.h"
#include "usart.h"
#if USE_FREERTOS
#include "cmsis_os.h"
#endif
/*内部接口*/
#include "CircularQueue.h"

/** Exported defines ----------------------------------------------------------*/
#define USE_FREERTOS 0
/** Exported typedefines -----------------------------------------------------*/
typedef enum
{
    UART_NUM_0 = 0,
    UART_NUM_1,
    UART_NUM_2,
    UART_NUM_3,
    UART_NUM_4,
    UART_NUM_5,
    UART_NUM_6,
}Uart_num_t;

typedef struct
{
    UART_HandleTypeDef *phuart;     //uart端口
    DMA_HandleTypeDef  *phdma_rx;
    CQ_handleTypeDef *cb;           //环形队列
    uint8_t *RX_Buff_Temp;          //接收缓冲
    uint16_t MAX_RX_Temp;           //最大接收数量
    int Is_Half_Duplex;             //半双工模式
#if USE_FREERTOS
    osSemaphoreId *pRX_Sem;         //接收二值信号量
#endif
}Uart_Dev_Handle_t;
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/

/*串口操作初始化*/
void Uart_Port_Init(void);
/*串口操作句柄获取*/
Uart_Dev_Handle_t *Uart_Port_Get_Handle(Uart_num_t uart_num);
/*串口发送数据*/
bool Uart_Port_Transmit_Data(Uart_Dev_Handle_t *uart_opt_handle, uint8_t *data, uint16_t size);
/*串口接收中断处理*/
void USER_UART_IRQHandler(UART_HandleTypeDef *huart);

#ifdef __cplusplus //end extern c
}
#endif
#endif
