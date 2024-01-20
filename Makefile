################################################################################
# Executables
################################################################################
# CC Configurations
CC=m68k-unknown-elf-gcc
CFLAGS=-march=68030 -m68881 -O0 -std=c99 -ffreestanding

# AS Configurations
AS=vasmm68k_mot
ASFLAGS=-m68030 -m68882 -no-opt -quiet

# LD Configurations
LD=m68k-unknown-elf-ld
LDFLAGS=

# Binutils Configurations
OBJCOPY=m68k-unknown-elf-objcopy
OBJDUMP=m68k-unknown-elf-objdump

# Tool CC Configurations
TCC=cc
TCFLAGS=

# Tool CXX Configurations
TCXX=cxx
TCXXFLAGS=

# Tool LD Configurations
TLD=ld
TLDFLAGS=

# Python Configurations
PYTHON=python3

# Other Commands
MKDIR=mkdir
DD=dd
CHMOD=chmod
M4=m4
GDB=m68k-unknown-elf-gdb
QEMU=../ehbc-qemu/build/qemu-system-m68k

################################################################################
# Directory / File Structure
################################################################################
# Source Directories
SRCDIR=src
RSRCDIR=rsrc
INCDIR=include
LDSDIR=ldscripts
TOOLSDIR=tools

# Build Directories
BUILDDIR?=build
TOOLBINDIR=$(BUILDDIR)/tools
OBJDIR=$(BUILDDIR)/obj

# File List
SOURCES=$(wildcard $(SRCDIR)/*.c $(SRCDIR)/**/*.c $(SRCDIR)/*.asm $(SRCDIR)/**/*.asm)
RESOURCES=$(wildcard $(RSRCDIR)/* $(RSRCDIR)/**/*)
OBJECTS=$(patsubst %, $(OBJDIR)/%.o, $(SOURCES) $(RESOURCES))

# Target Files
TARGET=$(BUILDDIR)/fw.elf
HEX=$(patsubst %.elf, %.hex, $(TARGET))
BIN=$(patsubst %.elf, %.bin, $(TARGET))
OBIN=$(patsubst %.elf, %.odd.bin, $(TARGET))
EBIN=$(patsubst %.elf, %.even.bin, $(TARGET))
SBIN=$(OBIN) $(EBIN)

# Build Tools
TOOLS=binsplit
TOOLTARGETS=$(patsubst %, $(TOOLBINDIR)/%, $(TOOLS))
BINSPLIT=$(TOOLBINDIR)/binsplit


################################################################################
# Makefile Commands
################################################################################
# Subcommands
all: build $(BIN) $(HEX) $(SBIN) $(BUILDDIR)/objdump.out
 
clean:
	rm -rf $(BUILDDIR)

build: prepdir $(TARGET)

run: $(BIN)
	$(QEMU) \
		-M ehbc -cpu m68030 -m 128M -S -s \
		-monitor stdio -display cocoa -g 640x480 \
		-bios $<

debug:
	${GDB} -ex "target remote :1234" -ex "layout asm"

prepdir:
	$(MKDIR) -p $(BUILDDIR)
	$(MKDIR) -p $(BUILDDIR)/obj
	$(MKDIR) -p $(BUILDDIR)/obj/$(SRCDIR)
	find $(SRCDIR) -type d -exec $(MKDIR) -p -- $(BUILDDIR)/obj/{} \;
	$(MKDIR) -p $(BUILDDIR)/obj/$(RSRCDIR)
	$(MKDIR) -p $(BUILDDIR)/tools

 $(BUILDDIR)/objdump.out: build
	$(OBJDUMP) -s -S -x -r $(TARGET) > $@

# Tools
$(TOOLBINDIR)/%: $(TOOLSDIR)/%
	$(MAKE) -C $< TARGET=$(CURDIR)/$@ \
		TCC=$(TCC) TCCFLAGS=$(TCCFLAGS) \
		TCXX=$(TCXX) TCXXFLAGS=$(TCXX) \
		TLD=$(TLD) TLDFLAGS=$(TLDFLAGS)
	$(CHMOD) +x $@

# Final Targets
$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -Tldscripts/firmware.ld -o $@ $^

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary --gap-fill 0x00 --pad-to 0x100000 $< $@

$(OBIN): $(BIN) $(BINSPLIT)
	$(BINSPLIT) --odd -o $@ $<

$(EBIN): $(BIN) $(BINSPLIT)
	$(BINSPLIT) --even -o $@ $<

$(HEX): $(TARGET)
	$(OBJCOPY) -O ihex --gap-fill 0x00 --pad-to 0x100000 $< $@


# Temporary Targets
$(OBJDIR)/$(SRCDIR)/%.c.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c -o $@ $<

$(OBJDIR)/$(SRCDIR)/%.asm.o: $(SRCDIR)/%.asm
	$(AS) $(ASFLAGS) -I$(INCDIR) -Felf -o $@ $<

$(BUILDDIR)/$(RSRCDIR)/%.o: $(RSRCDIR)/%
	$(OBJCOPY) -I binary -O elf32-m68k
