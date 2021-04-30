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
#define ENABLE_DFSDM_PERIPHERAL   0/**< 选择是否启用DFSDM模块*/

/*重置接收缓冲大小*/
#if ENABLE_DFSDM_PERIPHERAL
  #ifdef PCM_ONE_SAMPLE_NUM
    #undef PCM_ONE_SAMPLE_NUM
    #define PCM_ONE_SAMPLE_NUM    MONO_FRAME_SIZE
    #undef PCM_TWO_SAMPLE_NUM
    #define PCM_TWO_SAMPLE_NUM    (PCM_ONE_SAMPLE_NUM*2)
  #endif
#endif
/** Private constants --------------------------------------------------------*/

/** Public variables ---------------------------------------------------------*/
#if ENABLE_DFSDM_PERIPHERAL
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter1;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter2;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter3;
extern PDM2PCM_BUF_Typedef_t Pdm2Pcm_ChannelBuf[MIC_CHANNEL_NUM];
#endif
/*USB Microphone数据接收缓冲区*/
extern volatile int16_t g_UACRingBuf[UAC_BUFFER_SIZE];
extern volatile uint16_t g_UACWriteIndex;
extern volatile uint16_t g_UACReadIndex;
/** Private variables --------------------------------------------------------*/
/*DFSDM数据接收标志*/
static int16_t *PCM_Data_Ptr[MIC_CHANNEL_NUM]   = {NULL};
static volatile uint32_t DFSDM_DmaCanRead_Flag  = 0;

/*4MIC数据拷贝缓冲区*/
static int16_t MIC1_Audio_Buf[PCM_ONE_SAMPLE_NUM] = {0};
static int16_t MIC2_Audio_Buf[PCM_ONE_SAMPLE_NUM] = {0};
static int16_t MIC3_Audio_Buf[PCM_ONE_SAMPLE_NUM] = {0};
static int16_t MIC4_Audio_Buf[PCM_ONE_SAMPLE_NUM] = {0};

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
#if ENABLE_DFSDM_PERIPHERAL
/**
  * @brief  Half regular conversion complete callback. 
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  DFSDM_DmaCanRead_Flag = 1;
  PCM_Data_Ptr[0] = (int16_t*)Pdm2Pcm_ChannelBuf[0].PCM_Buf;
  PCM_Data_Ptr[1] = (int16_t*)Pdm2Pcm_ChannelBuf[1].PCM_Buf;
  PCM_Data_Ptr[2] = (int16_t*)Pdm2Pcm_ChannelBuf[2].PCM_Buf;
  PCM_Data_Ptr[3] = (int16_t*)Pdm2Pcm_ChannelBuf[3].PCM_Buf;
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
  DFSDM_DmaCanRead_Flag = 1;
  PCM_Data_Ptr[0] = (int16_t*)&Pdm2Pcm_ChannelBuf[0].PCM_Buf[PCM_ONE_SAMPLE_NUM];
  PCM_Data_Ptr[1] = (int16_t*)&Pdm2Pcm_ChannelBuf[1].PCM_Buf[PCM_ONE_SAMPLE_NUM];
  PCM_Data_Ptr[2] = (int16_t*)&Pdm2Pcm_ChannelBuf[2].PCM_Buf[PCM_ONE_SAMPLE_NUM];
  PCM_Data_Ptr[3] = (int16_t*)&Pdm2Pcm_ChannelBuf[3].PCM_Buf[PCM_ONE_SAMPLE_NUM];
}
#endif
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
#if ENABLE_DFSDM_PERIPHERAL
  if(DFSDM_DmaCanRead_Flag == 0)
  {
    return;
  }
  
  memmove((void *)MIC1_Audio_Buf, (void *)PCM_Data_Ptr[0], sizeof(int16_t)*PCM_ONE_SAMPLE_NUM);	
  memmove((void *)MIC2_Audio_Buf, (void *)PCM_Data_Ptr[1], sizeof(int16_t)*PCM_ONE_SAMPLE_NUM);	
  memmove((void *)MIC3_Audio_Buf, (void *)PCM_Data_Ptr[2], sizeof(int16_t)*PCM_ONE_SAMPLE_NUM);	
  memmove((void *)MIC4_Audio_Buf, (void *)PCM_Data_Ptr[3], sizeof(int16_t)*PCM_ONE_SAMPLE_NUM);	
  
  /*发送*/
  /*更新USB音频数据*/
  for(int i = 0; i < PCM_ONE_SAMPLE_NUM; i++)
  {
    g_UACRingBuf[g_UACWriteIndex] = MIC1_Audio_Buf[i];
    g_UACWriteIndex++;
    g_UACWriteIndex = (g_UACWriteIndex >= UAC_BUFFER_SIZE)?0:g_UACWriteIndex;

    g_UACRingBuf[g_UACWriteIndex] = MIC2_Audio_Buf[i];
    g_UACWriteIndex++;

    g_UACWriteIndex = (g_UACWriteIndex >= UAC_BUFFER_SIZE)?0:g_UACWriteIndex;
  }
  DFSDM_DmaCanRead_Flag = 0;
#endif
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
  if(HAL_DFSDM_FilterRegularMsbStart_DMA(&hdfsdm1_filter0, (int16_t *)(Pdm2Pcm_ChannelBuf[0].PCM_Buf), PCM_TWO_SAMPLE_NUM) == HAL_ERROR)
  {
    Error_Handler();
  }

  /*启动通道1采样*/		
  if(HAL_DFSDM_FilterRegularMsbStart_DMA(&hdfsdm1_filter1, (int16_t *)(Pdm2Pcm_ChannelBuf[1].PCM_Buf), PCM_TWO_SAMPLE_NUM) == HAL_ERROR)
  {
    Error_Handler();
  }
  
  /*启动通道2采样*/	
  if(HAL_DFSDM_FilterRegularMsbStart_DMA(&hdfsdm1_filter2, (int16_t *)(Pdm2Pcm_ChannelBuf[2].PCM_Buf), PCM_TWO_SAMPLE_NUM) == HAL_ERROR)
  {
    Error_Handler();
  }
  
  /*启动通道3采样*/	
  if(HAL_DFSDM_FilterRegularMsbStart_DMA(&hdfsdm1_filter3, (int16_t *)(Pdm2Pcm_ChannelBuf[3].PCM_Buf), PCM_TWO_SAMPLE_NUM) == HAL_ERROR)
  {
    Error_Handler();
  }
#endif
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
