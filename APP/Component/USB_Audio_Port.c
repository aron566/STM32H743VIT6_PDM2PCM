/**
 *  @file USB_Audio_Port.c
 *
 *  @date 2021-01-08
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief USB音频操作接口
 *
 *  @details 1、
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "USB_Audio_Port.h"
#include "main.h"
#include "usbd_audio_if.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/
#define CHANNEL_DEMUX_MASK    0x55
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
USBD_HandleTypeDef hUSBDDevice;
extern USBD_AUDIO_ItfTypeDef  USBD_AUDIO_fops;
extern USBD_DescriptorsTypeDef AUDIO_Desc;
extern I2S_HandleTypeDef hi2s1;
/** Private variables --------------------------------------------------------*/
static PDM2PCM_HandlerTypeDef PDM2PCM_Handler;

static uint16_t I2S_InternalBuffer[PDM_INTERNAL_BUFFER_SIZE];

static uint16_t AudioInVolume = 64;

uint8_t Channel_Demux[128] = {
  0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
  0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
  0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
  0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
  0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
  0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x02, 0x03,
  0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
  0x04, 0x05, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07,
  0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
  0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
  0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f,
  0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f,
  0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
  0x08, 0x09, 0x08, 0x09, 0x0a, 0x0b, 0x0a, 0x0b,
  0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f,
  0x0c, 0x0d, 0x0c, 0x0d, 0x0e, 0x0f, 0x0e, 0x0f
};
/** Private function prototypes ----------------------------------------------*/
static uint8_t AUDIO_IN_Timer_Init(void);
static uint8_t AUDIO_IN_Timer_Start(void);
static uint8_t AUDIO_IN_Timer_Stop(void);
static void AUDIO_IN_I2S_MspInit(void);
static void PDMDecoder_Init(uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut);
/** Private user code --------------------------------------------------------*/

/** Private application code -------------------------------------------------*/
/*******************************************************************************
*
*       Static code
*
********************************************************************************
*/
/**
* @brief  Initialize the PDM library.
* @param  AudioFreq: Audio sampling frequency
* @param  ChnlNbrIn: Number of input audio channels in the PDM buffer
* @param  ChnlNbrOut: Number of desired output audio channels in the  resulting PCM buffer
* @retval None
*/
static void PDMDecoder_Init(uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut)
{
//  uint32_t i = 0;
//  /* Enable CRC peripheral to unlock the PDM library */
//  __CRC_CLK_ENABLE();
//  
//  for(i = 0; i < ChnlNbrIn; i++)
//  {
//    /* Filter LP and HP Init */
//    Filter[i].LP_HZ = AudioFreq / 2;
//    Filter[i].HP_HZ = 10;
//    Filter[i].Fs = AudioFreq;
//    Filter[i].Out_MicChannels = ChnlNbrOut;
//    Filter[i].In_MicChannels = ChnlNbrIn;
//    PDM_Filter_Init((PDMFilter_InitStruct *)&Filter[i]);
//  }
}
/**
* @brief AUDIO IN I2S MSP Init
* @param None
* @retval None
*/
static void AUDIO_IN_I2S_MspInit(void)
{
//  static DMA_HandleTypeDef hdma_i2sRx;
//  GPIO_InitTypeDef  GPIO_InitStruct;
//  I2S_HandleTypeDef *hi2s = &hAudioInI2s;
//  
//  /* Enable the I2S2 peripheral clock */
//  AUDIO_IN_I2S_CLK_ENABLE();
//  
//  /* Enable I2S GPIO clocks */
//  
//  AUDIO_IN_I2S_SCK_GPIO_CLK_ENABLE();  
//  /* I2S2 pins configuration: SCK and MOSI pins ------------------------------*/
//  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull      = GPIO_NOPULL;
//  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;  
//  
//  GPIO_InitStruct.Pin       = AUDIO_IN_I2S_SCK_PIN; 
//  GPIO_InitStruct.Alternate = AUDIO_IN_I2S_SCK_AF;
//  HAL_GPIO_Init(AUDIO_IN_I2S_SCK_GPIO_PORT, &GPIO_InitStruct);
//  
//  GPIO_InitStruct.Pin       = AUDIO_IN_I2S_MOSI_PIN ;
//  GPIO_InitStruct.Alternate = AUDIO_IN_I2S_MOSI_AF;
//  HAL_GPIO_Init(AUDIO_IN_I2S_MOSI_GPIO_PORT, &GPIO_InitStruct); 
//  
//  /* Enable the DMA clock */
//  AUDIO_IN_I2S_DMAx_CLK_ENABLE();
//  
//  hdma_i2sRx.Init.Request = DMA_REQUEST_2;
//  hdma_i2sRx.Init.Direction = DMA_PERIPH_TO_MEMORY;                     /* M2M transfer mode                */
//  hdma_i2sRx.Init.PeriphInc = DMA_PINC_DISABLE;                         /* Peripheral increment mode Enable */
//  hdma_i2sRx.Init.MemInc = DMA_MINC_ENABLE;                             /* Memory increment mode Enable     */
//  hdma_i2sRx.Init.PeriphDataAlignment = AUDIO_IN_I2S_DMAx_PERIPH_DATA_SIZE;     /* Peripheral data alignment : Word */
//  hdma_i2sRx.Init.MemDataAlignment = AUDIO_IN_I2S_DMAx_MEM_DATA_SIZE;           /* memory data alignment : Word     */
//  hdma_i2sRx.Init.Mode = DMA_CIRCULAR;                         			/* Normal DMA mode                  */
//  hdma_i2sRx.Init.Priority = DMA_PRIORITY_HIGH;              			/* priority level : high            */
//  
//  if(hi2s->Instance == AUDIO_IN_I2S_INSTANCE)
//  {
//    
//    
//    hdma_i2sRx.Instance = AUDIO_IN_I2S_DMAx_INSTANCE;
//    /* Deinitialize the Stream for new transfer */
//    HAL_DMA_DeInit(&hdma_i2sRx);
//    
//    /* Associate the DMA handle */
//    __HAL_LINKDMA(hi2s, hdmarx, hdma_i2sRx);
//    
//    
//    
//    /* Configure the DMA Stream */
//    HAL_DMA_Init(&hdma_i2sRx);      
//  }
//  
//  /* I2S DMA IRQ Channel configuration */
//  HAL_NVIC_SetPriority(AUDIO_IN_I2S_DMAx_IRQ, 0, 0);
//  HAL_NVIC_EnableIRQ(AUDIO_IN_I2S_DMAx_IRQ); 
}


/**
* @brief Audio Timer Init
* @param None
* @retval None
*/
static uint8_t AUDIO_IN_Timer_Init(void)
{  
//  GPIO_InitTypeDef   GPIO_InitStruct;
//  
//  /* Enable AUDIO_TIMER clock*/
//  AUDIO_IN_TIMER_CLK_ENABLE(); 
//  AUDIO_IN_TIMER_CHOUT_GPIO_PORT_CLK_ENABLE();
//  AUDIO_IN_TIMER_CHIN_GPIO_PORT_CLK_ENABLE();
//  
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
//  
//  GPIO_InitStruct.Alternate = AUDIO_IN_TIMER_CHIN_AF;
//  GPIO_InitStruct.Pin = AUDIO_IN_TIMER_CHIN_PIN;
//  HAL_GPIO_Init(AUDIO_IN_TIMER_CHIN_GPIO_PORT, &GPIO_InitStruct);
//  
//  GPIO_InitStruct.Alternate = AUDIO_IN_TIMER_CHOUT_AF;
//  GPIO_InitStruct.Pin = AUDIO_IN_TIMER_CHOUT_PIN;
//  HAL_GPIO_Init(AUDIO_IN_TIMER_CHOUT_GPIO_PORT, &GPIO_InitStruct);  
//  
//  TimDividerHandle.Instance = AUDIO_IN_TIMER;  
//  
//  /* Configure the Input: channel_1 */
//  sICConfig.ICPolarity  = TIM_ICPOLARITY_RISING;
//  sICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
//  sICConfig.ICPrescaler = TIM_ICPSC_DIV1;
//  sICConfig.ICFilter = 0;  
//  if(HAL_TIM_IC_ConfigChannel(&TimDividerHandle, &sICConfig, TIM_CHANNEL_1) != HAL_OK)
//  {
//    return AUDIO_ERROR;
//  }
//  
//  /* Configure TIM1 in Gated Slave mode for the external trigger (Filtered Timer
//  Input 1) */
//  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
//  sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_EXTERNAL1;
//  if( HAL_TIM_SlaveConfigSynchronization(&TimDividerHandle, &sSlaveConfig) != HAL_OK)
//  {
//    return AUDIO_ERROR;
//  }  
//  
//  /* Initialize TIM3 peripheral in PWM mode*/
//  TimDividerHandle.Init.Period            = 1;
//  TimDividerHandle.Init.Prescaler         = 0;
//  TimDividerHandle.Init.ClockDivision     = 0;
//  TimDividerHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
//  // TimDividerHandle.Init.RepetitionCounter = 0;
//  if(HAL_TIM_PWM_Init(&TimDividerHandle) != HAL_OK)
//  {
//    return AUDIO_ERROR;
//  }
//  
//  /* Configure the PWM_channel_1  */
//  sOCConfig.OCMode     = TIM_OCMODE_PWM1;
//  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
//  sOCConfig.Pulse = 1;
//  if(HAL_TIM_PWM_ConfigChannel(&TimDividerHandle, &sOCConfig, TIM_CHANNEL_2) != HAL_OK)
//  {
//    return AUDIO_ERROR;
//  }  
  
  return AUDIO_OK;
}

/**
* @brief Audio Timer Start
* @param None
* @retval AUDIO_OK in case of success, AUDIO_ERROR otherwise 
*/
static uint8_t AUDIO_IN_Timer_Start(void)
{ 
//  if(HAL_TIM_IC_Start(&TimDividerHandle, TIM_CHANNEL_1) != HAL_OK)
//  {
//    return AUDIO_ERROR;
//  }  
//  /* Start the Output Compare */
//  if(HAL_TIM_OC_Start(&TimDividerHandle, TIM_CHANNEL_2) != HAL_OK)
//  {
//    return AUDIO_ERROR;
//  }  

  return AUDIO_OK;
}

/**
* @brief Audio Timer Stop
* @param None
* @retval AUDIO_OK in case of success, AUDIO_ERROR otherwise 
*/
static uint8_t AUDIO_IN_Timer_Stop()
{
//  if(HAL_TIM_IC_Stop(&TimDividerHandle, TIM_CHANNEL_1) != HAL_OK)
//  {
//    return AUDIO_ERROR;
//  }  
//  /* Start the Output Compare */
//  if(HAL_TIM_OC_Stop(&TimDividerHandle, TIM_CHANNEL_2) != HAL_OK)
//  {
//    return AUDIO_ERROR;
//  }  
  
  return AUDIO_OK;
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
  * @brief   音频输出设备初始化
  * @param   [in]AudioFreq 频率
  * @param   [in]Volume 音量 
  * @param   [in]options 保留
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_Init(uint32_t AudioFreq, uint32_t Volume, uint32_t options)
{
  UNUSED(AudioFreq);
  UNUSED(Volume);
  UNUSED(options);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备反初始化
  * @param   [in]options 保留
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_DeInit(uint32_t options)
{
  UNUSED(options);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备功能控制
  * @param   [in]pbuf 控制数据
  * @param   [in]size 控制数据大小 
  * @param   [in]cmd 功能
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_Command(uint8_t* pbuf, uint32_t size, uint8_t cmd)
{
//  AUDIO_CMD_START = 1,
//  AUDIO_CMD_PLAY,
//  AUDIO_CMD_STOP,

  UNUSED(pbuf);
  UNUSED(size);
  UNUSED(cmd);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备音量控制
  * @param   [in]vol 0-100音量等级
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_VolumeCtl(uint8_t vol)
{
  UNUSED(vol);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备消音控制
  * @param   [in]cmd 功能
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_MuteCtl(uint8_t cmd)
{
  UNUSED(cmd);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备周期控制操作
  * @param   [in]cmd 功能
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_PeriodicTC(uint8_t *pbuf, uint32_t size, uint8_t cmd)
{
  UNUSED(pbuf);
  UNUSED(size);
  UNUSED(cmd);
  return USBD_OK;
}

/**
  ******************************************************************
  * @brief   音频输出设备状态获取
  * @param   [in]cmd 功能
  * @return  USBD_OK 正常 USBD_FAIL失败
  * @author  aron566
  * @version V1.0
  * @date    2020-01-08
  ******************************************************************
  */
int8_t USB_Audio_OutDevice_GetState(void)
{
  return USBD_OK;
}

/**
* @brief  Initializes audio acquisition recording.
* @param  AudioFreq: Audio frequency to be configured for the peripherals.
* 		  Possible values are 8000, 16000, 32000 or 48000 Hz
* @param  BitRes: Not used.
* @param  ChnlNbr: Number of channels to be recorded.
* @retval AUDIO_OK in case of success, AUDIO_ERROR otherwise 
*/
uint8_t USB_Audio_IN_Init(uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr){
  
  /*Set Structure for internal state*/
  PDM2PCM_Handler.MicChannels=ChnlNbr;
  PDM2PCM_Handler.Sampling_Freq=AudioFreq;
  
  /*Timer Init*/
  if (AUDIO_IN_Timer_Init() != AUDIO_OK)  
  {
    return AUDIO_ERROR;
  }  
  
  switch(PDM2PCM_Handler.Sampling_Freq=AudioFreq)
  {   
  case AUDIOFREQ_8K:
    {
      PDM2PCM_Handler.DecimationFactor=128;
      PDM2PCM_Handler.PdmBufferSize=256;      
      break;
    }
  case AUDIOFREQ_16K:
    {
      PDM2PCM_Handler.DecimationFactor=64;
      PDM2PCM_Handler.PdmBufferSize=256;
      break;
    }    
  case AUDIOFREQ_32K:
    {
      PDM2PCM_Handler.DecimationFactor=64;
      PDM2PCM_Handler.PdmBufferSize=512;     
      
      break;
    }    
  case AUDIOFREQ_48K:
    {
      PDM2PCM_Handler.DecimationFactor=64;
      PDM2PCM_Handler.PdmBufferSize=768;     
      
      break;
    }
  default:
    {
      return AUDIO_ERROR;
    }
  }
  
//  /* Initialize the hAudioInI2s Instance parameter */
//  hAudioInI2s.Instance          = AUDIO_IN_I2S_INSTANCE;
//  
//  /* Disable I2S block */
//  __HAL_I2S_DISABLE(&hAudioInI2s);
//  
//  if(PDM2PCM_Handler.MicChannels==1)
//  {
//    hAudioInI2s.Init.DataFormat   = I2S_DATAFORMAT_16B;
//  }
//  else
//  {
//    hAudioInI2s.Init.DataFormat   = I2S_DATAFORMAT_32B;
//  }
//  
//  if(AudioFreq == AUDIOFREQ_8K)
//  {
//    hAudioInI2s.Init.AudioFreq    = 4 * AudioFreq;
//  }else
//  {
//    hAudioInI2s.Init.AudioFreq    = 2 * AudioFreq;
//  }  
//  
//  hAudioInI2s.Init.CPOL         = I2S_CPOL_HIGH;
//  hAudioInI2s.Init.MCLKOutput   = I2S_MCLKOUTPUT_DISABLE;
//  hAudioInI2s.Init.Mode         = I2S_MODE_MASTER_RX;
//  hAudioInI2s.Init.Standard     = I2S_STANDARD_MSB;
//  
//  
//  /* Initialize the I2S peripheral with the structure above */
//  if(HAL_I2S_GetState(&hAudioInI2s) == HAL_I2S_STATE_RESET)
//  { 
//    AUDIO_IN_I2S_MspInit();
//  }
//  
//  if(HAL_I2S_Init(&hAudioInI2s) != HAL_OK)
//  {
//    return AUDIO_ERROR;
//  }
//  /*End I2S Init*/ 
//  
//  /*PDM Filter Init*/
//  PDMDecoder_Init(PDM2PCM_Handler.Sampling_Freq,PDM2PCM_Handler.MicChannels, PDM2PCM_Handler.MicChannels);
//  /*End PDM Filter Init*/
  
  return AUDIO_OK;  
}

/**
* @brief  Starts audio recording.
* @param  * pbuf: Buffer that will contain 1 ms of PDM for each microphone.
				  Its dimension must be equal to (in uint16_t words): 
				  ((PCM sampling frequency)/1000 * DecimationFactor * Channels)/16
				  DecimationFactor is equal to 128 for 8000 KHZ sampling frequency, 64 in all the other cases
* @param  size: Not used in this driver.
* @retval AUDIO_OK in case of success, AUDIO_ERROR otherwise 
*/
uint8_t USB_Audio_IN_Record(uint16_t * pbuf, uint32_t size)
{   
  
  PDM2PCM_Handler.PDM_Data = pbuf;  
  
    if(PDM2PCM_Handler.MicChannels!=1)
  {
    if(AUDIO_IN_Timer_Start() != AUDIO_OK)
    {
      return AUDIO_ERROR;
    }
  }
  if(HAL_I2S_Receive_DMA(&hi2s1, I2S_InternalBuffer, PDM2PCM_Handler.PdmBufferSize/2) != HAL_OK)
  {
    return AUDIO_ERROR;
  }  
  /* Return 0 if all operations are OK */
  return AUDIO_OK; 
}

/**
* @brief  Stops audio recording.
* @param  None
* @retval AUDIO_OK in case of success, AUDIO_ERROR otherwise 
*/
uint8_t USB_Audio_IN_Stop(void)
{
  if(AUDIO_IN_Timer_Stop() != AUDIO_OK)
  {
    return AUDIO_ERROR;
  }    
  if(HAL_I2S_DMAStop(&hi2s1) != HAL_OK)
  {
    return AUDIO_ERROR;
  }   
  /* Return 0 if all operations are OK */
  return AUDIO_OK;  
}

/**
* @brief  Controls the audio in volume level.
* @param  Volume: Volume level to be set for the PDM to PCM conversion. 
		  Values must be in the range from 0 to 64
* @retval AUDIO_OK in case of success, AUDIO_ERROR otherwise 
*/
uint8_t USB_Audio_IN_SetVolume(uint8_t Volume)
{
  /* Set the Global variable AudioInVolume */
  AudioInVolume = Volume;  
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}

/**
* @brief  Converts audio format from PDM to PCM.
* @param  PDMBuf: Pointer to PDM buffer data
* @param  PCMBuf: Pointer to PCM buffer data
* @retval AUDIO_OK in case of success, AUDIO_ERROR otherwise 
*/
uint8_t USB_Audio_IN_PDMToPCM(uint16_t *PDMBuf, uint16_t *PCMBuf)
{  
  uint32_t index = 0;
  if(PDM2PCM_Handler.DecimationFactor == 128)
  {
    for(index = 0; index < PDM2PCM_Handler.MicChannels; index++)
    {
      //PDM_Filter_128_LSB(&((uint8_t*)(PDMBuf))[index], (uint16_t*)&(PCMBuf[index]), AudioInVolume , (PDMFilter_InitStruct *)&Filter[index]);
    }
  }
  
  if(PDM2PCM_Handler.DecimationFactor == 64)
  {
    for(index = 0; index < PDM2PCM_Handler.MicChannels; index++)
    {
      //PDM_Filter_64_LSB(&((uint8_t*)(PDMBuf))[index], (uint16_t*)&(PCMBuf[index]), AudioInVolume , (PDMFilter_InitStruct *)&Filter[index]);
    }
  }  
  
  return AUDIO_OK;
}


/**
* @brief  Pauses the audio file stream.
* @param  None
* @retval AUDIO_OK in case of success, AUDIO_ERROR otherwise 
*/
uint8_t USB_Audio_IN_Pause(void)
{    
  /* Call the Media layer pause function */
  HAL_I2S_DMAPause(&hi2s1);
  
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}

/**
* @brief  Resumes the audio file stream.
* @param  None    
* @retval AUDIO_OK in case of success, AUDIO_ERROR otherwise 
*/
uint8_t USB_Audio_IN_Resume(void)
{    
  /* Call the Media layer pause/resume function */
  HAL_I2S_DMAResume(&hi2s1);
  
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}
/**
* @brief Rx Transfer completed callbacks. It performs demuxing of the bit-interleaved PDM streams into 
	     byte-interleaved data suitable for PDM to PCM conversion. 1 ms of data for each microphone is 
		 written into the buffer that the user indicates when calling the USB_Audio_IN_Start(...) function.
* @param hi2s: I2S handle
* @retval None
*/
void USB_Audio_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  uint32_t index = 0;
  
  switch(PDM2PCM_Handler.MicChannels){
  case 1:
    {
      uint16_t * DataTempI2S = &I2S_InternalBuffer[PDM2PCM_Handler.PdmBufferSize/4] ;
      for(index = 0; index < PDM2PCM_Handler.PdmBufferSize/4; index++)
      {
        PDM2PCM_Handler.PDM_Data[index] = HTONS(DataTempI2S[index]);
      }
      break;
    }
    
  case 2:
    {      
      uint16_t * DataTempI2S = &(I2S_InternalBuffer[PDM2PCM_Handler.PdmBufferSize/2]);
      uint8_t a,b=0;
      for(index=0; index<PDM2PCM_Handler.PdmBufferSize/2; index++) {
        a = ((uint8_t *)(DataTempI2S))[(index*2)];
        b = ((uint8_t *)(DataTempI2S))[(index*2)+1];
        ((uint8_t *)(PDM2PCM_Handler.PDM_Data))[(index*2)] = Channel_Demux[a & CHANNEL_DEMUX_MASK] | Channel_Demux[b & CHANNEL_DEMUX_MASK] << 4;;
        ((uint8_t *)(PDM2PCM_Handler.PDM_Data))[(index*2)+1] = Channel_Demux[(a>>1) & CHANNEL_DEMUX_MASK] |Channel_Demux[(b>>1) & CHANNEL_DEMUX_MASK] << 4;
      }
      break;
    }    

  default:
    {
      
      break;
    }
    
  }
  USB_Audio_IN_TransferComplete_CallBack();
}

/**
* @brief Rx Transfer completed callbacks. It performs demuxing of the bit-interleaved PDM streams into 
	     byte-interleaved data suitable for PDM to PCM conversion. 1 ms of data for each microphone is 
		 written into the buffer that the user indicates when calling the USB_Audio_IN_Start(...) function.
* @param hi2s: I2S handle
* @retval None
*/
void USB_Audio_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  uint32_t index = 0;
  switch(PDM2PCM_Handler.MicChannels){
  case 1:
    {
      uint16_t * DataTempI2S = I2S_InternalBuffer;
      for(index = 0; index < PDM2PCM_Handler.PdmBufferSize/4; index++)
      {
        PDM2PCM_Handler.PDM_Data[index] = HTONS(DataTempI2S[index]);
      }
      break;
    }    
  case 2:
    {      
      uint16_t * DataTempI2S = I2S_InternalBuffer;
      uint8_t a,b=0;
      for(index=0; index<PDM2PCM_Handler.PdmBufferSize/2; index++) 
      {
        a = ((uint8_t *)(DataTempI2S))[(index*2)];
        b = ((uint8_t *)(DataTempI2S))[(index*2)+1];
        ((uint8_t *)(PDM2PCM_Handler.PDM_Data))[(index*2)] = Channel_Demux[a & CHANNEL_DEMUX_MASK] |
          Channel_Demux[b & CHANNEL_DEMUX_MASK] << 4;;
          ((uint8_t *)(PDM2PCM_Handler.PDM_Data))[(index*2)+1] = Channel_Demux[(a>>1) & CHANNEL_DEMUX_MASK] |
            Channel_Demux[(b>>1) & CHANNEL_DEMUX_MASK] << 4;
      }      
      break;
    }    
   
  default:
    {      
      break;
    }
    
  }
  USB_Audio_IN_HalfTransfer_CallBack();
}

/**
* @brief  User callback when record buffer is filled.
* @param  None
* @retval None
*/
__weak void USB_Audio_IN_TransferComplete_CallBack(void)
{
  /* This function should be implemented by the user application.
  It is called into this driver when the current buffer is filled
  to prepare the next buffer pointer and its size. */
}

/**
* @brief  User callback when record buffer is half filled.
* @param  None
* @retval None
*/
__weak void USB_Audio_IN_HalfTransfer_CallBack(void)
{
  /* This function should be implemented by the user application.
  It is called into this driver when the current buffer is filled
  to prepare the next buffer pointer and its size. */
}


/**
* @brief  Audio IN Error callback function
* @param  None
* @retval None
*/
__weak void USB_Audio_IN_Error_Callback(void)
{   
  /* This function is called when an Interrupt due to transfer error on or peripheral
  error occurs. */
}

/**
* @brief I2S error callbacks
* @param hi2s: I2S handle
* @retval None
*/
void USB_Audio_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{
  /* Manage the error generated on DMA FIFO: This function 
  should be coded by user (its prototype is already declared in stm32f4_discovery_audio.h) */  
//  if(hi2s->Instance == AUDIO_IN_I2S_INSTANCE)
//  {
//    USB_Audio_IN_Error_Callback();
//  }
}

/**
  ******************************************************************
  * @brief   音频USB初始化
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2020-01-24
  ******************************************************************
  */
void USB_Audio_Port_Init(void)
{
  /* Initialize USB descriptor basing on channels number and sampling frequency */
  USBD_AUDIO_Init_Microphone_Descriptor(&hUSBDDevice, AUDIO_SAMPLING_FREQUENCY, AUDIO_CHANNELS);
 /* Init Device Library */
  USBD_Init(&hUSBDDevice, &AUDIO_Desc, 0);
  /* Add Supported Class */
  USBD_RegisterClass(&hUSBDDevice, &USBD_AUDIO);
  /* Add Interface callbacks for AUDIO Class */  
  USBD_AUDIO_RegisterInterface(&hUSBDDevice, &USBD_AUDIO_fops);
  /* Start Device Process */
  USBD_Start(&hUSBDDevice);
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
