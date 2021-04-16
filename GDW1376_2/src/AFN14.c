/**
 ******************************************************************************
 * @file      AFN14.c
 * @brief     C Source file of AFN14.c.
 * @details   This file including all API functions's implement of AFN14.c.
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
 * @brief   输出路由请求抄读内容解析字符串
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
AFN14_FN01(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{   int i=3, j=1, k=1;
    pcb(pline_head);
    pcb("路由请求抄读内容");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        if(pin[0]==0)
        {
            pcb(pline_head);
            pcb("抄读失败");
            pcb(pline_end);
        }
        else if(pin[0]==1)
        {
            pcb(pline_head);
            pcb("抄读成功");
            pcb(pline_end);
            pcb(pline_head);
            pcb("路由请求数据长度L=0,从节点附属节点数量n=0");
            pcb(pline_end);
        }
        else if(pin[0]==2)
        {
            switch(pin[1])
            {
                case 0:
                    pcb(pline_head);
                    pcb("当前通信的应用数据和本地信道的通信延时无关");
                    pcb(pline_end);
                    break;
                case 1:
                    pcb(pline_head);
                    pcb("当前通信的应用数据和本地信道的通信延时相关");
                    pcb(pline_end);
                    break;
                default:
                    break;
            }
            sprintf(buf, "%s路由请求数据长度L=%d%s", pline_head, pin[2],pline_end);
            pcb(buf);
            pcb(pline_head);
            pcb("路由请求数据内容为");
            for(i=3; i<(3+pin[2]); i++)
            {
              (void)sprintf(buf, "%02X ", pin[i]);
              pcb(buf);
            }
            if(pin[3+pin[2]]>0)
            {
                pcb(pline_end);
                sprintf(buf, "%s从节点附属节点数量n=%d%s", pline_head, pin[3+pin[2]], pline_end);
                pcb(buf);
                for(j=1; j<pin[3+pin[3]]; j++)
                {
                  pcb(pline_head);
                  sprintf(buf, "从节点附属节点%d地址为", j);
                  pcb(buf);
                  for(k=1; k<7; k++)
                  {
                   (void)sprintf(buf, "%02X", pin[4+pin[3]+k]);
                    pcb(buf);
                  }
                   pcb(pline_end);
                }
            }
            else
            {
                pcb(pline_end);
                sprintf(buf, "%s从节点附属节点数量n=%d%s", pline_head, pin[3+pin[2]], pline_end);
                pcb(buf);
            }
        }
    }
    else    //上行
    {
        sprintf(buf, "%s通信相位[%d]:%s%d相%s", pline_head, pin[0],
                (pin[0] == 0) ? "未知相," : "", pin[0], pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点地址:[%02X %02X %02X %02X %02X %02X]%s",
                pline_head, pin[1], pin[2], pin[3], pin[4], pin[5], pin[6],
                pline_end);
        pcb(buf);
        sprintf(buf, "%s从节点序号[%02X %02X]:%d%s", pline_head,
                pin[7], pin[8],
                pin[7] | ((int)pin[8] << 8), pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出路由请求集中器时钟解析字符串
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
AFN14_FN02(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    pcb(pline_head);
    pcb("路由请求集中器时钟");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        CHK_APP_LEN(len, 6);
        (void)sprintf(buf, "%s%x年%x月%x日%x时%x分%x秒%s", pline_head, pin[5], pin[4],
                pin[3], pin[2], pin[1], pin[0], pline_end);
        pcb(buf);
    }
    else    //上行
    {
        pcb(pline_head);
        pcb("无数据单元");
        pcb(pline_end);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出请求依通信延时修正通信数据解析字符串
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
AFN14_FN03(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;

    pcb(pline_head);
    pcb("请求依通信延时修正通信数据");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        (void)sprintf(buf, "%s数据长度L为%x%s", pline_head, pin[0], pline_end);
        pcb(buf);
        pcb(pline_head);
        pcb("修正通信数据内容为");
        for(i=1; i<pin[0]; i++)
        {
            (void)sprintf(buf, "%02X ", pin[i]);
            pcb(buf);
        }
        pcb(pline_end);
    }
    else    //上行
    {
        sprintf(buf, "%s从节点地址:[%02X %02X %02X %02X %02X %02X]%s",
                pline_head, pin[0], pin[1], pin[2], pin[3], pin[4], pin[5],
                pline_end);
        pcb(buf);
        sprintf(buf, "%s预计延迟时间[%02X %02X]:%ds%s", pline_head,
                pin[6], pin[7],
                pin[6] | ((int)pin[7] << 8), pline_end);
        pcb(buf);
        sprintf(buf, "%s抄读信息长度L:%d%s", pline_head, pin[8], pline_end);
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

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出路由数据抄读解析字符串
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
print_AFN14(unsigned char dir,
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
        case 1: //路由请求抄读内容
            return AFN14_FN01(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 2: //路由请求集中器时钟
            return AFN14_FN02(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        case 3: //请求依通信延时修正通信数据
            return AFN14_FN03(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
//        case 4: //todo: 路由请求交采信息
//            return AFN14_FN04(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        default:
            break;
    }

    pcb(pline_head);
    pcb("ERROR:不支持的FN!");
    pcb(pline_end);

    return -ERR_AFN_FN;
}

/*--------------------------------AFN14.c------------------------------------*/
