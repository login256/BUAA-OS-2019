// See COPYRIGHT for copyright information.

#ifndef _FS_H_
#define _FS_H_ 1

#include <types.h>

// File nodes (both in-memory and on-disk)

// Bytes per file system block - same as page size
#define BY2BLK		BY2PG
#define BIT2BLK		(BY2BLK*8)

// Maximum size of a filename (a single path component), including null
#define MAXNAMELEN	128

// Maximum size of a complete pathname, including null
#define MAXPATHLEN	1024

// Number of (direct) block pointers in a File descriptor
#define NDIRECT		10
#define NINDIRECT	(BY2BLK/4)

#define MAXFILESIZE	(NINDIRECT*BY2BLK)

#define BY2FILE     256

struct File {
	u_char f_name[MAXNAMELEN];	// filename
	u_int f_size;			// file size in bytes
	u_int f_type;			// file type
	u_int f_direct[NDIRECT];
	u_int f_indirect;

	struct File *f_dir;		// the pointer to the dir where this file is in, valid only in memory.
	u_char f_pad[BY2FILE - MAXNAMELEN - 4 - 4 - NDIRECT * 4 - 4 - 4];
};

#define FILE2BLK	(BY2BLK/sizeof(struct File))

// File types
#define FTYPE_REG		0	// Regular file
#define FTYPE_DIR		1	// Directory


// File system super-block (both in-memory and on-disk)

#define FS_MAGIC	0x68286097	// Everyone's favorite OS class

struct Super {
	u_int s_magic;		// Magic number: FS_MAGIC
	u_int s_nblocks;	// Total number of blocks on disk
	struct File s_root;	// Root directory node
};

// Definitions for requests from clients to file system

#define FSREQ_OPEN	1
#define FSREQ_MAP	2
#define FSREQ_SET_SIZE	3
#define FSREQ_CLOSE	4
#define FSREQ_DIRTY	5
#define FSREQ_REMOVE	6
#define FSREQ_SYNC	7

struct Fsreq_open {
	char req_path[MAXPATHLEN];
	u_int req_omode;
};

struct Fsreq_map {
	int req_fileid;
	u_int req_offset;
};

struct Fsreq_set_size {
	int req_fileid;
	u_int req_size;
};

struct Fsreq_close {
	int req_fileid;
};

struct Fsreq_dirty {
	int req_fileid;
	u_int req_offset;
};

struct Fsreq_remove {
	u_char req_path[MAXPATHLEN];
};

#endif // _FS_H_
