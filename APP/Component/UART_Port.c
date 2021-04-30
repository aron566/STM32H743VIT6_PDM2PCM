/**
 *  @file UART_Port.c
 *
 *  @date 2020-02-22
 *
 *  @author aron566
 *
 *  @copyright ���пƼ��о�Ժ.
 *
 *  @brief ���ڲ���
 *
 *  @details None.
 *
 *  @version V1.0
 */
#ifdef __cplusplus //use C compiler
extern "C" {
#endif
#include "UART_Port.h"/*�ⲿ�ӿ�*/
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
#define UART_MAX_NUM    6/**< �������������*/
/*�����ڴ����ӿ�*/
#if !defined(uartport_malloc) && !defined(uartport_free)
#define uartport_malloc  malloc//pvPortMalloc
#define uartport_free  free//vPortFree
#endif

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/
/*Ԥ���崮���豸��Ϣ*/
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
 * ���������ں� �����豸ָ�� dma������ַ ����ʱ�����С ���ն��д�С ����ģʽ ��ֵ�ź���
 */
/************************************************************
  * @brief   ���������豸��Ϊ�佨��˫������-->ʹ�ܴ��ڿ����ж�
  * @param   [in]uart_num ���ں�.
  * @param   [in]huart    ���ھ��.
  * @param   [in]hdma_rx  ����dma���.
  * @param   [in]rx_temp_size  ������ʱ�����С.
  * @param   [in]rxsize   ���ڽ��ջ����С.
  * @param   [in]work_mode ���ڹ���ģʽ.
  * @param   [in]pRX_Sem  ���ڽ����ź���.  
  * @return  ���ڲ������.
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
    //�򿪿����ж�
    __HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);
    //ʹ��DMA����
    HAL_UART_Receive_DMA(huart, pUart_Dev->RX_Buff_Temp, pUart_Dev->MAX_RX_Temp);
    return pUart_Dev;
}

/************************************************************
  * @brief   ��õ�ǰ������Ϣ����
  * @param   [in]Instance ���ھ��.
  * @return  ������.
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
          ����ֹͣDMA���䣬
          1.��ֹ�����������ݽ��յ����������ţ���Ϊ��ʱ�����ݻ�δ����
          2.DMA��Ҫ�������á�
          */
          HAL_UART_DMAStop(Uart_pDevice[index]->phuart);
          /*��������жϱ�־�������һֱ���Ͻ����ж�*/
          __HAL_UART_CLEAR_IDLEFLAG(Uart_pDevice[index]->phuart);
          /*���㱾�ν������ݳ���*/
          uint32_t data_length  = Uart_pDevice[index]->MAX_RX_Temp - __HAL_DMA_GET_COUNTER(Uart_pDevice[index]->phdma_rx);
          /*�����ݼ�¼��������*/
          CQ_putData(Uart_pDevice[index]->cb ,Uart_pDevice[index]->RX_Buff_Temp ,(uint32_t)data_length);
#if USE_LOOPBACK 	
          HAL_UART_Transmit(DEBUG_UART, (uint8_t *)Uart_pDevice[index]->RX_Buff_Temp,(uint16_t)data_length,0xFFFF);
#endif
          /*�����ʱ������*/
          memset(Uart_pDevice[index]->RX_Buff_Temp ,0 ,data_length);
          data_length = 0;
          /*�򿪿����ж�*/
          __HAL_UART_ENABLE_IT(Uart_pDevice[index]->phuart ,UART_IT_IDLE);
          /*������ʼDMA����*/
          HAL_UART_Receive_DMA(Uart_pDevice[index]->phuart ,Uart_pDevice[index]->RX_Buff_Temp ,Uart_pDevice[index]->MAX_RX_Temp);
        }
    }
}  

/************************************************************
  * @brief   ���ڲ��������ȡ
  * @param   [in]uart_opt_handle ���ھ��.
  * @param   [in]data ����.
  * @param   [in]size ���ݴ�С.
  * @return  true ���ͳɹ�.
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
  * @brief   ���ڲ��������ȡ
  * @param   [in]uart_num ���ں�.
  * @return  ���ھ��.
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
  * @brief   ���ڲ�����ʼ��
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

#pragma import(__use_no_semihosting) // ȷ��û�д� C ������ʹ�ð������ĺ���
struct __FILE // ��׼����Ҫ��֧�ֺ���
{
	int handle;
};
/* FILE is typedef �� d in stdio.h. */
FILE __stdout;
void _ttywrch(int ch)
{
    ch=ch;
}

void _sys_exit(int x) //���� _sys_exit() �Ա���ʹ�ð�����ģʽ
{
    (void)x;
}

#if (USE_NEW_REDIRECT == 0)
#include "stdio.h"
/************************************************************
  * @brief   �ض���c�⺯��printf��HAL_UART_Transmit
  * @param   [in]ch ����ַ�.
  * @param   [in]f �ļ�ָ��
  * @return  �ַ�
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
  * @brief   �ض���c�⺯��getchar,scanf
  * @param   [in]f �ļ�ָ��
  * @return  �ַ�
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
/*��ʽ�ض���*/
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
