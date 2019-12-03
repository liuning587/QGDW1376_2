/**
 ******************************************************************************
 * @file      AFN11.c
 * @brief     C Source file of AFN11.c.
 * @details   This file including all API functions's implement of AFN11.c.
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
 * @brief   输出添加从节点解析字符串
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
AFN11_FN01(unsigned char dir,
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
        "DL/T698.45",
    };

    pcb(pline_head);
    pcb("添加从节点");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s从节点的数量:%d%s", pline_head, pin[0], pline_end);
        pcb(buf);
        if (len < (pin[0] * 7 + 1))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head,  pline_end);
            pcb(buf);
            return -1;
        }
        for (i = 0; i < pin[0]; i++)
        {
            sprintf(buf, "%s从节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 7 + 1], pin[i * 7 + 2],
                    pin[i * 7 + 3], pin[i * 7 + 4], pin[i * 7 + 5],
                    pin[i * 7 + 6], pline_end);
            pcb(buf);
            sprintf(buf, "%s通信协议类型[%d]:%s%s",
                    pline_head, pin[i * 7 + 7],
                    (pin[i * 7 + 7] < 4) ? type[pin[i * 7 + 7]] : "保留",
                    pline_end);
            pcb(buf);
        }
    }
    else    //上行
    {
        //无
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出删除从节点解析字符串
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
AFN11_FN02(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;

    pcb(pline_head);
    pcb("删除从节点");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s从节点的数量:%d%s", pline_head, pin[0], pline_end);
        pcb(buf);
        if (len < (pin[0] * 6 + 1))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head,  pline_end);
            pcb(buf);
            return -1;
        }
        for (i = 0; i < pin[0]; i++)
        {
            sprintf(buf, "%s从节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 6 + 1], pin[i * 6 + 2],
                    pin[i * 6 + 3], pin[i * 6 + 4], pin[i * 6 + 5],
                    pin[i * 6 + 6], pline_end);
            pcb(buf);
        }
    }
    else    //上行
    {
        //无
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出设置从节点固定中继路径解析字符串
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
AFN11_FN03(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;

    pcb(pline_head);
    pcb("设置从节点固定中继路径");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s从节点地址:[%02X %02X %02X %02X %02X %02X]%s",
                pline_head, pin[0], pin[1], pin[2], pin[3], pin[4], pin[5],
                pline_end);
        pcb(buf);
        sprintf(buf, "%s中继级别[%d]%s", pline_head, pin[6], pline_end);
        pcb(buf);
        if (len < (pin[6] * 6 + 7))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head,  pline_end);
            pcb(buf);
            return -1;
        }
        for (i = 0; i < pin[6]; i++)
        {
            sprintf(buf, "%s第%d级中继从节点地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 6 + 7], pin[i * 6 + 8],
                    pin[i * 6 + 9], pin[i * 6 + 10], pin[i * 6 + 11],
                    pin[i * 6 + 12], pline_end);
            pcb(buf);
        }
    }
    else    //上行
    {
        //无
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出设置路由工作模式解析字符串
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
AFN11_FN04(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("设置路由工作模式");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s运行状态字[%02x]:%d%s", pline_head, pin[0], pin[0], pline_end);
        pcb(buf);
        sprintf(buf, "%s-工作状态[%d]:%s%s", pline_head, BITS(pin[0], 0),
                BITS(pin[0], 0) ? "学习" : "抄表", pline_end);
        pcb(buf);
        sprintf(buf, "%s-注册允许状态[%d]:%s允许%s", pline_head, BITS(pin[0], 1),
                BITS(pin[0], 1) ? "不" : "", pline_end);
        pcb(buf);
        sprintf(buf, "%s-纠错编码[%d]:%s", pline_head, pin[0] >> 4, pline_end);
        pcb(buf);

        sprintf(buf, "%s通信速率:%d%s%s", pline_head,
                pin[1] | ((int)(pin[2] & 0x7f) << 8),
                (pin[2] & 0x80) ? "kbit/s" : "bit/s", pline_end);
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
 * @brief   输出激活从节点主动注册解析字符串
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
AFN11_FN05(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("激活从节点主动注册");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s开始时间:%02X年%02X月%02X日 %02X时%02X分%02X秒%s",
                pline_head, pin[5], pin[4], pin[3], pin[2], pin[1], pin[0],
                pline_end);
        pcb(buf);
        sprintf(buf, "%s持续时间[%02x %02x]:%d min%s", pline_head,
                pin[6], pin[7], pin[6] | ((int)pin[7] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点重发次数[%02x]:%d min%s", pline_head,
                pin[8], pin[8], pline_end);
        pcb(buf);
        sprintf(buf, "%s随机等待时间片个数[%02x]:%d (时间片指150ms)%s",
                pline_head, pin[9], pin[9], pline_end);
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
 * @brief   输出终止从节点主动注册解析字符串
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
AFN11_FN06(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("终止从节点主动注册");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        pcb(pline_head);
        pcb("无数据单元");
        pcb(pline_end);
    }
    else    //上行
    {
        //无
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出路由设置解析字符串
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
print_AFN11(unsigned char dir,
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
        case 1: //添加从节点
            return AFN11_FN01(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 2: //删除从节点
            return AFN11_FN02(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 3: //设置从节点固定中继路径
            return AFN11_FN03(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 4: //设置路由工作模式
            return AFN11_FN04(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 5: //激活从节点主动注册
            return AFN11_FN05(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 6: //终止从节点主动注册
            return AFN11_FN06(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 100: //todo: 设置网络规模
        case 101: //todo: 启动网络维护进程
        case 102: //todo: 启动组网
        default:
            break;
    }

    pcb(pline_head);
    pcb("ERROR:不支持的FN!");
    pcb(pline_end);

    return -ERR_AFN_FN;
}

/*--------------------------------AFN11.c------------------------------------*/
