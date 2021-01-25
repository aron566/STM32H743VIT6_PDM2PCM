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
#define SAI_DEFAULT_TIMEOUT      4U
#define SAI_LONG_TIMEOUT         1000U
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockA4;
extern DMA_HandleTypeDef hdma_sai1_a;
extern DMA_HandleTypeDef hdma_sai4_a;
/* Flag定义 */
volatile uint8_t SAI_Transmit_Complete_Flag  = 0;
volatile uint8_t SAI_Receive_Complete_Flag   = 0;
volatile uint8_t SAI_Can_Send_Data_Flag      = 1;
volatile uint8_t SAI_Can_Read_Data_Flag      = 0;
/** Private variables --------------------------------------------------------*/
///* 接收缓冲区 2xFrameLength */
//__attribute__ ((at(0x38001000))) volatile uint16_t SAI_RX_Buf_0[STEREO_FRAME_SIZE] = {0};
//__attribute__ ((at(0x38002000))) volatile uint16_t SAI_RX_Buf_1[STEREO_FRAME_SIZE] = {0};

///*PDM数据接收缓冲区*/
//__attribute__ ((at(0x38000000))) volatile uint16_t SAI_RX_Buf_1_6[STEREO_FRAME_SIZE] = {0};
//__attribute__ ((at(0x38003000+MONO_FRAME_SIZE*7))) volatile uint16_t SAI_RX_Buf_7_8[STEREO_FRAME_SIZE] = {0};

//__attribute__ ((at(0x24001000))) volatile PDM2PCM_BUF_Typedef_t Pdm2Pcm_ChannelBuf[MIC_CHANNEL_NUM] = {0};
__attribute__ ((at(0x38000000))) volatile PDM2PCM_BUF_Typedef_t Pdm2Pcm_ChannelBuf[MIC_CHANNEL_NUM] = {0};
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
	SAI_Can_Send_Data_Flag = 1;
	SAI_Transmit_Complete_Flag = 0;
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
	SAI_Can_Send_Data_Flag = 1;
	SAI_Transmit_Complete_Flag = 1;
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
  SAI_Can_Read_Data_Flag = 1;
  SAI_Receive_Complete_Flag = 0;
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
  SAI_Can_Read_Data_Flag = 1;
  SAI_Receive_Complete_Flag = 1;
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
  * @brief   SAI半接收完成中断
  * @param   [in]hsai sai句柄
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-19
  ******************************************************************
  */
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  UNUSED(hsai);
}

/**
  ******************************************************************
  * @brief   SAI接收完成中断
  * @param   [in]hsai sai句柄
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-19
  ******************************************************************
  */
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
  UNUSED(hsai);
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
  /*启动SAI3 DMA 双缓冲接收,双声道大小*/
  //HAL_SAI_MultiMemReceive_DMA(&hsai_BlockA1, (uint8_t*)Pdm2Pcm_ChannelBuf[0].PDM_RX_Buf, (uint8_t *)((uint16_t *)(Pdm2Pcm_ChannelBuf[0].PDM_RX_Buf+64)), 64*2);
}

/**
  ******************************************************************
  * @brief   SAI发送数据
  * @param   [in]data 数据
  * @param   [in]Size 配置时传输大小为单位的传输数量
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-01
  ******************************************************************
  */
void Sai_Port_Send_Data(uint8_t *data, uint16_t size)
{
//  HAL_SAI_Transmit_DMA(&hsai_BlockB3, data, size);
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
