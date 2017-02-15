// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
//	Author:		Eric Wang
//	E-mail:		bulepaper@msn.com
//
//
//	This code may be used in compiled form in any way you desire. This
//	file may be redistributed unmodified by any means PROVIDING it is 
//	not sold for profit without the authors written consent, and 
//	providing that this notice and the authors name and all copyright 
//	notices remains intact. 
//
//	An email letting me know how you are using it would be nice as well. 
//
//	This file is provided "as is" with no expressed or implied warranty.
//	The author accepts no liability for any damage/loss of business that
//	this product may cause.
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDAFX_H__1991667B_0B4B_430A_A8E1_2C6B06FFD84F__INCLUDED_)
#define AFX_STDAFX_H__1991667B_0B4B_430A_A8E1_2C6B06FFD84F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <WinSock2.h>
#include <vector>
#include <algorithm>

#include <httpd.h>
#include <http_protocol.h>
#include <http_config.h>
#include <apr.h>
#include <apr_base64.h>
#include <apr_strings.h>

#pragma comment(lib, "libhttpd")
#pragma comment(lib, "apr-1")
#pragma comment(lib, "libapr-1")
#pragma comment(lib, "libaprutil-1")

#include <openssl/x509.h>
#include <openssl/asn1.h>
#include <openssl/crypto.h>
#include <openssl/pkcs7.h>
#include <openssl/asn1t.h>
#include <openssl/evp.h>
#include <openssl/stack.h>
#include <openssl/pem.h>

#pragma comment(lib, "libeay32")
#pragma comment(lib, "ssleay32")
// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1991667B_0B4B_430A_A8E1_2C6B06FFD84F__INCLUDED_)
