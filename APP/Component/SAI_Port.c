/**
 *  @file SAI_Port.c
 *
 *  @date 2021-01-01
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief sai操作接口-MIC
 *
 *  @details 1、SAI-D1接收来自MC1、MC2通道PDM数据，MC1数据先
 *           2、SAI-D2接收来自MC3、MC4通道PDM数据，MC3数据先
 *           3、SAI-D3接收来自MC5、MC6通道PDM数据，MC5数据先
 *           4、SAI-D4接收来自MC7、MC8通道PDM数据，MC7数据先
 *           5、[MC1 16bit][MC2 16bit][MC3 16bit][MC4 16bit][MC5 16bit][MC6 16bit][MC7 16bit][MC8 16bit]
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "SAI_Port.h"
#include "usbd_audio.h"
/** Private typedef ----------------------------------------------------------*/
/** @defgroup SAI_Private_Typedefs  SAI Private Typedefs
  * @{
  */
typedef enum
{
  SAI_MODE_DMA,
  SAI_MODE_IT
} SAI_ModeTypedef;
/**
  * @}
  */
/** Private macros -----------------------------------------------------------*/
#define ENABLE_SAI_PERIPHERAL   1/**< 选择是否启用SAI模块*/

/*重置接收缓冲大小*/
#if ENABLE_SAI_PERIPHERAL
  #ifdef PCM_ONE_SAMPLE_NUM
    #undef PCM_ONE_SAMPLE_NUM
    #define PCM_ONE_SAMPLE_NUM    STEREO_FRAME_SIZE
    #undef PCM_TWO_SAMPLE_NUM
    #define PCM_TWO_SAMPLE_NUM    (PCM_ONE_SAMPLE_NUM*2)
  #endif
#endif

#define SAI_DEFAULT_TIMEOUT      4U
#define SAI_LONG_TIMEOUT         1000U
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;
extern SAI_HandleTypeDef hsai_BlockA2;
extern SAI_HandleTypeDef hsai_BlockB2;

/*USB Microphone数据接收缓冲区*/
extern volatile int16_t g_UACRingBuf[UAC_BUFFER_SIZE];
extern volatile uint16_t g_UACWriteIndex;
extern volatile uint16_t g_UACReadIndex;

/*音频转换数据接收缓冲区*/
//__attribute__ ((at(0x38000000))) volatile PDM2PCM_BUF_Typedef_t Pdm2Pcm_ChannelBuf[MIC_CHANNEL_NUM] = {0};
/*音频PCM数据接收缓冲区*/
__attribute__ ((at(0x38000000))) volatile int16_t PCM_ChannelBuf[MIC_CHANNEL_NUM][PCM_TWO_SAMPLE_NUM] = {0};

/** Private variables --------------------------------------------------------*/
static int16_t *PCM_Data_Ptr[MIC_CHANNEL_NUM]   = {NULL};
static volatile uint32_t SAI_DmaCanRead_Flag  = 0;

static int16_t MIC1_2_STEREO_Aidio_TempBuf[PCM_ONE_SAMPLE_NUM] = {0};
static int16_t MIC3_4_STEREO_Aidio_TempBuf[PCM_ONE_SAMPLE_NUM] = {0};
static int16_t MIC5_6_STEREO_Aidio_TempBuf[PCM_ONE_SAMPLE_NUM] = {0};
static int16_t MIC7_8_STEREO_Aidio_TempBuf[PCM_ONE_SAMPLE_NUM] = {0};
/** Private function prototypes ----------------------------------------------*/
/*中断回调*/
static void HAL_SAI_TxBuf0CpltCallback(DMA_HandleTypeDef *hdma);
static void HAL_SAI_TxBuf1CpltCallback(DMA_HandleTypeDef *hdma);
static void HAL_SAI_RxBuf0CpltCallback(DMA_HandleTypeDef *hdma);
static void HAL_SAI_RxBuf1CpltCallback(DMA_HandleTypeDef *hdma);

static uint32_t SAI_InterruptFlag(const SAI_HandleTypeDef *hsai, SAI_ModeTypedef mode);
/** Private user code --------------------------------------------------------*/

/** Private application code -------------------------------------------------*/
/*******************************************************************************
*
*       Static code
*
********************************************************************************
*/

/**
  ******************************************************************
  * @brief   SAI_TX_Buf_0发送完成中断
  * @param   [in]hdma dma句柄
  * @return  None.
  * @author  zgl
  * @version V1.0
  * @date    2021-01-07
  ******************************************************************
  */
static void HAL_SAI_TxBuf0CpltCallback(DMA_HandleTypeDef *hdma)
{
  UNUSED(hdma);
}

/**
  ******************************************************************
  * @brief   SAI_TX_Buf_1发送完成中断
  * @param   [in]hdma dma句柄
  * @return  None.
  * @author  zgl
  * @version V1.0
  * @date    2021-01-07
  ******************************************************************
  */
static void HAL_SAI_TxBuf1CpltCallback(DMA_HandleTypeDef *hdma)
{
  UNUSED(hdma);
}

/**
  ******************************************************************
  * @brief   SAI_RX_Buf_0接收完成中断
  * @param   [in]hdma dma句柄
  * @return  None.
  * @author  zgl
  * @version V1.0
  * @date    2021-01-07
  ******************************************************************
  */
static void HAL_SAI_RxBuf0CpltCallback(DMA_HandleTypeDef *hdma)
{
  SAI_DmaCanRead_Flag = 1;
}

/**
  ******************************************************************
  * @brief   SAI_RX_Buf_1接收完成中断
  * @param   [in]hdma dma句柄
  * @return  None.
  * @author  zgl
  * @version V1.0
  * @date    2021-01-07
  ******************************************************************
  */
static void HAL_SAI_RxBuf1CpltCallback(DMA_HandleTypeDef *hdma)
{
  SAI_DmaCanRead_Flag = 1;
}

/**
  ******************************************************************
  * @brief   SAI中断标识
  * @param   [in]None
  * @return  HAL_StatusTypeDef
  * @author  zgl
  * @version V1.0
  * @date    2020-01-01
  ******************************************************************
  */
static uint32_t SAI_InterruptFlag(const SAI_HandleTypeDef *hsai, SAI_ModeTypedef mode)
{
    uint32_t tmpIT = SAI_IT_OVRUDR;

    if (mode == SAI_MODE_IT)
    {
        tmpIT |= SAI_IT_FREQ;
    }

    if ((hsai->Init.Protocol == SAI_AC97_PROTOCOL) &&
            ((hsai->Init.AudioMode == SAI_MODESLAVE_RX) || (hsai->Init.AudioMode == SAI_MODEMASTER_RX)))
    {
        tmpIT |= SAI_IT_CNRDY;
    }

    if ((hsai->Init.AudioMode == SAI_MODESLAVE_RX) || (hsai->Init.AudioMode == SAI_MODESLAVE_TX))
    {
        tmpIT |= SAI_IT_AFSDET | SAI_IT_LFSDET;
    }
    else
    {
        /* hsai has been configured in master mode */
        tmpIT |= SAI_IT_WCKCFG;
    }
    return tmpIT;
}

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*
*       Public code
*
********************************************************************************
*/

/**
  ******************************************************************
  * @brief   SAI双缓冲发送
  * @param   [in]hsai sai句柄
  * @param   [in]pData DMA发送数据地址1
  * @param   [in]SecondMemAddress DMA发送数据地址2
  * @param   [in]Size 发送数据大小
  * @return  HAL_StatusTypeDef
  * @author  zgl
  * @version V1.0
  * @date    2021-01-07
  ******************************************************************
  */
HAL_StatusTypeDef HAL_SAI_MultiMemTransmit_DMA(SAI_HandleTypeDef *hsai, uint8_t *pData, uint8_t *SecondMemAddress, uint16_t Size)
{
    uint32_t tickstart = HAL_GetTick();
  
    if ((pData == NULL) || (Size == 0U))
    {
        return  HAL_ERROR;
    }

    if (hsai->State == HAL_SAI_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hsai);
      
        hsai->pBuffPtr = pData;
        hsai->XferSize = Size;
        hsai->XferCount = Size;
        hsai->ErrorCode = HAL_SAI_ERROR_NONE;
        hsai->State = HAL_SAI_STATE_BUSY_TX;

        /* Set the SAI Tx DMA Half transfer complete callback */
        hsai->hdmatx->XferHalfCpltCallback = NULL;

        /* Set the SAI TxDMA transfer complete callback */
        hsai->hdmatx->XferCpltCallback = HAL_SAI_TxBuf0CpltCallback;

		    hsai->hdmatx->XferM1CpltCallback = HAL_SAI_TxBuf1CpltCallback;
        /* Set the DMA error callback */
        hsai->hdmatx->XferErrorCallback = NULL;

        /* Set the DMA Tx abort callback */
        hsai->hdmatx->XferAbortCallback = NULL;

        /* Enable the Tx DMA Stream */
        if (HAL_DMAEx_MultiBufferStart_IT(hsai->hdmatx, (uint32_t)hsai->pBuffPtr, (uint32_t)&hsai->Instance->DR, (uint32_t)SecondMemAddress, hsai->XferSize) != HAL_OK)
        {
            __HAL_UNLOCK(hsai);
            return  HAL_ERROR;
        }

        /* Enable the interrupts for error handling */
        __HAL_SAI_ENABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_DMA));

        /* Enable SAI Tx DMA Request */
        hsai->Instance->CR1 |= SAI_xCR1_DMAEN;

        /* Wait untill FIFO is not empty */
        while ((hsai->Instance->SR & SAI_xSR_FLVL) == SAI_FIFOSTATUS_EMPTY)
        {
            /* Check for the Timeout */
            if ((HAL_GetTick() - tickstart) > SAI_LONG_TIMEOUT)
            {
                /* Update error code */
                hsai->ErrorCode |= HAL_SAI_ERROR_TIMEOUT;

                /* Process Unlocked */
                __HAL_UNLOCK(hsai);

                return HAL_TIMEOUT;
            }
        }

        /* Check if the SAI is already enabled */
        if ((hsai->Instance->CR1 & SAI_xCR1_SAIEN) == 0U)
        {
            /* Enable SAI peripheral */
            __HAL_SAI_ENABLE(hsai);
        }

        /* Process Unlocked */
        __HAL_UNLOCK(hsai);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  ******************************************************************
  * @brief   SAI双缓冲接收
  * @param   [in]hsai sai句柄
  * @param   [in]pData DMA接收地址1
  * @param   [in]SecondMemAddress DMA接收地址2
  * @param   [in]Size 接收大小
  * @return  HAL_StatusTypeDef
  * @author  zgl
  * @version V1.0
  * @date    2021-01-07
  ******************************************************************
  */
HAL_StatusTypeDef HAL_SAI_MultiMemReceive_DMA(SAI_HandleTypeDef *hsai, uint8_t *pData, uint8_t *SecondMemAddress, uint16_t Size)
{

    if ((pData == NULL) || (Size == 0U))
    {
        return  HAL_ERROR;
    }

    if (hsai->State == HAL_SAI_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hsai);

        hsai->pBuffPtr = pData;
        hsai->XferSize = Size;
        hsai->XferCount = Size;
        hsai->ErrorCode = HAL_SAI_ERROR_NONE;
        hsai->State = HAL_SAI_STATE_BUSY_RX;

        /* Set the SAI Rx DMA Half transfer complete callback */
        hsai->hdmarx->XferHalfCpltCallback = NULL;

        /* Set the SAI Rx DMA transfer complete callback */
        hsai->hdmarx->XferCpltCallback = HAL_SAI_RxBuf0CpltCallback;
		    hsai->hdmarx->XferM1CpltCallback = HAL_SAI_RxBuf1CpltCallback;
        /* Set the DMA error callback */
        hsai->hdmarx->XferErrorCallback = NULL;

        /* Set the DMA Rx abort callback */
        hsai->hdmarx->XferAbortCallback = NULL;

        /* Enable the Rx DMA Stream */
        if (HAL_DMAEx_MultiBufferStart_IT(hsai->hdmatx, (uint32_t)&hsai->Instance->DR, (uint32_t)hsai->pBuffPtr, (uint32_t)SecondMemAddress, hsai->XferSize) != HAL_OK)
        {
            __HAL_UNLOCK(hsai);
            return  HAL_ERROR;
        }

        /* Enable the interrupts for error handling */
        __HAL_SAI_ENABLE_IT(hsai, SAI_InterruptFlag(hsai, SAI_MODE_DMA));

        /* Enable SAI Rx DMA Request */
        hsai->Instance->CR1 |= SAI_xCR1_DMAEN;

        /* Check if the SAI is already enabled */
        if ((hsai->Instance->CR1 & SAI_xCR1_SAIEN) == 0U)
        {
            /* Enable SAI peripheral */
            __HAL_SAI_ENABLE(hsai);
        }

        /* Process Unlocked */
        __HAL_UNLOCK(hsai);

        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  ******************************************************************
  * @brief   强定义SAI半接收完成中断
  * @param   [in]hsai sai句柄
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-19
  ******************************************************************
  */
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  SAI_DmaCanRead_Flag = 1;
  PCM_Data_Ptr[0] = (int16_t*)PCM_ChannelBuf[0];
  PCM_Data_Ptr[1] = (int16_t*)PCM_ChannelBuf[1];
  PCM_Data_Ptr[2] = (int16_t*)PCM_ChannelBuf[2];
  PCM_Data_Ptr[3] = (int16_t*)PCM_ChannelBuf[3];
}

/**
  ******************************************************************
  * @brief   强定义SAI接收完成中断
  * @param   [in]hsai sai句柄
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-19
  ******************************************************************
  */
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
  SAI_DmaCanRead_Flag = 1;
  PCM_Data_Ptr[0] = (int16_t*)&PCM_ChannelBuf[0][PCM_ONE_SAMPLE_NUM];
  PCM_Data_Ptr[1] = (int16_t*)&PCM_ChannelBuf[1][PCM_ONE_SAMPLE_NUM];
  PCM_Data_Ptr[2] = (int16_t*)&PCM_ChannelBuf[2][PCM_ONE_SAMPLE_NUM];
  PCM_Data_Ptr[3] = (int16_t*)&PCM_ChannelBuf[3][PCM_ONE_SAMPLE_NUM];
}

/**
  ******************************************************************
  * @brief   SAI发送数据
  * @param   [in]data 数据
  * @param   [in]Size 以在CubeMX上配置的单位数量，配置半字，这里写1就传输1个半字
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-01
  ******************************************************************
  */
void Sai_Port_Send_Data(uint8_t *data, uint16_t size)
{
  UNUSED(data);
  UNUSED(size);
}

/**
  ******************************************************************
  * @brief   SAI启动数据处理
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-27
  ******************************************************************
  */
void Sai_Port_Start(void)
{
  if(SAI_DmaCanRead_Flag == 0)
  {
    return; 
  }

  memmove(MIC1_2_STEREO_Aidio_TempBuf, PCM_Data_Ptr[0], sizeof(int16_t)*PCM_ONE_SAMPLE_NUM);
  memmove(MIC3_4_STEREO_Aidio_TempBuf, PCM_Data_Ptr[1], sizeof(int16_t)*PCM_ONE_SAMPLE_NUM);
  memmove(MIC5_6_STEREO_Aidio_TempBuf, PCM_Data_Ptr[2], sizeof(int16_t)*PCM_ONE_SAMPLE_NUM);
  memmove(MIC7_8_STEREO_Aidio_TempBuf, PCM_Data_Ptr[3], sizeof(int16_t)*PCM_ONE_SAMPLE_NUM);
  
  /*分离通道数据：L-R-L-R-L-R......*/
//  for(int i = 0; i < MONO_FRAME_SIZE; i++)
//  {
//    MIC7_Aidio_TempBuf[i] = STEREO_Aidio_TempBuf[i*2];
//    MIC8_Aidio_TempBuf[i] = STEREO_Aidio_TempBuf[i*2+1];
//  }

  /*发送*/
  /*更新USB音频数据*/
  for(int i = 0; i < MONO_FRAME_SIZE; i++)
  {
    g_UACRingBuf[g_UACWriteIndex] = MIC3_4_STEREO_Aidio_TempBuf[i*2];
    g_UACWriteIndex++;
    g_UACWriteIndex = (g_UACWriteIndex >= UAC_BUFFER_SIZE)?0:g_UACWriteIndex;

    g_UACRingBuf[g_UACWriteIndex] = MIC3_4_STEREO_Aidio_TempBuf[i*2+1];
    g_UACWriteIndex++;

    g_UACWriteIndex = (g_UACWriteIndex >= UAC_BUFFER_SIZE)?0:g_UACWriteIndex;
  }
  SAI_DmaCanRead_Flag = 0;
}

/**
  ******************************************************************
  * @brief   SAI操作初始化
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-01
  ******************************************************************
  */
void Sai_Port_Init(void)
{
#if ENABLE_SAI_PERIPHERAL
  /*启动SAI3 DMA 双缓冲接收,双声道大小*/
  //HAL_SAI_MultiMemReceive_DMA(&hsai_BlockA1, (uint8_t*)Pdm2Pcm_ChannelBuf[0].PCM_Buf, (uint8_t *)((int16_t *)(Pdm2Pcm_ChannelBuf[0].PCM_Buf+PCM_ONE_SAMPLE_NUM)), sizeof(int16_t)*PCM_ONE_SAMPLE_NUM);

  /*启动SAI DMA数据接收*/
  HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t*)PCM_ChannelBuf[0], PCM_TWO_SAMPLE_NUM);
  HAL_SAI_Receive_DMA(&hsai_BlockB1, (uint8_t*)PCM_ChannelBuf[1], PCM_TWO_SAMPLE_NUM);
//  HAL_SAI_Receive_DMA(&hsai_BlockA2, (uint8_t*)PCM_ChannelBuf[2], PCM_TWO_SAMPLE_NUM);
  HAL_SAI_Receive_DMA(&hsai_BlockB2, (uint8_t*)PCM_ChannelBuf[3], PCM_TWO_SAMPLE_NUM);
#endif
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
