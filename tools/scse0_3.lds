OUTPUT_ARCH(mips)
ENTRY(_start)

SECTIONS
{
  . = 0x80010000;
  .text : {
    *(.int)
	*(.text)
	*(.fini)
        }
  .data : {
	*(.data)
	}
  .bss  : {
	*(.bss)
    }
  .sdata : {
    *(.sdata)
    }
  . = 0x80400000;
  end = . ;
}
