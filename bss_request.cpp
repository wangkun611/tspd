//////////////////////////////////////////////////////////////////////////
// bss_request.cpp : 
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
#include "tsp_private.h"

static int  request_write(BIO *h, const char *buf, int num);
static int  request_read(BIO *h, char *buf, int size);
static int  request_puts(BIO *h, const char *str);
static int  request_gets(BIO *h, char *str, int size);
static long request_ctrl(BIO *h, int cmd, long arg1, void *arg2);
static int  request_new(BIO *h);
static int  request_free(BIO *data);

#define BIO_TYPE_REQUEST  (50|BIO_TYPE_DESCRIPTOR)

static BIO_METHOD methods_requestp=
{
	BIO_TYPE_REQUEST,
		"Apache request_rec",
		request_write,
		request_read,
		request_puts,
		request_gets,
		request_ctrl,
		request_new,
		request_free,
		NULL,
};

BIO_METHOD *BIO_s_request(void)
{
	return(&methods_requestp);
}

BIO *BIO_new_request(request_rec* r)
{
	BIO *ret;
	
	if ((ret=BIO_new(BIO_s_request())) == NULL)
		return(NULL);
	
	ret->init = 1;
	ret->ptr = (void*)r;
	return(ret);
}

//////////////////////////////////////////////////////////////////////////
static int  request_new(BIO *bi)
{
	bi->init=1;
	bi->num=0;
	bi->ptr=NULL;
	bi->flags=0;
	return(1);
}

static int request_free(BIO *bi)
{
	if (bi == NULL) return(0);
	if (bi->shutdown)
	{
		if ((bi->init) && (bi->ptr != NULL))
		{
			bi->ptr=NULL;
			bi->flags=0;
		}
		bi->init=0;
	}
	return(1);
}

static int  request_write(BIO *bi, const char *buf, int num)
{
	int ret=0;
	
	if (bi->init && (buf != NULL))
	{
		ret = ap_rwrite(buf, num, (request_rec*)bi->ptr);
	}
	return ret;
}

static long request_ctrl(BIO *bi, int cmd, long arg1, void *arg2)
{
	return 1;
}

static int  request_gets(BIO *bi, char *str, int size)
{
	return 0;
}

static int  request_puts(BIO *bi, const char *str)
{
	return request_write(bi, str, strlen(str));
}

static int  request_read(BIO *bi, char *buf, int size)
{
	int ret=0;
	
	if (bi->init && (buf != NULL))
	{
		ret = ap_get_client_block((request_rec*)bi->ptr, buf, size);
	}
	return ret;
}
