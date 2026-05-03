CC      = gcc
CC32    = gcc
CFLAGS  = -Wall -O2 -fvisibility=hidden
DEF     = GDW1376_2.def

SRC_DIR = src
INC_DIR = inc

SRCS    = $(SRC_DIR)/GDW1376_2.c \
          $(SRC_DIR)/chkfrm.c \
          $(SRC_DIR)/lib.c \
          $(SRC_DIR)/format.c \
          $(SRC_DIR)/AFN.c \
          $(SRC_DIR)/AFN00.c \
          $(SRC_DIR)/AFN01.c \
          $(SRC_DIR)/AFN02.c \
          $(SRC_DIR)/AFN03.c \
          $(SRC_DIR)/AFN04.c \
          $(SRC_DIR)/AFN05.c \
          $(SRC_DIR)/AFN06.c \
          $(SRC_DIR)/AFN10.c \
          $(SRC_DIR)/AFN11.c \
          $(SRC_DIR)/AFN12.c \
          $(SRC_DIR)/AFN13.c \
          $(SRC_DIR)/AFN14.c \
          $(SRC_DIR)/AFN15.c \
          $(SRC_DIR)/AFNf0.c \
          $(SRC_DIR)/AFNf1.c

OBJS_32 = $(SRCS:.c=.o32)
OBJS_64 = $(SRCS:.c=.o64)

TARGET_32 = GDW1376_2.dll
TARGET_64 = GDW1376_2_64.dll

.PHONY: all dll32 dll64 clean

all: dll32 dll64

dll32: $(TARGET_32)

dll64: $(TARGET_64)

$(TARGET_32): $(OBJS_32)
	$(CC32) -m32 -shared -o $@ $^ -Wl,--out-implib,libGDW1376_2.a $(DEF)

$(TARGET_64): $(OBJS_64)
	$(CC) -shared -o $@ $^ -Wl,--out-implib,libGDW1376_2_64.a $(DEF)

%.o32: %.c
	$(CC32) -m32 $(CFLAGS) -I$(SRC_DIR) -I$(INC_DIR) -c -o $@ $<

%.o64: %.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -I$(INC_DIR) -c -o $@ $<

clean:
	$(RM) *.dll *.a
	$(RM) $(OBJS_32) $(OBJS_64)
