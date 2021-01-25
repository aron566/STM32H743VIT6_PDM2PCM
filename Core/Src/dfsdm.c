/**
  ******************************************************************************
  * @file    dfsdm.c
  * @brief   This file provides code for the configuration
  *          of the DFSDM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "dfsdm.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
DFSDM_Filter_HandleTypeDef hdfsdm1_filter1;
DFSDM_Filter_HandleTypeDef hdfsdm1_filter2;
DFSDM_Filter_HandleTypeDef hdfsdm1_filter3;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel0;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel1;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel2;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel3;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel4;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel5;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel6;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel7;
DMA_HandleTypeDef hdma_dfsdm1_flt0;
DMA_HandleTypeDef hdma_dfsdm1_flt1;
DMA_HandleTypeDef hdma_dfsdm1_flt2;
DMA_HandleTypeDef hdma_dfsdm1_flt3;

/* DFSDM1 init function */
void MX_DFSDM1_Init(void)
{

  hdfsdm1_filter0.Instance = DFSDM1_Filter0;
  hdfsdm1_filter0.Init.RegularParam.Trigger = DFSDM_FILTER_SW_TRIGGER;
  hdfsdm1_filter0.Init.RegularParam.FastMode = ENABLE;
  hdfsdm1_filter0.Init.RegularParam.DmaMode = ENABLE;
  hdfsdm1_filter0.Init.FilterParam.SincOrder = DFSDM_FILTER_SINC5_ORDER;
  hdfsdm1_filter0.Init.FilterParam.Oversampling = 64;
  hdfsdm1_filter0.Init.FilterParam.IntOversampling = 1;
  if (HAL_DFSDM_FilterInit(&hdfsdm1_filter0) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_filter1.Instance = DFSDM1_Filter1;
  hdfsdm1_filter1.Init.RegularParam.Trigger = DFSDM_FILTER_SW_TRIGGER;
  hdfsdm1_filter1.Init.RegularParam.FastMode = ENABLE;
  hdfsdm1_filter1.Init.RegularParam.DmaMode = ENABLE;
  hdfsdm1_filter1.Init.FilterParam.SincOrder = DFSDM_FILTER_SINC5_ORDER;
  hdfsdm1_filter1.Init.FilterParam.Oversampling = 64;
  hdfsdm1_filter1.Init.FilterParam.IntOversampling = 1;
  if (HAL_DFSDM_FilterInit(&hdfsdm1_filter1) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_filter2.Instance = DFSDM1_Filter2;
  hdfsdm1_filter2.Init.RegularParam.Trigger = DFSDM_FILTER_SW_TRIGGER;
  hdfsdm1_filter2.Init.RegularParam.FastMode = ENABLE;
  hdfsdm1_filter2.Init.RegularParam.DmaMode = ENABLE;
  hdfsdm1_filter2.Init.FilterParam.SincOrder = DFSDM_FILTER_SINC5_ORDER;
  hdfsdm1_filter2.Init.FilterParam.Oversampling = 64;
  hdfsdm1_filter2.Init.FilterParam.IntOversampling = 1;
  if (HAL_DFSDM_FilterInit(&hdfsdm1_filter2) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_filter3.Instance = DFSDM1_Filter3;
  hdfsdm1_filter3.Init.RegularParam.Trigger = DFSDM_FILTER_SW_TRIGGER;
  hdfsdm1_filter3.Init.RegularParam.FastMode = ENABLE;
  hdfsdm1_filter3.Init.RegularParam.DmaMode = ENABLE;
  hdfsdm1_filter3.Init.FilterParam.SincOrder = DFSDM_FILTER_SINC5_ORDER;
  hdfsdm1_filter3.Init.FilterParam.Oversampling = 64;
  hdfsdm1_filter3.Init.FilterParam.IntOversampling = 1;
  if (HAL_DFSDM_FilterInit(&hdfsdm1_filter3) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel0.Instance = DFSDM1_Channel0;
  hdfsdm1_channel0.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel0.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  hdfsdm1_channel0.Init.OutputClock.Divider = 2;
  hdfsdm1_channel0.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel0.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel0.Init.Input.Pins = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  hdfsdm1_channel0.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel0.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel0.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel0.Init.Awd.Oversampling = 1;
  hdfsdm1_channel0.Init.Offset = 0;
  hdfsdm1_channel0.Init.RightBitShift = 0x07;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel0) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel1.Instance = DFSDM1_Channel1;
  hdfsdm1_channel1.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel1.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  hdfsdm1_channel1.Init.OutputClock.Divider = 2;
  hdfsdm1_channel1.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel1.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel1.Init.Input.Pins = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  hdfsdm1_channel1.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_FALLING;
  hdfsdm1_channel1.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel1.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel1.Init.Awd.Oversampling = 1;
  hdfsdm1_channel1.Init.Offset = 0;
  hdfsdm1_channel1.Init.RightBitShift = 0x07;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel1) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel2.Instance = DFSDM1_Channel2;
  hdfsdm1_channel2.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel2.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  hdfsdm1_channel2.Init.OutputClock.Divider = 2;
  hdfsdm1_channel2.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel2.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel2.Init.Input.Pins = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  hdfsdm1_channel2.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel2.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel2.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel2.Init.Awd.Oversampling = 1;
  hdfsdm1_channel2.Init.Offset = 0;
  hdfsdm1_channel2.Init.RightBitShift = 0x07;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel2) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel3.Instance = DFSDM1_Channel3;
  hdfsdm1_channel3.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel3.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  hdfsdm1_channel3.Init.OutputClock.Divider = 2;
  hdfsdm1_channel3.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel3.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel3.Init.Input.Pins = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  hdfsdm1_channel3.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_FALLING;
  hdfsdm1_channel3.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel3.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel3.Init.Awd.Oversampling = 1;
  hdfsdm1_channel3.Init.Offset = 0;
  hdfsdm1_channel3.Init.RightBitShift = 0x07;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel3) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel4.Instance = DFSDM1_Channel4;
  hdfsdm1_channel4.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel4.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  hdfsdm1_channel4.Init.OutputClock.Divider = 2;
  hdfsdm1_channel4.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel4.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel4.Init.Input.Pins = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  hdfsdm1_channel4.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel4.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel4.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel4.Init.Awd.Oversampling = 1;
  hdfsdm1_channel4.Init.Offset = 0;
  hdfsdm1_channel4.Init.RightBitShift = 0x07;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel4) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel5.Instance = DFSDM1_Channel5;
  hdfsdm1_channel5.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel5.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  hdfsdm1_channel5.Init.OutputClock.Divider = 2;
  hdfsdm1_channel5.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel5.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel5.Init.Input.Pins = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  hdfsdm1_channel5.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_FALLING;
  hdfsdm1_channel5.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel5.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel5.Init.Awd.Oversampling = 1;
  hdfsdm1_channel5.Init.Offset = 0;
  hdfsdm1_channel5.Init.RightBitShift = 0x07;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel5) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel6.Instance = DFSDM1_Channel6;
  hdfsdm1_channel6.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel6.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  hdfsdm1_channel6.Init.OutputClock.Divider = 2;
  hdfsdm1_channel6.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel6.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel6.Init.Input.Pins = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  hdfsdm1_channel6.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel6.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel6.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel6.Init.Awd.Oversampling = 1;
  hdfsdm1_channel6.Init.Offset = 0;
  hdfsdm1_channel6.Init.RightBitShift = 0x07;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel6) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel7.Instance = DFSDM1_Channel7;
  hdfsdm1_channel7.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel7.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  hdfsdm1_channel7.Init.OutputClock.Divider = 2;
  hdfsdm1_channel7.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel7.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel7.Init.Input.Pins = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  hdfsdm1_channel7.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_FALLING;
  hdfsdm1_channel7.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel7.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel7.Init.Awd.Oversampling = 1;
  hdfsdm1_channel7.Init.Offset = 0;
  hdfsdm1_channel7.Init.RightBitShift = 0x07;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel7) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DFSDM_FilterConfigRegChannel(&hdfsdm1_filter0, DFSDM_CHANNEL_0, DFSDM_CONTINUOUS_CONV_ON) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DFSDM_FilterConfigRegChannel(&hdfsdm1_filter1, DFSDM_CHANNEL_1, DFSDM_CONTINUOUS_CONV_ON) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DFSDM_FilterConfigRegChannel(&hdfsdm1_filter2, DFSDM_CHANNEL_2, DFSDM_CONTINUOUS_CONV_ON) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DFSDM_FilterConfigRegChannel(&hdfsdm1_filter3, DFSDM_CHANNEL_3, DFSDM_CONTINUOUS_CONV_ON) != HAL_OK)
  {
    Error_Handler();
  }

}

static uint32_t HAL_RCC_DFSDM1_CLK_ENABLED=0;

static uint32_t DFSDM1_Init = 0;

void HAL_DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef* dfsdm_filterHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(DFSDM1_Init == 0)
  {
  /* USER CODE BEGIN DFSDM1_MspInit 0 */

  /* USER CODE END DFSDM1_MspInit 0 */
    /* DFSDM1 clock enable */
    HAL_RCC_DFSDM1_CLK_ENABLED++;
    if(HAL_RCC_DFSDM1_CLK_ENABLED==1){
      __HAL_RCC_DFSDM1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**DFSDM1 GPIO Configuration
    PE4     ------> DFSDM1_DATIN3
    PC2_C     ------> DFSDM1_CKOUT
    PB1     ------> DFSDM1_DATIN1
    PC11     ------> DFSDM1_DATIN5
    PB9     ------> DFSDM1_DATIN7
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_DFSDM1;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_DFSDM1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_DFSDM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_DFSDM1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_DFSDM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN DFSDM1_MspInit 1 */

  /* USER CODE END DFSDM1_MspInit 1 */
  DFSDM1_Init++;
  }

    /* DFSDM1 DMA Init */
    /* DFSDM1_FLT0 Init */
  if(dfsdm_filterHandle->Instance == DFSDM1_Filter0){
    hdma_dfsdm1_flt0.Instance = DMA1_Stream3;
    hdma_dfsdm1_flt0.Init.Request = DMA_REQUEST_DFSDM1_FLT0;
    hdma_dfsdm1_flt0.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_dfsdm1_flt0.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dfsdm1_flt0.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dfsdm1_flt0.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_dfsdm1_flt0.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_dfsdm1_flt0.Init.Mode = DMA_CIRCULAR;
    hdma_dfsdm1_flt0.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_dfsdm1_flt0.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_dfsdm1_flt0) != HAL_OK)
    {
      Error_Handler();
    }

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one channel to perform all the requested DMAs. */
    __HAL_LINKDMA(dfsdm_filterHandle,hdmaInj,hdma_dfsdm1_flt0);
    __HAL_LINKDMA(dfsdm_filterHandle,hdmaReg,hdma_dfsdm1_flt0);
  }

    /* DFSDM1_FLT1 Init */
  if(dfsdm_filterHandle->Instance == DFSDM1_Filter1){
    hdma_dfsdm1_flt1.Instance = DMA1_Stream4;
    hdma_dfsdm1_flt1.Init.Request = DMA_REQUEST_DFSDM1_FLT1;
    hdma_dfsdm1_flt1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_dfsdm1_flt1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dfsdm1_flt1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dfsdm1_flt1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_dfsdm1_flt1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_dfsdm1_flt1.Init.Mode = DMA_CIRCULAR;
    hdma_dfsdm1_flt1.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_dfsdm1_flt1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_dfsdm1_flt1) != HAL_OK)
    {
      Error_Handler();
    }

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one channel to perform all the requested DMAs. */
    __HAL_LINKDMA(dfsdm_filterHandle,hdmaInj,hdma_dfsdm1_flt1);
    __HAL_LINKDMA(dfsdm_filterHandle,hdmaReg,hdma_dfsdm1_flt1);
  }

    /* DFSDM1_FLT2 Init */
  if(dfsdm_filterHandle->Instance == DFSDM1_Filter2){
    hdma_dfsdm1_flt2.Instance = DMA1_Stream5;
    hdma_dfsdm1_flt2.Init.Request = DMA_REQUEST_DFSDM1_FLT2;
    hdma_dfsdm1_flt2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_dfsdm1_flt2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dfsdm1_flt2.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dfsdm1_flt2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_dfsdm1_flt2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_dfsdm1_flt2.Init.Mode = DMA_CIRCULAR;
    hdma_dfsdm1_flt2.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_dfsdm1_flt2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_dfsdm1_flt2) != HAL_OK)
    {
      Error_Handler();
    }

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one channel to perform all the requested DMAs. */
    __HAL_LINKDMA(dfsdm_filterHandle,hdmaInj,hdma_dfsdm1_flt2);
    __HAL_LINKDMA(dfsdm_filterHandle,hdmaReg,hdma_dfsdm1_flt2);
  }

    /* DFSDM1_FLT3 Init */
  if(dfsdm_filterHandle->Instance == DFSDM1_Filter3){
    hdma_dfsdm1_flt3.Instance = DMA1_Stream6;
    hdma_dfsdm1_flt3.Init.Request = DMA_REQUEST_DFSDM1_FLT3;
    hdma_dfsdm1_flt3.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_dfsdm1_flt3.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dfsdm1_flt3.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dfsdm1_flt3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_dfsdm1_flt3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_dfsdm1_flt3.Init.Mode = DMA_CIRCULAR;
    hdma_dfsdm1_flt3.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_dfsdm1_flt3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_dfsdm1_flt3) != HAL_OK)
    {
      Error_Handler();
    }

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one channel to perform all the requested DMAs. */
    __HAL_LINKDMA(dfsdm_filterHandle,hdmaInj,hdma_dfsdm1_flt3);
    __HAL_LINKDMA(dfsdm_filterHandle,hdmaReg,hdma_dfsdm1_flt3);
  }

}

void HAL_DFSDM_ChannelMspInit(DFSDM_Channel_HandleTypeDef* dfsdm_channelHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(DFSDM1_Init == 0)
  {
  /* USER CODE BEGIN DFSDM1_MspInit 0 */

  /* USER CODE END DFSDM1_MspInit 0 */
    /* DFSDM1 clock enable */
    HAL_RCC_DFSDM1_CLK_ENABLED++;
    if(HAL_RCC_DFSDM1_CLK_ENABLED==1){
      __HAL_RCC_DFSDM1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**DFSDM1 GPIO Configuration
    PE4     ------> DFSDM1_DATIN3
    PC2_C     ------> DFSDM1_CKOUT
    PB1     ------> DFSDM1_DATIN1
    PC11     ------> DFSDM1_DATIN5
    PB9     ------> DFSDM1_DATIN7
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_DFSDM1;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_DFSDM1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_DFSDM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_DFSDM1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_DFSDM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN DFSDM1_MspInit 1 */

  /* USER CODE END DFSDM1_MspInit 1 */
  DFSDM1_Init++;
  }
}

void HAL_DFSDM_FilterMspDeInit(DFSDM_Filter_HandleTypeDef* dfsdm_filterHandle)
{

  DFSDM1_Init-- ;
  if(DFSDM1_Init == 0)
    {
  /* USER CODE BEGIN DFSDM1_MspDeInit 0 */

  /* USER CODE END DFSDM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DFSDM1_CLK_DISABLE();

    /**DFSDM1 GPIO Configuration
    PE4     ------> DFSDM1_DATIN3
    PC2_C     ------> DFSDM1_CKOUT
    PB1     ------> DFSDM1_DATIN1
    PC11     ------> DFSDM1_DATIN5
    PB9     ------> DFSDM1_DATIN7
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_2|GPIO_PIN_11);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1|GPIO_PIN_9);

    /* DFSDM1 DMA DeInit */
    HAL_DMA_DeInit(dfsdm_filterHandle->hdmaInj);
    HAL_DMA_DeInit(dfsdm_filterHandle->hdmaReg);

  /* USER CODE BEGIN DFSDM1_MspDeInit 1 */

  /* USER CODE END DFSDM1_MspDeInit 1 */
  }
}

void HAL_DFSDM_ChannelMspDeInit(DFSDM_Channel_HandleTypeDef* dfsdm_channelHandle)
{

  DFSDM1_Init-- ;
  if(DFSDM1_Init == 0)
    {
  /* USER CODE BEGIN DFSDM1_MspDeInit 0 */

  /* USER CODE END DFSDM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DFSDM1_CLK_DISABLE();

    /**DFSDM1 GPIO Configuration
    PE4     ------> DFSDM1_DATIN3
    PC2_C     ------> DFSDM1_CKOUT
    PB1     ------> DFSDM1_DATIN1
    PC11     ------> DFSDM1_DATIN5
    PB9     ------> DFSDM1_DATIN7
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_2|GPIO_PIN_11);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1|GPIO_PIN_9);

  /* USER CODE BEGIN DFSDM1_MspDeInit 1 */

  /* USER CODE END DFSDM1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
