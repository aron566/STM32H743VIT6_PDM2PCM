#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_audio.h"
#include "usbd_audio_if.h"

USBD_HandleTypeDef USBD_Device;

void UAC_Init(void)
{
  HAL_PWREx_EnableUSBVoltageDetector();
  
  /* Init Device Library */
  USBD_Init(&USBD_Device, &AUDIO_Desc, 0);

  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_AUDIO_CLASS);

  /* Add Interface callbacks for AUDIO Class */
  USBD_AUDIO_RegisterInterface(&USBD_Device, &USBD_AUDIO_fops);

  /* Start Device Process */
  USBD_Start(&USBD_Device);
}

