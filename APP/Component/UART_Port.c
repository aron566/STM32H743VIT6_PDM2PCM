/**
 *  @file UART_Port.c
 *
 *  @date 2020-02-22
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 串口操作
 *
 *  @details None.
 *
 *  @version V1.0
 */
#ifdef __cplusplus //use C compiler
extern "C" {
#endif
#include "UART_Port.h"/*外部接口*/
/* External variables --------------------------------------------------------*/
//extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
//extern UART_HandleTypeDef huart3;
//extern UART_HandleTypeDef huart4;
//extern UART_HandleTypeDef huart5;
//extern UART_HandleTypeDef huart6; 
//extern DMA_HandleTypeDef hdma_usart1_rx;
//extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
//extern DMA_HandleTypeDef hdma_usart2_tx;
//extern DMA_HandleTypeDef hdma_usart3_rx;
//extern DMA_HandleTypeDef hdma_usart3_tx;
//extern DMA_HandleTypeDef hdma_usart4_rx;
//extern DMA_HandleTypeDef hdma_usart4_tx;
//extern DMA_HandleTypeDef hdma_usart5_rx;
//extern DMA_HandleTypeDef hdma_usart5_tx;
//extern DMA_HandleTypeDef hdma_usart6_rx;
//extern DMA_HandleTypeDef hdma_usart6_tx;
/** Private typedef ----------------------------------------------------------*/
/** Private macros -----------------------------------------------------------*/
#define USE_LOOPBACK 	0
#define DEBUG_UART		&huart2
#define UART_MAX_NUM    6/**< 最大允许串口数量*/
/*定义内存管理接口*/
#if !defined(uartport_malloc) && !defined(uartport_free)
#define uartport_malloc  malloc//pvPortMalloc
#define uartport_free  free//vPortFree
#endif

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
/*预定义串口设备信息*/
static Uart_Dev_Handle_t *Uart_pDevice[UART_MAX_NUM+1];
/** Private function prototypes ----------------------------------------------*/
static uint8_t Get_Uart_Index(USART_TypeDef *Instance);
static Uart_Dev_Handle_t *Create_Uart_Dev(Uart_num_t uart_num, UART_HandleTypeDef *huart 
                                        ,DMA_HandleTypeDef *hdma_rx, uint16_t rx_temp_size 
                                        ,uint32_t rxsize, int work_mode
#if USE_FREERTOS  
                                        , osSemaphoreId *pRX_Sem
#endif
                                        );
/** Private user code --------------------------------------------------------*/
/** Private application code -------------------------------------------------*/
/*******************************************************************************
*
*       Static code
*
********************************************************************************
*/
/*
 * 
 *
 * 参数：串口号 串口设备指针 dma操作地址 ，临时缓冲大小 接收队列大小 工作模式 二值信号量
 */
/************************************************************
  * @brief   建立串口设备，为其建立双缓冲区-->使能串口空闲中断
  * @param   [in]uart_num 串口号.
  * @param   [in]huart    串口句柄.
  * @param   [in]hdma_rx  串口dma句柄.
  * @param   [in]rx_temp_size  串口临时缓存大小.
  * @param   [in]rxsize   串口接收缓存大小.
  * @param   [in]work_mode 串口工作模式.
  * @param   [in]pRX_Sem  串口接收信号量.  
  * @return  串口操作句柄.
  * @author  aron566
  * @date    2020/3/15
  * @version v1.0
  * @note    @@
  ***********************************************************/
static Uart_Dev_Handle_t *Create_Uart_Dev(Uart_num_t uart_num ,UART_HandleTypeDef *huart 
                                        ,DMA_HandleTypeDef *hdma_rx ,uint16_t rx_temp_size 
                                        ,uint32_t rxsize ,int work_mode 
#if USE_FREERTOS  
                                        ,osSemaphoreId *pRX_Sem
#endif
                                        )
{
    Uart_Dev_Handle_t *pUart_Dev = (Uart_Dev_Handle_t *)uartport_malloc(sizeof(Uart_Dev_Handle_t));
    pUart_Dev->phuart = huart;
    pUart_Dev->phdma_rx = hdma_rx;
    pUart_Dev->cb = cb_create(rxsize);
    pUart_Dev->MAX_RX_Temp = rx_temp_size;
    pUart_Dev->RX_Buff_Temp = (uint8_t *)uartport_malloc(sizeof(uint8_t)*rx_temp_size);
    if(NULL == pUart_Dev->RX_Buff_Temp)
    {
      return NULL;
    }
    pUart_Dev->Is_Half_Duplex = work_mode;
#if USE_FREERTOS
    pUart_Dev->pRX_Sem = pRX_Sem;
#endif
    Uart_pDevice[uart_num] = pUart_Dev;
    //打开空闲中断
    __HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);
    //使能DMA接收
    HAL_UART_Receive_DMA(huart, pUart_Dev->RX_Buff_Temp, pUart_Dev->MAX_RX_Temp);
    return pUart_Dev;
}

/************************************************************
  * @brief   获得当前串口信息索引
  * @param   [in]Instance 串口句柄.
  * @return  索引号.
  * @author  aron566
  * @date    2020/3/15
  * @version v1.0
  * @note    @@
  ***********************************************************/
static uint8_t Get_Uart_Index(USART_TypeDef *Instance)
{
    uint8_t index = 0;
    for(;index < UART_MAX_NUM+1;index++)
    {
        if(Uart_pDevice[index] == NULL)
        {
          continue;
        }
        if(Uart_pDevice[index]->phuart->Instance == Instance)
        {
          return index;
        }  
    }
    return 0;
}

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*
*       Public code
*
********************************************************************************
*/

/************************************************************
  * @brief   Rx Transfer IRQ
  * @param   huart UART handle.
  * @return  None
  * @author  aron566
  * @date    2020/3/15
  * @version v1.0
  * @note    @@
  ***********************************************************/
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    uint8_t index = Get_Uart_Index(huart->Instance);
    if(index != 0)
    {
        if((__HAL_UART_GET_FLAG(Uart_pDevice[index]->phuart ,UART_FLAG_IDLE) != RESET))
        {
          /*
          首先停止DMA传输，
          1.防止后面又有数据接收到，产生干扰，因为此时的数据还未处理。
          2.DMA需要重新配置。
          */
          HAL_UART_DMAStop(Uart_pDevice[index]->phuart);
          /*清楚空闲中断标志，否则会一直不断进入中断*/
          __HAL_UART_CLEAR_IDLEFLAG(Uart_pDevice[index]->phuart);
          /*计算本次接收数据长度*/
          uint32_t data_length  = Uart_pDevice[index]->MAX_RX_Temp - __HAL_DMA_GET_COUNTER(Uart_pDevice[index]->phdma_rx);
          /*将数据记录至环形区*/
          CQ_putData(Uart_pDevice[index]->cb ,Uart_pDevice[index]->RX_Buff_Temp ,(uint32_t)data_length);
#if USE_LOOPBACK 	
          HAL_UART_Transmit(DEBUG_UART, (uint8_t *)Uart_pDevice[index]->RX_Buff_Temp,(uint16_t)data_length,0xFFFF);
#endif
          /*清空临时缓冲区*/
          memset(Uart_pDevice[index]->RX_Buff_Temp ,0 ,data_length);
          data_length = 0;
          /*打开空闲中断*/
          __HAL_UART_ENABLE_IT(Uart_pDevice[index]->phuart ,UART_IT_IDLE);
          /*重启开始DMA传输*/
          HAL_UART_Receive_DMA(Uart_pDevice[index]->phuart ,Uart_pDevice[index]->RX_Buff_Temp ,Uart_pDevice[index]->MAX_RX_Temp);
        }
    }
}  

/************************************************************
  * @brief   串口操作句柄获取
  * @param   [in]uart_opt_handle 串口句柄.
  * @param   [in]data 数据.
  * @param   [in]size 数据大小.
  * @return  true 发送成功.
  * @author  aron566
  * @date    2020/3/15
  * @version v1.0
  * @note    @@
  ***********************************************************/
bool Uart_Port_Transmit_Data(Uart_Dev_Handle_t *uart_opt_handle, uint8_t *data, uint16_t size)
{
  if(uart_opt_handle == NULL)
  {
    return false;
  }
  if(HAL_UART_Transmit_IT(uart_opt_handle->phuart, data, size) != HAL_OK)
  {
    return false;
  }
  return true;
}  

/************************************************************
  * @brief   串口操作句柄获取
  * @param   [in]uart_num 串口号.
  * @return  串口句柄.
  * @author  aron566
  * @date    2020/3/15
  * @version v1.0
  * @note    @@
  ***********************************************************/
Uart_Dev_Handle_t *Uart_Port_Get_Handle(Uart_num_t uart_num)
{
    if(uart_num >= UART_MAX_NUM)
    {
      return NULL;
    }
    return Uart_pDevice[uart_num];
}

/************************************************************
  * @brief   串口操作初始化
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @date    2020/3/15
  * @version v1.0
  * @note    @@
  ***********************************************************/
void Uart_Port_Init(void)
{
    Uart_pDevice[UART_NUM_2] = Create_Uart_Dev(UART_NUM_2, &huart2, 
                                                &hdma_usart2_rx, 
                                                128, 
                                                (uint32_t)CQ_BUF_256B, 0
#if USE_FREERTOS
  , NULL
#endif
  );
    if(Uart_pDevice[UART_NUM_2] == NULL)
    {
      printf("create uart opt handle faild.\n");
    }
}

#pragma import(__use_no_semihosting) // 确保没有从 C 库链接使用半主机的函数
struct __FILE // 标准库需要的支持函数
{
	int handle;
};
/* FILE is typedef ’ d in stdio.h. */
FILE __stdout;
void _ttywrch(int ch)
{
    ch=ch;
}

void _sys_exit(int x) //定义 _sys_exit() 以避免使用半主机模式
{
    (void)x;
}

#if (USE_NEW_REDIRECT == 0)
#include "stdio.h"
/************************************************************
  * @brief   重定向c库函数printf到HAL_UART_Transmit
  * @param   [in]ch 输出字符.
  * @param   [in]f 文件指针
  * @return  字符
  * @author  aron566
  * @date    2020/3/15
  * @version v1.0
  * @note    @@
  ***********************************************************/
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(DEBUG_UART, (uint8_t *)&ch, 1, 10);
  return ch;
}

/************************************************************
  * @brief   重定向c库函数getchar,scanf
  * @param   [in]f 文件指针
  * @return  字符
  * @author  aron566
  * @date    2020/3/15
  * @version v1.0
  * @note    @@
  ***********************************************************/
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  while(HAL_UART_Receive(DEBUG_UART, &ch, 1, 0xffff) != HAL_OK);
  return ch;
}

#else
/*新式重定向*/
#include "stdio.h"
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart1 ,()uint8_t)&ch ,1 ,0xFFFF);
    return ch;
}
int __write(int file, char *ptr, int len)
{
    int DataIdx;
    for(DataIdx = 0; DataIdx < len; DataIdx++)
    {
        __io_putchar(*ptr++);
    }
    return len;
}
#endif

#ifdef __cplusplus //end extern c
}
#endif
