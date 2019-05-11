/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurred during read the IDE disk, panic. 
// 	
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	u_int offset_begin = secno * 0x200;
	u_int offset_end = offset_begin + nsecs * 0x200;
	u_int offset = 0;
	u_int dev_addr = 0x13000000;
	u_int status = 0;

	while (offset_begin + offset < offset_end) {
		u_int now_offset = offset_begin + offset;
		u_int read_value = 0;
		if (syscall_write_dev((u_int)&diskno, dev_addr + 0x10, 4) < 0)
		{
			user_panic("ide_read error!");
		}
		if (syscall_write_dev((u_int)&now_offset, dev_addr + 0x0, 4) < 0)
		{
			user_panic("ide_read error!");
		}
		if (syscall_write_dev((u_int)&read_value, dev_addr + 0x20, 1) < 0)
		{
			user_panic("ide_read error!");
		}
		status = 0;
		if (syscall_read_dev((u_int)&status, dev_addr + 0x30, 1) < 0)
		{
			user_panic("ide_read error!");
		}
		if (status == 0)
		{
			user_panic("ide read faild!");
		}
		if (syscall_read_dev((u_int)dst + offset, dev_addr + 0x4000, 0x200) < 0)
		{
			user_panic("ide_read error!");
		}
		offset += 0x200;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurred during read the IDE disk, panic.
//	
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	u_int offset_begin = secno * 0x200;
	u_int offset_end = offset_begin + nsecs * 0x200;
	u_int offset = 0;
	u_int dev_addr = 0x13000000;
	u_int status = 0;

	writef("diskno: %d\n", diskno);

	while (offset_begin + offset < offset_end) {
		u_int now_offset = offset_begin + offset;
		u_int write_value = 1;
		if (syscall_write_dev((u_int)&diskno, dev_addr + 0x10, 4) < 0)
		{
			user_panic("ide_write error!");
		}
		if (syscall_write_dev((u_int)&now_offset, dev_addr + 0x0, 4) < 0)
		{
			user_panic("ide_write error!");
		}
		if (syscall_write_dev((u_int)src + offset, dev_addr + 0x4000, 0x200) < 0)
		{
			user_panic("ide_write error!");
		}
		if (syscall_write_dev((u_int)&write_value, dev_addr + 0x20, 1) < 0)
		{
			user_panic("ide_write error!");
		}
		status = 0;
		if (syscall_read_dev((u_int)&status, dev_addr + 0x30, 1) < 0)
		{
			user_panic("ide_write error!");
		}
		if (status == 0)
		{
			user_panic("ide write faild!");
		}
		offset += 0x200;
	}

}

