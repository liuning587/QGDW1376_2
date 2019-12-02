/**
 ******************************************************************************
 * @file      AFN.c
 * @brief     C Source file of AFN.c.
 * @details   This file including all API functions's implement of AFN.c.
 * @copyright
 ******************************************************************************
 */
 
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "AFN.h"
#include "../inc/GDW1376_2.h"

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
extern int
print_AFN00(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN01(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN02(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN03(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN04(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN05(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN06(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN10(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN11(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN12(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN13(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN14(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFN15(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFNf0(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

extern int
print_AFNf1(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end);

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
/**
 ******************************************************************************
 * @brief   输出应用层解析字符串
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
print_AFN(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    switch (pin[0]) //AFN
    {
        case 0x00:  //确认/否认
            return print_AFN00(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x01:  //初始化
            return print_AFN01(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x02:  //数据转发
            return print_AFN02(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x03:  //查询数据
            return print_AFN03(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x04:  //链路检测
            return print_AFN04(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x05:  //控制命令
            return print_AFN05(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x06:  //主动上报
            return print_AFN06(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x10:  //路由查询
            return print_AFN10(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x11:  //路由设置
            return print_AFN11(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x12:  //路由控制
            return print_AFN12(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x13:  //路由数据转发
            return print_AFN13(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x14:  //路由数据抄读
            return print_AFN14(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0x15:  //文件传输
            return print_AFN15(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0xf0:  //内部测试
            return print_AFNf0(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        case 0xf1:  //并发抄表
            return print_AFNf1(dir, pin + 1, len - 1, pcb, pline_head, pline_end);
        default:
            break;
    }
    return -1;
}

/*---------------------------------AFN.c-------------------------------------*/
