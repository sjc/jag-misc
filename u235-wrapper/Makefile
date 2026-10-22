include $(JAGSDK)/tools/build/jagdefs.mk

CFLAGS += -mshort -Wall -fno-builtin -nostdinc -fomit-frame-pointer

# pass SILENT=1 to build with sfx and music turned off
ifeq ($(SILENT),1)
	ASMFLAGS += -dSILENT
endif

OBJS = startup.o main.o draw.o sound.o sound_utils.o sound_data.o

PROGS = sound.cof

sound.cof: $(OBJS)
	$(LINK) -r$(LINKALIGN) -w -m -z -n -a 802000 x 4000 -o $@ $(OBJS) > mem.map

include $(JAGSDK)/tools/build/jagrules.mk
