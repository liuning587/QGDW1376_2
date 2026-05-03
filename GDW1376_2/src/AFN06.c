/**
 ******************************************************************************
 * @file      AFN06.c
 * @brief     C Source file of AFN06.c.
 * @details   This file including all API functions's implement of AFN06.c.
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
 * @brief   输出上报从节点信息解析字符串
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
AFN06_FN01(unsigned char dir,
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
    pcb("上报从节点信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        //无
    }
    else    //上行
    {
        sprintf(buf, "%s上报从节点的数量:%d%s", pline_head, pin[0], pline_end);
        pcb(buf);

        if (len < (pin[0] * 9 + 1))
        {
            pcb(pline_head);
            pcb("ERROR:长度有误!");
            pcb(pline_end);
            return -1;
        }
        for (i = 0; i < pin[0]; i++)
        {
            sprintf(buf, "%s从节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[9*i + 1], pin[9*i + 2], pin[9*i + 3],
                    pin[9*i + 4], pin[9*i + 5], pin[9*i + 6],
                    pline_end);
            pcb(buf);
            sprintf(buf, "%s从节点%d通信协议类型[%d]:%s%s",
                    pline_head, i, pin[9*i + 7],
                    (pin[9*i + 7] < 4) ? type[pin[9*i + 7]] : "保留",
                    pline_end);
            pcb(buf);
            sprintf(buf, "%s从节点%d序号[%02X %02X]:%d%s", pline_head, i,
                    pin[9*i + 8], pin[9*i + 9],
                    pin[9*i + 8] | ((int)pin[9*i + 9] << 8), pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出上报抄读数据解析字符串
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
AFN06_FN02(unsigned char dir,
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
    pcb("上报抄读数据");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        //无
    }
    else    //上行
    {
        sprintf(buf, "%s从节点序号[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1],
                pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点通信协议类型[%d]:%s%s",
                pline_head, pin[2],
                (pin[2] < 4) ? type[pin[2]] : "保留",
                pline_end);
        pcb(buf);
        sprintf(buf, "%s当前报文本地通信上行时长[%02X %02X]:%ds%s", pline_head,
                pin[3], pin[4],
                pin[3] | ((int)pin[4] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s报文长度L:%d%s", pline_head, pin[5], pline_end);
        pcb(buf);

        pcb(pline_head);
        pcb(" 报文内容:");
        pcb(pline_end);
        pcb(pline_head);
        for (i = 0; i < pin[5]; i++)
        {
            sprintf(buf, "%02X ", pin[i + 6]);
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
 * @brief   输出上报路由工况变动信息解析字符串
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
AFN06_FN03(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("上报路由工况变动信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        //无
    }
    else    //上行
    {
        if (len < 1)
        {
            pcb(pline_head);
            pcb("ERROR:长度有误!");
            pcb(pline_end);
            return -ERR_APP_LEN;
        }
        sprintf(buf, "%s路由工作任务变动类型[%d]:%s%s", pline_head, pin[0],
                (pin[0] == 1) ? "抄表任务结束"
                : ((pin[0] == 2) ? "搜表任务结束"
                : ((pin[0] == 3) ? "台区识别任务结束" : "保留")), pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出上报从节点信息及设备类型解析字符串
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
AFN06_FN04(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int k;
    int j;
    unsigned int pos;
    unsigned char n;
    unsigned char M;
    unsigned char m;
    const char * const type[] =
    {
        "透明传输",
        "DL/T645-1997",
        "DL/T645-2007",
        "DL/T698.45",
    };

    pcb(pline_head);
    pcb("上报从节点信息及设备类型");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        //无
    }
    else    //上行（表 53）
    {
        if (len < 1)
        {
            pcb(pline_head);
            pcb("ERROR:长度有误!");
            pcb(pline_end);
            return -ERR_APP_LEN;
        }
        n = pin[0];
        sprintf(buf, "%s上报从节点的数量 n:%u%s", pline_head, (unsigned)n, pline_end);
        pcb(buf);
        pos = 1u;
        for (k = 0; k < (int)n; k++)
        {
            if (len < (int)(pos + 12u))
            {
                pcb(pline_head);
                pcb("ERROR:长度不足(从节点头域)!");
                pcb(pline_end);
                return -ERR_APP_LEN;
            }
            M = pin[pos + 10u];
            m = pin[pos + 11u];
            if (len < (int)(pos + 12u + 7u * (unsigned int)m))
            {
                pcb(pline_head);
                pcb("ERROR:长度不足(下接从节点列表)!");
                pcb(pline_end);
                return -ERR_APP_LEN;
            }
            sprintf(buf, "%s--- 从节点 #%d ---%s", pline_head, k + 1, pline_end);
            pcb(buf);
            sprintf(buf, "%s通信地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, pin[pos], pin[pos + 1u], pin[pos + 2u],
                    pin[pos + 3u], pin[pos + 4u], pin[pos + 5u], pline_end);
            pcb(buf);
            sprintf(buf, "%s通信协议类型[%02X]:%s%s", pline_head, pin[pos + 6u],
                    (pin[pos + 6u] < 4) ? type[pin[pos + 6u]] : "保留", pline_end);
            pcb(buf);
            sprintf(buf, "%s序号(LE):%u%s", pline_head,
                    (unsigned)(pin[pos + 7u] | (pin[pos + 8u] << 8)), pline_end);
            pcb(buf);
            sprintf(buf, "%s设备类型[%02X]:%s%s", pline_head, pin[pos + 9u],
                    (pin[pos + 9u] == 0) ? "采集器"
                    : ((pin[pos + 9u] == 1) ? "电能表" : "保留"), pline_end);
            pcb(buf);
            sprintf(buf, "%s下接从节点总数量 M:%u%s", pline_head, (unsigned)M, pline_end);
            pcb(buf);
            sprintf(buf, "%s本次上报下接从节点数量 m:%u%s", pline_head, (unsigned)m, pline_end);
            pcb(buf);
            for (j = 0; j < (int)m; j++)
            {
                unsigned int cbase = pos + 12u + (unsigned int)j * 7u;

                sprintf(buf, "%s  下接从节点 #%d 地址:[%02X %02X %02X %02X %02X %02X]%s",
                        pline_head, j + 1,
                        pin[cbase], pin[cbase + 1u], pin[cbase + 2u],
                        pin[cbase + 3u], pin[cbase + 4u], pin[cbase + 5u],
                        pline_end);
                pcb(buf);
                sprintf(buf, "%s  下接从节点 #%d 通信协议类型[%02X]:%s%s", pline_head, j + 1,
                        pin[cbase + 6u],
                        (pin[cbase + 6u] < 4) ? type[pin[cbase + 6u]] : "保留",
                        pline_end);
                pcb(buf);
            }
            pos += 12u + 7u * (unsigned int)m;
        }
        if (pos != (unsigned int)len)
        {
            sprintf(buf, "%s提示: 解析结束偏移%u 与数据单元长度%d 不一致（尾随数据或帧异常）%s",
                    pline_head, pos, len, pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出上报从节点事件解析字符串
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
AFN06_FN05(unsigned char dir,
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
    pcb("上报从节点事件");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        //无
    }
    else    //上行
    {
        sprintf(buf, "%s从节点设备类型[%d]:%s%s", pline_head, pin[0],
                (pin[0] == 0) ? "采集器" : ((pin[0] == 1) ? "电能表":"保留"), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点通信协议类型[%d]:%s%s",
                pline_head, pin[1],
                (pin[1] < 4) ? type[pin[1]] : "保留",
                pline_end);
        pcb(buf);
        sprintf(buf, "%s报文长度L:%d%s", pline_head, pin[2], pline_end);
        pcb(buf);

        pcb(pline_head);
        pcb(" 报文内容:");
        pcb(pline_end);
        pcb(pline_head);
        for (i = 0; i < pin[2]; i++)
        {
            sprintf(buf, "%02X ", pin[i + 3]);
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
 * @brief   输出主动上报解析字符串
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
print_AFN06(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    unsigned char DT1 = pin[0];
    unsigned char DT2 = pin[1];
    int fn = get_clear_fn(&DT1, &DT2);

    if (!dir)
    {
        pcb(pline_head);
        pcb("ERROR:AFN(06H)主动上报无下行报文");
        pcb(pline_end);
        return -1;
    }

    switch (fn)
    {
        case 1: //上报从节点信息
            return AFN06_FN01(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 2: //上报抄读数据
            return AFN06_FN02(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 3: //上报路由工况变动信息
            return AFN06_FN03(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 4: //上报从节点信息及设备类型
            return AFN06_FN04(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 5: //上报从节点事件
            return AFN06_FN05(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        default:
            break;
    }

    pcb(pline_head);
    pcb("ERROR:不支持的FN!");
    pcb(pline_end);

    return -ERR_AFN_FN;
}

/*--------------------------------AFN06.c------------------------------------*/
