GDB=m68k-unknown-elf-gdb
QEMU=../ehbc-qemu/build/qemu-system-m68k

.PHONY=run debug
run: build/firmware.bin
	$(QEMU) \
		-M ehbc -cpu m68030 -m 128M -S -s \
		-serial stdio -hda disk.img \
		--trace "ide_bus_*" \
		--trace "serial_*" \
		--trace "pckbd_*" \
		--trace "cpu_*" \
		--trace "vga_*" \
		-bios $<

debug: build/firmware.elf
	${GDB} \
		-ex "tui new-layout all -horizontal {{-horizontal src 3 asm 2} 2 cmd 1} 3 {status 2 regs 1} 1" \
		-ex "layout all" \
		-ex "symbol-file $<" \
		-ex "target remote :1234" \
		-ex "tui reg all"
