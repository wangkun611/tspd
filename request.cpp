//////////////////////////////////////////////////////////////////////////
// request.cpp :
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

#include "StdAfx.h"
#include "request.h"

ASN1_SEQUENCE(TimeStampRequestContent) = {
	ASN1_SIMPLE(TimeStampRequestContent, type, ASN1_OBJECT),
	ASN1_EXP_OPT(TimeStampRequestContent, content, ASN1_OCTET_STRING, 0)
} ASN1_SEQUENCE_END(TimeStampRequestContent)

IMPLEMENT_ASN1_FUNCTIONS(TimeStampRequestContent)

ASN1_SEQUENCE(TimeStampRequest) = {
	ASN1_SIMPLE(TimeStampRequest, type, ASN1_OBJECT),
	ASN1_SIMPLE(TimeStampRequest, content, TimeStampRequestContent)
} ASN1_SEQUENCE_END(TimeStampRequest)

IMPLEMENT_ASN1_FUNCTIONS(TimeStampRequest)