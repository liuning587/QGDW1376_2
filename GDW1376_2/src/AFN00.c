/**
 ******************************************************************************
 * @file      AFN00.c
 * @brief     C Source file of AFN00.c.
 * @details   This file including all API functions's implement of AFN00.c.
 * @copyright
 ******************************************************************************
 */
 
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include "../inc/GDW1376_2.h"
#include "types.h"
#include "lib.h"
#include "maths.h"

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
/* NONE */ 

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
/* NONE */

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
 * @brief   输出确认解析字符串
 * @param[in]   dir         : 传输方向:1上行(模块->集中器)" 0下行(集中器->模块)
 * @param[in]  *pin         : 输入报文
 * @param[in]   len         : 应用层报文长度
 * @param[in]  *pcb         : 回调函数
 * @param[in]  *pline_head  : 每行起始填充字符串
 * @param[in]  *pline_end   : 每行结束填充字符串
 *
 * @retval  解析错误码
 ******************************************************************************
 */
static int
AFN00_FN01(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;

    CHK_APP_LEN(len, 6);

    pcb(pline_head);
    pcb("确认报文:");
    pcb(pline_end);
    sprintf(buf, "%s命令执行状态[%d]:%s处理%s", pline_head, BITS(pin[0], 0),
            BITS(pin[0], 0) ? "已" : "未", pline_end);
    pcb(buf);

    pcb(pline_head);
    pcb("信道状态: ");
    for (i = 1; i < 8; i++)
    {
        sprintf(buf, "%d:%s ", i, BITS(pin[0], i) ? "闲" : "忙");
        pcb(buf);
    }
    pcb(pline_end);
    for (i = 0; i < 8; i++)
    {
        sprintf(buf, "%d:%s ", i + 8, BITS(pin[1], i) ? "闲" : "忙");
        pcb(buf);
    }
    pcb(pline_end);
    for (i = 0; i < 8; i++)
    {
        sprintf(buf, "%d:%s ", i + 16, BITS(pin[2], i) ? "闲" : "忙");
        pcb(buf);
    }
    pcb(pline_end);
    for (i = 0; i < 8; i++)
    {
        sprintf(buf, "%d:%s ", i + 24, BITS(pin[3], i) ? "闲" : "忙");
        pcb(buf);
    }
    pcb(pline_end);

    sprintf(buf, "%s等待时间(单位s):%d%s", pline_head,
            pin[4] | (((int)pin[5]) << 8), pline_end);
    pcb(buf);
    pcb(pline_end);

    return ERR_NONE;
}

/**
 ******************************************************************************
 * @brief   输出否认解析字符串
 * @param[in]   dir         : 传输方向:1上行(模块->集中器)" 0下行(集中器->模块)
 * @param[in]  *pin         : 输入报文
 * @param[in]   len         : 应用层报文长度
 * @param[in]  *pcb         : 回调函数
 * @param[in]  *pline_head  : 每行起始填充字符串
 * @param[in]  *pline_end   : 每行结束填充字符串
 *
 * @retval  解析错误码
 ******************************************************************************
 */
static int
AFN00_FN02(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    const char * const err_str[] =
    {
        "通信超时",
        "无效数据单元",
        "长度错",
        "校验错误",
        "信息类不存在",
        "格式错误", //5
        "表号重复",
        "表号不存在",
        "电表应用层无应答",
        "主节点忙",
        "主节点不支持此命令",    //10
        "从节点不应答",
        "从节点不在网内",
    };

    pcb(pline_head);
    pcb("否认报文:");
    CHK_APP_LEN(len, 1);

    if (pin[0] < 13)
    {
        pcb(err_str[pin[0]]);
    }
    else
    {
        pcb("备用");
    }
    pcb(pline_end);

    return ERR_NONE;
}

/**
 ******************************************************************************
 * @brief   输出确认/否认解析字符串
 * @param[in]   dir         : 传输方向:1上行(模块->集中器)" 0下行(集中器->模块)
 * @param[in]  *pin         : 输入报文
 * @param[in]   len         : 应用层报文长度
 * @param[in]  *pcb         : 回调函数
 * @param[in]  *pline_head  : 每行起始填充字符串
 * @param[in]  *pline_end   : 每行结束填充字符串
 *
 * @retval  解析错误码
 ******************************************************************************
 */
int
print_AFN00(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    unsigned char DT1 = pin[0];
    unsigned char DT2 = pin[1];
    int fn = get_clear_fn(&DT1, &DT2);

    switch (fn)
    {
        case 1: //确认
            return AFN00_FN01(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 2: //否认
            return AFN00_FN02(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        default:
            break;
    }

    pcb(pline_head);
    pcb("ERROR:不支持的FN!");
    pcb(pline_end);

    return -ERR_AFN_FN;
}

/*--------------------------------AFN00.c------------------------------------*/
