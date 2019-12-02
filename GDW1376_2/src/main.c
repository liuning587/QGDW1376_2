/*
 ============================================================================
 Name        : GDW1376_2.c
 Author      : LiuNing
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "../inc/GDW1376_2.h"

#ifdef DEBUG_MAIN
static const unsigned char test[] = {0x68, 0x0F, 0x00, 0x41, 0x01, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0x03, 0x01, 0x00, 0x45, 0x16};

static void
print_cb(const char *pstr)
{
    printf(pstr);
}

int main(void) {
    int ret;
	ret = GDW1376_2_parse(test, sizeof(test), print_cb, "", "\n");
	if (ret)
	{
	    puts(GDW1376_2_error(ret));
	}
	return EXIT_SUCCESS;
}
#endif
