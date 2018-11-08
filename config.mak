# Build Version info
VERINFO				= V001

#########################################################################
# build environment
##########################################################################
CFLAGS				:=
DEBUG				?= n

# chip name
#CHIPNAME			?= sip_s31nx
CHIPNAME			?= nxp3220

# (ddr3/lpddr3) memory type
#MEMTYPE				?= ddr4
MEMTYPE				?= ddr3
#MEMTYPE				?= lpddr4
#MEMTYPE				?= lpddr3
MEMTEST				?= n

# board name
BOARD				?= vtk
#BOARD				?= evb
#BOARD				?= trike

# pmic name
#PMIC				?= sm5011
PMIC				?= nxe1500
#PMIC				?= nxe2000

# system log Message
SYSLOG				?= y
EARLY_SERIAL			?= y

# cross-tool pre-header
ifeq ($(OS),Windows_NT)
CROSS_COMPILE_TOP		?=
CROSS_COMPILE			?= $(CROSS_COMPILE_TOP)arm-none-eabi-
else
CROSS_COMPILE_TOP		?=
#CROSS_COMPILE			?= $(CROSS_COMPILE_TOP)arm-linux-gnueabihf-
CROSS_COMPILE			?= $(CROSS_COMPILE_TOP)arm-none-eabi-
endif

# Top Names
PROJECT_NAME			=
TARGET_NAME			= bl2-$(shell echo $(BOARD) | tr A-Z a-z)
LDS_NAME			= bl2

# Directories
DIR_PROJECT_TOP			=

DIR_OBJOUTPUT			= obj
DIR_TARGETOUTPUT		= out

# Build Environment
ARCH				= armv7-a
CPU				= cortex-a7
CC				= $(CROSS_COMPILE)gcc
LD 				= $(CROSS_COMPILE)ld
AS 				= $(CROSS_COMPILE)as
AR 				= $(CROSS_COMPILE)ar
MAKEBIN				= $(CROSS_COMPILE)objcopy
OBJCOPY				= $(CROSS_COMPILE)objcopy
RANLIB 				= $(CROSS_COMPILE)ranlib

GCC_LIB				= $(shell $(CC) -print-libgcc-file-name)

ifeq ($(DEBUG), y)
CFLAGS				= -DNX_DEBUG -O0
Q				=
else
CFLAGS				= -DNX_RELEASE -Os
Q				= @
endif

# MISC tools for MS-DOS
ifeq ($(OS),Windows_NT)
MKDIR				= mkdir
RM				= del /q /F
MV				= move
CD				= cd
CP				= copy
ECHO				= echo
RMDIR				= rmdir /S /Q
else
MKDIR				= mkdir
RM				= rm -f
MV				= mv
CD				= cd
CP				= cp
ECHO				= echo
RMDIR				= rm -rf
endif

#########################################################################
# flags variables
#########################################################################
# FLAGS
ARFLAGS				= rcs
ARFLAGS_REMOVE			= -d
ARLIBFLAGS			= -v -s

ASFLAG				= -D__ASSEMBLY__

CFLAGS				+=	-g -Wall					\
					-Wextra -ffreestanding -fno-builtin		\
					-mcpu=$(CPU)					\
					-mfloat-abi=hard				\
					-mfpu=vfpv3-d16 				\
					-mlittle-endian					\
					-mstructure-size-boundary=32			\
					-DCHIPID_$(shell echo $(CHIPNAME) | tr a-z A-Z)	\
					-D$(shell echo $(MEMTYPE) | tr a-z A-Z)		\
					-D$(shell echo $(BOARD) | tr a-z A-Z)

ifeq ($(EARLY_SERIAL),y)
CFLAGS				+=	-DEARLY_SERIAL
endif

# system log messgae
ifeq ($(SYSLOG), y)
CFLAGS				+= 	-DSYSLOG_ON
endif

# memory test
ifeq ($(MEMTEST), y)
MEMTEST_TYPE			+=	STANDARD
#MEMTEST_TYPE			+=	SIMPLE
CFLAGS				+=	-D$(MEMTEST_TYPE)_MEMTEST
endif
