#include "fs.h"
#include "lib.h"

int
strecmp(char *a, char *b)
{
	while (*b)
		if (*a++ != *b++) {
			return 1;
		}

	return 0;
}

static char *msg = "This is the NEW message of the day!\r\n\r\n";
static char *diff_msg = "This is a different massage of the day!\r\n\r\n";

void
fs_test(void)
{
	struct File *f;
	int r;
	void *blk;
	u_int *bits;

	// back up bitmap
	if ((r = syscall_mem_alloc(0, BY2PG, PTE_V | PTE_R)) < 0) {
		user_panic("syscall_mem_alloc: %e", r);
	}

	bits = (u_int *)BY2PG;
	user_bcopy(bitmap, bits, BY2PG);

	// allocate block
	if ((r = alloc_block()) < 0) {
		user_panic("alloc_block: %e", r);
	}

	// check that block was free
	user_assert(bits[r / 32] & (1 << (r % 32)));
	// and is not free any more
	user_assert(!(bitmap[r / 32] & (1 << (r % 32))));
	writef("alloc_block is good\n");

	if ((r = file_open("/not-found", &f)) < 0 && r != -E_NOT_FOUND) {
		user_panic("file_open /not-found: %e", r);
	} else if (r == 0) {
		user_panic("file_open /not-found succeeded!");
	}

	if ((r = file_open("/newmotd", &f)) < 0) {
		user_panic("file_open /newmotd: %d", r);
	}

	writef("file_open is good\n");

	if ((r = file_get_block(f, 0, &blk)) < 0) {
		user_panic("file_get_block: %e", r);
	}

	if (strecmp(blk, msg) != 0) {
		user_panic("file_get_block returned wrong data");
	}

	writef("file_get_block is good\n");

	*(volatile char *)blk = *(volatile char *)blk;
	file_flush(f);
	writef("file_flush is good\n");

	if ((r = file_set_size(f, 0)) < 0) {
		user_panic("file_set_size: %e", r);
	}

	user_assert(f->f_direct[0] == 0);
	writef("file_truncate is good\n");

	if ((r = file_set_size(f, strlen(diff_msg))) < 0) {
		user_panic("file_set_size 2: %e", r);
	}

	if ((r = file_get_block(f, 0, &blk)) < 0) {
		user_panic("file_get_block 2: %e", r);
	}

	strcpy((char *)blk, diff_msg);
	file_flush(f);
	file_close(f);
	writef("file rewrite is good\n");
}
