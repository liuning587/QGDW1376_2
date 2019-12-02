/**
 ******************************************************************************
 * @file      AFN15.c
 * @brief     C Source file of AFN15.c.
 * @details   This file including all API functions's implement of AFN15.c.
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
#pragma pack(push, 1)

typedef struct
{
    uint8_t file_flag;          /**< 文件标识 */
    uint8_t file_attr;          /**< 文件属性 */
    uint8_t file_instr;         /**< 文件指令 */
    uint16_t seg_count;         /**< 总段数n */
    uint32_t seg_flag;          /**< 第i段标识(i=0~n) */
    uint16_t seg_len;           /**< 第i段数据长度 */
} file_tran_req_t;

#pragma pack(pop)

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
 * @brief   输出文件传输方式1解析字符串
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
AFN15_FN01(unsigned char dir,
        const unsigned char *pin,
        int len,
        pcallback pcb,
        const char *pline_head,
        const char *pline_end)
{
    int i;
    pcb(pline_head);
    pcb("文件传输方式1");
    pcb(pline_end);
    if (dir == 0)   //下行
    {
        if (len < (int)sizeof(file_tran_req_t))
        {
            sprintf(buf, "%s输入报文长度不足解析失败!%s", pline_head,  pline_end);
            pcb(buf);
            return -1;
        }

        sprintf(buf, "%s文件标识[%02X]:", pline_head, pin[0]);
        pcb(buf);
        switch (pin[0])
        {
            case 0:
                pcb("清除下装文件\n");
                break;
            case 3:
                pcb("本地通信模块升级文件\n");
                break;
            case 7:
                pcb("主节点和子节点模块升级\n");
                break;
            case 8:
                pcb("主节点模块升级\n");
                break;
            default:
                pcb("备用\n");
                break;
        }
        pcb(pline_end);

        sprintf(buf, "%s文件属性[%02X]:%s%s", pline_head, pin[1], !pin[1] ? "起始帧、中间帧" : "结束帧", pline_end);
        pcb(buf);
        sprintf(buf, "%s文件指令[%02X]:%s%s", pline_head, pin[2], !pin[2] ? "报文方式下装" : "备用", pline_end);
        pcb(buf);
        sprintf(buf, "%s总段数[%02X %02X]:%d%s", pline_head, pin[3], pin[4], (pin[4] << 8) | pin[3], pline_end);
        pcb(buf);
        sprintf(buf, "%s第i段标识[%02X %02X %02X %02X]:%d%s", pline_head, pin[5], pin[6], pin[7], pin[8],
                (pin[8] << 24) | (pin[7] << 16) | (pin[6] << 8) | pin[5], pline_end);
        pcb(buf);
        sprintf(buf, "%s第i段数据长度[%02X %02X]:%d%s", pline_head, pin[9], pin[10], (pin[10] << 8) | pin[9], pline_end);
        pcb(buf);

        CHK_APP_LEN(len - sizeof(file_tran_req_t), ((pin[10] << 8) | pin[9]));

        pcb(pline_head);

        pcb("数据:");
        pcb(pline_end);
        for (i = 0; i < ((pin[10] << 8) | pin[9]); i++)
        {
            sprintf(buf, "%02X ", pin[i + 11]);
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
        CHK_APP_LEN(len, 4);
        sprintf(buf, "%s当前段标识[%02X %02X %02X %02X]:%d%s", pline_head, pin[0], pin[1], pin[2], pin[3],
                (pin[3] << 24) | (pin[2] << 16) | (pin[1] << 8) | pin[0], pline_end);
        pcb(buf);
    }

    return 0;
}

/**
 ******************************************************************************
 * @brief   输出文件传输解析字符串
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
print_AFN15(unsigned char dir,
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
        case 1: //文件传输方式1
            return AFN15_FN01(dir, pin + 2, len - 2, pcb, pline_head, pline_end);
        default:
            break;
    }

    pcb(pline_head);
    pcb("ERROR:不支持的FN!");
    pcb(pline_end);

    return -ERR_AFN_FN;
}

/*--------------------------------AFN15.c------------------------------------*/
