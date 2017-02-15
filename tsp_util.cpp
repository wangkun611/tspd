//////////////////////////////////////////////////////////////////////////
// tsp_util.cpp : 
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

BOOL tsp_util_path_check(tsp_pathcheck_t pcm, const char *path, apr_pool_t *p)
{
    apr_finfo_t finfo;
	
    if (path == NULL)
        return FALSE;
    if (pcm & TSP_PCM_EXISTS && apr_stat(&finfo, path,
		APR_FINFO_TYPE|APR_FINFO_SIZE, p) != 0)
        return FALSE;
    if (pcm & TSP_PCM_ISREG && finfo.filetype != APR_REG)
        return FALSE;
    if (pcm & TSP_PCM_ISDIR && finfo.filetype != APR_DIR)
        return FALSE;
    if (pcm & TSP_PCM_ISNONZERO && finfo.size <= 0)
        return FALSE;
    return TRUE;
}
