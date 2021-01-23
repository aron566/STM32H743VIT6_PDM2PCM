/**
  ******************************************************************************
  * @file    usbd_audio.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   This file provides the Audio core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                AUDIO Class  Description
  *          ===================================================================
 *           This driver manages the Audio Class 1.0 following the "USB Device Class Definition for
  *           Audio Devices V1.0 Mar 18, 98".
  *           This driver implements the following aspects of the specification:
  *             - Device descriptor management
  *             - Configuration descriptor management
  *             - Standard AC Interface Descriptor management
  *             - 1 Audio Streaming Interface (with single channel, PCM, Stereo mode)
  *             - 1 Audio Streaming Endpoint
  *             - 1 Audio Terminal Input (1 channel)
  *             - Audio Class-Specific AC Interfaces
  *             - Audio Class-Specific AS Interfaces
  *             - AudioControl Requests: only SET_CUR and GET_CUR requests are supported (for Mute)
  *             - Audio Feature Unit (limited to Mute control)
  *             - Audio Synchronization type: Asynchronous
  *             - Single fixed audio sampling rate (configurable in usbd_conf.h file)
  *          The current audio class version supports the following audio features:
  *             - Pulse Coded Modulation (PCM) format
  *             - sampling rate: 48KHz. 
  *             - Bit resolution: 16
  *             - Number of channels: 2
  *             - No volume control
  *             - Mute/Unmute capability
  *             - Asynchronous Endpoints 
  *          
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *           
  *      
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_audio.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_AUDIO 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_AUDIO_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_AUDIO_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBD_AUDIO_Private_Macros
  * @{
  */ 
#define AUDIO_SAMPLE_FREQ(frq)      (uint8_t)(frq), (uint8_t)((frq >> 8)), (uint8_t)((frq >> 16))

#define AUDIO_PACKET_SZE(frq)          (uint8_t)(((frq * 2 * 2)/1000) & 0xFF), \
                                       (uint8_t)((((frq * 2 * 2)/1000) >> 8) & 0xFF)
                                         
/**
  * @}
  */ 




/** @defgroup USBD_AUDIO_Private_FunctionPrototypes
  * @{
  */


static uint8_t  USBD_AUDIO_Init (USBD_HandleTypeDef *pdev, 
                               uint8_t cfgidx);

static uint8_t  USBD_AUDIO_DeInit (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx);

static uint8_t  USBD_AUDIO_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req);

static uint8_t  *USBD_AUDIO_GetCfgDesc (uint16_t *length);

static uint8_t  *USBD_AUDIO_GetDeviceQualifierDesc (uint16_t *length);

static uint8_t  USBD_AUDIO_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_AUDIO_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_AUDIO_EP0_RxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_AUDIO_EP0_TxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_AUDIO_SOF (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_AUDIO_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_AUDIO_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

static void AUDIO_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

static void AUDIO_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

/**
  * @}
  */ 
static uint32_t RecFlag = 0;
//__attribute__((section(".RAM_D1"))) 
uint8_t  IsocInBuff[512] = {0};
static uint16_t u16DataLen;

//__attribute__((section(".RAM_D1"))) volatile int16_t g_UACRingBuf[UAC_BUFFER_SIZE];
volatile int16_t g_UACRingBuf[UAC_BUFFER_SIZE];
volatile uint16_t g_UACWriteIndex = 0;
volatile uint16_t g_UACReadIndex = 0;

/** @defgroup USBD_AUDIO_Private_Variables
  * @{
  */ 

USBD_ClassTypeDef  USBD_AUDIO = 
{
  USBD_AUDIO_Init,
  USBD_AUDIO_DeInit,
  USBD_AUDIO_Setup,
  USBD_AUDIO_EP0_TxReady,  
  USBD_AUDIO_EP0_RxReady,
  USBD_AUDIO_DataIn,
  USBD_AUDIO_DataOut,
  USBD_AUDIO_SOF,
  USBD_AUDIO_IsoINIncomplete,
  USBD_AUDIO_IsoOutIncomplete,      
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetCfgDesc, 
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetDeviceQualifierDesc,
};

/* USB AUDIO device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_AUDIO_CfgDesc[USB_AUDIO_CONFIG_DESC_SIZ] __ALIGN_END =
{
//0
0x09,
0x02,
0x6E,
0x00,
0x02,                                 
0x01,                              
0x00,                                
0xC0,                                
0x32,

//1
0x09,
0x04,  //?????(Interface Descriptor)
0x00,  //?????0
0x00,
0x00,
0x01,  //Audio class ??
0x01,  //Audio class Control  ++++++++++
0x00,
0x00,

//2
0x09,
0x24, 
0x01, // descriptor subtype (1..Header subtype)
0x00,          
0x01,
0x28,                               
0x00,
0x01,                               
0x01, 

//3
0x0c,
0x24,
0x02,// descriptor subtype (INPUT_TERMINAL subtype)
0x04,                                                                                           
0x01,// Terminal Type (Microphone In)                                 
0x02,// Terminal Type (Microphone In)                                 
0x00,                                 
0x02,// Number channels                                                                               
0x03,//Channel Config bitmap(Left and Right)                                                                         
0x00,
0x00,                                 
0x00,   

//4
0x0A,               	
0x24,               	
0x06,// descriptor subtype (FEATURE_UNIT)               	
0x05,//This value is used in all requests to address this Unit 	
0x04,            		
0x01,               	
0x01,               	
0x02,               	
0x02,
0x00,   

//5
0x09,
0x24,
0x03,// bDescriptorSubType:OUTPUT_TERMINAL
0x02,
0x01,                                																															
0x01,                      
0x00,                                                               
0x05,
0x00,                                                                                        

/***********************************************************************/

//6
0x09,      
0x04, //?????(Interface Descriptor)                                                                                        
0x01, //?????1                                                                                     
0x00, //alternate setting (0)                                                                                  
0x00, //number of endpoints (0)                                                                               
0x01, //Audio class ??                                                                                                             
0x02, //audio streaming                                                                                     
0x00,                                                                                           
0x00,     

//7
0x09,      
0x04, //?????(Interface Descriptor)
0x01, //?????1                               
0x01, //alternate setting (1)                                 
0x01, //number of endpoints (1)
0x01, //Audio class ??
0x02, //audio streaming
0x00,
0x00,

//8
0x07,  
0x24,      
0x01,                                 
0x02,                                
0x01,                                 
0x01,               
0x00,        

#if 1
//9
0x0B,                               
0x24,      
0x02,          
0x01,                
0x02,                              
0x02,//0x02                                
0x10,                                 
0x01,  
B3VAL(USBD_AUDIO_FREQ),
//0xc0,
//0x5d,
//0x00,
#else
    0x0B,                           // bLength
    0x24,                           // bDescriptorType:CS_INTERFACE
    0x02,                           // bDescriptorSubType:FORMAT_TYPE
    0x01,                           // bFormatType:FORMAT_TYPE_I
    0x02,                           // bNrChannels
    0x03,                           // bSubFrameSize : 24bit
    0x18,                           // bBitResolution	
    0x01,                           // bSamFreqType : 0 continuous; 1 discrete
	  B3VAL(USBD_AUDIO_FREQ),         // Sample Frequency 
#endif
//10
0x09,
0x05,// DescriptorType (endpoint descriptor type)               
0x81,                        
0x0D, //0x01                 
LOBYTE(EP1_MAX_PKT_SIZE),    /* wMaxPacketSize */  
HIBYTE(EP1_MAX_PKT_SIZE),
0x01,                                
0x00,                                
0x00,                                    

//11
0x07,   
0x25,       
0x01,              
0x00,                                 
0x00,                                 
0x00,                                 
0x00,
} ;

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_AUDIO_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END=
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_Private_Functions
  * @{
  */ 

/**
  * @brief  USBD_AUDIO_Init
  *         Initialize the AUDIO interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_AUDIO_Init (USBD_HandleTypeDef *pdev, 
                               uint8_t cfgidx)
{
#if 1  
  /* Open EP IN */
  USBD_LL_OpenEP(pdev,
                 AUDIO_IN_EP,
                 USBD_EP_TYPE_ISOC,
                 AUDIO_IN_PACKET);
    
#endif
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_Init
  *         DeInitialize the AUDIO layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_AUDIO_DeInit (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx)
{
#if 1
  USBD_LL_CloseEP(pdev,
              AUDIO_IN_EP);
#endif  
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_Setup
  *         Handle the AUDIO specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
const uint16_t VOL_MIN = 0xF800; /* Volume Minimum Value */
const uint16_t VOL_MAX = 0x0600; /* Volume Maximum Value */
const uint16_t VOL_RES = 0x0200; /* Volume Resolution */

static uint8_t  USBD_AUDIO_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_HandleTypeDef   *haudio;
  uint16_t len;
  uint8_t *pbuf;
  uint8_t ret = USBD_OK;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :  
    switch (req->bRequest)
    {
    case AUDIO_REQ_GET_CUR:
      AUDIO_REQ_GetCurrent(pdev, req);
      break;
      
    case AUDIO_REQ_SET_CUR:
      AUDIO_REQ_SetCurrent(pdev, req);   
      break;
		
		case AUDIO_REQ_GET_MIN:
				USBD_CtlSendData(pdev, (uint8_t *)&VOL_MIN, req->wLength);
				break;
		case AUDIO_REQ_GET_MAX:
				USBD_CtlSendData(pdev, (uint8_t *)&VOL_MAX, req->wLength);
				break;
		case AUDIO_REQ_GET_RES:
				USBD_CtlSendData(pdev, (uint8_t *)&VOL_RES, req->wLength);
				break;		
      
    default:
      USBD_CtlError (pdev, req);
      ret = USBD_FAIL; 
    }
    break;
    
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR:      
      if( (req->wValue >> 8) == AUDIO_DESCRIPTOR_TYPE)
      {
        pbuf = USBD_AUDIO_CfgDesc + 18;
        len = MIN(USB_AUDIO_DESC_SIZ , req->wLength);
        
        
        USBD_CtlSendData (pdev, 
                          pbuf,
                          len);
      }
      break;
      
    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData (pdev,
                        (uint8_t *)&(haudio->alt_setting),
                        1);
      break;
      
    case USB_REQ_SET_INTERFACE :
      if ((uint8_t)(req->wValue) <= USBD_MAX_NUM_INTERFACES)
      {
        haudio->alt_setting = (uint8_t)(req->wValue);
				if (haudio->alt_setting == 1)
				{
					RecFlag = 1;
					g_UACWriteIndex = 0;
					g_UACReadIndex = 0;
				}
				else
				{
					RecFlag = 0;
					USBD_LL_FlushEP(pdev, AUDIO_IN_EP);
				}				
      }
      else
      {
        /* Call the error management function (command will be nacked */
        USBD_CtlError (pdev, req);
      }
      break;      
      
    default:
      USBD_CtlError (pdev, req);
      ret = USBD_FAIL;     
    }
  }
  return ret;
}


/**
  * @brief  USBD_AUDIO_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_AUDIO_GetCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_AUDIO_CfgDesc);
  return USBD_AUDIO_CfgDesc;
}

/**
  * @brief  USBD_AUDIO_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_AUDIO_EP0_RxReady (USBD_HandleTypeDef *pdev)
{
  return USBD_OK;
}
/**
  * @brief  USBD_AUDIO_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_AUDIO_EP0_TxReady (USBD_HandleTypeDef *pdev)
{
  /* Only OUT control data are processed */
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_AUDIO_DataIn (USBD_HandleTypeDef *pdev, 
                              uint8_t epnum)
{
#if 1
	int i;
    
	if ( g_UACWriteIndex >= g_UACReadIndex ) 
	{
		u16DataLen = g_UACWriteIndex - g_UACReadIndex;	
	}
	else 
	{
		u16DataLen = (UAC_BUFFER_SIZE - g_UACReadIndex) + g_UACWriteIndex;	
	}

	// Too many data in buffer, accelerate data transmission
	if (u16DataLen > UAC_BUFFER_HIGH_THRESHOLD)
	{
			u16DataLen = (DSCP_BUFFER_SIZE >> 1) + 4;
	}
	// Too few data in buffer, decelerate data transmission
	else if (u16DataLen < UAC_BUFFER_HIGH_THRESHOLD)
	{
			u16DataLen = (DSCP_BUFFER_SIZE >> 1) - 4;
	}	
	else if (u16DataLen > (DSCP_BUFFER_SIZE >> 1))
	{
			u16DataLen = (DSCP_BUFFER_SIZE >> 1);
	}
	
	for (i=0;i<u16DataLen;i++)
	{
			IsocInBuff[i*2] = g_UACRingBuf[g_UACReadIndex] & 0xff;
			IsocInBuff[i*2+1] = (g_UACRingBuf[g_UACReadIndex] >> 8) & 0xff;
			g_UACReadIndex++;
			if (g_UACReadIndex >= UAC_BUFFER_SIZE)
			{
					g_UACReadIndex = 0;
			}
	}	
  /* Only IN data are processed */
	USBD_LL_FlushEP(pdev, AUDIO_IN_EP);
	//USBD_LL_Transmit(pdev, AUDIO_IN_EP, IsocInBuff, 96);
	USBD_LL_Transmit(pdev, AUDIO_IN_EP, IsocInBuff, u16DataLen*DSCP_DATA_BYTE);
  return USBD_OK;
#else
  static int offset = 0;
	if(offset >= 4096)
		offset = 0;
  USBD_LL_FlushEP(pdev, AUDIO_IN_EP);
	USBD_LL_Transmit(pdev, AUDIO_IN_EP, (uint8_t *)(src_buf+offset), 64);
	//USBD_LL_Transmit(pdev, AUDIO_IN_EP, IsocInBuff, u16DataLen*DSCP_DATA_BYTE);
  return USBD_OK;
#endif
}
/**
  * @brief  USBD_AUDIO_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_AUDIO_SOF (USBD_HandleTypeDef *pdev)
{
#if 1
  /* Check if there are available data in stream buffer. */
  if (RecFlag == 1)
  {
		USBD_LL_Transmit(pdev, AUDIO_IN_EP, NULL, AUDIO_IN_PACKET);
    RecFlag = 0;
  }
#endif
  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
void  USBD_AUDIO_Sync (USBD_HandleTypeDef *pdev, AUDIO_OffsetTypeDef offset)
{
  int8_t shift = 0;
  USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  haudio->offset =  offset; 
  
  
  if(haudio->rd_enable == 1)
  {
    haudio->rd_ptr += AUDIO_TOTAL_BUF_SIZE/2;
    
    if (haudio->rd_ptr == AUDIO_TOTAL_BUF_SIZE)
    {
      /* roll back */
      haudio->rd_ptr = 0;
    }
  }
  
  if(haudio->rd_ptr > haudio->wr_ptr)
  {
    if((haudio->rd_ptr - haudio->wr_ptr) < AUDIO_OUT_PACKET)
    {
      shift = -4;
    }
    else if((haudio->rd_ptr - haudio->wr_ptr) > (AUDIO_TOTAL_BUF_SIZE - AUDIO_OUT_PACKET))
    {
      shift = 4;
    }    

  }
  else
  {
    if((haudio->wr_ptr - haudio->rd_ptr) < AUDIO_OUT_PACKET)
    {
      shift = 4;
    }
    else if((haudio->wr_ptr - haudio->rd_ptr) > (AUDIO_TOTAL_BUF_SIZE - AUDIO_OUT_PACKET))
    {
      shift = -4;
    }  
  }

  if(haudio->offset == AUDIO_OFFSET_FULL)
  {
    ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData)->AudioCmd(&haudio->buffer[0],
                                                         AUDIO_TOTAL_BUF_SIZE/2 - shift,
                                                         AUDIO_CMD_PLAY); 
      haudio->offset = AUDIO_OFFSET_NONE;           
  }
}

/**
  * @brief  USBD_AUDIO_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_AUDIO_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return USBD_OK;
}
/**
  * @brief  USBD_AUDIO_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_AUDIO_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_AUDIO_DataOut (USBD_HandleTypeDef *pdev, 
                              uint8_t epnum)
{ 
  return USBD_OK;
}

/**
  * @brief  AUDIO_Req_GetCurrent
  *         Handles the GET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{  
  USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  memset(haudio->control.data, 0, 64);
  /* Send the current mute state */
  USBD_CtlSendData (pdev, 
                    haudio->control.data,
                    req->wLength);
}

/**
  * @brief  AUDIO_Req_SetCurrent
  *         Handles the SET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{ 
  USBD_AUDIO_HandleTypeDef   *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef*) pdev->pClassData;
  
  if (req->wLength)
  {
    /* Prepare the reception of the buffer over EP0 */
    USBD_CtlPrepareRx (pdev,
                       haudio->control.data,                                  
                       req->wLength);    
    
    haudio->control.cmd = AUDIO_REQ_SET_CUR;     /* Set the request value */
    haudio->control.len = req->wLength;          /* Set the request data length */
    haudio->control.unit = HIBYTE(req->wIndex);  /* Set the request target unit */
  }
}


/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static uint8_t  *USBD_AUDIO_GetDeviceQualifierDesc (uint16_t *length)
{
  *length = sizeof (USBD_AUDIO_DeviceQualifierDesc);
  return USBD_AUDIO_DeviceQualifierDesc;
}

/**
* @brief  USBD_AUDIO_RegisterInterface
* @param  fops: Audio interface callback
* @retval status
*/
uint8_t  USBD_AUDIO_RegisterInterface  (USBD_HandleTypeDef   *pdev, 
                                        USBD_AUDIO_ItfTypeDef *fops)
{
  if(fops != NULL)
  {
    pdev->pUserData= fops;
  }
  return 0;
}
/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
