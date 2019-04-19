OUTPUT_ARCH(mips)
ENTRY(_start)

SECTIONS
{
	. = 0x80000080;
	.expect_vec3 : {
		*(.text.exc_vec3)
	}
	. = 0x80010000;
	.text : {
		*(.text)
	}

	.bss  : {
		*(.bss)
	}

	.data : {
		*(.data)
	}

	.sdata : {
		*(.sdata)
	}

	. = 0x80400000;

	end = . ;
}
