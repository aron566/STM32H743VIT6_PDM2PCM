/**
 *  @file protocol_frame.c
 *
 *  @date 2021-01-01
 *
 *  @author zgl
 *
 *  @copyright 爱谛科技研究院.
 *
 *  @brief 通讯协议报文解析
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
#include "protocol_frame.h"
#include "utilities_crc.h"
#include "UART_Port.h"
/** Private typedef ----------------------------------------------------------*/
/*报文检测结果*/
typedef struct
{
	uint16_t DataLen;
	uint16_t DataOffset;
	uint8_t* frame_data;
}FRAME_CHECK_RESULT_Typedef_t;

/*帧检测状态*/
typedef enum
{
  LONG_FRAME_CHECK_OK = 0,/**< 长帧检测正常*/
  LONG_FRAME_CRC_ERROR,   /**< 长帧CRC错误*/
  SHORT_FRAME_CHECK_OK,   
  SHORT_FRAME_CRC_ERROR,
  FRAME_DATA_NOT_FULL,    /**< 帧不全*/
  UNKNOW_FRAME_ERROR,     /**< 未知帧*/
}FRAME_CHECK_STATUS_Typedef_t;

/*返回类型*/
typedef enum RETURN_TYPE
{
  RETURN_OK = 0,
  RETURN_ERROR,
}RETURN_TYPE_Typedef_t;

/*报文处理映射*/
typedef RETURN_TYPE_Typedef_t (*CMD_CALLBACK_FUNC_t)(uint16_t type, uint16_t len, uint8_t* data);
typedef struct
{
    uint16_t cmd;
    CMD_CALLBACK_FUNC_t func;
}COMMAND_PROCESS_MAP_Typedef_t;

/*发送报文*/
typedef struct
{
	int rsp_flag;
	int rsp_type;
	int rsp_cmd;
	int rsp_rst;
	int rsp_size;
	uint8_t *rsp_data;
}TX_INFO_Typedef_t;

/** Private macros -----------------------------------------------------------*/
#define SOFTWARE_V_H                  2
#define SOFTWARE_V_L                  1
#define SOFTWARE_V                    (SOFTWARE_V_H << 4 | SOFTWARE_V_L)
      
#define GET_SOFT_VERION               (uint8_t)0x09
#define SWITCH_CTR                    (uint8_t)0x06
#define PARA_RX                       (uint8_t)0x03

/*帧类型*/
#define SHORT_FRAME_TYPE              0x5A
#define LONG_FRAME_TYPE               0xA5

#define FRAME_SIZE_MAX                256
#define FRAME_SIZE_MIN                10/**< 最小帧大小*/
#define IS_LESS_MIN_FRAME_SIZE(size)  ((size < FRAME_SIZE_MIN)?1:0)
#define LONG_FRAME_PACKAGE_LEN(len)   (len + FRAME_SIZE_MIN)
#define SHORT_FRAME_PACKAGE_LEN       FRAME_SIZE_MIN
/** Public variables ---------------------------------------------------------*/
/*算法参数*/
ALGORITHM_PAR_Typedef_t g_Algorithm_Par = 
{
  .vol = 0
};
/*算法选择*/
ALGORIYHM_FUNCTION_Typdef_t g_Algorithm_Func_Select = FUNCTON_MAX;
/** Private variables --------------------------------------------------------*/
static Uart_Dev_Handle_t *uart_opt_handle = NULL;/**< 串口操作句柄*/

/** Private function prototypes ----------------------------------------------*/
/*回复处理*/
static RETURN_TYPE_Typedef_t get_soft_version(uint16_t type, uint16_t len, uint8_t* data);
static RETURN_TYPE_Typedef_t get_arithm_para(uint16_t type, uint16_t len, uint8_t* data);
static RETURN_TYPE_Typedef_t set_func(uint16_t type, uint16_t len, uint8_t* data);

/*解析处理*/
static RETURN_TYPE_Typedef_t decode_frame_data(TX_INFO_Typedef_t *tx_inf);
static FRAME_CHECK_STATUS_Typedef_t frame_check_parse(CQ_handleTypeDef *cb, FRAME_CHECK_RESULT_Typedef_t *result);
static void reply_data_tx(TX_INFO_Typedef_t *inf);
static int16_t get_command_index(uint8_t cmd);
static RETURN_TYPE_Typedef_t get_arithm_para(uint16_t type, uint16_t len, uint8_t* data);

/** Private constants --------------------------------------------------------*/
/***********when in the cmd_list add a new cmd,please add a mate func  *********/
static const COMMAND_PROCESS_MAP_Typedef_t uart_cmd_list[] = 
{
  {GET_SOFT_VERION, get_soft_version},
  {PARA_RX, get_arithm_para},
  {SWITCH_CTR, set_func}
};

static const uint8_t uart_cmd_num = sizeof(uart_cmd_list)/sizeof( uart_cmd_list[0]);
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
	* @brief   协议解析
  * @param   [out]tx_inf 发送数据信息.
	* @return  RETURN_TYPE_Typedef_t
	* @author  zgl
	* @version V1.0
	* @date    2021-1-5
	******************************************************************
	*/
static RETURN_TYPE_Typedef_t decode_frame_data(TX_INFO_Typedef_t *tx_inf)
{ 
  FRAME_CHECK_RESULT_Typedef_t result;
  
  /*串口操作句柄为空时直接返回*/
  if(uart_opt_handle == NULL || tx_inf == NULL)
  {
    return RETURN_ERROR;
  }
  
  /*报文检测*/
  FRAME_CHECK_STATUS_Typedef_t check_state = frame_check_parse(uart_opt_handle->cb, &result);
  if(check_state != LONG_FRAME_CHECK_OK || check_state != SHORT_FRAME_CHECK_OK)
  {
    return RETURN_ERROR;
  }
   
  /*获得功能码*/
  uint8_t cmd = result.frame_data[3];
  uint16_t type = result.frame_data[5] << 8;
  type += result.frame_data[4];
  uint16_t len = result.DataLen;
  uint8_t *data = result.frame_data + result.DataOffset;

  /*调用功能*/
  int16_t idx = get_command_index(cmd);
  if(idx == -1)
  {
    return RETURN_ERROR;
  }
  RETURN_TYPE_Typedef_t ret = uart_cmd_list[idx].func(type, len, data);	
  if(ret != RETURN_OK)
  {
    return RETURN_ERROR;
  }
  
  /*填充上报数据*/
  switch(cmd)
  {
   case GET_SOFT_VERION:			
    tx_inf->rsp_flag = 1;
    tx_inf->rsp_cmd = cmd;
    tx_inf->rsp_type = SHORT_FRAME_TYPE;
    tx_inf->rsp_rst =  SOFTWARE_V;
    return RETURN_OK;
   case SWITCH_CTR:
    tx_inf->rsp_flag = 1;
    tx_inf->rsp_cmd = cmd;
    tx_inf->rsp_type = SHORT_FRAME_TYPE;
    tx_inf->rsp_rst =  0x01;
    return RETURN_OK;							
   case PARA_RX:
    tx_inf->rsp_flag = 1;
    tx_inf->rsp_cmd = cmd;
    tx_inf->rsp_type = LONG_FRAME_TYPE;
    tx_inf->rsp_rst =  0x01;	
    return RETURN_OK;
   default:
     break;
  }
  return RETURN_ERROR;
}

/**
	******************************************************************
	* @brief   回复数据
  * @param   [out]tx_inf 发送数据信息.
	* @return  None.
	* @author  zgl
	* @version V1.0
	* @date    2021-1-5
	******************************************************************
	*/
static void reply_data_tx(TX_INFO_Typedef_t *inf)
{
    uint8_t buf[6] = {0x75, 0xAA};
		if(inf->rsp_type == SHORT_FRAME_TYPE)
		{
			  buf[2] = SHORT_FRAME_TYPE;
				buf[3] = inf->rsp_cmd;
				buf[4] = inf->rsp_rst;
				Uart_Port_Transmit_Data(uart_opt_handle, buf, 5);
		}
		else if(inf->rsp_type == LONG_FRAME_TYPE)
		{
//			  buf[2] = LONG_FRAME;
//				buf[3] = inf->rsp_cmd;			   						 				 	 
		}
}

/**
  ******************************************************************
  * @brief   报文检查
  * @param   [in]cb 缓冲区句柄
  * @param   [out]result 报文检测结果
  * @return  FRAME_CHECK_STATUS_Typedef_t 报文检测状态
  * @author  zgl
  * @version V1.0
  * @date    2020-01-01
  ******************************************************************
  */
static FRAME_CHECK_STATUS_Typedef_t frame_check_parse(CQ_handleTypeDef *cb, FRAME_CHECK_RESULT_Typedef_t *result)
{
  static uint8_t databuf[FRAME_SIZE_MAX];
  
  /*判断帧头,跳过无效帧头*/
  if(CQ_ManualGet_Offset_Data(cb, 0) != 0x7A || CQ_ManualGet_Offset_Data(cb, 1) != 0x55)
  {
    if(CQ_skipInvaildU8Header(cb, 0x7A) == 0)
    {
      return FRAME_DATA_NOT_FULL;
    }
  }
  
  /*判断可读数据是否小于最小帧长*/
  uint32_t rxnb = CQ_getLength(cb); 
  if(IS_LESS_MIN_FRAME_SIZE(rxnb))
  {
    return FRAME_DATA_NOT_FULL;
  }

  /*判断帧类型*/
  uint8_t frame_type = CQ_ManualGet_Offset_Data(cb, 2);
  uint16_t length = 0;
  uint32_t package_len = 0;
  switch(frame_type)
  {
    /*长帧*/
    case LONG_FRAME_TYPE:
      length = CQ_ManualGet_Offset_Data(cb, 7) << 8;
      length += CQ_ManualGet_Offset_Data(cb, 6);
      package_len = LONG_FRAME_PACKAGE_LEN(length);
      package_len = (package_len > FRAME_SIZE_MAX)?FRAME_SIZE_MAX:package_len;
      if(rxnb >= package_len)
      {
        CQ_ManualGetData(cb, databuf, package_len);
        if(modbus_get_crc_result(databuf, package_len-2) == true)
        {
          result->frame_data = databuf;
          result->DataLen = length;
          result->DataOffset = 8;
          CQ_ManualOffsetInc(cb, package_len);
          return LONG_FRAME_CHECK_OK;
        }
      }
      break;
    /*短帧*/
    case SHORT_FRAME_TYPE:
      package_len = SHORT_FRAME_PACKAGE_LEN;
      package_len = (package_len > FRAME_SIZE_MAX)?FRAME_SIZE_MAX:package_len;
      if(rxnb >= SHORT_FRAME_PACKAGE_LEN)
      {
        CQ_ManualGetData(cb, databuf, package_len);
        if(modbus_get_crc_result(databuf, package_len-2) == true)
        {
          result->frame_data = databuf;
          result->DataLen = 2;
          result->DataOffset = 6;
          CQ_ManualOffsetInc(cb, package_len);
          return SHORT_FRAME_CHECK_OK;
        }
      }
      break;
    default:
      /*TODO*/
      CQ_ManualOffsetInc(cb, 1);
      return UNKNOW_FRAME_ERROR;
  }
  
  CQ_ManualOffsetInc(cb, 1);
  return SHORT_FRAME_CRC_ERROR;
  
}

/**
  ******************************************************************
  * @brief   获取功能码索引号
  * @param   [in]cmd 功能码
  * @return  AnalyzRslt
  * @author  zgl
  * @version V1.0
  * @date    2020-01-01
  ******************************************************************
  */
static int16_t get_command_index(uint8_t cmd)
{
  for(int16_t index = 0; index < uart_cmd_num; index++)
  {
      if(cmd == uart_cmd_list[index].cmd)
      {
          return index;					
      }
  }
  return -1;
}

/**
  ******************************************************************
  * @brief   功能-获取软件版本
  * @param   [in]type 类型
  * @param   [in]len 数据长度
  * @param   [in]data 数据
  * @return  RETURN_OK 正常
  * @author  zgl
  * @version V1.0
  * @date    2020-01-01
  ******************************************************************
  */
static RETURN_TYPE_Typedef_t get_soft_version(uint16_t type, uint16_t len, uint8_t* data)
{
  uint8_t sign = data[1];
  if(sign == 1)
  {	
    return RETURN_OK;
  }
  else
  {
    return RETURN_ERROR;
  }
}

/**
  ******************************************************************
  * @brief   功能-设置算法启用选择
  * @param   [in]type 类型
  * @param   [in]len 数据长度
  * @param   [in]data 数据
  * @return  AnalyzRslt
  * @author  zgl
  * @version V1.0
  * @date    2020-01-01
  ******************************************************************
  */
static RETURN_TYPE_Typedef_t set_func(uint16_t type, uint16_t len, uint8_t* data)
{
  uint8_t indx = data[1];
  if(indx == 0)
  {	
    return RETURN_ERROR;
  }
  else
  {
    g_Algorithm_Func_Select = (ALGORIYHM_FUNCTION_Typdef_t)indx;
    return RETURN_OK;
  }
}

/**
	******************************************************************
	* @brief   功能-操作算法参数
	* @param   [in]type.
	* @param   [in]len.
	* @param   [in]data.
	* @return  None
	* @author  zgl
	* @version V1.0
	* @date    2021-1-5
	******************************************************************
	*/
static RETURN_TYPE_Typedef_t get_arithm_para(uint16_t type, uint16_t len, uint8_t* data)
{
  uint16_t class_para = type;
  switch(class_para)
  {
    case 0x0100:
      memcpy(g_Algorithm_Par.vol, data, len);
      break;
    case 0x0200:
      memcpy(g_Algorithm_Par.eq, data, len);
      break;
    case 0x0300:
      memcpy(g_Algorithm_Par.wdr, data, len);
      init_wdrc_ab();
      break;
    case 0x0400:
      memcpy(g_Algorithm_Par.agc, data, len);
      init_agc_ab();
      break;
    case 0x0500:
      memcpy(g_Algorithm_Par.level, data, len);
      break;
    default:
      return RETURN_ERROR;
  }
  return RETURN_OK;
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
	* @brief   启动协议栈
	* @param   [in]None.
	* @return  None
	* @author  zgl
	* @version V1.0
	* @date    2021-1-5
	******************************************************************
	*/
void Protocol_Stack_Start(void)
{
	TX_INFO_Typedef_t tx_inf;
  RETURN_TYPE_Typedef_t ret = decode_frame_data(&tx_inf);
  if(ret != RETURN_OK)
  {
     return;
  }

  /*发送数据*/
	reply_data_tx(&tx_inf);
}

/**
	******************************************************************
	* @brief   协议栈初始化
	* @param   [in]None.
	* @return  None
	* @author  zgl
	* @version V1.0
	* @date    2021-1-5
	******************************************************************
	*/
void Protocol_Stack_Init(void)
{
  /*获取串口操作句柄*/
  uart_opt_handle = Uart_Port_Get_Handle(UART_NUM_1);
  if(uart_opt_handle == NULL)
  {
    printf("get uart opt handle faild.\n");
    return;
  }
}

#ifdef __cplusplus ///<end extern c
}
#endif
/******************************** End of file *********************************/
