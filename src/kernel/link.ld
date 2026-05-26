OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
	.multiboot : {
		KEEP(*(.multiboot))
	}
	.text : {
		*(.text)
	}
	.rodata : {*(.rodata)}
	.data : {*(.data)}
	.bss  : {*(.bss)}
}