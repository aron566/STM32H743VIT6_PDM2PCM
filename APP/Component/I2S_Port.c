/**
 *  @file I2S_Port.c
 *
 *  @date 2021-01-15
 *
 *  @author aron566
 *
 *  @copyright None.
 *
 *  @brief 
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
#include "I2S_Port.h"
#include "main.h"
#include "usbd_audio.h"
/** Private typedef ----------------------------------------------------------*/

/** Private macros -----------------------------------------------------------*/

/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
extern I2S_HandleTypeDef hi2s1;
extern volatile int16_t g_UACRingBuf[UAC_BUFFER_SIZE];
extern volatile PDM2PCM_BUF_Typedef_t Pdm2Pcm_ChannelBuf[MIC_CHANNEL_NUM];
extern volatile uint8_t SAI_Can_Read_Data_Flag;
/** Private variables --------------------------------------------------------*/
uint16_t txBuf[128];
uint16_t pdmRxBuf[128];
uint16_t MidBuffer[16];
uint8_t txstate = 0;
uint8_t rxstate = 0;


uint16_t fifobuf[256];
uint8_t fifo_w_ptr = 0;
uint8_t fifo_r_ptr = 0;
uint8_t fifo_read_enabled = 0;
/** Private function prototypes ----------------------------------------------*/

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
  * @brief   
  * @param   [in]None
  * @return  TRUE  成功
  * @return  FALSE 失败
  * @author  aron566
  * @version V1.0
  * @date    2021-01-19
  ******************************************************************
  */
void FifoWrite(uint16_t data) {
	fifobuf[fifo_w_ptr] = data;
	fifo_w_ptr++;
}

uint16_t FifoRead() {
	uint16_t val = fifobuf[fifo_r_ptr];
	fifo_r_ptr++;
	return val;
}
/** Public application code --------------------------------------------------*/
/*******************************************************************************
*
*       Public code
*
********************************************************************************
*/
void HAL_I2S_TxHalfCpltCallback (I2S_HandleTypeDef *hi2s) {
	txstate = 1;
}

void HAL_I2S_TxCpltCallback (I2S_HandleTypeDef *hi2s) {
	txstate = 2;
}

void HAL_I2S_RxHalfCpltCallback (I2S_HandleTypeDef *hi2s) {
  SAI_Can_Read_Data_Flag = 1;
  SAI_Receive_Complete_Flag = 0;
}

void HAL_I2S_RxCpltCallback (I2S_HandleTypeDef *hi2s) {
  SAI_Can_Read_Data_Flag = 1;
  SAI_Receive_Complete_Flag = 1;
}

/**
  ******************************************************************
  * @brief   I2S接口启动
  * @param   [in]None
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2021-01-24
  ******************************************************************
  */
void I2S_Port_Start(void)
{
    if (rxstate==1) {
      PDM_To_PCM_Stream(&pdmRxBuf[0],&MidBuffer[0]);
    	for (int i=0; i<16;i++) { FifoWrite(MidBuffer[i]); }
    	if (fifo_w_ptr-fifo_r_ptr > 128) fifo_read_enabled=1;
    	rxstate=0;

      /*send*/
  			for (int i=0; i<64;i=i+4) {
				uint16_t data = FifoRead();
				txBuf[i] = data;
				txBuf[i+2] = data;
        g_UACRingBuf[i] = data;
			}
    }

    if (rxstate==2) {
      PDM_To_PCM_Stream(&pdmRxBuf[64],&MidBuffer[0]);
    	for (int i=0; i<16;i++) { FifoWrite(MidBuffer[i]); }
    	rxstate=0;
      
      /*send*/
			for (int i=64; i<128;i=i+4) {
				uint16_t data = FifoRead();
				txBuf[i] = data;
				txBuf[i+2] = data;
        g_UACRingBuf[i] = data;
			}
    }

    if (txstate==1) {
    	if (fifo_read_enabled==1) {
			for (int i=0; i<64;i=i+4) {
				uint16_t data = FifoRead();
				txBuf[i] = data;
				txBuf[i+2] = data;
        g_UACRingBuf[i] = data;
			}
    	}
    	txstate=0;
    }

    if (txstate==2) {
    	if (fifo_read_enabled==1) {
			for (int i=64; i<128;i=i+4) {
				uint16_t data = FifoRead();
				txBuf[i] = data;
				txBuf[i+2] = data;
        g_UACRingBuf[i] = data;
			}

		}
    	txstate=0;
    }
}

/**
  ******************************************************************
  * @brief   I2S接口初始化
  * @param   [in]None
  * @return  None
  * @author  aron566
  * @version V1.0
  * @date    2021-01-24
  ******************************************************************
  */
void I2S_Port_Init(void)
{
  HAL_I2S_Receive_DMA(&hi2s1, (uint16_t *)Pdm2Pcm_ChannelBuf[0].PDM_RX_Buf, PDM_TWO_SAMPLE_NUM);
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
