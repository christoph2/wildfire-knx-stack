/*
**
**	Definition of Datatypes,Macros etc.
**
*/


#ifndef	__HC12DEF_H
#define	__HC12DEF_H

#ifndef	BYTE
	typedef	unsigned char BYTE;
#endif

#ifndef	WORD
	typedef	unsigned int WORD;
#endif

#ifndef	DWORD
	typedef	unsigned long DWORD;
#endif

#ifndef	LOBYTE
	#define LOBYTE(w)	((w) & 0x00ff)
#endif

#ifndef	HIBYTE
	#define LOBYTE(w)	(((w) & 0xff00)>>8)
#endif

#ifndef MAKEWORD
	#define MAKEWORD(l,h)	((((h) & 0xff)<<8) | ((l) & 0xff))
#endif


#endif	// __HC12DEF_H