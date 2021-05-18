/**
 ******************************************************************************
 * @file      AFN10.c
 * @brief     C Source file of AFN10.c.
 * @details   This file including all API functions's implement of AFN10.c.
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
 * @brief   输出从节点数量解析字符串
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
AFN10_FN01(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("从节点数量");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        pcb(pline_head);
        pcb("无数据单元");
        pcb(pline_end);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 4);
        sprintf(buf, "%s从节点总数量[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s路由支持最大从节点数量[%02x %02x]:%d%s", pline_head,
                pin[2], pin[3], pin[2] | ((int)pin[3] << 8), pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出从节点信息解析字符串
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
AFN10_FN02(unsigned char dir,
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
    pcb("从节点信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s从节点起始序号[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点数量[%02x]:%d%s", pline_head, pin[2], pin[2], pline_end);
        pcb(buf);
    }
    else    //上行
    {
        sprintf(buf, "%s从节点总数量[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s本次应答的从节点数量:%d%s", pline_head, pin[2], pline_end);
        pcb(buf);
        if (len < (pin[2] * 8 + 3))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head,  pline_end);
            pcb(buf);
            return -1;
        }

        for (i = 0; i < pin[2]; i++)
        {
            sprintf(buf, "%s从节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 8 + 3], pin[i * 8 + 4],
                    pin[i * 8 + 5], pin[i * 8 + 6], pin[i * 8 + 7],
                    pin[i * 8 + 8], pline_end);
            pcb(buf);

            sprintf(buf, "%s中继级别:%d%s", pline_head, pin[i * 8 + 9] & 0x0f, pline_end);
            pcb(buf);
            sprintf(buf, "%s侦听信号品质:%d%s", pline_head, pin[i * 8 + 9] >> 4, pline_end);
            pcb(buf);
            sprintf(buf, "%s相位[%d]:%s%s", pline_head, pin[i * 8 + 10] & 0x07,
                    "(D8 D9 D10置\"1\"依次表示第1 2 3相)", pline_end);
            pcb(buf);
            sprintf(buf, "%s通信协议类型[%d]:%s%s",
                    pline_head, (pin[i * 8 + 10] >> 3) & 0x07,
                    (((pin[i * 8 + 10] >> 3) & 0x07) < 4) ? type[(pin[i * 8 + 10] >> 3) & 0x07] : "保留",
                    pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出指定从节点的上一级中继路由信息解析字符串
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
AFN10_FN03(unsigned char dir,
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
    pcb("指定从节点的上一级中继路由信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 6);
        sprintf(buf, "%s从节点地址:[%02X %02X %02X %02X %02X %02X]%s",
                pline_head, pin[0], pin[1], pin[2], pin[3], pin[4], pin[5],
                pline_end);
        pcb(buf);
    }
    else    //上行
    {
        sprintf(buf, "%s提供路由的从节点总数量:%d%s", pline_head, pin[0], pline_end);
        pcb(buf);
        if (len < (pin[0] * 8 + 1))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head,  pline_end);
            pcb(buf);
            return -1;
        }

        for (i = 0; i < pin[0]; i++)
        {
            sprintf(buf, "%s提供路由的从节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 8 + 1], pin[i * 8 + 2],
                    pin[i * 8 + 3], pin[i * 8 + 4],
                    pin[i * 8 + 5], pin[i * 8 + 6], pline_end);
            pcb(buf);

            sprintf(buf, "%s中继级别:%d%s", pline_head, pin[i * 8 + 7] & 0x0f, pline_end);
            pcb(buf);
            sprintf(buf, "%s侦听信号品质:%d%s", pline_head, pin[i * 8 + 7] >> 4, pline_end);
            pcb(buf);
            sprintf(buf, "%s相位[%d]:%s%s", pline_head, pin[i * 8 + 8] & 0x07,
                    "(D8 D9 D10置\"1\"依次表示第1 2 3相)", pline_end);
            pcb(buf);
            sprintf(buf, "%s通信协议类型[%d]:%s%s",
                    pline_head, (pin[i * 8 + 8] >> 3) & 0x07,
                    (((pin[i * 8 + 8] >> 3) & 0x07) < 4) ? type[(pin[i * 8 + 8] >> 3) & 0x07] : "保留",
                    pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出路由运行状态解析字符串
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
AFN10_FN04(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;
    const char * const work_step[] =
    {
        "备用",   //0
        "初始状态",
        "值抄",
        "中继",
        "监控状态",
        "广播状态", //5
        "广播招读电表",
        "读侦听信息",
        "空闲",
    };

    pcb(pline_head);
    pcb("路由运行状态");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        pcb(pline_head);
        pcb("无数据单元");
        pcb(pline_end);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 16);
        sprintf(buf, "%s 运行状态字[%02x]:%d%s", pline_head, pin[0], pin[0], pline_end);
        pcb(buf);
        sprintf(buf, "%s 路由完成标志[%d]:路由学习%s完成%s", pline_head, BITS(pin[0], 0),
                BITS(pin[0], 0) ? "" : "未", pline_end);
        pcb(buf);
        sprintf(buf, "%s 工作标志[%d]:%s工作%s", pline_head, BITS(pin[0], 1),
                BITS(pin[0], 0) ? "正在" : "停止", pline_end);
        pcb(buf);
        sprintf(buf, "%s 上报事件标志[%d]:%s从节点上报事件%s", pline_head, BITS(pin[0], 2),
                BITS(pin[0], 2) ? "有" : "无", pline_end);
        pcb(buf);
        sprintf(buf, "%s 纠错编码[%d]:%s", pline_head, pin[0] >> 4, pline_end);
        pcb(buf);

        sprintf(buf, "%s 从节点总数量[%02X %02X]:%d%s", pline_head,
                pin[1], pin[2], pin[1] | ((int)pin[2] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s 已抄从节点数量[%02X %02X]:%d%s", pline_head,
                pin[3], pin[4], pin[3] | ((int)pin[4] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s 中继抄到从节点数量[%02X %02X]:%d%s", pline_head,
                pin[5], pin[6], pin[5] | ((int)pin[6] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s 工作开关[%02x]:%d%s", pline_head, pin[7], pin[7], pline_end);
        pcb(buf);
        sprintf(buf, "%s 工作状态[%d]:%s%s", pline_head, BITS(pin[7], 0),
                BITS(pin[7], 0) ? "学习" : "抄表", pline_end);
        pcb(buf);
        sprintf(buf, "%s 注册允许状态[%d]:%s允许%s", pline_head, BITS(pin[7], 1),
                BITS(pin[7], 1) ? "" : "不", pline_end);
        pcb(buf);

        sprintf(buf, "%s 通信速率[%02X %02X]:%d%s", pline_head,
                pin[8], pin[9], pin[8] | ((int)pin[9] << 8), pline_end);
        pcb(buf);

        for (i = 1; i < 4; i++)
        {
            sprintf(buf, "%s 第%d中继级别[%02x]:%d%s", pline_head, i, pin[9 + i], pin[9 + i] & 0x0f, pline_end);
            pcb(buf);
        }

        for (i = 1; i < 4; i++)
        {
            sprintf(buf, "%s 第%d工作步骤[%02x]:%s%s", pline_head, i, pin[12 + i],
                    (pin[12 + i] < 9) ? work_step[pin[12 + i]] : "备用", pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出未抄读成功的从节点信息解析字符串
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
AFN10_FN05(unsigned char dir,
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
    pcb("未抄读成功的从节点信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 3);
        sprintf(buf, "%s从节点起始序号[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点数量[%02x]:%d%s", pline_head, pin[2], pin[2], pline_end);
        pcb(buf);
    }
    else    //上行
    {
        sprintf(buf, "%s从节点总数量[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s本次应答的从节点数量:%d%s", pline_head, pin[2], pline_end);
        pcb(buf);
        if (len < (pin[2] * 8 + 3))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head,  pline_end);
            pcb(buf);
            return -1;
        }

        for (i = 0; i < pin[2]; i++)
        {
            sprintf(buf, "%s从节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 8 + 3], pin[i * 8 + 4],
                    pin[i * 8 + 5], pin[i * 8 + 6], pin[i * 8 + 7],
                    pin[i * 8 + 8], pline_end);
            pcb(buf);

            sprintf(buf, "%s中继级别:%d%s", pline_head, pin[i * 8 + 9] & 0x0f, pline_end);
            pcb(buf);
            sprintf(buf, "%s侦听信号品质:%d%s", pline_head, pin[i * 8 + 9] >> 4, pline_end);
            pcb(buf);
            sprintf(buf, "%s相位[%d]:%s%s", pline_head, pin[i * 8 + 10] & 0x07,
                    "(D8 D9 D10置\"1\"依次表示第1 2 3相)", pline_end);
            pcb(buf);
            sprintf(buf, "%s通信协议类型[%d]:%s%s",
                    pline_head, (pin[i * 8 + 10] >> 3) & 0x07,
                    (((pin[i * 8 + 10] >> 3) & 0x07) < 4) ? type[(pin[i * 8 + 10] >> 3) & 0x07] : "保留",
                    pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出主动注册的从节点信息解析字符串
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
AFN10_FN06(unsigned char dir,
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
    pcb("主动注册的从节点信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s从节点起始序号[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点数量[%02x]:%d%s", pline_head, pin[2], pin[2], pline_end);
        pcb(buf);
    }
    else    //上行
    {
        sprintf(buf, "%s从节点总数量[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s本次应答的从节点数量:%d%s", pline_head, pin[2], pline_end);
        pcb(buf);
        if (len < (pin[2] * 8 + 3))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head,  pline_end);
            pcb(buf);
            return -1;
        }

        for (i = 0; i < pin[2]; i++)
        {
            sprintf(buf, "%s从节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 8 + 3], pin[i * 8 + 4],
                    pin[i * 8 + 5], pin[i * 8 + 6], pin[i * 8 + 7],
                    pin[i * 8 + 8], pline_end);
            pcb(buf);

            sprintf(buf, "%s中继级别:%d%s", pline_head, pin[i * 8 + 9] & 0x0f, pline_end);
            pcb(buf);
            sprintf(buf, "%s侦听信号品质:%d%s", pline_head, pin[i * 8 + 9] >> 4, pline_end);
            pcb(buf);
            sprintf(buf, "%s相位[%d]:%s%s", pline_head, pin[i * 8 + 10] & 0x07,
                    "(D8 D9 D10置\"1\"依次表示第1 2 3相)", pline_end);
            pcb(buf);
            sprintf(buf, "%s通信协议类型[%d]:%s%s",
                    pline_head, (pin[i * 8 + 10] >> 3) & 0x07,
                    (((pin[i * 8 + 10] >> 3) & 0x07) < 4) ? type[(pin[i * 8 + 10] >> 3) & 0x07] : "保留",
                    pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   查询从节点ID号信息
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
AFN10_FN07(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i, moduleid ;
    const char * const idformat[] =
    {
        "组合格式",
        "BCD",
        "BIN",
        "ASCII",
    };

    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 3);
        sprintf(buf, "%s从节点起始序号[%02X %02X]:%d%s", pline_head,
                        pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
                pcb(buf);
        sprintf(buf, "%s从节点数量[%02x]:%d%s", pline_head, pin[2], pin[2], pline_end);
                pcb(buf);

    }
    else    //上行
    {
        sprintf(buf, "%s从节点总数量[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s本次应答的从节点数量n:%d%s", pline_head, pin[2], pline_end);
        pcb(buf);
        if (len < (pin[2] * (11 + pin[12]) + 3)) //数据部分长度
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head,  pline_end);
            pcb(buf);
            return -1;
        }

        for (i = 0; i < pin[2]; i++)
        {
            sprintf(buf, "%s从节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 12 + 3], pin[i * 12 + 4],
                    pin[i * 12 + 5], pin[i * 12 + 6], pin[i * 12 + 7],
                    pin[i * 12 + 8], pline_end);
            pcb(buf);

            sprintf(buf, "%s从节点%d地址类型:%d%s", pline_head, i+1, pin[i * 8 + 9], pline_end);
            pcb(buf);
            moduleid = (pin[i * 8 + 11] << 8) | pin[i * 8 + 10];
            sprintf(buf, "%s从节点%d模块厂商代码:%d%s", pline_head, i+1, moduleid, pline_end);
            pcb(buf);
            sprintf(buf, "%s从节点%d模块ID号长度:%d%s", pline_head, i+1,pin[i * 8 + 12], pline_end);
            pcb(buf);
            sprintf(buf, "%s从节点%d模块ID号格式:%d%s", pline_head, i+1,pin[i * 8 + 13], pline_end);
            pcb(buf);

            sprintf(buf, "%s从节点%d模块ID号格式:%s%s", pline_head, pin[i * 8 + 13],
                    (pin[0] < 4) ? idformat[pin[i * 8 + 13]] : "保留",
                    pline_end);
            pcb(buf);
            sprintf(buf, "%s从节点%d模块ID号:%d%s", pline_head, i+1, pin[i * 8 + 14], pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   查询HPLC网络规模
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
AFN10_FN09(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("查询HPLC网络规模");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 2);
        sprintf(buf, "%sHPLC网络规模[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   查询拓扑网络信息
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
AFN10_FN21(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;
    pcb(pline_head);
    pcb("查询拓扑网络规模");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 3); //
        sprintf(buf, "%s从节点起始序号[%02X %02X]:%d%s", pline_head, pin[0], pin[1],
                pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点数量[%02X]:%d%s", pline_head, pin[2], pin[2],
                pline_end);
        pcb(buf);
    }
    else    //上行
    {
        sprintf(buf, "%s从节点总数量[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点起始序号[%02X %02X]:%d%s", pline_head,
                pin[2], pin[3], pin[2] | ((int)pin[2] << 8),pline_end);
        pcb(buf);
        sprintf(buf, "%s本次应答的节点数量:%d%s", pline_head, pin[4], pline_end);
        pcb(buf);
        if (len < (pin[4] * 11 + 5))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head, pline_end);
            pcb(buf);
            return -1;
        }
        for (i =0; i < pin[4]; i++) //节点序号1开始
        {
            sprintf(buf, "%s节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i+1, pin[i * 11 + 5], pin[i * 11 + 6],
                    pin[i * 11 + 7], pin[i * 11 + 8], pin[i * 11 + 9],
                    pin[i * 11 + 10], pline_end);
            pcb(buf);
            sprintf(buf, "%s节点%d网络拓扑信息:[%02X %02X %02X %02X %02X]%s",
                    pline_head, i+1, pin[i * 11 + 11], pin[i * 11 + 12],
                    pin[i * 11 + 13], pin[i * 11 + 14], pin[i * 11 + 15],
                    pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   查询相线信息
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
AFN10_FN31(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;
    pcb(pline_head);
    pcb("查询相线");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 3);
        sprintf(buf, "%s从节点起始序号[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点数量[%02X]:%d%s", pline_head, pin[2], pin[2], pline_end);
        pcb(buf);
    }
    else    //上行
    {
        sprintf(buf, "%s从节点总数量[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点起始序号[%02X %02X]:%d%s", pline_head,
                pin[2], pin[3], pin[2] | ((int)pin[2] << 8),pline_end);
        pcb(buf);
        sprintf(buf, "%s本次应答的节点数量:%d%s", pline_head,pin[4], pline_end);
        pcb(buf);
        if (len < (pin[2] * 8 + 3))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head, pline_end);
            pcb(buf);
            return -1;
        }
        for (i = 0; i < pin[4]; i++)
        {
            sprintf(buf, "%s节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 8 + 5], pin[i * 8 + 6],
                    pin[i * 8 + 7], pin[i * 8 + 8], pin[i * 8 + 9],
                    pin[i * 8 + 10], pline_end);
            pcb(buf);
            sprintf(buf, "%s节点%d相线信息:[%02X %02X]%s", pline_head, i,
                    pin[i * 8 + 11], pin[i * 8 + 12], pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   流水线查询ID信息
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
AFN10_FN40(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    const char * const devtype[] =
    {
        "未定义的设备类型",
        "抄控器",
        "CCO",
        "电表通信单元",
        "中继器",
        "II型采集器",
        "I型采集器",
        "三相表通信单元",
    };
    const char * const idtype[] =
    {
        "未定义的ID类型",
        "芯片ID",
        "模块ID",
    };

    pcb(pline_head);
    pcb("流水线查询ID信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 8);
        sprintf(buf, "%s设备类型[%d]:%s%s", pline_head, pin[0],
               (pin[0] < 8) ? devtype[pin[0]] : "未定义的设备类型", pline_end);
        pcb(buf);
        sprintf(buf, "%s节点地址:[%02X %02X %02X %02X %02X %02X]%s",
                pline_head, pin[1], pin[2],pin[3], pin[4], pin[5],
                pin[6], pline_end);
        pcb(buf);
        sprintf(buf, "%sID类型[%d]:%s%s", pline_head, pin[6],
                (pin[0] < 3) ? idtype[pin[6]] : "未定义的ID类型", pline_end);
        pcb(buf);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 33);//若为芯片ID，len=33；若为模块ID，其ID长度定义在其它文档中
        sprintf(buf, "%s设备类型[%d]:%s%s", pline_head, pin[0],
                (pin[0] < 8) ? devtype[pin[0]] : "未定义的设备类型", pline_end);
        pcb(buf);
        sprintf(buf, "%s节点地址:[%02X %02X %02X %02X %02X %02X]%s",
                pline_head, pin[1], pin[2],pin[3], pin[4], pin[5],
                pin[6], pline_end);
        pcb(buf);
        sprintf(buf, "%sID类型[%d]:%s%s", pline_head, pin[6],
                (pin[0] < 3) ? idtype[pin[6]] : "未定义的ID类型", pline_end);
        pcb(buf);
        sprintf(buf, "%sID长度L:%d%s", pline_head, pin[7], pline_end);
        pcb(buf);
        if(pin[0] == 1) //如果是芯片ID则输出对应的ID信息，模组ID不知道是几个字节376.2协议中未说明
        {
            sprintf(buf, "%sID信息:[%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X "
                    "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X]%s",
                    pline_head, pin[8], pin[9],pin[10], pin[11], pin[12],pin[13],
                    pin[14], pin[15],pin[16], pin[17], pin[18],pin[19],
                    pin[20], pin[21],pin[22], pin[23], pin[24],pin[25],
                    pin[26], pin[27],pin[28], pin[29], pin[30],pin[31],pline_end);
            pcb(buf);
        }
        else
        {
             pcb(pline_head);
             pcb("未定义");
             pcb(pline_end);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出查询网络规模解析字符串
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
AFN10_FN100(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("查询网络规模");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 2);
        sprintf(buf, "%s网络规模[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出查询微功率无线从节点信息解析字符串
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
AFN10_FN101(unsigned char dir,
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
    pcb("查询微功率无线从节点信息");
    pcb(pline_end);

    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 3);
        sprintf(buf, "%s从节点起始序号[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点数量[%02x]:%d%s", pline_head, pin[2], pin[2], pline_end);
        pcb(buf);
    }
    else    //上行
    {
        sprintf(buf, "%s从节点总数量[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s本次应答的从节点数量:%d%s", pline_head, pin[2], pline_end);
        pcb(buf);
        if (len < (pin[2] * 8 + 3))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head,  pline_end);
            pcb(buf);
            return -1;
        }

        for (i = 0; i < pin[2]; i++)
        {
            sprintf(buf, "%s从节点%d地址:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 8 + 3], pin[i * 8 + 4],
                    pin[i * 8 + 5], pin[i * 8 + 6], pin[i * 8 + 7],
                    pin[i * 8 + 8], pline_end);
            pcb(buf);

            sprintf(buf, "%s中继级别:%d%s", pline_head, pin[i * 8 + 9] & 0x0f, pline_end);
            pcb(buf);
            sprintf(buf, "%s侦听信号品质:%d%s", pline_head, pin[i * 8 + 9] >> 4, pline_end);
            pcb(buf);
            sprintf(buf, "%s相位[%d]:%s%s", pline_head, pin[i * 8 + 10] & 0x07,
                    "(D8 D9 D10置\"1\"依次表示第1 2 3相)", pline_end);
            pcb(buf);
            sprintf(buf, "%s通信协议类型[%d]:%s%s",
                    pline_head, (pin[i * 8 + 10] >> 3) & 0x07,
                    (((pin[i * 8 + 10] >> 3) & 0x07) < 4) ? type[(pin[i * 8 + 10] >> 3) & 0x07] : "保留",
                    pline_end);
            pcb(buf);
            sprintf(buf, "%s软件版本[%02X %02X %02X]: 应用程序版本:%02X%02X boot版本:%02X%s", pline_head,
                    pin[i * 8 + 11], pin[i * 8 + 12], pin[i * 8 + 13],
                    pin[i * 8 + 11], pin[i * 8 + 12], pin[i * 8 + 13],
                    pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出查询升级后模块版本信息解析字符串
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
AFN10_FN104(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("查询升级后模块版本信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
//        CHK_APP_LEN(len, 2);
//        sprintf(buf, "%s网络规模[%02X %02X]:%d%s", pline_head,
//                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
//        pcb(buf);
        pcb("TOTO: 未定义数据格式");
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出查询网络信息解析字符串
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
AFN10_FN111(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;

    pcb(pline_head);
    pcb("查询网络信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
        if (len < (pin[0] * 3 + 10))
        {
            pcb(pline_head);
            pcb("ERROR:长度有误!");
            pcb(pline_end);
            return -1;
        }
        sprintf(buf, "%s多网络节点总数量[%02X]:%d%s", pline_head, pin[0], pin[0], pline_end);
        pcb(buf);
        sprintf(buf, "%s本节点网络标识号[%02X %02X %02X]:%d%s", pline_head,
                pin[1], pin[2], pin[3], (pin[1] << 16) | (pin[2] << 8)| pin[3],
                pline_end);
        pcb(buf);
        sprintf(buf, "%s主节点地址:[%02X %02X %02X %02X %02X %02X]%s",
                pline_head, pin[4], pin[5], pin[6], pin[7], pin[8], pin[9],
                pline_end);
        pcb(buf);
        for (i = 0; i < pin[2]; i++)
        {
            sprintf(buf, "%s邻居%d 节点网络标识号[%02X %02X %02X]:%d%s", pline_head, i,
                    pin[10 + i*3], pin[11 + i*3], pin[12 + i*3],
                    (pin[10 + i*3] << 16) | (pin[11 + i*3] << 8)| pin[12 + i*3],
                    pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出查询宽带载波芯片信息解析字符串
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
AFN10_FN112(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;
    const char * const type[] =
    {
        "保留",
        "抄控器",
        "集中器本地通信单元",
        "单相电能表通信单元",
        "中继器",
        "II型采集器",
        "I型采集器",
        "三相电能表通信单元",
    };

    pcb(pline_head);
    pcb("查询宽带载波芯片信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 3);
        sprintf(buf, "%s从节点起始序号[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点数量[%02x]:%d%s", pline_head, pin[2], pin[2], pline_end);
        pcb(buf);
    }
    else    //上行
    {
        if (len < (pin[4] * 33 + 5))
        {
            pcb(pline_head);
            pcb("ERROR:长度有误!");
            pcb(pline_end);
            return -1;
        }

        sprintf(buf, "%s从节点总数量[%02X %02X]:%d%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s节点起始序号[%02X %02X]:%d%s", pline_head,
                pin[2], pin[3], pin[2] | ((int)pin[3] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s本次应答的从节点数量:%d%s", pline_head, pin[4], pline_end);
        pcb(buf);

        for (i = 0; i < pin[4]; i++)
        {
            pcb(pline_end);
            int pos =  i*33 + 5;
            sprintf(buf, "%s节点%d 地址:[%02X %02X %02X %02X %02X %02X]%s", pline_head, i+1,
                    pin[pos], pin[pos + 1], pin[pos + 2], pin[pos + 3], pin[pos + 4], pin[pos + 5],
                    pline_end);
            pcb(buf);
            sprintf(buf, "%s节点%d 设备类型[%02X]:%s%s", pline_head, i+1, pin[pos + 6],
                    (pin[pos + 6] < 8) ? type[pin[pos + 6]] : "保留",  pline_end);
            pcb(buf);
            sprintf(buf, "%s节点%d ID信息:[%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X "
                    "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i+1,
                    pin[pos + 7],  pin[pos + 8],  pin[pos + 9], pin[pos + 10], pin[pos + 11], pin[pos + 12],
                    pin[pos + 13], pin[pos + 14], pin[pos + 15],pin[pos + 16], pin[pos + 17], pin[pos + 18],
                    pin[pos + 19], pin[pos + 20], pin[pos + 21],pin[pos + 22], pin[pos + 23], pin[pos + 24],
                    pin[pos + 25], pin[pos + 26], pin[pos + 27],pin[pos + 28], pin[pos + 29], pin[pos + 30],
                    pline_end);
            pcb(buf);
            sprintf(buf, "%s节点%d 软件版本信息[%02X %02X]:%d%s", pline_head, i+1,
                    pin[pos + 31], pin[pos + 32], pin[pos + 31] | ((int)pin[pos + 32] << 8), pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出路由查询解析字符串
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
print_AFN10(unsigned char dir,
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
        case 1: //从节点数量
            return AFN10_FN01(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 2: //从节点信息
            return AFN10_FN02(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 3: //指定从节点的上一级中继路由信息
            return AFN10_FN03(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 4: //路由运行状态
            return AFN10_FN04(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 5: //未抄读成功的从节点信息
            return AFN10_FN05(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 6: //主动注册的从节点信息
            return AFN10_FN06(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 7: //todo: 查询从节点ID号信息(待完善)
            return AFN10_FN07(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 9: //查询HPLC网络规模
            return AFN10_FN09(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 21: //查询网络拓扑信息
            return AFN10_FN21(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 31: //查询相线信息
            return AFN10_FN31(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 40: //查询ID信息
            return AFN10_FN40(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 100: //查询无线网络规模
            return AFN10_FN100(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 101: //查询微功率无线从节点信息
            return AFN10_FN101(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 104: //todo: 查询升级后模块版本信息(376.2协议中没有)
            return AFN10_FN104(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 111: //查询网络信息
            return AFN10_FN111(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 112: //查询宽带载波芯片信息
            return AFN10_FN112(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        default:
            break;
    }

    pcb(pline_head);
    pcb("ERROR:不支持的FN!");
    pcb(pline_end);

    return -ERR_AFN_FN;
}

/*--------------------------------AFN10.c------------------------------------*/
