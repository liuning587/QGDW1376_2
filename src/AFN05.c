/**
 ******************************************************************************
 * @file      AFN05.c
 * @brief     C Source file of AFN05.c.
 * @details   This file including all API functions's implement of AFN05.c.
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
 * @brief   输出设置主节点地址解析字符串
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
AFN05_FN01(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("设置主节点地址");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 6);
        sprintf(buf, "%s主节点地址:[%02X %02X %02X %02X %02X %02X]%s",
                pline_head, pin[0], pin[1], pin[2], pin[3], pin[4], pin[5],
                pline_end);
        pcb(buf);
    }
    else    //上行
    {
        //无
    }

    return ERR_NONE;
}

/**
 ******************************************************************************
 * @brief   输出允许从节点上报解析字符串
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
AFN05_FN02(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("允许从节点上报");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 1);
        sprintf(buf, "%s事件上报状态标志[%d]:%s%s", pline_head, pin[0],
                pin[0] ? "允许" : "禁止", pline_end);
        pcb(buf);
    }
    else    //上行
    {
        //无
    }

    return ERR_NONE;
}

/**
 ******************************************************************************
 * @brief   输出启动广播解析字符串
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
AFN05_FN03(unsigned char dir,
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
        "相位识别功能",
    };

    pcb(pline_head);
    pcb("启动广播");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s控制字[%d]:%s%s",
                pline_head, pin[0],
                (pin[0] < 4) ? type[pin[0]] : "保留",
                pline_end);
        pcb(buf);

        sprintf(buf, "%s报文长度L:%d%s", pline_head, pin[1], pline_end);
        pcb(buf);

        pcb(pline_head);
        pcb(" 报文内容:");
        pcb(pline_end);
        pcb(pline_head);
        for (i = 0; i < pin[1]; i++)
        {
            sprintf(buf, "%02X ", pin[i + 2]);
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
        //无
    }

    return ERR_NONE;
}

/**
 ******************************************************************************
 * @brief   输出设置从节点监控最大超时时间解析字符串
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
AFN05_FN04(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("设置从节点监控最大超时时间");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 1);
        sprintf(buf, "%s最大超时时间[%02X]:%ds%s", pline_head,
                pin[0], pin[0], pline_end);
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
 * @brief   输出设置无线通信参数解析字符串
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
AFN05_FN05(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    const char * const pwr[] =
    {
        "00最高发射功率",
        "01次高发射功率",
        "02次低发射功率",
        "03最低发射功率",
    };

    pcb(pline_head);
    pcb("设置无线通信参数");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 2);
        sprintf(buf, "%s无线信道组[%02X]:", pline_head, pin[0]);
        pcb(buf);
        if (pin[0] < 64)
        {
            sprintf(buf, "%d组", pin[0]);
        }
        else if (pin[0] < 254)
        {
            sprintf(buf, "保留");
        }
        else if (pin[0] == 254)
        {
            sprintf(buf, "自动选择");
        }
        else    //255
        {
            sprintf(buf, "保持不变");
        }
        pcb(buf);
        pcb(pline_end);

        sprintf(buf, "%s无线主节点发射功率[%02X]:%s%s", pline_head, pin[1],
                (pin[1] < 4) ? pwr[pin[1]] : ((pin[1] == 254) ? "保持不变" : "保留"), pline_end);
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
 * @brief   输出控制命令解析字符串
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
print_AFN05(unsigned char dir,
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
        case 1: //设置主节点地址
            return AFN05_FN01(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 2: //允许从节点上报
            return AFN05_FN02(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 3: //启动广播
            return AFN05_FN03(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 4: //设置从节点监控最大超时时间
            return AFN05_FN04(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 5: //设置无线通信参数
            return AFN05_FN05(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        default:
            break;
    }

    pcb(pline_head);
    pcb("ERROR:不支持的FN!");
    pcb(pline_end);

    return -ERR_AFN_FN;
}

/*--------------------------------AFN05.c------------------------------------*/
