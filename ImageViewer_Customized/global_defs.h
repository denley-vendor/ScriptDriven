//
// global_defs.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#ifndef __H_global_defs__
#define __H_global_defs__

#define kElementBaseID 8800

typedef enum
{
	IF_Unknown		= 0,
	IF_Open			= 1,
	IF_Reload		= 2,
	IF_Clean		= 3,
	IF_Invert		= 4,
	IF_Greyscale	= 5,
	IF_Logo			= 6,
	IF_Save			= 7,
	IF_SystemTime	= 8,
	IF_About		= 9,
	IF_Minimize		= 10,
	IF_Exit			= 11
} IMAGE_FUNCTION;

#endif // __H_global_defs__