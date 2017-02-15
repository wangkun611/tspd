//////////////////////////////////////////////////////////////////////////
// request.h : 
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
#ifndef __REQUEST_H__
#define __REQUEST_H__

struct TimeStampRequestContent_st
{
	ASN1_OBJECT* type;
	ASN1_OCTET_STRING* content;
};
typedef TimeStampRequestContent_st TimeStampRequestContent;
struct TimeStampRequest_st 
{
	ASN1_OBJECT* type;
	TimeStampRequestContent * content;
};
typedef TimeStampRequest_st TimeStampRequest;

DECLARE_ASN1_FUNCTIONS(TimeStampRequestContent);
DECLARE_ASN1_FUNCTIONS(TimeStampRequest)
#endif //__REQUEST_H__