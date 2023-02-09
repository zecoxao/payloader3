#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(PSL1GHT)),)
$(error "Please set PSL1GHT in your environment. export PSL1GHT=<path>")
endif

include $(PSL1GHT)/ppu_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
#MAKEFILE	:=	ethdebug
#TARGET		:=	$(MAKEFILE).bin
#TARGET		:=	$(MAKEFILE).bin
#TARGETS		:=	$(MAKEFILE)/$(MAKEFILE).bin
#BUILD		:=	$(MAKEFILE)/build
#SOURCE		:=	$(MAKEFILE)
#DATA		:=	$(MAKEFILE)/data
#LINKFILE	:=	$(MAKEFILE)/kammy.x
#INCLUDES	:=	$(SOURCE)
#MAPFILE		:=	$(BUILD)/$(MAKEFILE).map
#TEXTBASE	:=	0xFAC3B01000000000

MAKEFILE        :=      $(notdir $(CURDIR))
TARGET          :=      $(MAKEFILE).bin
TARGETS         :=      $(MAKEFILE).bin
BUILD           :=      build
SOURCE          :=      source
DATA            :=      data
INCLUDES        :=      include
LINKFILE        :=      $(INCLUDES)/kammy.x
MAPFILE         :=      $(BUILD)/$(MAKEFILE).map
TEXTBASE        :=      0xFAC3B01000000000

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
LD		:=	ppu-ld
XXD		:=	xxd -r -p
CFLAGS		+=	-g -Os -Wall -mcpu=cell \
			-nodefaultlibs -nostdlib \
			-fno-builtin -fno-exceptions \
			$(MACHDEP) $(INCLUDE) \
			-DLV2
CXXFLAGS	:=	$(CFLAGS) -fno-rtti
LDFLAGS		:=	-L. -s -T $(LINKPATH) \
			-Ttext $(TEXTBASE) \
			-Map $(MAPPATH) --no-demangle

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS		:=	

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS		:=	$(PSL1GHT)/ppu

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(notdir $(BUILD)),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export OUTPUT	:=	$(CURDIR)/$(TARGET)
export OUTPUTS	:=	$(foreach dir,$(TARGETS),$(CURDIR)/$(dir))
export VPATH	:=	$(foreach dir,$(SOURCE),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))
export BUILDDIR	:=	$(CURDIR)/$(BUILD)
export DEPSDIR	:=	$(BUILDDIR)
export LINKPATH	:=	$(CURDIR)/$(LINKFILE)
export MAPPATH	:=	$(CURDIR)/$(MAPFILE)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.bin)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(CFILES:.c=.o) \
			$(CPPFILES:.cpp=.o) \
			$(SFILES:.S=.o)
export BINFILES	:=	$(BINFILES:.bin=.bin.h)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			$(LIBPSL1GHT_INC) \
			-I$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
			$(LIBPSL1GHT_LIB) -L $(PSL1GHT)/ppu/lib -L$(PS3DEV)/portlibs/ppu/lib
export OUTPUT	:=	$(CURDIR)/$(TARGET)

#---------------------------------------------------------------------------------
.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
objdump: $(OUTPUT)
	@ppu-objdump -D -EB -b binary -m powerpc:common64 $(BUILD)/$(TARGET)

#---------------------------------------------------------------------------------
clean:
	@echo Clean...
	@rm -rf $(BUILD) $(OUTPUT) $(OUTPUTS)

#---------------------------------------------------------------------------------
else

DEPENDS		:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(BUILD): $(OUTPUTS)

ifneq ($(OUTPUTS),)
$(OUTPUTS): $(OUTPUT)
	@cp $< $@
endif

GET_MAIN = '/^ +0x[0-9a-z]+ +_start$$/ {print $$1}'
GET_TEXT = '/^\.text +0x[0-9a-z]+ +0x[0-9a-z]+$$/ {print $$2}'
GET_BSS_END = '/^ +0x[0-9a-z]+ +__bss_end = \.$$/ {print $$1}'
GET_BSS_START = '/^ +0x[0-9a-z]+ +__bss_start = \.$$/ {print $$1}'
$(OUTPUT): $(OFILES)
	@echo "[LD]  $@"
	@$(LD) $(LDFLAGS) $(OFILES) -o $(BUILDDIR)/$(TARGET)
	@echo 1337baad00000020 | $(XXD) > $@
	@awk $(GET_MAIN) $(MAPPATH) | $(XXD) >> $@
	@awk $(GET_TEXT) $(MAPPATH) | $(XXD) >> $@
	@awk $(GET_BSS_END) $(MAPPATH) | $(XXD) >> $@
	@cat $(BUILDDIR)/$(TARGET) >> $@
	@dd bs=1 count="`calc \`awk $(GET_BSS_END) $(MAPPATH)\` - \`awk $(GET_BSS_START) $(MAPPATH)\` + 0x10`" if=/dev/zero >> $@ 2> /dev/null
$(OFILES):

#---------------------------------------------------------------------------------
-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

