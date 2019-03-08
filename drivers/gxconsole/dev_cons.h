#ifndef	TESTMACHINE_CONS_H
#define	TESTMACHINE_CONS_H

/*
 *  Definitions used by the "cons" device in GXemul.
 *
 *  $Id: dev_cons.h,v 1.2 2006/07/05 05:38:36 debug Exp $
 *  This file is in the public domain.
 */


//#define	DEV_CONS_ADDRESS		0x180003f8
#define	DEV_CONS_ADDRESS		0x10000000
#define	DEV_CONS_LENGTH			0x0000000000000020
#define	    DEV_CONS_PUTGETCHAR		    0x0000
#define	    DEV_CONS_HALT		    0x0010


#endif	/*  TESTMACHINE_CONS_H  */
