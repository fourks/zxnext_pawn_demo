################################################################################
# Stefan Bylund 2017
#
# Makefile for compiling the Pawn demo program for Sinclair ZX Spectrum Next.
# Supports both the SCCZ80 and SDCC compilers.
################################################################################

MKDIR := mkdir -p

RM := rm -rf

CP := cp -r

ZIP := zip -r -q

ZXNEXT_LAYER2 := ../zxnext_layer2

ZXNEXT_LAYER2_INCLUDE := $(ZXNEXT_LAYER2)/include

ZXNEXT_LAYER2_LIB_SCCZ80 := $(ZXNEXT_LAYER2)/lib/sccz80

ZXNEXT_LAYER2_LIB_SDCC_IX := $(ZXNEXT_LAYER2)/lib/sdcc_ix

ZXNEXT_LAYER2_LIB_SDCC_IY := $(ZXNEXT_LAYER2)/lib/sdcc_iy

VT_SOUND_INCLUDE := vt_sound/include

SRCS := src/pawn.c \
src/world_pawn.c \
src/adventure.c \
src/str_util.c \
src/gfx_util.c \
src/zx_01_input_kbd_inkey_custom \
src/tshr_01_output_fzx_custom

VT_SOUND_SRCS := vt_sound/src/vt_sound.asm vt_sound/src/PT3PROM.asm

BINDIR_SCCZ80 := bin/sccz80

BINDIR_SDCC_IX := bin/sdcc_ix

BINDIR_SDCC_IY := bin/sdcc_iy

ifeq ($(BUILD_SNA),true)
SNA := -subtype=sna -Cz"--clean --exclude-sections VT_SOUND"
endif

DEBUGFLAGS := --list --c-code-in-asm

all: all_sccz80 all_sdcc_ix all_sdcc_iy

all_sccz80:
	$(MKDIR) $(BINDIR_SCCZ80)
	zcc +zxn $(SNA) -vn -O3 -startup=24 -clib=new -m $(DEBUG) -L$(ZXNEXT_LAYER2_LIB_SCCZ80) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) -I$(VT_SOUND_INCLUDE) $(SRCS) $(VT_SOUND_SRCS) -pragma-include:src/zpragma.inc -o $(BINDIR_SCCZ80)/pawndemo -create-app
	$(CP) $(BINDIR_SCCZ80)/pawndemo_VT_SOUND.bin vt_sound.bin

all_sdcc_ix:
	$(MKDIR) $(BINDIR_SDCC_IX)
	zcc +zxn $(SNA) -vn -SO3 -startup=24 -clib=sdcc_ix -m $(DEBUG) --max-allocs-per-node200000 -L$(ZXNEXT_LAYER2_LIB_SDCC_IX) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) -I$(VT_SOUND_INCLUDE) $(SRCS) $(VT_SOUND_SRCS) -pragma-include:src/zpragma.inc -o $(BINDIR_SDCC_IX)/pawndemo -create-app
	$(CP) $(BINDIR_SDCC_IX)/pawndemo_VT_SOUND.bin vt_sound.bin

all_sdcc_iy:
	$(MKDIR) $(BINDIR_SDCC_IY)
	zcc +zxn $(SNA) -vn -SO3 -startup=24 -clib=sdcc_iy -m $(DEBUG) --max-allocs-per-node200000 -L$(ZXNEXT_LAYER2_LIB_SDCC_IY) -lzxnext_layer2 -I$(ZXNEXT_LAYER2_INCLUDE) -I$(VT_SOUND_INCLUDE) $(SRCS) $(VT_SOUND_SRCS) -pragma-include:src/zpragma.inc -o $(BINDIR_SDCC_IY)/pawndemo -create-app
	$(CP) $(BINDIR_SDCC_IY)/pawndemo_VT_SOUND.bin vt_sound.bin

debug_sccz80: DEBUG = $(DEBUGFLAGS)

debug_sdcc_ix: DEBUG = $(DEBUGFLAGS)

debug_sdcc_iy: DEBUG = $(DEBUGFLAGS)

debug_sccz80: all_sccz80

debug_sdcc_ix: all_sdcc_ix

debug_sdcc_iy: all_sdcc_iy

distro:
	$(MAKE) clean all
	$(MAKE) all BUILD_SNA=true
	$(RM) tmp
	$(MKDIR) tmp/pawndemo
	$(CP) bin/sdcc_iy/pawndemo.tap tmp/pawndemo
	$(CP) bin/sdcc_iy/pawndemo.sna tmp/pawndemo
	$(CP) vt_sound.bin tmp/pawndemo
	$(CP) text/ tmp/pawndemo
	$(CP) gfx/ tmp/pawndemo
	$(CP) sound/ tmp/pawndemo
	$(CP) README.md tmp/pawndemo
	$(CP) run_cspect.bat tmp/pawndemo
	$(CP) run_zesarux.bat tmp/pawndemo
	$(RM) build/zxnext_pawn_demo.zip
	cd tmp; $(ZIP) ../build/zxnext_pawn_demo.zip pawndemo
	$(RM) tmp

clean:
	$(RM) bin tmp zcc_opt.def zcc_proj.lst vt_sound.bin src/*.lis
