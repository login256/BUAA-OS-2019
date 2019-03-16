OUTPUT_ARCH(mips)
/*
Set the architecture to mips.
*/
ENTRY(_start)
/*
Set the ENTRY point of the program to _start.
*/
SECTIONS
{
	. = 0x80010000;
	.text : {
		*(.text)
	}
	.data : {
		*(.date)
	}
	.bss : {
		*(.bss)
	}
/*To do:
  fill in the correct address of the key section
  such as text data bss ...
*/

end =. ;
}
