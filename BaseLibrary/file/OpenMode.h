#ifndef _OPENMODE_H_
#define _OPENMODE_H_

enum OpenFlags {
	modeRead	= (int) 0x00000,
	modeWrite	= (int) 0x00080,
	modeCreate	= (int) 0x01000,
	modeAll		= (int) 0x08000,
};

#endif