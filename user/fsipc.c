#include "lib.h"
#include <fs.h>
#include <env.h>

#define debug 0

extern u_char fsipcbuf[BY2PG];		// page-aligned, declared in entry.S

// Overview:
//	Send an IPC request to the file server, and wait for a reply.
//
// Parameters:
//	@type: request code, passed as the simple integer IPC value.
// 	@fsreq: page to send containing additional request data, usually fsipcbuf.
//		Can be modified by server to return additional response info.
// 	@dstva: virtual address at which to receive reply page, 0 if none.
// 	@*perm: permissions of received page.
//
// Returns:
//	0 if successful,
//	< 0 on failure.
static int
fsipc(u_int type, void *fsreq, u_int dstva, u_int *perm)
{
	u_int whom;
	// NOTEICE: Our file system no.1 process!
	ipc_send(envs[1].env_id, type, (u_int)fsreq, PTE_V | PTE_R);
	return ipc_recv(&whom, dstva, perm);
}

// Overview:
//	Send file-open request to the file server. Includes path and
//	omode in request, sets *fileid and *size from reply.
//
// Returns:
//	0 on success,
//	< 0 on failure.
int
fsipc_open(const char *path, u_int omode, struct Fd *fd)
{
	u_int perm;
	struct Fsreq_open *req;

	req = (struct Fsreq_open *)fsipcbuf;

	// The path is too long.
	if (strlen(path) >= MAXPATHLEN) {
		return -E_BAD_PATH;
	}

	strcpy((char *)req->req_path, path);
	req->req_omode = omode;
	return fsipc(FSREQ_OPEN, req, (u_int)fd, &perm);
}

// Overview:
//	Make a map-block request to the file server. We send the fileid and
//	the (byte) offset of the desired block in the file, and the server sends
//	us back a mapping for a page containing that block.
//
// Returns:
//	0 on success,
//	< 0 on failure.
int
fsipc_map(u_int fileid, u_int offset, u_int dstva)
{
	int r;
	u_int perm;
	struct Fsreq_map *req;

	req = (struct Fsreq_map *)fsipcbuf;
	req->req_fileid = fileid;
	req->req_offset = offset;

	if ((r = fsipc(FSREQ_MAP, req, dstva, &perm)) < 0) {
		return r;
	}

	if ((perm & ~(PTE_R | PTE_LIBRARY)) != (PTE_V)) {
		user_panic("fsipc_map: unexpected permissions %08x for dstva %08x", perm,
				   dstva);
	}

	return 0;
}

// Overview:
//	Make a set-file-size request to the file server.
int
fsipc_set_size(u_int fileid, u_int size)
{
	struct Fsreq_set_size *req;

	req = (struct Fsreq_set_size *)fsipcbuf;
	req->req_fileid = fileid;
	req->req_size = size;
	return fsipc(FSREQ_SET_SIZE, req, 0, 0);
}

// Overview:
//	Make a file-close request to the file server. After this the fileid is invalid.
int
fsipc_close(u_int fileid)
{
	struct Fsreq_close *req;

	req = (struct Fsreq_close *)fsipcbuf;
	req->req_fileid = fileid;
	return fsipc(FSREQ_CLOSE, req, 0, 0);
}

// Overview:
//	Ask the file server to mark a particular file block dirty.
int
fsipc_dirty(u_int fileid, u_int offset)
{
	struct Fsreq_dirty *req;

	req = (struct Fsreq_dirty *)fsipcbuf;
	req->req_fileid = fileid;
	req->req_offset = offset;
	return fsipc(FSREQ_DIRTY, req, 0, 0);
}

// Overview:
//	Ask the file server to delete a file, given its pathname.
int
fsipc_remove(const char *path)
{
	// Step 1: decide if the path is valid.

	// Step 2: Send request to fs server with IPC.
}

// Overview:
//	Ask the file server to update the disk by writing any dirty
//	blocks in the buffer cache.
int
fsipc_sync(void)
{
	return fsipc(FSREQ_SYNC, fsipcbuf, 0, 0);
}

