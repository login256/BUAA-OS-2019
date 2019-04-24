#include "lib.h"
#include <fs.h>
#include <env.h>

#define debug 0

extern u_char fsipcbuf[BY2PG];		// page-aligned, declared in entry.S

// Send an IP request to the file server, and wait for a reply.
// type: request code, passed as the simple integer IPC value.
// fsreq: page to send containing additional request data, usually fsipcbuf.
//	  Can be modified by server to return additional response info.
// dstva: virtual address at which to receive reply page, 0 if none.
// *perm: permissions of received page.
// Returns 0 if successful, < 0 on failure.
static int
fsipc(u_int type, void *fsreq, u_int dstva, u_int *perm)
{
	u_int whom;
	int r;
	//we file system no. is 000000000000000000
	ipc_send(envs[1].env_id, type, (u_int)fsreq, PTE_V | PTE_R);
	//	writef("fsipc:ending ipc send & recv dst:%x\n",dstva);
	r =  ipc_recv(&whom, dstva, perm);
	//writef("fsipc:r = %d\n",r);
	return r;
}

// Send file-open request to the file server.
// Includes path and omode in request, sets *fileid and *size from reply.
// Returns 0 on success, < 0 on failure.
int
fsipc_open(const char *path, u_int omode, struct Fd *fd)
{
	u_int perm;
	struct Fsreq_open *req;
	//writef("TTTTTTTTTT fsipc_open:enter path:%s\n",path);
	req = (struct Fsreq_open *)fsipcbuf;

	if (strlen(path) >= MAXPATHLEN) {
		return -E_BAD_PATH;
	}

	strcpy((char *)req->req_path, path);
	req->req_omode = omode;
	//writef("fsipc_open:will to ipc send\n");
	//writef("fsipcbuf = %x,	req->req_path = %x,	vpd[req/PDMAP]=%x,	vpt[req/BY2PG]=%x\n",fsipcbuf,(u_int)req->req_path,vpd[(u_int)req/PDMAP],vpt[(u_int)req/BY2PG]);
	//writef("UTOP = %x,	ULIM = %x,	UTEXT = %x\n",UTOP,ULIM,UTEXT);
	//writef("UUUUUUUUUUUUUUfsipc_open:fd = %x\n",(u_int)fd);
	return fsipc(FSREQ_OPEN, req, (u_int)fd, &perm);
}

// Make a map-block request to the file server.
// We send the fileid and the (byte) offset of the desired block in the file,
// and the server sends us back a mapping for a page containing that block.
// Returns 0 on success, < 0 on failure.
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

// Make a set-file-size request to the file server.
int
fsipc_set_size(u_int fileid, u_int size)
{
	struct Fsreq_set_size *req;

	req = (struct Fsreq_set_size *)fsipcbuf;
	req->req_fileid = fileid;
	req->req_size = size;
	return fsipc(FSREQ_SET_SIZE, req, 0, 0);
}

// Make a file-close request to the file server.
// After this the fileid is invalid.
int
fsipc_close(u_int fileid)
{
	struct Fsreq_close *req;

	req = (struct Fsreq_close *)fsipcbuf;
	req->req_fileid = fileid;
	return fsipc(FSREQ_CLOSE, req, 0, 0);
}

// Ask the file server to mark a particular file block dirty.
int
fsipc_dirty(u_int fileid, u_int offset)
{
	struct Fsreq_dirty *req;

	req = (struct Fsreq_dirty *)fsipcbuf;
	req->req_fileid = fileid;
	req->req_offset = offset;
	return fsipc(FSREQ_DIRTY, req, 0, 0);
}

// Ask the file server to delete a file, given its pathname.
int
fsipc_remove(const char *path)
{
	struct Fsreq_remove *req;

	req = (struct Fsreq_remove *)fsipcbuf;

	if (strlen(path) >= MAXPATHLEN) {
		return -E_BAD_PATH;
	}

	strcpy((char *)req->req_path, path);
	return fsipc(FSREQ_REMOVE, req, 0, 0);
}

// Ask the file server to update the disk
// by writing any dirty blocks in the buffer cache.
int
fsipc_sync(void)
{
	return fsipc(FSREQ_SYNC, fsipcbuf, 0, 0);
}

