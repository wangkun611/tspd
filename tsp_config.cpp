//////////////////////////////////////////////////////////////////////////
// tsp_config.cpp
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


#define TSP_FLAGS_CHECK_FILE \
	(TSP_PCM_EXISTS|TSP_PCM_ISREG|TSP_PCM_ISNONZERO)

#define SSL_FLAGS_CHECK_DIR \
	(TSP_PCM_EXISTS|TSP_PCM_ISDIR)

static const char *tsp_cmd_check_file(cmd_parms *parms,
                                      const char **file)
{
    const char *filepath = ap_server_root_relative(parms->pool, *file);
	
    if (!filepath) {
        return apr_pstrcat(parms->pool, parms->cmd->name,
			": Invalid file path ", *file, NULL);
    }
    *file = filepath;
	
    if (tsp_util_path_check(TSP_FLAGS_CHECK_FILE, *file, parms->pool)) {
        return NULL;
    }
	
    return apr_pstrcat(parms->pool, parms->cmd->name,
		": file '", *file,
		"' does not exist or is empty", NULL);
	
}

//////////////////////////////////////////////////////////////////////////
#define cfgMerge(el,unset)  mrg->el = (add->el == (unset)) ? base->el : add->el
#define cfgMergeArray(el)   mrg->el = apr_array_append(p, add->el, base->el)
#define cfgMergeString(el)  cfgMerge(el, NULL)
#define cfgMergeBool(el)    cfgMerge(el, UNSET)
#define cfgMergeInt(el)     cfgMerge(el, UNSET)

static apr_status_t tsp_config_cleanup(void *data)
{
	modtsp_ctx_t* dc = (modtsp_ctx_t*)data;
	if (dc)
	{
		if (dc->cert)
		{
			if (CRYPTO_add(&dc->cert->references, -1, 0) == 0)
			{
				X509_free(dc->cert);
				dc->cert = NULL;
			}
		}
		if (dc->key)
		{
			if (CRYPTO_add(&dc->key->references, -1, 0) == 0)
			{
				EVP_PKEY_free(dc->key);
				dc->key = NULL;
			}
		}
	}
	return APR_SUCCESS;
}

static void *tsp_alloc_config(apr_pool_t* p)
{
	modtsp_ctx_t *dc = (modtsp_ctx_t*)apr_palloc(p, sizeof(*dc));
	
	if (!dc)
	{
		return dc;
	}

	dc->cert_file = NULL;
	dc->key_file = NULL;
	dc->cert = NULL;
	dc->key = NULL;
	dc->dwFlags = 0;

	apr_pool_cleanup_register(p, (void*)dc, tsp_config_cleanup, apr_pool_cleanup_null);

	return dc;
}

static apr_status_t tsp_config_init(modtsp_ctx_t *dc)
{
	if (!dc)
	{
		return APR_EGENERAL;
	}

	if (dc->cert_file && !(dc->dwFlags&0x01))
	{
		FILE *fp;
		fp = fopen(dc->cert_file, "rb");
		if (fp)
		{
			dc->cert = PEM_read_X509(fp, NULL, NULL, 0);
			fclose(fp);
		}
		dc->dwFlags |= 0x01;
	}
	if (dc->key_file && !(dc->dwFlags&0x02))
	{
		FILE *fp;
		fp = fopen(dc->key_file, "rb");
		if (fp)
		{
			dc->key = PEM_read_PrivateKey(fp, NULL, NULL, 0);
			fclose(fp);
		}
		dc->dwFlags |= 0x02;
	}
	if (dc->cert && dc->key)
	{
		return APR_SUCCESS;
	}
	return APR_EGENERAL;
}

/*
*  Create per-directory TSP configuration
*/
void *tsp_config_perdir_create(apr_pool_t *p, char *dir)
{
	return tsp_alloc_config(p);
}

/*
*  Merge per-directory TSP configurations
*/
void *tsp_config_perdir_merge(apr_pool_t *p, void *basev, void *addv)
{
	modtsp_ctx_t *base = (modtsp_ctx_t*)basev;
	modtsp_ctx_t *add = (modtsp_ctx_t*)addv;

	modtsp_ctx_t*  mrg = (modtsp_ctx_t*)tsp_alloc_config(p);

	if (!mrg)
	{
		return NULL;
	}
	
	tsp_config_init(base);
	tsp_config_init(add);

	cfgMergeString(cert_file);
	cfgMergeString(key_file);
	cfgMerge(cert, NULL);
	if (mrg->cert)
	{
		CRYPTO_add(&mrg->cert->references, 1, 0);
	}
	cfgMerge(key, NULL);
	if (mrg->key)
	{
		CRYPTO_add(&mrg->key->references, 1, 0);
	}
	return	mrg;
}


const char  *tsp_cmd_TSPCertificateFile(cmd_parms *cmd,	void *dcfg, const char *arg)
{
	if (!dcfg)
	{
		return NULL;
	}

	modtsp_ctx_t* dc = (modtsp_ctx_t*)dcfg;
	const char *err = NULL;
	if ((err = tsp_cmd_check_file(cmd, &arg)))
	{
		return err;
	}
	dc->cert_file = arg;
	return NULL;
}

const char *tsp_cmd_TSPCertificateKeyFile(cmd_parms *cmd, void *dcfg, const char *arg)
{
	if (!dcfg)
	{
		return NULL;
	}
	
	modtsp_ctx_t* dc = (modtsp_ctx_t*)dcfg;
	const char *err = NULL;
	if ((err = tsp_cmd_check_file(cmd, &arg)))
	{
		return err;
	}
	dc->key_file = arg;
	return NULL;
}


/**
 *
 */
void tsp_hook_ConfigTest(apr_pool_t *pconf, server_rec *s)
{
	/* Dump the filenames of all configured certificates to
		* stdout. */
	printf("%s\n", "certificates");
}
