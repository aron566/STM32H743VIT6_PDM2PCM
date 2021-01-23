#include "stm32h743i_eval_audio.h"
//////////////////////////////////////////////////////////////////////////////////	 
////////////////////////////////////////////////////////////////////////////////// 	

void audio_sai_dma_callback(void) 
{      
	BSP_AUDIO_OUT_TransferComplete_CallBack();	
}   


uint8_t BSP_AUDIO_OUT_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{ 
	return 0; 
}


uint8_t BSP_AUDIO_OUT_Play(uint16_t* pBuffer, uint32_t Size)
{	

	return 0;
}

void BSP_AUDIO_OUT_ChangeBuffer(uint16_t *pData, uint16_t Size)
{ 

}


uint8_t BSP_AUDIO_OUT_Stop(uint32_t Option)
{
	return 0;
}


uint8_t BSP_AUDIO_OUT_SetVolume(uint8_t Volume)
{ 
	return 0;
}


uint8_t BSP_AUDIO_OUT_SetMute(uint32_t Cmd)
{
	return 0;
}
