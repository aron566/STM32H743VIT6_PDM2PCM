/**
 *  @file PDM_Datacov.c
 *
 *  @date 2021-01-19
 *
 *  @author aron566
 *
 *  @copyright ���пƼ��о�Ժ.
 *
 *  @brief PDM����ת��
 *
 *  @details 1��PDMΪ�����ܶȵ��� ���������ܶȶ�Ӧ��ģ���źŵķ��ȡ�
 *              ������1s��Ӧ�ڸߣ���������ֵ����������0s��Ӧ�ڵͣ���������ֵ�������1s��0s��Ӧ�ڷ���ֵ0
 *           2��PCMΪ������� ��PCM�ź��У�����ķ���ֵ������Ϊ����
 *
 *  @version V1.0
 */
#ifdef __cplusplus ///<use C compiler
extern "C" {
#endif
/** Includes -----------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "PDM_Datacov.h"
#include "pdm2pcm.h"
/** Private typedef ----------------------------------------------------------*/
/*PDMתPCM����ֵ*/
typedef enum
{
  USER_PDM_FILTER_NO_ERROR              = 0x0000, ///< �޴���
  USER_PDM_FILTER_ENDIANNESS_ERROR      = 0x0001, ///< ��֧�ֵ��ֽ���
  USER_PDM_FILTER_BIT_ORDER_ERROR       = 0x0002, ///< ��֧�ֵ�λ��
  USER_PDM_FILTER_CRC_LOCK_ERROR        = 0x0004, ///< Ŀ�겻��STM32
  USER_PDM_FILTER_DECIMATION_ERROR      = 0x0008, ///< ��֧�ֵĳ�ȡ����
  USER_PDM_FILTER_INIT_ERROR            = 0x0010, ///< -
  USER_PDM_FILTER_CONFIG_ERROR          = 0x0020, ///< -
  USER_PDM_FILTER_GAIN_ERROR            = 0x0040, ///< ��֧�ֵ���˷�����
  USER_PDM_FILTER_SAMPLES_NUMBER_ERROR  = 0x0080, ///< ��֧�ֵ�������
}PDM2PCM_ERROR_CODE_Typedef_t;
/** Private macros -----------------------------------------------------------*/
#define ENABLE_DEBUG    1
/** Private constants --------------------------------------------------------*/
/** Public variables ---------------------------------------------------------*/
/** Private variables --------------------------------------------------------*/

/** Private function prototypes ----------------------------------------------*/

/** Private user code --------------------------------------------------------*/

/** Private application code -------------------------------------------------*/
/*******************************************************************************
*
*       Static code
*
********************************************************************************
*/
#if ENABLE_DEBUG
/**
  ******************************************************************
  * @brief   PDM��ת��PCM���������������
  * @param   [in]code ������
  * @return  code
  * @author  aron566
  * @version V1.0
  * @date    2021-01-19
  ******************************************************************
  */
static uint8_t Decode_PDM2PCM_Return_Code(uint8_t code)
{
  if(code == 0)
  {
//    printf("pdm to pcm no error.\r\n");
    return code;
  }
  
  if(code & USER_PDM_FILTER_ENDIANNESS_ERROR)
  {
    printf("��֧�ֵ��ֽ���.\r\n");
  }
  else if(code & USER_PDM_FILTER_BIT_ORDER_ERROR)
  {
    printf("��֧�ֵ�λ��.\r\n");
  }
  else if(code & USER_PDM_FILTER_CRC_LOCK_ERROR)
  {
    printf("Ŀ�겻��STM32.\r\n");
  }
  else if(code & USER_PDM_FILTER_DECIMATION_ERROR)
  {
    printf("��֧�ֵĳ�ȡ����.\r\n");
  }
  else if(code & USER_PDM_FILTER_GAIN_ERROR)
  {
    printf("��֧�ֵ���˷�����.\r\n");
  }
  else if(code & USER_PDM_FILTER_SAMPLES_NUMBER_ERROR)
  {
    printf("��֧�ֵ�������.\r\n");
  }
  return code;
}
#endif

/** Public application code --------------------------------------------------*/
/*******************************************************************************
*
*       Public code
*
********************************************************************************
*/
/**
  ******************************************************************
  * @brief   PDM��ת��PCM�������ӿ�
  * @param   [in]PDMBuf PDM������
  * @param   [out]PCMBuf PCM�������洢��
  * @return  ת������״̬��
  * @author  aron566
  * @version V1.0
  * @date    2021-01-19
  ******************************************************************
  */
uint8_t PDM_To_PCM_Stream(uint16_t *PDMBuf, uint16_t *PCMBuf)
{
#if ENABLE_DEBUG
  return Decode_PDM2PCM_Return_Code(MX_PDM2PCM_Process(PDMBuf, PCMBuf));
#else
  return MX_PDM2PCM_Process(PDMBuf, PCMBuf);
#endif
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
