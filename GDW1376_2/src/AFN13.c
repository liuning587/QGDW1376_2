/**
 ******************************************************************************
 * @file      AFN13.c
 * @brief     C Source file of AFN13.c.
 * @details   This file including all API functions's implement of AFN13.c.
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
 * @brief   输出监控从节点解析字符串
 * @param[in]   dir         : 传输方向:1上行(模块->集中器)" 0下行(集中器->模块)
 * @param[in]  *pin         : 输入报文
 * @param[in]   len         : 应用层报文长度
 * @param[in]  *pcb         : 回调函数
 * @param[in]  *pline_head  : 每行起始填充字符串
 * @param[in]  *pline_end   : 每行结束填充字符串
 *
 * @return  0 : 解析成功
 * @return -1 : 解析失败
 ******************************************************************************
 */
static int
AFN13_FN01(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;
    int pos = 0;
    const char * const type[] =
    {
        "透明传输",
        "DL/T645-1997",
        "DL/T645-2007",
    };

    pcb(pline_head);
    pcb("监控从节点");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s通信协议类型[%d]:%s%s", pline_head, pin[0],
                (pin[0] < 3) ? type[pin[0]] : "保留",
                pline_end);
        pcb(buf);

        sprintf(buf, "%s通信延时相关标志[%d]:", pline_head, pin[1]);
        pcb(buf);
        switch (pin[1])
        {
            case 0:
                pcb("通信数据与通信延时无关,无需本地通信延时修正具体报文信息");
                break;
            case 1:
                pcb("通信数据与通信延时相关,需要依据本地通信延时进行具体通信");
                pcb(pline_end);
                pcb(pline_head);
                pcb("报文的内容修正,后续路由选择好路径后,需要提请集中器修正报文数据;");
                break;
            default:
                pcb("保留");
                break;
        }
        pcb(pline_end);

        sprintf(buf, "%s从节点附属节点数量[%02X]:%d%s", pline_head, pin[2], pin[2], pline_end);
        pcb(buf);
        if (len < (pin[2] * 6 + 3))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head,  pline_end);
            pcb(buf);
            return -1;
        }
        for (i = 0; i < pin[2]; i++)
        {
            sprintf(buf, "%s从节点附属节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 6 + 3], pin[i * 6 + 4],
                    pin[i * 6 + 5], pin[i * 6 + 6], pin[i * 6 + 7],
                    pin[i * 6 + 8], pline_end);
            pcb(buf);
        }
        pos = i * 6 + 3;
        sprintf(buf, "%s报文长度L[%02X]:%d%s", pline_head, pin[pos], pin[pos], pline_end);
        pcb(buf);

        pcb(pline_head);
        pcb(" 报文内容:");
        pcb(pline_end);
        pcb(pline_head);
        for (i = 0; i < pin[pos]; i++)
        {
            sprintf(buf, "%02X ", pin[i + pos + 1]);
            pcb(buf);
            if (((i + 1) % 16) == 0)
            {
                pcb(pline_end);
                pcb(pline_head);
            }
        }
        pcb(pline_end);

    }
    else    //上行
    {
        sprintf(buf, "%s当前报文本地通信上行时长[%02x %02x]:%ds%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s通信协议类型[%d]:%s%s", pline_head, pin[2],
                (pin[2] < 3) ? type[pin[2]] : "保留",
                pline_end);
        pcb(buf);
        sprintf(buf, "%s报文长度L:%d%s", pline_head, pin[3], pline_end);
        pcb(buf);

        pcb(pline_head);
        pcb(" 报文内容:");
        pcb(pline_end);
        pcb(pline_head);
        for (i = 0; i < pin[3]; i++)
        {
            sprintf(buf, "%02X ", pin[i + pos + 4]);
            pcb(buf);
            if (((i + 1) % 16) == 0)
            {
                pcb(pline_end);
                pcb(pline_head);
            }
        }
        pcb(pline_end);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出路由数据转发解析字符串
 * @param[in]   dir         : 传输方向:1上行(模块->集中器)" 0下行(集中器->模块)
 * @param[in]  *pin         : 输入报文
 * @param[in]   len         : 应用层报文长度
 * @param[in]  *pcb         : 回调函数
 * @param[in]  *pline_head  : 每行起始填充字符串
 * @param[in]  *pline_end   : 每行结束填充字符串
 *
 * @return  0 : 解析成功
 * @return -1 : 解析失败
 ******************************************************************************
 */
int
print_AFN13(unsigned char dir,
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
        case 1: //监控从节点
            return AFN13_FN01(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        default:
            break;
    }

    pcb(pline_head);
    pcb("ERROR:不支持的FN!");
    pcb(pline_end);

    return -ERR_AFN_FN;
}

/*--------------------------------AFN13.c------------------------------------*/
