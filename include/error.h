/* See COPYRIGHT for copyright information. */

#ifndef _ERROR_H_
#define _ERROR_H_

// Kernel error codes -- keep in sync with list in kern/printf.c.
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

#endif // _ERROR_H_
