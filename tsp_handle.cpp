//////////////////////////////////////////////////////////////////////////
// tsp_handle.cpp
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
#include "stdafx.h"
#include "tsp_private.h"
#include "request.h"
#include "aprallocator.h"

using std::vector;

BIO *BIO_new_request(request_rec* r);

typedef vector<char, aprStdAllocator<char> > vectorBuffer;
#define BUFFERLEN 512

int timestamp_handler(request_rec *r)
{
	if (!r->handler || stricmp(r->handler, "timestamp"))
	{
		return DECLINED;
	}
	
	modtsp_ctx_t* dc = myDirConfig(r);

	if (!dc || !dc->cert || !dc->key)
	{
		return HTTP_INTERNAL_SERVER_ERROR;
	}

	if (r->method_number != M_POST)
	{
		return HTTP_METHOD_NOT_ALLOWED;
	}

	apr_pool_t* pool = r->pool;

	// 开始接收数据
	if (ap_setup_client_block(r, REQUEST_CHUNKED_DECHUNK) != OK)
	{
		ap_rputs("<p>Bad request body.</p>\n", r);
		return HTTP_BAD_REQUEST;
	}

	aprStdAllocator<char> _allocator(pool);

	vectorBuffer tspReq(_allocator);
	if (ap_should_client_block(r))
	{
		char buffer[BUFFERLEN];
		size_t bytes;
		for (bytes = ap_get_client_block(r, buffer, BUFFERLEN); bytes > 0; 
			 bytes = ap_get_client_block(r, buffer, BUFFERLEN))
		{
			tspReq.insert(tspReq.end(), buffer, buffer+bytes);
		}
	}

	if (tspReq.size() <= 0)
	{
		return HTTP_BAD_REQUEST;
	}

	if (tspReq[0] != '\x30')
	{// BASE64	编码
		vectorBuffer tspReqTemp(_allocator);
		tspReqTemp.swap(tspReq);
		tspReqTemp.erase(std::remove(tspReqTemp.begin(), tspReqTemp.end(), '\x00'), tspReqTemp.end());

		int outl = 0;
		int outTotal = 0;

		tspReq.resize(tspReqTemp.size()*4/3);
		EVP_ENCODE_CTX dctx;
		EVP_DecodeInit(&dctx);
		if (EVP_DecodeUpdate(&dctx, (unsigned char *)tspReq.begin(), &outl, (unsigned char *)tspReqTemp.begin(), tspReqTemp.size()) == -1)
		{
			return HTTP_BAD_REQUEST;
		}
		else
		{
			outTotal = outl;
		}

		EVP_DecodeFinal(&dctx, (unsigned char *)(tspReq.begin()+outl), &outl);
		outTotal += outl;
		tspReq.resize(outTotal);
	}

// 解析ASN.1
	TimeStampRequest* pTspReq = TimeStampRequest_new();
	const unsigned char* pBuffer = (unsigned char*)&tspReq[0];
	d2i_TimeStampRequest(&pTspReq, &pBuffer, tspReq.size());
	if (!pTspReq)
	{
		return HTTP_BAD_REQUEST;
	}

	if (OBJ_obj2nid(pTspReq->type) != OBJ_ln2nid("timestampRequest")
		|| OBJ_obj2nid(pTspReq->content->type) != NID_pkcs7_data
		|| pTspReq->content->content == NULL
		|| pTspReq->content->content->length <= 0)
	{
		TimeStampRequest_free(pTspReq);
		return HTTP_BAD_REQUEST;
	}

	PKCS7* pkcs7 = PKCS7_new();
	PKCS7_set_type(pkcs7, NID_pkcs7_signed);
	PKCS7_content_new(pkcs7, NID_pkcs7_data);
	PKCS7_set_detached(pkcs7, 0);

	PKCS7_SIGNER_INFO* signer_info = PKCS7_add_signature(pkcs7, dc->cert, dc->key, EVP_md5());
	PKCS7_add_signed_attribute(signer_info, NID_pkcs9_contentType,
		V_ASN1_OBJECT, OBJ_nid2obj(NID_pkcs7_data));
	PKCS7_add_certificate(pkcs7, dc->cert);

	BIO* p7bio = NULL;
	p7bio = PKCS7_dataInit(pkcs7, p7bio);
	BIO_write(p7bio, pTspReq->content->content->data, pTspReq->content->content->length);
	PKCS7_dataFinal(pkcs7, p7bio);
	BIO_free(p7bio);

	ap_set_content_type(r, "application/octet-stream");

	BIO* b64 = BIO_new(BIO_f_base64());
	BIO* bio = BIO_new_request(r);

	BIO_push(b64, bio);

	i2d_PKCS7_bio(b64, pkcs7);

	BIO_flush(b64);
	BIO_free(b64);
	BIO_free(bio);

	PKCS7_free(pkcs7);
	TimeStampRequest_free(pTspReq);
	return OK;
}
