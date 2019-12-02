/**
 ******************************************************************************
 * @file       types.h
 * @brief      本文重定义数据类型.
 * @details    This file including all API functions's declare of types.h.
 * @copyright
 ******************************************************************************
 */
#ifndef __TYPES_H__
#define __TYPES_H__

/*-----------------------------------------------------------------------------
 Section: Macro Definitions
 ----------------------------------------------------------------------------*/
/* 下面是POSIX标志的typedef */
typedef          char        char_t;
typedef signed   char        int8_t;
typedef signed   short       int16_t;
typedef signed   int         int32_t;
typedef signed   long        int64_t;
typedef unsigned char        uint8_t;
typedef unsigned short       uint16_t;
typedef unsigned int         uint32_t;
typedef unsigned long long   uint64_t;
typedef          float       float32_t;
typedef          double      float64_t;
typedef long     double      float128_t;
//typedef signed   int         size_t;

#define __INLINE    inline
typedef enum
{
    FALSE = 0,
    TRUE = 1
} bool_e;
typedef int     status_t;

#define OK      0
#define ERROR  -1

#ifndef NULL
#define NULL    ((void *)0)
#endif

typedef void    (*OSFUNCPTR) (void *);  /* ptr to function returning int */
typedef int     (*FUNCPTR) ();          /* ptr to function returning int */
typedef void    (*VOIDFUNCPTR) ();      /* ptr to function returning void */
typedef double  (*DBLFUNCPTR) ();       /* ptr to function returning double*/
typedef float   (*FLTFUNCPTR) ();       /* ptr to function returning float */
typedef void    (*VOIDFUNCPTRBOOL)(bool_e b);
typedef void    (*VOIDFUNCPTRBOOLE)(bool_e b);

#define ARRAY_SIZE(a) (sizeof (a) / sizeof ((a)[0]))

#define WEAK __attribute__ ((weak))

#endif /* __TYPES_H__ */
/*-----------------------------End of types.h--------------------------------*/
