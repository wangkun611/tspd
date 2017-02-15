// timestamp.cpp : Defines the entry point for the DLL application.
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

BOOL APIENTRY DllMain( HMODULE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//  For optimization.
		DisableThreadLibraryCalls(hModule);
		break;
	case DLL_PROCESS_DETACH:
	    break;
	default:
	    break;
	}
    return TRUE;
}

/*
*  the table of configuration directives we provide
*/

#define TSP_CMD_ALL(name, args, desc) \
	AP_INIT_##args("TSP"#name, (cmd_func)tsp_cmd_TSP##name, \
	NULL, RSRC_CONF|OR_AUTHCFG, desc)

#define TSP_CMD_SRV(name, args, desc) \
	AP_INIT_##args("TSP"#name, (cmd_func)tsp_cmd_TSP##name, \
	NULL, RSRC_CONF, desc)

#define TSP_CMD_DIR(name, type, args, desc) \
	AP_INIT_##args("TSP"#name, (cmd_func)tsp_cmd_TSP##name, \
	NULL, OR_##type, desc)

#define AP_END_CMD { NULL }

static void timestamp_hooks(apr_pool_t *pool)
{
	OBJ_create("1.3.6.1.4.1.311.3.2.1", "timestampRequest", "timestampRequest");
	SSLeay_add_all_algorithms();

//	
	ap_hook_handler(timestamp_handler, NULL, NULL, APR_HOOK_MIDDLE);
	ap_hook_test_config(tsp_hook_ConfigTest, NULL, NULL, APR_HOOK_MIDDLE);
}

extern "C"{

static const command_rec tsp_config_cmds[] = {
	TSP_CMD_DIR(CertificateFile, ALL, TAKE1, "TSP Server Certificate file (`/path/to/file' - PEM or DER encoded)"),
	TSP_CMD_DIR(CertificateKeyFile, ALL, TAKE1, ""),
	AP_END_CMD
};

module AP_MODULE_DECLARE_DATA timestamp_module = {
	STANDARD20_MODULE_STUFF,
		tsp_config_perdir_create,
		tsp_config_perdir_merge,
		NULL,
		NULL,
		tsp_config_cmds,
		timestamp_hooks
};

#pragma comment(linker, "/include:_X509_pubkey_digest")

}; //extern "C"