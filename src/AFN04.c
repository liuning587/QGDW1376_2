/**
 ******************************************************************************
 * @file      AFN04.c
 * @brief     C Source file of AFN04.c.
 * @details   This file including all API functions's implement of AFN04.c.
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
 * @brief   输出发送测试(主/从节点检测命令)解析字符串
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
AFN04_FN01(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("发送测试(主/从节点检测命令)");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s持续时间:%d%s", pline_head, pin[0], pline_end);
        pcb(buf);
    }
    else    //上行
    {
        //无
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出从节点点名字符串
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
AFN04_FN02(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("从节点点名");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s无数据单元%s", pline_head, pline_end);
        pcb(buf);
    }
    else    //上行
    {
        //无
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出本地通信模块报文通信测试解析字符串
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
AFN04_FN03(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;
    const char * const type[] =
    {
        "透明传输",
        "DL/T645-1997",
        "DL/T645-2007",
    };

    pcb(pline_head);
    pcb("本地通信模块报文通信测试");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s测试通信序号:%d%s", pline_head, pin[0], pline_end);
        pcb(buf);
        sprintf(buf, "%s目标地址:[%02X %02X %02X %02X %02X %02X]%s",
                pline_head, pin[1], pin[2], pin[3], pin[4], pin[5], pin[6],
                pline_end);
        pcb(buf);
        sprintf(buf, "%s通信协议类型[%d]:%s%s",
                pline_head, pin[7],
                (pin[7] < 3) ? type[pin[7]] : "无效",
                pline_end);
        pcb(buf);

        sprintf(buf, "%s报文长度L:%d%s", pline_head, pin[8], pline_end);
        pcb(buf);

        pcb(pline_head);
        pcb(" 报文内容:");
        pcb(pline_end);
        pcb(pline_head);
        for (i = 0; i < pin[8]; i++)
        {
            sprintf(buf, "%02X ", pin[i + 9]);
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
        //无上行报文
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出链路检测解析字符串
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
print_AFN04(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    unsigned char DT1 = pin[0];
    unsigned char DT2 = pin[1];
    int fn = get_clear_fn(&DT1, &DT2);

    if (dir)
    {
        pcb(pline_head);
        pcb("ERROR:上行报文为确认/否认报文,详见5.5.1");
        pcb(pline_end);
        return -1;
    }

    switch (fn)
    {
        case 1: //发送测试(主/从节点检测命令)
            return AFN04_FN01(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 2: //从节点点名
            return AFN04_FN02(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 3: //本地通信模块报文通信测试
            return AFN04_FN03(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        default:
            break;
    }

    pcb(pline_head);
    pcb("ERROR:不支持的FN!");
    pcb(pline_end);

    return -ERR_AFN_FN;
}

/*--------------------------------AFN04.c------------------------------------*/
