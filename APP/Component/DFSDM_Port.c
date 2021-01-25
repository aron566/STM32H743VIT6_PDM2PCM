/**
 *  @file DFSDM_Port.c
 *
 *  @date 2021-01-22
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief DFSDM操作接口：PDM转PCM
 *
 *  @details 1、通道0和通道1指数据接入，物理连接-->共用通道1口-->分离到各自滤波器转出PCM数据（滤波器0接通道0，滤波器1接通道1）
 *           2、通道2和通道3指数据接入，物理连接-->共用通道3口-->分离到各自滤波器转出PCM数据（滤波器2接通道2，滤波器3接通道3）
 *           3、通道4和通道5指数据接入，物理连接-->共用通道5口-->分离到各自滤波器转出PCM数据（无滤波器可用）
 *           4、通道6和通道7指数据接入，物理连接-->共用通道7口-->分离到各自滤波器转出PCM数据（无滤波器可用）
 *           5、采样率 = 时钟源/分频值/(Fosr*Iosr) == 16khz = 2048000hz/2/(64*1)
 *           6、偶数通道号，采集上升沿数据即SEL接GND的MIC设备
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "DFSDM_Port.h"
#include "main.h"
#include "usbd_audio.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
#define ENABLE_DFSDM_PERIPHERAL   1/**< 选择是否启用DFSDM模块*/
#define FRONT_CAN_READ            1
#define BACK_CAN_READ             2
#define NOTHING_CAN_READ          0
/** Private constants --------------------------------------------------------*/

/** Public variables ---------------------------------------------------------*/
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter1;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter2;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter3;
extern DFSDM_Channel_HandleTypeDef hdfsdm1_channel0;
extern DFSDM_Channel_HandleTypeDef hdfsdm1_channel1;
extern DFSDM_Channel_HandleTypeDef hdfsdm1_channel2;
extern DFSDM_Channel_HandleTypeDef hdfsdm1_channel3;
extern DFSDM_Channel_HandleTypeDef hdfsdm1_channel4;
extern DFSDM_Channel_HandleTypeDef hdfsdm1_channel5;
extern DFSDM_Channel_HandleTypeDef hdfsdm1_channel6;
extern DFSDM_Channel_HandleTypeDef hdfsdm1_channel7;
extern volatile PDM2PCM_BUF_Typedef_t Pdm2Pcm_ChannelBuf[MIC_CHANNEL_NUM];

extern volatile int16_t g_UACRingBuf[UAC_BUFFER_SIZE];
extern volatile uint16_t g_UACWriteIndex;
extern volatile uint16_t g_UACReadIndex;
/** Private variables --------------------------------------------------------*/

/*microphone 1 声音扎耳*/
static uint8_t DmaRecHalfBuffCplt_1  = NOTHING_CAN_READ;
static uint8_t DmaRecBuffCplt_1      = NOTHING_CAN_READ;

/*microphone 2 有尾音*/
static uint8_t DmaRecHalfBuffCplt_2  = NOTHING_CAN_READ;
static uint8_t DmaRecBuffCplt_2      = NOTHING_CAN_READ;

/*microphone 3 声音非常小*/
static uint8_t DmaRecHalfBuffCplt_3  = NOTHING_CAN_READ;
static uint8_t DmaRecBuffCplt_3      = NOTHING_CAN_READ;

/*microphone 4 声音非常小*/
static uint8_t DmaRecHalfBuffCplt_4  = NOTHING_CAN_READ;
static uint8_t DmaRecBuffCplt_4      = NOTHING_CAN_READ;

/*microphone 5 声音非常小*/

/*microphone 78 没有输出*/

static volatile uint8_t DmaCanRead_Flag        = NOTHING_CAN_READ;
static volatile uint8_t DmaRecHalfBuffCplt_Num = NOTHING_CAN_READ;
static volatile uint8_t DmaRecBuffCplt_Num     = NOTHING_CAN_READ;
/** Private function prototypes ----------------------------------------------*/

/** Private user code --------------------------------------------------------*/

/** Private application code -------------------------------------------------*/
/*******************************************************************************
*
*       Static code
*
********************************************************************************
*/

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*
*       Public code
*
********************************************************************************
*/
/**
  * @brief  Half regular conversion complete callback. 
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  if(hdfsdm_filter == &hdfsdm1_filter0)
  {
    DmaRecHalfBuffCplt_Num = 1;
    DmaRecHalfBuffCplt_1 = 1;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&Pdm2Pcm_ChannelBuf[0].PCM_Buf[0], PCM_TWO_SAMPLE_NUM);
  }
  else if(hdfsdm_filter == &hdfsdm1_filter1)
  {
    DmaRecHalfBuffCplt_Num = 2;
    DmaRecHalfBuffCplt_2 = 1;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&Pdm2Pcm_ChannelBuf[1].PCM_Buf[0], PCM_TWO_SAMPLE_NUM);
  }
  else if(hdfsdm_filter == &hdfsdm1_filter2)
  {
    DmaRecHalfBuffCplt_Num = 3;
    DmaRecHalfBuffCplt_3 = 1;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&Pdm2Pcm_ChannelBuf[2].PCM_Buf[0], PCM_TWO_SAMPLE_NUM);
  }
  else if(hdfsdm_filter == &hdfsdm1_filter3)
  {
    DmaRecHalfBuffCplt_Num = 4;
    DmaRecHalfBuffCplt_4 = 4;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&Pdm2Pcm_ChannelBuf[3].PCM_Buf[0], PCM_TWO_SAMPLE_NUM);
  }
  DmaCanRead_Flag = FRONT_CAN_READ;
}

/**
  * @brief  Regular conversion complete callback. 
  * @note   In interrupt mode, user has to read conversion value in this function
            using HAL_DFSDM_FilterGetRegularValue.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  if(hdfsdm_filter == &hdfsdm1_filter0)
  {
    DmaRecBuffCplt_Num = 1;
    DmaRecBuffCplt_1 = 1;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&Pdm2Pcm_ChannelBuf[0].PCM_Buf[PCM_ONE_SAMPLE_NUM], PCM_TWO_SAMPLE_NUM);
  }
  else if(hdfsdm_filter == &hdfsdm1_filter1)
  {
    DmaRecBuffCplt_Num = 2;
    DmaRecBuffCplt_2 = 1;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&Pdm2Pcm_ChannelBuf[1].PCM_Buf[PCM_ONE_SAMPLE_NUM], PCM_TWO_SAMPLE_NUM);
  }
  else if(hdfsdm_filter == &hdfsdm1_filter2)
  {
    DmaRecBuffCplt_Num = 3;
    DmaRecBuffCplt_3 = 1;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&Pdm2Pcm_ChannelBuf[2].PCM_Buf[PCM_ONE_SAMPLE_NUM], PCM_TWO_SAMPLE_NUM);
  }
  else if(hdfsdm_filter == &hdfsdm1_filter3)
  {
    DmaRecBuffCplt_Num = 4;
    DmaRecBuffCplt_4 = 1;
    /* Invalidate Data Cache to get the updated content of the SRAM*/
    SCB_InvalidateDCache_by_Addr((uint32_t*)&Pdm2Pcm_ChannelBuf[3].PCM_Buf[PCM_ONE_SAMPLE_NUM], PCM_TWO_SAMPLE_NUM);
  }
  DmaCanRead_Flag = BACK_CAN_READ;
}

/**
  ******************************************************************
  * @brief   DFSDM启动PDM转换
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-22
  ******************************************************************
  */
void DFSDM_Port_Start(void)
{
  if(DmaCanRead_Flag == NOTHING_CAN_READ)
  {
    return;
  }

  int16_t *PCM_Data_Ptr = NULL;
  if(DmaCanRead_Flag == FRONT_CAN_READ)
  {
    if(DmaRecBuffCplt_Num == 1)
    {
      PCM_Data_Ptr = (DmaRecHalfBuffCplt_Num == 1)?(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf:(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf+PCM_ONE_SAMPLE_NUM;
    }
    if(DmaRecBuffCplt_Num == 2)
    {
      PCM_Data_Ptr = (DmaRecHalfBuffCplt_Num == 2)?(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf:(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf+PCM_ONE_SAMPLE_NUM;
    }
    if(DmaRecBuffCplt_Num == 3)
    {
      PCM_Data_Ptr = (DmaRecHalfBuffCplt_Num == 3)?(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf:(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf+PCM_ONE_SAMPLE_NUM;
    }
    if(DmaRecBuffCplt_Num == 4)
    {
      PCM_Data_Ptr = (DmaRecHalfBuffCplt_Num == 4)?(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf:(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf+PCM_ONE_SAMPLE_NUM;
    }
  }
  else if(DmaCanRead_Flag == BACK_CAN_READ)
  {
    if(DmaRecBuffCplt_Num == 1)
    {
      PCM_Data_Ptr = (DmaRecHalfBuffCplt_Num == 1)?(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf:(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf+PCM_ONE_SAMPLE_NUM;
    }
    if(DmaRecBuffCplt_Num == 2)
    {
      PCM_Data_Ptr = (DmaRecHalfBuffCplt_Num == 2)?(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf:(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf+PCM_ONE_SAMPLE_NUM;
    }
    if(DmaRecBuffCplt_Num == 3)
    {
      PCM_Data_Ptr = (DmaRecHalfBuffCplt_Num == 3)?(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf:(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf+PCM_ONE_SAMPLE_NUM;
    }
    if(DmaRecBuffCplt_Num == 4)
    {
      PCM_Data_Ptr = (DmaRecHalfBuffCplt_Num == 4)?(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf:(int16_t *)Pdm2Pcm_ChannelBuf[DmaRecBuffCplt_Num-1].PCM_Buf+PCM_ONE_SAMPLE_NUM;
    }
  }

  /*发送*/
  /*更新USB音频数据*/
  for(int i = 0; i < PCM_ONE_SAMPLE_NUM; i++)
  {
    g_UACRingBuf[g_UACWriteIndex] = PCM_Data_Ptr[i];
    g_UACWriteIndex++;
    g_UACWriteIndex = (g_UACWriteIndex >= UAC_BUFFER_SIZE)?0:g_UACWriteIndex;

    g_UACRingBuf[g_UACWriteIndex] = PCM_Data_Ptr[i];
    g_UACWriteIndex++;

    g_UACWriteIndex = (g_UACWriteIndex >= UAC_BUFFER_SIZE)?0:g_UACWriteIndex;
  }
}

/**
  ******************************************************************
  * @brief   DFSDM初始化
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-01-22
  ******************************************************************
  */
void DFSDM_Port_Init(void)
{
#if ENABLE_DFSDM_PERIPHERAL
	/*启动通道0采样*/	
//  if(HAL_DFSDM_FilterRegularMsbStart_DMA(&hdfsdm1_filter0, (int16_t *)Pdm2Pcm_ChannelBuf[0].PCM_Buf, PCM_TWO_SAMPLE_NUM) == HAL_ERROR)
//  {
//    Error_Handler();
//  }

  /*启动通道1采样*/		
  if(HAL_DFSDM_FilterRegularMsbStart_DMA(&hdfsdm1_filter1, (int16_t *)(Pdm2Pcm_ChannelBuf[1].PCM_Buf), PCM_ONE_SAMPLE_NUM) == HAL_ERROR)
  {
    Error_Handler();
  }
  
  /*启动通道2采样*/	
//  if(HAL_DFSDM_FilterRegularMsbStart_DMA(&hdfsdm1_filter2, (int16_t *)(Pdm2Pcm_ChannelBuf[2].PCM_Buf), PCM_ONE_SAMPLE_NUM) == HAL_ERROR)
//  {
//    Error_Handler();
//  }
  
  /*启动通道3采样*/	
//  if(HAL_DFSDM_FilterRegularMsbStart_DMA(&hdfsdm1_filter3, (int16_t *)(Pdm2Pcm_ChannelBuf[3].PCM_Buf), PCM_ONE_SAMPLE_NUM) == HAL_ERROR)
//  {
//    Error_Handler();
//  }
#endif
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
