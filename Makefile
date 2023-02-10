#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(PSL1GHT)),)
$(error "Please set PSL1GHT in your environment. export PSL1GHT=<path>")
endif

include $(PSL1GHT)/ppu_rules

# Payload type is usually normal
PAYLOAD_TYPE	:=	-DNORMAL
#PAYLOAD_TYPE	:=	-DSTANDALONE

# Firmware version, any of:
#   3.01
#   3.10
#   3.15
#   3.21
#   3.30
#   3.40
#   3.41
#   3.41.dex
#   3.41.kiosk
#   3.55
FIRMWARE_VERSION ?= 3.41

# NO edits needed below here
 
FIRMWARE	:=	-DFIRMWARE_$(subst .,_,$(FIRMWARE_VERSION))

APPEGO		:=	PL3
APPNAME		:=	payloader3
TITLE		:=	$(APPNAME)
APPVERSION	:=	005
APPID		:=	$(APPNAME)$(APPVERSION)
APPRAND		:=	$(shell openssl rand -hex 8 | tr [:lower:] [:upper:])
CONTENTID	:=	$(APPEGO)-$(APPID)_00-$(APPRAND)
SFOXML		:=	pkg/SFO.XML
ICON0		:=	pkg/ICON0.PNG
PIC1		:=	pkg/extras/PIC1.PNG

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build
SOURCES		:=	source
INCLUDES	:=	include
DATA		:=	data
SHADERS		:=	shaders

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS		=	-g -O3 -Wall --std=gnu99 -mcpu=cell $(MACHDEP) $(LIBPATHS) $(INCLUDE) $(PAYLOAD_TYPE) $(FIRMWARE) $(DEBUG) -mminimal-toc -maltivec -funroll-loops -ftree-loop-linear -ftree-vectorize
CXXFLAGS	=	$(CFLAGS)
LDFLAGS		=	$(MACHDEP) -Wl,-Map,$(notdir $@).map

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS		:=	-lrsx -lgcm_sys -lresc -lsysutil -lsysmodule -lio -lpngdec -lfreetype -lz -laudio -lrt -llv2 -lm
#LIBS		:=	-lrsx -lgcm_sys -lresc -lsysutil -lsysmodule -lio -lpngdec -lfreetype2 -laudio -lrt -llv2 -lm

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS		:=	$(PS3DEV)/portlibs/ppu $(PSL1GHT)/ppu
#LIBDIRS		:=	$(PSL1GHT)/ppu

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export OUTPUT	:=	$(CURDIR)/$(TARGET)
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
			$(foreach dir,$(SHADERS),$(CURDIR)/$(dir))
export BUILDDIR	:=	$(CURDIR)/$(BUILD)
export DEPSDIR	:=	$(BUILDDIR)

export PKGFILES		:=	$(CURDIR)/pkg/extras
export PAYLOADSDIR	:=	$(PKGFILES)/USRDIR/payloads
export TOOLSDIR		:=	$(PKGFILES)/USRDIR/tools
export PL3DIR		:=	$(CURDIR)/PL3

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.bin)))
TTFFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.ttf)))
PNGFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.png)))
RAWFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.raw)))
VCGFILES	:=	$(foreach dir,$(SHADERS),$(notdir $(wildcard $(dir)/*.vcg)))
FCGFILES	:=	$(foreach dir,$(SHADERS),$(notdir $(wildcard $(dir)/*.fcg)))
VPOFILES	:=	$(VCGFILES:.vcg=.vpo)
FPOFILES	:=	$(FCGFILES:.fcg=.fpo)

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
			$(addsuffix .o,$(TTFFILES)) \
			$(addsuffix .o,$(PNGFILES)) \
			$(addsuffix .o,$(RAWFILES)) \
			$(addsuffix .o,$(VPOFILES)) \
			$(addsuffix .o,$(FPOFILES)) \
			$(CFILES:.c=.o) \
			$(CPPFILES:.cpp=.o) \
			$(sFILES:.s=.o) $(SFILES:.S=.o)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES), -I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			$(LIBPSL1GHT_INC) \
			-I$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
			$(LIBPSL1GHT_LIB) -L $(PSL1GHT)/ppu/lib
export OUTPUT	:=	$(CURDIR)/$(TARGET)

#---------------------------------------------------------------------------------
.PHONY: $(BUILD) PL3 psgroove ps3load payloads tools clean bin pkg run

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@echo compiling payloader3
	@$(MAKE) --no-print-directory -C $(BUILDDIR) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
self:	$(BUILD) $(OUTPUT).self

#---------------------------------------------------------------------------------
payloads: PL3 psgroove
	@echo copying payloads
	@mkdir -p $(PAYLOADSDIR)
	@cp -r $(PL3DIR)/*$(subst .,_,$(FIRMWARE_VERSION)).bin $(PAYLOADSDIR)
	@cp -r $(PL3DIR)/dump_lv2.bin $(PAYLOADSDIR)
	@cp -r psgroove/payload_groove_hermes.bin $(PAYLOADSDIR)
	#@cp -r ethdebug/ethdebug.bin $(PAYLOADSDIR)
	#@cp -r cobra/payload_groove_cobrav2.bin $(PAYLOADSDIR)

#---------------------------------------------------------------------------------
tools:	ps3load
	@echo copying tools
	@mkdir -p $(PAYLOADSDIR)
	@cp -r ps3load/ps3load.self $(TOOLSDIR)

#---------------------------------------------------------------------------------
clean:
	@echo cleaning ...
	@[ ! -d PL3 ] || $(MAKE) --no-print-directory -C PL3 clean > /dev/null
	@[ ! -d psgroove ] || $(MAKE) --no-print-directory -C psgroove clean > /dev/null
	@[ ! -d ethdebug ] || $(MAKE) --no-print-directory -C ethdebug clean > /dev/null
	@rm -rf $(BUILD) $(OUTPUT).a $(OUTPUT)*.elf $(OUTPUT)*.self $(OUTPUT)*.pkg $(PAYLOADSDIR)

#---------------------------------------------------------------------------------
run:	self
	ps3load $(OUTPUT).self

#---------------------------------------------------------------------------------
PL3:
	@echo compiling PL3 payloads
	@$(MAKE) --no-print-directory -C PL3 -f Makefile 1> /dev/null 2>&1

#---------------------------------------------------------------------------------
psgroove:
	@echo compiling psgroove payload
	@$(MAKE) --no-print-directory -C psgroove -f Makefile 1> /dev/null 2>&1
	
#---------------------------------------------------------------------------------
ps3load:
	@echo compiling ps3load tool
	@$(MAKE) --no-print-directory -C ps3load -f Makefile 1> /dev/null 2>&1

#---------------------------------------------------------------------------------
ethdebug:
	@echo compiling ethdebug tool
	@$(MAKE) --no-print-directory -C ethdebug -f Makefile 1> /dev/null 2>&1

#---------------------------------------------------------------------------------
pkg:	self payloads tools $(OUTPUT).pkg

#---------------------------------------------------------------------------------
dex:	$(BUILD) $(OUTPUT).dex.self

#---------------------------------------------------------------------------------
else

DEPENDS		:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).self:	$(OUTPUT).elf
$(OUTPUT).elf:	$(OFILES)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .bin extension
#---------------------------------------------------------------------------------
%.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.ttf.o	:	%.ttf
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.png.o	:	%.png
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.raw.o	:	%.raw
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.vpo.o	:	%.vpo
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.fpo.o	:	%.fpo
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

