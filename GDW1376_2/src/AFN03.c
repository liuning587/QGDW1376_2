/**
 ******************************************************************************
 * @file      AFN03.c
 * @brief     C Source file of AFN03.c.
 * @details   This file including all API functions's implement of AFN03.c.
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
 * @brief   输出厂商代码和版本信息解析字符串
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
AFN03_FN01(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("厂商代码和版本信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 9);

        sprintf(buf, "%s  厂商代码[%02X %02X]:%c%c%s", pline_head, pin[0], pin[1], pin[0], pin[1], pline_end);
        pcb(buf);
        sprintf(buf, "%s  芯片代码[%02X %02X]:%c%c%s", pline_head, pin[2], pin[3], pin[2], pin[3], pline_end);
        pcb(buf);
        sprintf(buf, "%s  版本日期[%02X %02X %02X]:%02x年%02x月%02x日%s", pline_head, pin[6], pin[5], pin[4], pin[6], pin[5], pin[4], pline_end);
        pcb(buf);
        sprintf(buf, "%s  版本[%02X %02X]:%02x.%02x%s", pline_head, pin[7], pin[8], pin[7], pin[8], pline_end);
        pcb(buf);
    }

    return ERR_NONE;
}

/**
 ******************************************************************************
 * @brief   输出噪声值解析字符串
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
AFN03_FN02(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("噪声值");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 1);
        sprintf(buf, "%s噪声值:%d%s", pline_head, pin[0] & 0x0f, pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出从节点侦听信息解析字符串
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
AFN03_FN03(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;

    pcb(pline_head);
    pcb("从节点侦听信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 2);
        sprintf(buf, "%s开始节点指针:%d%s", pline_head, pin[0], pline_end);
        pcb(buf);
        sprintf(buf, "%s读取节点的数量N<=16:%d%s", pline_head, pin[1], pline_end);
        pcb(buf);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 2 + pin[1] * 8);
        sprintf(buf, "%s侦听到的从节点总数量:%d%s", pline_head, pin[0], pline_end);
        pcb(buf);
        sprintf(buf, "%s侦听到的本帧传输的从节点数量:%d%s", pline_head, pin[1], pline_end);
        pcb(buf);

        for (i = 0; i < pin[1]; i++)
        {
            sprintf(buf, "%s从节点地址%d:[%02X %02X %02X %02X %02X %02X]%s",
                    pline_head, i, pin[i * 8 + 2], pin[i * 8 + 3],
                    pin[i * 8 + 4], pin[i * 8 + 5], pin[i * 8 + 6],
                    pin[i * 8 + 7], pline_end);
            pcb(buf);

            sprintf(buf, "%s中继级别:%d%s", pline_head, pin[i * 8 + 8] & 0x0f, pline_end);
            pcb(buf);
            sprintf(buf, "%s侦听信号品质:%d%s", pline_head, pin[i * 8 + 8] >> 4, pline_end);
            pcb(buf);
            sprintf(buf, "%s侦听次数:%d%s", pline_head, pin[i * 8 + 9] & 0x0f, pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出主节点地址解析字符串
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
AFN03_FN04(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("主节点地址");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 6);
        sprintf(buf, "%s主节点地址:[%02X %02X %02X %02X %02X %02X]%s",
                pline_head, pin[0], pin[1], pin[2], pin[3], pin[4], pin[5],
                pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出主节点状态字和通信速率解析字符串
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
AFN03_FN05(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;
    const char * const mode[] =
    {
        "00通信模块对两种抄表模式都支持",
        "01通信模块仅支持集中器主导的抄表模式",
        "10通信模块仅支持路由主导的抄表模式",
        "11无效",
    };
    const char * const feature[] =
    {
        "微功率无线",
        "单相供电单相传输",
        "单相供电三相传输",
        "三相供电三相传输",
    };

    pcb(pline_head);
    pcb("主节点状态字和通信速率");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
        sprintf(buf, "%s周期抄表模式[%d]:%s%s", pline_head, pin[0] >> 6,
                mode[pin[0] >> 6], pline_end);
        pcb(buf);
        sprintf(buf, "%s周期抄表模式[%d]:%s%s", pline_head, (pin[0] >> 4) & 0x03,
                feature[(pin[0] >> 4) & 0x03], pline_end);
        pcb(buf);
        sprintf(buf, "%s速率数量:%d%s", pline_head, pin[0] & 0x0f, pline_end);
        pcb(buf);
        sprintf(buf, "%s信道数量:%d%s", pline_head, pin[1] & 0x0f, pline_end);
        pcb(buf);
        if (((pin[0] & 0x0f) * 2 + 2) > len)
        {
            return -1;
        }
        for (i = 0; i < (pin[0] & 0x0f); i++)
        {
            sprintf(buf, "%s通信速率[%02X %02X]:%d%s%s", pline_head,
                    pin[i*2 + 2], pin[i*2 + 2 + 1],
                    pin[i*2 + 2] | ((int)(pin[i*2 + 2 + 1] & 0x7f) << 8),
                    (pin[i*2 + 2 + 1] & 0x80) ? "kbit/s" : "bit/s",pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出主节点干扰状态解析字符串
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
AFN03_FN06(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("主节点干扰状态");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 1);
        sprintf(buf, "%s持续时间:%d min(指等待查询执行的时间)%s",
                pline_head, pin[0], pline_end);
        pcb(buf);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 1);
        sprintf(buf, "%s主节点干扰状态[%d]:%s干扰%s", pline_head, pin[0],
                pin[0] ? "有" : "没有", pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出读取从节点监控最大超时时间解析字符串
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
AFN03_FN07(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("读取从节点监控最大超时时间");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 1);
        sprintf(buf, "%s最大超时时间[%02X]:%ds%s", pline_head,
                pin[0], pin[0], pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出查询无线通信参数解析字符串
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
AFN03_FN08(unsigned char dir,
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
    pcb("查询无线通信参数");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 2);
        sprintf(buf, "%s无线信道组0~63[%02X]:%d%s", pline_head, pin[0],
                pin[0], pline_end);
        pcb(buf);
        sprintf(buf, "%s无线主节点发射功率[%02X]:%s%s", pline_head, pin[1],
                (pin[1] < 4) ? pwr[pin[1]] : "无效", pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出查询从节点通信延时解析字符串
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
AFN03_FN09(unsigned char dir,
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
    pcb("查询从节点通信延时");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        sprintf(buf, "%s通信协议类型[%d]:%s%s",
                pline_head, pin[0],
                (pin[0] < 4) ? type[pin[0]] : "无效",
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
        sprintf(buf, "%s广播通信延迟时间[%02X %02X]:%ds%s", pline_head,
                pin[0], pin[1], pin[0] | ((int)pin[1] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s通信协议类型[%d]:%s%s",
                pline_head, pin[2],
                (pin[2] < 4) ? type[pin[2]] : "无效",
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
            sprintf(buf, "%02X ", pin[i + 4]);
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
 * @brief   输出本地通信模块运行模式信息解析字符串
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
AFN03_FN10(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;

    pcb(pline_head);
    pcb("本地通信模块运行模式信息");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 39 + 2 * (pin[3] & 0x0f));
        /* 1. 通信方式 */
        sprintf(buf, "%s通信方式[%d]:", pline_head, pin[0] & 0x0f);
        pcb(buf);
        switch (pin[0] & 0x0f)
        {
            case 1:
                pcb("窄带电力线载波通信");
                break;
            case 2:
                pcb("宽带电力线载波通信");
                break;
            case 3:
                pcb("微功率无线通信");
                break;
            default:
                pcb("其他保留");
                break;
        }
        pcb(pline_end);
        /* 2. 路由管理方式 */
        sprintf(buf, "%s路由管理方式 [%d]:%s%s", pline_head, (BITS(pin[0], 4)),
                (BITS(pin[0], 4)) ? "广播命令要根据报文中的信道标识逐个来发送"
                        : "执行广播命令不需要信道标识", pline_end);
        pcb(buf);
        /* 3. 从节点信息模式 */
        sprintf(buf, "%s从节点信息模式[%d]:%s%s", pline_head, (BITS(pin[0], 5)),
                (BITS(pin[0], 5)) ? "表示需要下发从节点信息"
                        : "表示不需要下发从节点信息", pline_end);
        pcb(buf);
        /* 4. 周期抄表模式 */
        sprintf(buf, "%s周期抄表模式[%d]:", pline_head, pin[0] >> 6u);
        pcb(buf);
        switch (pin[0] >> 6u)
        {
            case 1:
                pcb("表示通信模块仅支持集中器主导的抄表模式");
                break;
            case 2:
                pcb("表示通信模块仅支持路由主导的抄表模式");
                break;
            case 3:
                pcb("表示通信模块对两种抄表模式都支持");
                break;
            default:
                pcb("其他保留");
                break;
        }
        pcb(pline_end);
        /*
         * 5. 传输延时参数支持
         *   由D2、D1、D0等三个位表示，分别代表广播、从节点监控、路由主动抄表等
         *   三种命令支持向集中器提供传输延时参数的情况，
         *   1 表示支持提供传输延时参数
         *   0 表示不支持
         */
        sprintf(buf, "%s传输延时参数支持[%d]:%s", pline_head, pin[1] & 0x03, pline_end);
        pcb(buf);
        sprintf(buf, "%s          广播:%s%s", pline_head, BITS(pin[1], 2) ? "支持" : "不支持", pline_end);
        pcb(buf);
        sprintf(buf, "%s    从节点监控:%s%s", pline_head, BITS(pin[1], 1) ? "支持" : "不支持", pline_end);
        pcb(buf);
        sprintf(buf, "%s  路由主动抄表:%s%s", pline_head, BITS(pin[1], 0) ? "支持" : "不支持", pline_end);
        pcb(buf);
        /*
         * 6. 失败节点切换发起方式
         *   D3置1 表示通信模块自主切换待抄节点
         *   D4置1 表示集中器发起通知通信模块切换待抄节点
         */
        sprintf(buf, "%s失败节点切换发起方式[%d]:", pline_head, (pin[1] >> 3) & 0x03);
        pcb(buf);
        switch ((pin[1] >> 3) & 0x03)
        {
            case 1:
                pcb("表示通信模块自主切换待抄节点");
                break;
            case 2:
                pcb("表示集中器发起通知通信模块切换待抄节点");
                break;
            default:
                pcb("保留");
                break;
        }
        pcb(pline_end);
        /*
         * 7. 广播命令确认方式
         *   0 表示广播命令在本地通信模块执行广播通信过程完毕后返回确认报文
         *   1 表示广播命令在本地信道执行广播通信之前就返回确认报文，
         *     需要集中器等待的时间在确认报文的"等待执行时间"信息中体现
         */
        sprintf(buf, "%s广播命令确认方式[%d]:%s%s", pline_head, (BITS(pin[1], 5)),
                (BITS(pin[1], 5)) ? "广播命令在本地信道执行广播通信之前就返回确认报文需要集中器等待的时间在确认报文的\"等待执行时间\"信息中体现"
                        : "广播命令在本地通信模块执行广播通信过程完毕后返回确认报文", pline_end);
        pcb(buf);
        /*
         * 8. 广播命令信道执行方式
         *   0 表示执行广播命令不需要信道标识
         *   1 表示广播命令要根据报文中的信道标识逐个来发送
         *   其他取值保留
         */
        sprintf(buf, "%s广播命令信道执行方式[%d]:", pline_head, pin[1] >> 6);
        pcb(buf);
        switch (pin[1] >> 6)
        {
            case 0:
                pcb("表示执行广播命令不需要信道标识");
                break;
            case 1:
                pcb("表示广播命令要根据报文中的信道标识逐个来发送");
                break;
            default:
                pcb("保留");
                break;
        }
        pcb(pline_end);
        /* 9. 信道数量 */
        sprintf(buf, "%s信道数量[%d]%s", pline_head, pin[2] & 0x1f, pline_end);
        pcb(buf);
        /*
         * 10. 低压电网掉电信息(仅对于带有过个逻辑信道的低压电力线载波有效)
         *   由D7、D6、D5等三个位表示，分别表示A B C三相的掉电信息
         *   0 表示未掉电 1 表示掉电
         */
        sprintf(buf, "%s低压电网掉电信息[%d]%s", pline_head, pin[2] >> 5, pline_end);
        pcb(buf);
        sprintf(buf, "  A相:%s", BITS(pin[2], 7) ? "掉电" : "未掉电");
        pcb(buf);
        sprintf(buf, "  B相:%s", BITS(pin[2], 6) ? "掉电" : "未掉电");
        pcb(buf);
        sprintf(buf, "  C相:%s%s", BITS(pin[2], 5) ? "掉电" : "未掉电", pline_end);
        pcb(buf);
        /* 11. 速率数量n */
        sprintf(buf, "%s速率数量n[%d]%s", pline_head, (pin[3] & 0x0f), pline_end);
        pcb(buf);
        /* 12. 从节点监控最大超时时间 */
        sprintf(buf, "%s从节点监控最大超时时间[%02X]:%ds%s", pline_head,
                pin[6], pin[6], pline_end);
        pcb(buf);
        /* 13. 广播命令最大超时时间 */
        sprintf(buf, "%s广播命令最大超时时间[%02X %02X]:%ds%s", pline_head,
                pin[7], pin[8], (pin[8] << 8) | pin[7], pline_end);
        pcb(buf);
        /* 14. 最大支持的报文长度 */
        sprintf(buf, "%s最大支持的报文长度[%02X %02X]:%d%s", pline_head,
                pin[9], pin[10], (pin[10] << 8) | pin[9], pline_end);
        pcb(buf);
        /* 15. 文件传输支持的最大单个数据包长度 */
        sprintf(buf, "%s文件传输支持的最大单个数据包长度[%02X %02X]:%d%s", pline_head,
                pin[11], pin[12], (pin[12] << 8) | pin[11], pline_end);
        pcb(buf);
        /* 16. 升级操作等待时间 */
        sprintf(buf, "%s升级操作等待时间[%02X]:%ds%s", pline_head,
                pin[13], pin[13], pline_end);
        pcb(buf);
        /* 17. 主节点地址 */
        sprintf(buf, "%s主节点地址:[%02X %02X %02X %02X %02X %02X]%s",
                pline_head, pin[14], pin[15], pin[16], pin[17], pin[18], pin[19],
                pline_end);
        pcb(buf);
        /* 18. 支持最大从节点数量 */
        sprintf(buf, "%s支持最大从节点数量[%02X %02X]:%d%s", pline_head,
                pin[20], pin[21], (pin[21] << 8) | pin[20], pline_end);
        pcb(buf);
        /* 18. 当前从节点数量 */
        sprintf(buf, "%s当前从节点数量[%02X %02X]:%d%s", pline_head,
                pin[22], pin[23], (pin[23] << 8) | pin[22], pline_end);
        pcb(buf);
        /* 19. 通信模块使用的协议发布日期(BCD) YYMMDD */
        sprintf(buf, "%s通信模块使用的协议发布日期[%02X %02X %02X]:%02X-%02X-%02X%s", pline_head,
                pin[24], pin[25], pin[26], pin[24], pin[25], pin[26], pline_end);
        pcb(buf);
        /* 20. 通信模块使用的协议最后备案日期(BCD) YYMMDD */
        sprintf(buf, "%s通信模块使用的协议最后备案日期[%02X %02X %02X]:%02X-%02X-%02X%s", pline_head,
                pin[27], pin[28], pin[29], pin[27], pin[28], pin[29], pline_end);
        pcb(buf);
        /* 21. 通信模块厂商代码及版本信息 */
        sprintf(buf, "%s  厂商代码[%02X %02X]:%c%c%s", pline_head, pin[31], pin[30], pin[31], pin[30], pline_end);
        pcb(buf);
        sprintf(buf, "%s  芯片代码[%02X %02X]:%c%c%s", pline_head, pin[33], pin[32], pin[33], pin[32], pline_end);
        pcb(buf);
        sprintf(buf, "%s  版本日期[%02X %02X %02X]:%02x年%02x月%02x日%s", pline_head, pin[36], pin[35], pin[34], pin[36], pin[35], pin[34], pline_end);
        pcb(buf);
        sprintf(buf, "%s  版本[%02X %02X]:%02x.%02x%s", pline_head, pin[37], pin[38], pin[37], pin[38], pline_end);
        pcb(buf);

        for (i = 0; i < (pin[3] & 0x0f); i++)
        {
            sprintf(buf, "%s通信速率[%02X %02X]:%d%s%s", pline_head,
                    pin[i*2 + 39], pin[i*2 + 39 + 1],
                    pin[i*2 + 39] | ((int)(pin[i*2 + 39 + 1] & 0x7f) << 8),
                    (pin[i*2 + 39 + 1] & 0x80) ? "kbit/s" : "bit/s",pline_end);
            pcb(buf);
        }
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出本地通信模块AFN索引解析字符串
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
AFN03_FN11(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int cnt = 0;
    int i;

    pcb(pline_head);
    pcb("本地通信模块AFN索引");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 1);
        sprintf(buf, "%sAFN功能码[0x%02X]:%d%s",
                pline_head, pin[0], pin[0], pline_end);
        pcb(buf);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 1 + 32);
        sprintf(buf, "%sAFN功能码[0x%02X]:%d%s",
                pline_head, pin[0], pin[0], pline_end);
        pcb(buf);
        sprintf(buf, "%s支持FN:", pline_head);
        pcb(buf);
        for (i = 0; i < 32 * 8; i++)
        {
            if (BITS(pin[1 + i / 8], i))
            {
                if (!(cnt % 10))
                {
                    sprintf(buf, "%s  ", pline_end);
                    pcb(buf);
                }
                sprintf(buf, "F%d、", i + 1);
                pcb(buf);
                cnt++;
            }
        }
        pcb(pline_end);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出查询场强门限解析字符串
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
AFN03_FN100(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("查询场强门限");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 0);
    }
    else    //上行
    {
        CHK_APP_LEN(len, 1);
        sprintf(buf, "%s场强门限[0x%02X]:%d(取值50~120,默认96)%s",
                pline_head, pin[0], pin[0], pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出查询数据解析字符串
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
print_AFN03(unsigned char dir,
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
        case 1: //厂商代码和版本信息
            return AFN03_FN01(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 2: //噪声值
            return AFN03_FN02(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 3: //从节点侦听信息
            return AFN03_FN03(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 4: //主节点地址
            return AFN03_FN04(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 5: //主节点状态字和通信速率
            return AFN03_FN05(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 6: //主节点干扰状态
            return AFN03_FN06(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 7: //读取从节点监控最大超时时间
            return AFN03_FN07(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 8: //查询无线通信参数
            return AFN03_FN08(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 9: //查询从节点通信延时
            return AFN03_FN09(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 10: //本地通信模块运行模式信息
            return AFN03_FN10(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 11: //本地通信模块AFN索引
            return AFN03_FN11(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
//        case 12: //todo: 询本地主节点通信模块 ID号信息
//            return AFN03_FN12(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
//        case 16: //todo: 查询宽带载波通信参数
//            return AFN03_FN16(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 100: //查询场强门限
            return AFN03_FN100(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        default:
            break;
    }

    pcb(pline_head);
    pcb("ERROR:不支持的FN!");
    pcb(pline_end);

    return -ERR_AFN_FN;
}

/*--------------------------------AFN03.c------------------------------------*/
