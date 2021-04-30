/**
 *  @file CS43L12_Driver.c
 *
 *  @date 2021-03-10
 *
 *  @author aron566
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief CS43L12 DAC芯片驱动接口
 *
 *  @details 1、CS43L12
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "CS43L12_Driver.h"
#include "main.h"
#include "SAI_Port.h"
/** Private typedef ----------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
/** Private macros -----------------------------------------------------------*/
#define ENABLE_CS43L12_DEBUG    1/**< 启动打印调试*/
/*CS43L12设备I2C地址，7Bit地址模式[即：100101<AD0><R/W>]，最后一位由AD0引脚控制*/
#define AD0_PIN_LOW_LEVEL       1   /**< 初始化为下拉低电平状态*/  
#if AD0_PIN_LOW_LEVEL
  #define DEV_ADDR              0x94
#else
  #define DEV_ADDR              0x96
#endif
/*CS43L12设备I2C写操作地址*/
#define DEV_ADDR_WRITE          (DEV_ADDR|0x00)
/*CS43L12设备I2C读操作地址*/
#define DEV_ADDR_READ           (DEV_ADDR|0x01)

/*CS43L12寄存器地址*/
#define REG_CHIP_ID             0x01
#define REG_POWER_CTL           0x02
#define REG_SPEED_CTL           0x03
#define REG_INTERFACE_CFG       0x04
#define REG_DAC_OUT_CTL         0x08
#define REG_DAC_CTL             0x09/**< default:bit7:0 bit6:0 PCM数据直接DAC输出 01b则通过处理引擎*/

/*The DATA_SEL[1:0] bit in Reg09h must be set to 01b to enable function control in this register.*/
#define REG_PCMA_MIXER_CTL      0x10
#define REG_PCMB_MIXER_CTL      0x11

#define REG_BEEP_FQ_TIMG_CFG    0x12
#define REG_BEEP_OFF_TIME_VOL   0x13

#define REG_BEEP_CFG_TONE_CFG   0x14

#define REG_TONE_CTL            0x15
        
#define REG_A_OUT_VOL_CTL       0x16/**< 输出音量控制*/
#define REG_B_OUT_VOL_CTL       0x17

#define REG_PCM_MIXER_CH_SEL    0x18

#define REG_LIMIT_THRESHOLD_SZC 0x19

#define REG_LIMIT_RELEASE_RATE  0x1A
#define REG_LIMIT_ATTACK_RATE   0x1B

#define REG_STATUS_READ_ONLY    0x20

#define REG_CHARGE_PUMP_FQ      0x21
        
/*16BIT位*/        
#define REG_BIT0                (1U<<0)
#define REG_BIT1                (1U<<1)
#define REG_BIT2                (1U<<2)
#define REG_BIT3                (1U<<3)
#define REG_BIT4                (1U<<4)
#define REG_BIT5                (1U<<5)
#define REG_BIT6                (1U<<6)
#define REG_BIT7                (1U<<7)
#define REG_BIT8                (1U<<8)
#define REG_BIT9                (1U<<9)
#define REG_BIT10               (1U<<10)
#define REG_BIT11               (1U<<11)
#define REG_BIT12               (1U<<12)
#define REG_BIT13               (1U<<13)
#define REG_BIT14               (1U<<14)
#define REG_BIT15               (1U<<15)

#define CS43L12_DAC_HANDLE      &hi2c1
/** Private constants --------------------------------------------------------*/

/** Private variables --------------------------------------------------------*/
static uint8_t Send_Val_Buf[2];
static uint8_t Reg_Val_Buf[2];
/** Private function prototypes ----------------------------------------------*/
#if ENABLE_CS43L12_DEBUG
static void Display_Chip_ID(void);
#endif
static void Set_Device_AD0(void);
static void Power_On_Off(CS43L12_DRIVER_CH_Typedef_t Ch, CS43L12_DRIVER_POWER_Typedef_t On_Off);
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
  * @brief   CS43L12 I2C发送通讯接口
  * @param   [in]DevAddress 设备地址.
  * @param   [in]pData 发送数据地址.
  * @param   [in]Size 发送大小字节数.
  * @param   [in]Timeout 等待超时时间.
  * @return  -1 错误.
  * @author  aron566
  * @version V1.0
  * @date    2021-04-29
  ******************************************************************
  */
static int I2C_Master_Transmit_Port(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  if(HAL_OK != HAL_I2C_Master_Transmit(CS43L12_DAC_HANDLE, DevAddress, pData, Size, Timeout))
  {
    return -1;
  }
  return 0;
}

/**
  ******************************************************************
  * @brief   CS43L12 I2C接收通讯接口
  * @param   [in]DevAddress 设备地址.
  * @param   [in]pData 接收数据地址.
  * @param   [in]Size 接收大小字节数.
  * @param   [in]Timeout 等待超时时间.
  * @return  -1 错误.
  * @author  aron566
  * @version V1.0
  * @date    2021-04-29
  ******************************************************************
  */
static int I2C_Master_Receive_Port(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  if(HAL_OK != HAL_I2C_Master_Receive(CS43L12_DAC_HANDLE, DevAddress, pData, Size, Timeout))
  {
    return -1;
  }
  return 0;
}

#if ENABLE_CS43L12_DEBUG
/**
  ******************************************************************
  * @brief   CS43L12 chip id打印
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-10
  ******************************************************************
  */
static inline void Display_Chip_ID(void)
{
  Send_Val_Buf[0] = REG_CHIP_ID;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  printf("Chip ID REG:%02X, VAL:%02X.\r\n", Send_Val_Buf[0], Reg_Val_Buf[0]);/**< 0xD8 0xD9*/
}
#endif

/**
  ******************************************************************
  * @brief   CS43L12 上电初始化
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-11
  ******************************************************************
  */
static inline void Device_PowerOn_Init(void)
{
  /*HOLD RESET PIN LOW LEVEL 10ms*/
  HAL_GPIO_WritePin(DAC_RST_GPIO_Port, DAC_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(DAC_RST_GPIO_Port, DAC_RST_Pin, GPIO_PIN_SET);
}

/**
  ******************************************************************
  * @brief   CS43L12 进入配置模式
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-11
  ******************************************************************
  */
static inline void Entry_Config_Mode(void)
{
  /*读取电源设置状态*/
  Send_Val_Buf[0] = REG_POWER_CTL;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  
  /*开启PDN Bit解锁设置*/
  Send_Val_Buf[1] = Reg_Val_Buf[0] | (REG_BIT0);
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
  
#if ENABLE_CS43L12_DEBUG
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  printf("Entry DAC Power REG:%02X, VAL:%02X.\r\n", Send_Val_Buf[0], Reg_Val_Buf[0]);
#endif
}

/**
  ******************************************************************
  * @brief   CS43L12 退出配置模式
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-11
  ******************************************************************
  */
static inline void Exit_Config_Mode(void)
{
  /*读取电源设置状态*/
  Send_Val_Buf[0] = REG_POWER_CTL;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  
  /*关闭PDN Bit上锁*/
  Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT0);
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);

#if ENABLE_CS43L12_DEBUG
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  printf("Exit DAC Power REG:%02X, VAL:%02X.\r\n", Send_Val_Buf[0], Reg_Val_Buf[0]);
#endif
}

/**
  ******************************************************************
  * @brief   设置设备地址
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-10
  ******************************************************************
  */
static inline void Set_Device_AD0(void)
{
#if AD0_PIN_LOW_LEVEL
  HAL_GPIO_WritePin(DAC_ADDR_GPIO_Port, DAC_ADDR_Pin, GPIO_PIN_RESET);
#else
  HAL_GPIO_WritePin(DAC_ADDR_GPIO_Port, DAC_ADDR_Pin, GPIO_PIN_SET);
#endif
}

/**
  ******************************************************************
  * @brief   启停设备DACx电源
  * @param   [in]Ch 声道编号
  * @param   [in]On_Off 启动或关闭
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-10
  ******************************************************************
  */
static void Power_On_Off(CS43L12_DRIVER_CH_Typedef_t Ch, CS43L12_DRIVER_POWER_Typedef_t On_Off)
{
  /*读取电源设置状态*/
  Send_Val_Buf[0] = REG_POWER_CTL;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  Send_Val_Buf[1] = Reg_Val_Buf[0];
  
  /*关闭*/
  if(On_Off == CS43L12_DRIVER_POWER_OFF)
  {
    /*左*/
    if(Ch == CS43L12_DRIVER_LEFT_CH)
    {
      Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT5);
    }
    /*右*/
    if(Ch == CS43L12_DRIVER_RIGHT_CH)
    {
      Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT6);
    }
  }
  /*打开*/
  else
  {
    /*左*/
    if(Ch == CS43L12_DRIVER_LEFT_CH)
    {
      Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT5);
    }
    /*右*/
    if(Ch == CS43L12_DRIVER_RIGHT_CH)
    {
      Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT6);
    }
  }
  
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
  
#if ENABLE_CS43L12_DEBUG
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  printf("DAC Power REG:%02X, VAL:%02X.\r\n", Send_Val_Buf[0], Reg_Val_Buf[0]);
#endif
}

/**
  ******************************************************************
  * @brief   设置主时钟速率
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-10
  ******************************************************************
  */
static void DAC_CLKSpeed_Set(void)
{
  /*读取参数*/
  Send_Val_Buf[0] = REG_SPEED_CTL;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  Send_Val_Buf[1] = Reg_Val_Buf[0];

  /*设置MCLK/2*/
  Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT0);
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
}

/**
  ******************************************************************
  * @brief   设置DAC输出
  * @param   [in]Gain_dB 增益值 0 - 7，0xFF时静音使能
  * @param   [in]Channel 通道号
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-04-29
  ******************************************************************
  */
static void DAC_Volume_Set(uint8_t Gain_dB, CS43L12_DRIVER_CH_Typedef_t Channel)
{
  /*读取参数*/
  Send_Val_Buf[0] = REG_DAC_OUT_CTL;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  Send_Val_Buf[1] = Reg_Val_Buf[0];

  /*静音使能控制*/
  if(Gain_dB == 0xFF)
  {
    if(Channel == CS43L12_DRIVER_LR_CH)
    {
      Send_Val_Buf[1] = Send_Val_Buf[1] | ((REG_BIT0) | (REG_BIT1));
    }
    else if(Channel == CS43L12_DRIVER_LEFT_CH)
    {
      Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT0);
    }
    else if(Channel == CS43L12_DRIVER_RIGHT_CH)
    {
      Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT1);
    }
    I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
    return;
  }

  /*左右通道音量独立控制*/
  Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT4);

  /*设置耳机模拟增益Bit7-5：110b 即 1倍 ，Datasheet：Page 42*/
  Gain_dB = ((Gain_dB << 5)&0xE0);
  Send_Val_Buf[1] |= Gain_dB;
  // Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT7);
  // Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT6);
  // Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT5);
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
}

/**
  ******************************************************************
  * @brief   设置DAC功能启用
  * @param   [in]None.
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-04-29
  ******************************************************************
  */
static void DAC_Config(void)
{
  /*读取参数*/
  Send_Val_Buf[0] = REG_DAC_CTL;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  Send_Val_Buf[1] = Reg_Val_Buf[0];

  /*DAC数据通道：00透传 ，01经过调节*/
  Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT7);
  Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT6);

  /*冻结参数修改：0允许参数修改立即保存，1允许参数修改暂不保存*/
  Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT5);

  /*44.4Khz数字滤波器使能：失能*/
  Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT3);

  /*启用自动静音：启用*/
  Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT2);
  
  /*调节响应过程：默认10立即响应设置， 01输出过0时响应， 10平滑缓慢响应，11过零及平滑缓慢响应*/
  Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT1);
  Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT0);
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
}

/**
  ******************************************************************
  * @brief   设置混响器输出增益
  * @param   [in]Gain_dB 增益 -51.5 ~ +12dB
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-04-29
  ******************************************************************
  */
static void MIXPath_Volume_Set(float Gain_dB)
{
  if(Gain_dB >= 0 && Gain_dB <= 12.f)
  {
    Send_Val_Buf[1] = (uint8_t)(Gain_dB * 2.f);
  }
  else if(Gain_dB < 0 && Gain_dB >= -51.5f)
  {
    Send_Val_Buf[1] = (uint8_t)(128.f + Gain_dB * 2.f);
  }

  /*混频静音使能：使能*/
  Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT7);

  Send_Val_Buf[0] = REG_PCMA_MIXER_CTL;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);

  Send_Val_Buf[0] = REG_PCMB_MIXER_CTL;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
}

/**
  ******************************************************************
  * @brief   设置音效输出增益
  * @param   [in]Treble_FQ 高音频率 5、7、10、15kHz
  * @param   [in]Bass_FQ 低音频率 50、100、200、250Hz
  * @param   [in]Treble_dB 高音增益 -10.5 ~ +12dB
  * @param   [in]Bass_dB 低音增益 -10.5 ~ +12dB
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-04-29
  ******************************************************************
  */
static void Tone_EQ_Set(uint16_t Treble_FQ, uint16_t Bass_FQ, float Treble_dB, float Bass_dB)
{
  /*读取参数*/
  Send_Val_Buf[0] = REG_BEEP_CFG_TONE_CFG;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  Send_Val_Buf[1] = Reg_Val_Buf[0];

  /*使能音效控制*/
  Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT0);

  /*设置高音频率*/
  switch(Treble_FQ)
  {
  case 5000:
    Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT4);
    Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT3);
    break;
  case 7000:
    Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT4);
    Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT3);
    break;
  case 10000:
    Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT4);
    Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT3);
    break;
  case 15000:
    Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT4);
    Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT3);
    break;
  default:
    break;
  }
  /*设置低音频率*/
  switch(Bass_FQ)
  {
  case 50:
    Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT2);
    Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT1);
    break;
  case 100:
    Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT2);
    Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT1);
    break;
  case 200:
    Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT2);
    Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT1);
    break;
  case 250:
    Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT2);
    Send_Val_Buf[1] = Send_Val_Buf[1] | (REG_BIT1);
    break;
  default:
    break;
  }
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);

  /*设置高音增益*/
  if(Treble_dB >= 0 && Treble_dB <= 12.f)
  {
    Send_Val_Buf[1] = (((uint8_t)(8.f - Treble_dB / 1.5f))&0x0F);
  }
  else if(Treble_dB < 0 && Treble_dB >= -10.5f)
  {
    Send_Val_Buf[1] = (((uint8_t)(8.f + Treble_dB / -1.5f))&0x0F);
  }
  Send_Val_Buf[1] <<= 4;

  /*设置低音增益*/
  if(Bass_dB >= 0 && Bass_dB <= 12.f)
  {
    Send_Val_Buf[1] |= (((uint8_t)(8.f - (Bass_dB / 1.5f)))&0x0F);
  }
  else if(Bass_dB < 0 && Bass_dB >= -10.5f)
  {
    Send_Val_Buf[1] |= (((uint8_t)(8.f + Bass_dB / -1.5f))&0x0F);
  }

  /*混频静音使能：使能*/
  Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT7);

  Send_Val_Buf[0] = REG_TONE_CTL;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
}

/**
  ******************************************************************
  * @brief   设置模拟输出增益
  * @param   [in]db 增益 -102 ~ +12dB
  * @param   [in]Channel 通道号
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-04-29
  ******************************************************************
  */
static void AOutPath_Volume_Set(float Gain_dB, CS43L12_DRIVER_CH_Typedef_t Channel)
{
  if(Gain_dB >= 0 && Gain_dB <= 12.f)
  {
    Send_Val_Buf[1] = (uint8_t)(Gain_dB * 2.f);
  }
  else if(Gain_dB < 0 && Gain_dB >= -102.f)
  {
    Send_Val_Buf[1] = (uint8_t)(256.f + Gain_dB * 2.f);
  }

  if(Channel == CS43L12_DRIVER_LEFT_CH)
  {
    Send_Val_Buf[0] = REG_A_OUT_VOL_CTL;
    I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
  }
  else if(Channel == CS43L12_DRIVER_RIGHT_CH)
  {
    Send_Val_Buf[0] = REG_B_OUT_VOL_CTL;
    I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
  }
  else if(Channel == CS43L12_DRIVER_LR_CH)
  {
    Send_Val_Buf[0] = REG_A_OUT_VOL_CTL;
    I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
    Send_Val_Buf[0] = REG_B_OUT_VOL_CTL;
    I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);  
  }
}

/**
  ******************************************************************
  * @brief   设置设备参数
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-10
  ******************************************************************
  */
static inline void Set_Cfg(void)
{  
  /*读取参数*/
  Send_Val_Buf[0] = REG_INTERFACE_CFG;
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
#if ENABLE_CS43L12_DEBUG
  printf("Interface CFG REG:%02X, VAL:%02X.\r\n", Send_Val_Buf[0], Reg_Val_Buf[0]);
#endif
  
  /*设置为Slave*/
  Send_Val_Buf[1] = Reg_Val_Buf[0] & (~REG_BIT6);
  /*设置数据格式 101*/
  //Send_Val_Buf[1] = Send_Val_Buf[1] | REG_BIT5;
  Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT5);
  Send_Val_Buf[1] = Send_Val_Buf[1] & (~REG_BIT4);
  Send_Val_Buf[1] = Send_Val_Buf[1] | REG_BIT3;
#if ENABLE_CS43L12_DEBUG
  printf("Set Interface CFG REG:%02X, VAL:%02X.\r\n", Send_Val_Buf[0], Send_Val_Buf[1]);
#endif
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 2, 100);
  
#if ENABLE_CS43L12_DEBUG
  I2C_Master_Transmit_Port(DEV_ADDR_WRITE, Send_Val_Buf, 1, 100);
  I2C_Master_Receive_Port(DEV_ADDR_READ, Reg_Val_Buf, 1, 100);
  printf("After Interface CFG REG:%02X, VAL:%02X.\r\n", Send_Val_Buf[0], Reg_Val_Buf[0]);
#endif
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
  * @brief   CS43L12 驱动初始化
  * @param   [in]None
  * @return  None.
  * @author  aron566
  * @version V1.0
  * @date    2021-03-10
  ******************************************************************
  */
void CS43L12_Driver_Init(void)
{
  /*设备上电初始化*/
  Device_PowerOn_Init();
  
  /*设置设备地址*/
  Set_Device_AD0();
  
  /*打印设备ID*/
#if ENABLE_CS43L12_DEBUG
  Display_Chip_ID();
#endif
  
  /*进入配置模式*/
  Entry_Config_Mode();

  /*配置设备为Slave 右对齐16bit*/
  Set_Cfg();
  
  /*设置通道电源*/
  Power_On_Off(CS43L12_DRIVER_LEFT_CH, CS43L12_DRIVER_POWER_ON);
  Power_On_Off(CS43L12_DRIVER_RIGHT_CH, CS43L12_DRIVER_POWER_ON);
  
  /*DAC速率设置*/
  //DAC_CLKSpeed_Set();
  
  /*DAC功能配置*/
  DAC_Config();
  
  /*设置DAC输出增益*/
  DAC_Volume_Set(6, CS43L12_DRIVER_LR_CH);
  
  /*设置混响增益*/
  MIXPath_Volume_Set(0);
  
  /*设置EQ增益*/
  Tone_EQ_Set(5000, 50, 0, 0);
    
  /*调节音量增益*/
  AOutPath_Volume_Set(6.f, CS43L12_DRIVER_LR_CH);
    
  /*退出配置模式*/
  Exit_Config_Mode();
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
