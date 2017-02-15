//////////////////////////////////////////////////////////////////////////
// tsp_private.h:
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

#ifndef __TSP_PRIVATE_H__
#define __TSP_PRIVATE_H__

int timestamp_handler(request_rec *r);

#ifdef __cplusplus
extern "C" {
#endif

/**
* Define the Path Checking modes
	*/
#define TSP_PCM_EXISTS     1
#define TSP_PCM_ISREG      2
#define TSP_PCM_ISDIR      4
#define TSP_PCM_ISNONZERO  8
typedef unsigned int tsp_pathcheck_t;


#define myDirConfig(req) (modtsp_ctx_t *)ap_get_module_config(req->per_dir_config, &timestamp_module)

typedef struct  
{
	const char *cert_file;
	const char *key_file;
	X509 *cert;
	EVP_PKEY *key;
	long dwFlags;  // 
}modtsp_ctx_t;

/**
*  function prototypes
*/

/**  API glue structures  */
extern module AP_MODULE_DECLARE_DATA timestamp_module;

/**  configuration handling   */
void        *tsp_config_perdir_create(apr_pool_t *, char *);
void        *tsp_config_perdir_merge(apr_pool_t *, void *, void *);

const char  *tsp_cmd_TSPCertificateFile(cmd_parms *, void *, const char *);
const char  *tsp_cmd_TSPCertificateKeyFile(cmd_parms *, void *, const char *);


/**  Apache API hooks  */

void tsp_hook_ConfigTest(apr_pool_t *pconf, server_rec *s);


/**  Utility Functions  */
BOOL         tsp_util_path_check(tsp_pathcheck_t, const char *, apr_pool_t *);

BIO *BIO_new_request(request_rec* r);

#ifdef __cplusplus
}
#endif

#endif //__TSP_PRIVATE_H__
