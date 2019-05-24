#ifndef _MMU_H_
#define _MMU_H_


/*
 * This file contains:
 *
 *	Part 1.  MIPS definitions.
 *	Part 2.  Our conventions.
 *	Part 3.  Our helper functions.
 */

/*
 * Part 1.  MIPS definitions.
 */
#define BY2PG		4096		// bytes to a page
#define PDMAP		(4*1024*1024)	// bytes mapped by a page directory entry
#define PGSHIFT		12
#define PDSHIFT		22		// log2(PDMAP)
#define PDX(va)		((((u_long)(va))>>22) & 0x03FF)
#define PTX(va)		((((u_long)(va))>>12) & 0x03FF)
#define PTE_ADDR(pte)	((u_long)(pte)&~0xFFF)

// page number field of address
#define PPN(va)		(((u_long)(va))>>12)
#define VPN(va)		PPN(va)

#define VA2PFN(va)		(((u_long)(va)) & 0xFFFFF000 ) // va 2 PFN for EntryLo0/1
#define PTE2PT		1024
//$#define VA2PDE(va)		(((u_long)(va)) & 0xFFC00000 ) // for context

/* Page Table/Directory Entry flags
 *   these are defined by the hardware
 */
#define PTE_G		0x0100	// Global bit
#define PTE_V		0x0200	// Valid bit
#define PTE_R		0x0400	// Dirty bit ,'0' means only read ,otherwise make interrupt
#define PTE_D		0x0002	// fileSystem Cached is dirty
#define PTE_COW		0x0001	// Copy On Write
#define PTE_UC		0x0800	// unCached
#define PTE_LIBRARY		0x0004	// share memmory
/*
 * Part 2.  Our conventions.
 */

/*
 o     4G ----------->  +----------------------------+------------0x100000000
 o                      |       ...                  |  kseg3
 o                      +----------------------------+------------0xe000 0000
 o                      |       ...                  |  kseg2
 o                      +----------------------------+------------0xc000 0000
 o                      |   Interrupts & Exception   |  kseg1
 o                      +----------------------------+------------0xa000 0000
 o                      |      Invalid memory        |   /|\
 o                      +----------------------------+----|-------Physics Memory Max
 o                      |       ...                  |  kseg0
 o  VPT,KSTACKTOP-----> +----------------------------+----|-------0x8040 0000-------end
 o                      |       Kernel Stack         |    | KSTKSIZE            /|\
 o                      +----------------------------+----|------                |
 o                      |       Kernel Text          |    |                    PDMAP
 o      KERNBASE -----> +----------------------------+----|-------0x8001 0000    | 
 o                      |   Interrupts & Exception   |   \|/                    \|/
 o      ULIM     -----> +----------------------------+------------0x8000 0000-------    
 o                      |         User VPT           |     PDMAP                /|\ 
 o      UVPT     -----> +----------------------------+------------0x7fc0 0000    |
 o                      |         PAGES              |     PDMAP                 |
 o      UPAGES   -----> +----------------------------+------------0x7f80 0000    |
 o                      |         ENVS               |     PDMAP                 |
 o  UTOP,UENVS   -----> +----------------------------+------------0x7f40 0000    |
 o  UXSTACKTOP -/       |     user exception stack   |     BY2PG                 |
 o                      +----------------------------+------------0x7f3f f000    |
 o                      |       Invalid memory       |     BY2PG                 |
 o      USTACKTOP ----> +----------------------------+------------0x7f3f e000    |
 o                      |     normal user stack      |     BY2PG                 |
 o                      +----------------------------+------------0x7f3f d000    |
 a                      |                            |                           |
 a                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                           |
 a                      .                            .                           |
 a                      .                            .                         kuseg
 a                      .                            .                           |
 a                      |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|                           |
 a                      |                            |                           |
 o       UTEXT   -----> +----------------------------+                           |
 o                      |                            |     2 * PDMAP            \|/
 a     0 ------------>  +----------------------------+ -----------------------------
 o
*/

#define KERNBASE 0x80010000

#define VPT (ULIM + PDMAP )
#define KSTACKTOP (VPT-0x100)
#define KSTKSIZE (8*BY2PG)
#define ULIM 0x80000000

#define UVPT (ULIM - PDMAP)
#define UPAGES (UVPT - PDMAP)
#define UENVS (UPAGES - PDMAP)

#define UTOP UENVS
#define UXSTACKTOP (UTOP)
#define TIMESTACK 0x82000000

#define USTACKTOP (UTOP - 2*BY2PG)
#define UTEXT 0x00400000


#define E_UNSPECIFIED	1	// Unspecified or unknown problem
#define E_BAD_ENV       2       // Environment doesn't exist or otherwise
				// cannot be used in requested action
#define E_INVAL		3	// Invalid parameter
#define E_NO_MEM	4	// Request failed due to memory shortage
#define E_NO_FREE_ENV   5       // Attempt to create a new environment beyond
				// the maximum allowed
#define E_IPC_NOT_RECV  6	// Attempt to send to env that is not recving.

// File system error codes -- only seen in user-level
#define	E_NO_DISK	7	// No free space left on disk
#define E_MAX_OPEN	8	// Too many files are open
#define E_NOT_FOUND	9 	// File or block not found
#define E_BAD_PATH	10	// Bad path
#define E_FILE_EXISTS	11	// File already exists
#define E_NOT_EXEC	12	// File not a valid executable

#define MAXERROR 12

#ifndef __ASSEMBLER__

/*
 * Part 3.  Our helper functions.
 */
#include "types.h"
void bcopy(const void *, void *, size_t);
void bzero(void *, size_t);

extern char bootstacktop[], bootstack[];

extern u_long npage;

typedef u_long Pde;
typedef u_long Pte;

extern volatile Pte* vpt[];
extern volatile Pde* vpd[];

#define PADDR(kva)						\
({								\
	u_long a = (u_long) (kva);				\
	if (a < ULIM)					\
		panic("PADDR called with invalid kva %08lx", a);\
	a - ULIM;						\
})


// translates from physical address to kernel virtual address
#define KADDR(pa)						\
({								\
	u_long ppn = PPN(pa);					\
	if (ppn >= npage)					\
		panic("KADDR called with invalid pa %08lx", (u_long)pa);\
	(pa) + ULIM;					\
})

#define assert(x)	\
	do {	if (!(x)) panic("assertion failed: %s", #x); } while (0)

#define TRUP(_p)   						\
({								\
	register typeof((_p)) __m_p = (_p);			\
	(u_int) __m_p > ULIM ? (typeof(_p)) ULIM : __m_p;	\
})



extern void tlb_out(u_int entryhi);
#endif //!__ASSEMBLER__
#endif // !_MMU_H_
