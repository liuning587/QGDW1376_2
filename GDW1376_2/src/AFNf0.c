/**
 ******************************************************************************
 * @file      AFNf0.c
 * @brief     C Source file of AFNf0.c.
 * @details   AFN=F0H 内部调试：标准 6.5.14 为厂家自定义，此处仅输出 Fn 与数据域 HEX。
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
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
/**
 ******************************************************************************
 * @brief   输出内部调试（AFN=F0H）解析字符串
 ******************************************************************************
 */
int
print_AFNf0(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;
    int fn;
    int paylen;

    (void)dir;

    if (len < 2)
    {
        pcb(pline_head);
        pcb("ERROR:长度不足(DT1/DT2)!");
        pcb(pline_end);
        return -ERR_APP_LEN;
    }

    fn = get_fn(pin[0], pin[1]);
    paylen = len - 2;

    pcb(pline_head);
    pcb("内部调试(AFN=F0H,厂家自定义)");
    pcb(pline_end);
    sprintf(buf, "%sDT1=[%02X] DT2=[%02X] 推算Fn=%d%s", pline_head,
            pin[0], pin[1], fn, pline_end);
    pcb(buf);

    if (paylen <= 0)
    {
        pcb(pline_head);
        pcb("无数据单元");
        pcb(pline_end);
    }
    else
    {
        pcb(pline_head);
        pcb("数据单元(原始):");
        pcb(pline_end);
        pcb(pline_head);
        for (i = 0; i < paylen; i++)
        {
            sprintf(buf, "%02X ", pin[2 + i]);
            pcb(buf);
            if (((i + 1) % 16) == 0)
            {
                pcb(pline_end);
                pcb(pline_head);
            }
        }
        pcb(pline_end);
    }

    return ERR_NONE;
}

/*--------------------------------AFNf0.c------------------------------------*/
