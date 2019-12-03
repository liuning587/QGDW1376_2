/**
 ******************************************************************************
 * @file      GDW1376_2.c
 * @brief     C Source file of GDW1376_2.c.
 * @details   This file including all API functions's implement of GDW1376_2.c.
 * @copyright
 ******************************************************************************
 */
 
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include "types.h"
#include "maths.h"
#include "../inc/GDW1376_2.h"
#include "chkfrm.h"
#include "AFN.h"
#include "lib.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
#pragma pack(push, 1)
typedef struct
{
    unsigned char type:6;       /**< 通讯方式 */
    unsigned char dir:1;        /**< 方向位,0-下行, 1-上行 */
    unsigned char prm:1;        /**< 启动位,1-启动, 0-从动 */
} ctrl_t;
#pragma pack(pop)

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
#define VER_STR         "v0.0.1"    /**< 版本字符串 */
#define CHANGE_LOG  \
        "-----------------------------------\n"\
        "2014年2月24日:完成解析框架\n"\
        ""

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
static int user_data_pos = 0;
//static unsigned char AFN = 0;
//static unsigned int fn = 0;

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
/**
 ******************************************************************************
 * @brief   输出原始报文
 * @param[in]  *pin         : 输入报文
 * @param[in]   len         : 输入报文长度
 * @param[in]  *pcb         : 回调函数
 * @param[in]  *pline_head  : 每行起始填充字符串
 * @param[in]  *pline_end   : 每行结束填充字符串
 *
 * @return  None
 ******************************************************************************
 */
static void
print_org(const unsigned char *pin,
        unsigned int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;

    pcb(pline_head);
    pcb("--------------------原始报文---------------------");
    pcb(pline_end);
    pcb(pline_head);
    for (i = 0; i < len; i++)
    {
        sprintf(buf, "%02X ", pin[i]);
        pcb(buf);
        if (((i + 1) % 16) == 0)
        {
            pcb(pline_end);
            pcb(pline_head);
        }
    }
    pcb(pline_end);
    pcb(pline_head);
    pcb("-------------------------------------------------");
    pcb(pline_end);
}

/**
 ******************************************************************************
 * @brief   获取通信方式定义
 * @param[in]  commtype : 通信方式值
 *
 * @retval  通信方式定义字符串
 ******************************************************************************
 */
static const char *
get_commtype_str(unsigned char commtype)
{
    if (commtype > 63)
    {
        return "无效通信方式";
    }

    switch (commtype)
    {
    	case 0:
    	    return "保留";
        case 1:
            return "集中式路由载波通信";
        case 2:
            return "分布式路由载波通信";
        case 3:
            return "宽带载波通信";
        case 10:
            return "微功率无线通信";
        case 20:
            return "以太网通信";
        default:
            break;
    }
    return "备用";
}

/**
 ******************************************************************************
 * @brief   获取电能表通道特征
 * @param[in]  value : 电能表通道特征值
 *
 * @retval  电能表通道特征
 ******************************************************************************
 */
static const char *
get_meter_comm_type(unsigned char value)
{
    if (value > 15)
    {
        return "无效的电能表通道特征值";
    }

    switch (value)
    {
        case 1:
            return "物理信道为单相供电,逻辑信道为单信道";
        case 2:
            return "物理信道为单相供电,逻辑信道为两信道";
        case 3:
            return "物理信道为单相供电,逻辑信道为三信道";
        case 4:
            return "物理信道为三相供电,逻辑信道为三信道";
        default:
            break;
    }
    return "保留";
}

/**
 ******************************************************************************
 * @brief   获取电能表通道特征
 * @param[in]  value : 电能表通道特征值
 *
 * @retval  电能表通道特征
 ******************************************************************************
 */
static const char *
get_errcode_type(unsigned char value)
{
    if (value > 15)
    {
        return "无效";
    }

    switch (value)
    {
        case 0:
            return "未编码";
        case 1:
            return "RS编码";
        default:
            break;
    }
    return "保留";
}

/**
 ******************************************************************************
 * @brief   获取AFN名字
 * @param[in]  afn : AFN值
 *
 * @retval  AFN名字
 ******************************************************************************
 */
static const char *
get_AFN_name(unsigned char afn)
{
    switch (afn)
    {
        case 0x00:
            return "确认/否认";
        case 0x01:
            return "初始化";
        case 0x02:
            return "数据转发";
        case 0x03:
            return "查询数据";
        case 0x04:
            return "链路接口检测";
        case 0x05:
            return "控制命令";
        case 0x06:
            return "主动上报";
        case 0x10:
            return "路由查询";
        case 0x11:
            return "路由设置";
        case 0x12:
            return "路由控制";
        case 0x13:
            return "路由数据转发";
        case 0x14:
            return "路由数据抄读";
        case 0x15:
            return "文件传输";
        case 0xf0:
            return "内部调试";
        case 0xf1:
            return "并发抄表";
        default:
            break;
    }
    return "备用";
}
/**
 ******************************************************************************
 * @brief   输出报文结构解析
 * @param[in]  *pin        : 输入报文
 * @param[in]   len        : 输入报文长度
 * @param[in]  *pcb        : 回调函数
 * @param[in]  *pline_head : 每行起始填充字符串
 * @param[in]  *pline_end  : 每行结束填充字符串
 *
 * @return  None
 ******************************************************************************
 */
static void
print_struct(const unsigned char *pin,
        unsigned int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;

    pcb(pline_head);
    pcb("------------------报文结构解析-------------------");
    pcb(pline_end);
    sprintf(buf, "%s报文长度:%d bytes%s", pline_head, len, pline_end);
    pcb(buf);
    pcb(pline_head);
    pcb("-------------------------------------------------");
    pcb(pline_end);
    //长度
    sprintf(buf, "%s  L = [%02X %02X]  %d%s", pline_head, pin[1], pin[2],
            ((int)pin[1]) | (((int)pin[2]) << 8), pline_end);
    pcb(buf);
    //控制域
    sprintf(buf, "%s  C = [%02X] DIR =%d %s PRM=%d %s%s", pline_head, pin[3],
            BITS(pin[3], 7),
            BITS(pin[3], 7) ? "上行(模块->集中器)" : "下行(集中器->模块)",
            BITS(pin[3], 6),
            BITS(pin[3], 6) ? "启动" : "从动", pline_end);
    pcb(buf);
    sprintf(buf, "%s           TYPE=%d %s%s", pline_head, pin[3] & 0x3f,
            get_commtype_str(pin[3] & 0x3f), pline_end);
    pcb(buf);
    //信息域R
    sprintf(buf, "%s  R = [%02X %02X %02X %02X %02X %02X]%s", pline_head,
            pin[4], pin[5], pin[6], pin[7], pin[8], pin[9], pline_end);
    pcb(buf);

    if BITS(pin[3], 7)  //上行
    {
        sprintf(buf, "%s      路由标识[%d]: %s%s", pline_head, pin[4] & 0x1,
                (pin[4] & 0x1) ? "通信模块不带路由或工作在旁路模式"
                        : "通信模块带路由或工作在路由模式", pline_end);
        pcb(buf);
        sprintf(buf, "%s      通信模块标识[%d]: %s%s", pline_head, (pin[4] >> 2) & 0x1,
                ( (pin[4] >> 2) & 0x1) ? "对从节点操作" : "对主节点操作", pline_end);
        pcb(buf);
        sprintf(buf, "%s      中继级别[%d]: %s%s", pline_head, pin[4] >> 4,
                (pin[4] >> 4) ? "" : "无中继", pline_end);
        pcb(buf);
        sprintf(buf, "%s      信道标识[%d]: 第%d信道%s", pline_head, pin[5] & 0x0f,
                pin[5] & 0x0f, pline_end);
        pcb(buf);
        if (pin[6] & 0x0f)
        {
            sprintf(buf, "%s      实测相线标识[%d]: 第%d相%s", pline_head, pin[6] & 0x0f,
                    pin[6] & 0x0f, pline_end);
        }
        else
        {
            sprintf(buf, "%s      实测相线标识[0]: 不确定%s", pline_head, pline_end);
        }
        pcb(buf);
        sprintf(buf, "%s      电能表通道特征[%d]: %s%s", pline_head, pin[6] >> 8,
                get_meter_comm_type(pin[6] >> 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s      末级命令信号品质(0无1最低)[%d]%s", pline_head,
                pin[7] & 0x0f, pline_end);
        pcb(buf);
        sprintf(buf, "%s      末级应答信号品质(0无1最低)[%d]%s", pline_head,
                pin[7] >> 4, pline_end);
        pcb(buf);
        sprintf(buf, "%s      事件标识[%d]: %s%s", pline_head, pin[8] & 0x01,
                (pin[8] & 0x01) ? "有事件上报" : "无事件上报", pline_end);
        pcb(buf);
        sprintf(buf, "%s      报文序列号[%d]%s", pline_head, pin[9], pline_end);
        pcb(buf);
    }
    else    //下行
    {
        sprintf(buf, "%s      路由标识[%d]: %s%s", pline_head, pin[4] & 0x1,
                (pin[4] & 0x1) ? "通信模块不带路由或工作在旁路模式"
                        : "通信模块带路由或工作在路由模式", pline_end);
        pcb(buf);
        sprintf(buf, "%s      附属节点标识[%d]: %s附加节点%s", pline_head, BITS(pin[4], 1),
                BITS(pin[4], 1) ? "有" : "无", pline_end);
        pcb(buf);
        sprintf(buf, "%s      通信模块标识[%d]: %s%s", pline_head, BITS(pin[4], 2),
                BITS(pin[4], 2) ? "对从节点操作" : "对主节点操作", pline_end);
        pcb(buf);
        sprintf(buf, "%s      冲突检测[%d]: %s进行冲突检测%s", pline_head, BITS(pin[4], 3),
                BITS(pin[4], 3) ? "要" : "不", pline_end);
        pcb(buf);
        sprintf(buf, "%s      中继级别[%d]: %s%s", pline_head, pin[4] >> 4,
                (pin[4] >> 4) ? "" : "无中继", pline_end);
        pcb(buf);
        if (pin[5] & 0x0f)
        {
            sprintf(buf, "%s      信道标识[%d]: 第%d信道%s", pline_head, pin[5] & 0x0f,
                    pin[5] & 0x0f, pline_end);
        }
        else
        {
            sprintf(buf, "%s      信道标识[%d]: 不分信道%s", pline_head,
                    pin[5] & 0x0f, pline_end);
        }
        pcb(buf);
        sprintf(buf, "%s      纠错编码标识[%d]: %s%s", pline_head, pin[5] >> 4,
                get_errcode_type(pin[5] >> 4), pline_end);
        pcb(buf);
        sprintf(buf, "%s      预计应答字节数[%02X]:%d%s", pline_head, pin[6], pin[6], pline_end);
        pcb(buf);
        sprintf(buf, "%s      通信速率[%d]: %s%s%s", pline_head, pin[7] | (int)((pin[8] & 0x7f) << 8),
                (pin[7] | (int)((pin[8] & 0x7f) << 8)) ? "" : "默认速率",
                (pin[8] & 0x80) ? "kbit/s" : "bit/s", pline_end);
        pcb(buf);
        sprintf(buf, "%s      报文序列号[%d]%s", pline_head, pin[9], pline_end);
        pcb(buf);
    }
    //地址域A
    if (pin[4] & 0x4)   //信息域的"通信模块标 识"为0时,无地址域A
    {
        sprintf(buf, "%s  源地址  A1 = [%02X %02X %02X %02X %02X %02X]%s", pline_head,
                pin[10], pin[11], pin[12], pin[13], pin[14], pin[15], pline_end);
        pcb(buf);

        i = 0;
        if (!BITS(pin[3], 7))  //下行报文才有中继地址
        {
            for (i = 0; i < (pin[4] >> 4); i++)
            {
                sprintf(buf, "%s  中继地址A2 = [%02X %02X %02X %02X %02X %02X]%s", pline_head,
                        pin[16 + i * 6], pin[17 + i * 6], pin[18 + i * 6],
                        pin[19 + i * 6], pin[20 + i * 6], pin[21 + i * 6], pline_end);
                pcb(buf);
            }
        }
        sprintf(buf, "%s  目的地址A3 = [%02X %02X %02X %02X %02X %02X]%s", pline_head,
                pin[16 + i * 6], pin[17 + i * 6], pin[18 + i * 6],
                pin[19 + i * 6], pin[20 + i * 6], pin[21 + i * 6], pline_end);
        pcb(buf);
        user_data_pos = 22 + i * 6;
    }
    else
    {
        sprintf(buf, "%s  A = 信息域的\"通信模块标识\"为0时,无地址域A%s",
                pline_head, pline_end);
        pcb(buf);
        user_data_pos = 10;
    }

    //应用数据
    sprintf(buf, "%sDAT = [", pline_head);
    pcb(buf);
    for (i = 0; i < len - user_data_pos - 3; i++)
    {
        sprintf(buf, "%02X ", pin[i + user_data_pos]);
        pcb(buf);
        if (((i + 1) % 16) == 0)
        {
            sprintf(buf, "%s%s       ", pline_end, pline_head);
            pcb(buf);
        }
    }
    sprintf(buf, "%02X]%s%s      AFN=[%02X] %s%s", pin[i + user_data_pos],
            pline_end, pline_head, pin[user_data_pos],
            get_AFN_name(pin[user_data_pos]), pline_end);
    pcb(buf);

    sprintf(buf, "%s      FN =[%02X %02X]: %d%s", pline_head,
            pin[user_data_pos + 1], pin[user_data_pos + 2],
            get_fn(pin[user_data_pos + 1], pin[user_data_pos + 2]), pline_end);
    pcb(buf);

    //CS
    sprintf(buf, "%s CS = [%02X]%s", pline_head, pin[len - 2], pline_end);
    pcb(buf);
    pcb(pline_head);
    pcb("-------------------------------------------------");
    pcb(pline_end);
}

/**
 ******************************************************************************
 * @brief   GDW1376_2报文解析
 * @param[in]  *pin        : 输入报文
 * @param[in]   len        : 输入报文长度
 * @param[in]  *pcb        : 回调函数
 * @param[in]  *pline_head : 每行起始填充字符串
 * @param[in]  *pline_end  : 每行结束填充字符串
 *
 * @retval  -1  : 解析出错
 * @retval   0  : 解析成功
 ******************************************************************************
 */
int
GDW1376_2_parse(const unsigned char *pin,
        unsigned int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int ret;

    //1. 参数检查
    if ((pin == NULL) || (pcb == NULL))
    {
        return -ERR_INPUT;
    }

    //2. 报文检查
    ret = chkfrm_GDW1376_2(pin, len);
    if (ret)
    {
        return ret;
    }

    //3. 设置默认解析参数
    pline_head = (pline_head == NULL) ? "" : pline_head;
    pline_end = (pline_end == NULL) ? "\n" : pline_end;

    //4. 输出原始报文
    print_org(pin, len, pcb, pline_head, pline_end);

    //5. 结构解析
    print_struct(pin, len, pcb, pline_head, pline_end);

    //6. 解析用户数据区
    ret = print_AFN(BITS(pin[3], 7), pin + user_data_pos,
            len - user_data_pos - 2, pcb, pline_head, pline_end);

    pcb(pline_head);
    if (!ret)
        pcb("-------------------解析成功----------------------");
    else
        pcb("-------------------解析失败----------------------");

    pcb(pline_end);

    return ret;
}

/**
 ******************************************************************************
 * @brief   获取错误信息
 * @param[in]  errid : 错误ID,GDW1376_2_parse的返回值
 *
 * @retval  错误信息字符串
 ******************************************************************************
 */
const char *
GDW1376_2_error(int errid)
{
    const char * const perror[] =
    {
        "解析正确",
        "输入参数有误",
        "输入报文头非0x68",
        "输入报文长度无效",
        "输入报文尾非0x16",
        "报文cs错误",
        "不支持的FN",
        "应用数据LEN有误",
    };

    errid = (errid < 0) ? -errid : errid;

    if ((errid >= 0) && (errid < ARRAY_SIZE(perror)))
    {
        return perror[errid];
    }

    return "ERR:输入无效的ID";
}

/**
 ******************************************************************************
 * @brief   获取版本字符串
 * @retval  版本字符串首地址
 ******************************************************************************
 */
const char *
GDW1376_2_ver_str(void)
{
    return VER_STR;
}

/**
 ******************************************************************************
 * @brief   获取修改日志
 * @retval  版本字符串首地址
 ******************************************************************************
 */
const char *
GDW1376_2_changelog(void)
{
    return CHANGE_LOG;
}

/*------------------------------GDW1376_2.c----------------------------------*/
