/**
 ******************************************************************************
 * @file       lib.h
 * @brief      API include file of lib.h.
 * @details    This file including all API functions's declare of lib.h.
 * @copyright
 ******************************************************************************
 */
#ifndef LIB_H_
#define LIB_H_ 

/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include "../inc/GDW1376_2.h"

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
#define CHK_APP_LEN(len, chklen)     {\
    if (len != chklen)  \
    {   \
        pcb_len_err(pcb, pline_head, pline_end, len, chklen);\
        return -ERR_APP_LEN;    \
     }  \
}

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Globals
 ----------------------------------------------------------------------------*/
extern char buf[256];

/*-----------------------------------------------------------------------------
 Section: Function Prototypes
 ----------------------------------------------------------------------------*/
extern unsigned char
get_cs(const unsigned char *pfbuf,
        unsigned int len);

extern int
get_fn(unsigned char DT1,
        unsigned char DT2);

extern int
get_clear_fn(unsigned char *DT1,
        unsigned char *DT2);

extern void
pcb_len_err(pcallback pcb,
        const char *pline_head,
        const char *pline_end,
        int len,
        int checklen);

extern int
txt_to_buf(const char *pin,
        int inlen,
        unsigned char  *pout,
        int olen);

#endif /* LIB_H_ */
/*------------------------------End of lib.h---------------------------------*/
