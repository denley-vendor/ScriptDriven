//
// safe_defs.h
//
// --------------------------------------------------------
// A Practical Guide to Script-Driven Software Development
// Author: Qiming Lu                        Date: 6/1/2006
// MSN Messager: luqiming26@hotmail.com
// MSN Blog: http://spaces.msn.com/jemylu
// --------------------------------------------------------

#pragma once

#define SAFE_DELETE(p) { if(p) { delete (p);   (p)=0; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=0; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release();   (p)=0; } }
#define SAFE_CLOSEHANDLE(h) { if (h) { CloseHandle(h); (h)=0; } }
#define SAFE_DELETEOBJECT(h) { if (h) { DeleteObject(h); (h)=0; } }