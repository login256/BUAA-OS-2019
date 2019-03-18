/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

#ifndef EI_DATA
#define EI_DATA 5
#endif

#define REVERSE_32(n_n) \
( (((n_n) & 0xff) << 24) | (((n_n) & 0xff00) << 8) | (((n_n)  >> 8) & 0xff00) | (((n_n) >> 24) & 0xff) )


#define REVERSE_16(n_n) \
( (((n_n) & 0xff) << 8) | (((n_n)  >> 8) & 0xff) )

int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr;

        Elf32_Shdr *shdr = NULL;

        u_char *ptr_sh_table = NULL;
        Elf32_Half sh_entry_count;
        Elf32_Half sh_entry_size;


        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return -1;
        }

		char elf_Data = ehdr -> e_ident[EI_DATA];

		if (elf_Data == 1)
		{
			// get section table addr, section header number and section header size.
			ptr_sh_table = binary + ehdr -> e_shoff;
			sh_entry_count = ehdr -> e_shnum;
			sh_entry_size = ehdr -> e_shentsize;
			// for each section header, output section number and section addr.
			for (Nr = 0; Nr < sh_entry_count; Nr++) {
				shdr = (Elf32_Shdr *)(ptr_sh_table + Nr * sh_entry_size);
				printf("%d:0x%x\n", Nr, shdr->sh_addr);
			}
		}
		else
		{
			// get section table addr, section header number and section header size.
			ptr_sh_table = binary + REVERSE_32(ehdr -> e_shoff);
			sh_entry_count = REVERSE_16(ehdr -> e_shnum);
			sh_entry_size = REVERSE_16(ehdr -> e_shentsize);
			// for each section header, output section number and section addr.
			for (Nr = 0; Nr < sh_entry_count; Nr++) {
				shdr = (Elf32_Shdr *)(ptr_sh_table + Nr * sh_entry_size);
				printf("%d:0x%x\n", Nr, REVERSE_32(shdr->sh_addr));
			}
		}

		return 0;
}

