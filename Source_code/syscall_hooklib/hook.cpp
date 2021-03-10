
// =============== configs ===============
#include "configuration.h"
// =============== configs ===============

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <dlfcn.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <set>
#include <stdarg.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <semaphore.h>

using namespace std;

#include "define.h"

#include "simple_futex.h"
#include "models.h"
#include "hash.h"


#include "common.h"
#include "global_vars.h"

#include "hook.h"
#include "util.h"
#include "util_rsrc.h"
#include "util_logio.h"

#define SIZE_MB 1024*1024

void __attribute__ ((constructor)) my_init(void)
{
	hook_init(true);
}

void __attribute__ ((destructor)) my_fini(void)
{
	hook_fini();
}


//
// this mutex is within a system -- across processes but not across dockers
//
int inline hook_init_mutex(bool first_run)
{
	int shmsize 			= 1 * SIZE_MB; 
	int reterror;
	int error_cnt = 0;
	
	SHMEM_OBJ* pshm_mutex = smcreate( 0x1024, shmsize, first_run, &reterror);
	
	
	// inline unsigned int smalloc( SHMEM_OBJ* pobj, int size, bool init_zero = false )
	unsigned int log_mutex = 0;
	unsigned int logrsrc_mutex = 40;
	unsigned int sharedmap_mutex = 80;
		
	if( first_run ) {
		if( smalloc(pshm_mutex, 40, true) != log_mutex ) {
			error_cnt += 0x1;
		}
		if( smalloc(pshm_mutex, 40, true) != logrsrc_mutex ) {
			error_cnt += 0x10;
		}
		if( smalloc(pshm_mutex, 40, true) != sharedmap_mutex ) {
			error_cnt += 0x100;
		}
	}
	
	g_logmutex = (simplemu)sm_getptr(pshm_mutex, log_mutex);
	g_logrsrc_mutex = (simplemu)sm_getptr(pshm_mutex, logrsrc_mutex);
	g_sharedmapmutex = (simplemu)sm_getptr(pshm_mutex, sharedmap_mutex);
	
	if( first_run ) {
		simplefu_mutex_init( g_logmutex );
		simplefu_mutex_init( g_logrsrc_mutex );
		simplefu_mutex_init( g_sharedmapmutex );
	}
	
	/*
	const int sharedmem_size = 120;
	char filename[1024];
	strcpy( filename, SHARED_MEM );
	int fd = orgfn_open(filename, O_RDWR, 0666);
	unsigned char *psharedmem = (unsigned char*)mmap( NULL, sharedmem_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );
	g_logmutex = (simplemu)psharedmem; // 0
	g_logrsrc_mutex = (simplemu)(&psharedmem[40]); // 40 
	g_sharedmapmutex = (simplemu)(&psharedmem[80]); // 80
	if( first_run ) {
		memset(psharedmem, 0, sharedmem_size);
		
		simplefu_mutex_init( g_logmutex );
		simplefu_mutex_init( g_logrsrc_mutex );
		simplefu_mutex_init( g_sharedmapmutex );
	}
	*/
	
	return error_cnt;
}



inline void hook_init_functions()
{
	orgfn_open = (PFNORG_OPEN)dlsym(RTLD_NEXT, "open");
	orgfn_fopen = (PFNORG_FOPEN)dlsym(RTLD_NEXT, "fopen");	
	orgfn_fwrite = (PFNORG_FWRITE)dlsym(RTLD_NEXT, "fwrite");
	orgfn_fread = (PFNORG_FREAD)dlsym(RTLD_NEXT, "fread");
	orgfn_fclose = (PFNORG_FCLOSE)dlsym(RTLD_NEXT, "fclose");
	orgfn_fflush = (PFNORG_FFLUSH)dlsym(RTLD_NEXT, "fflush");
	orgfn_readlink = (PFNORG_READLINK)dlsym(RTLD_NEXT, "readlink");
	orgfn_dladdr = (PFNORG_DLADDR)dlsym(RTLD_NEXT, "dladdr");
	orgfn_fseek = (PFNORG_FSEEK)dlsym(RTLD_NEXT, "fseek");
	orgfn_ftell = (PFNORG_FTELL)dlsym(RTLD_NEXT, "ftell");
	orgfn_rewind = (PFNORG_REWIND)dlsym(RTLD_NEXT, "rewind");
	orgfn_getenv = (PFNORG_GETENV)dlsym(RTLD_NEXT, "getenv");
	orgfn_fileno = (PFNORG_FILENO)dlsym(RTLD_NEXT, "fileno");
	orgfn_fstat = (PFNORG_FSTAT)dlsym(RTLD_NEXT, "fstat");
	orgfn_feof = (PFNORG_FEOF)dlsym(RTLD_NEXT, "feof");
}


// mutex will be used by the log

void inline hook_init(int first_run)
{
	if( !IS_INITED() ) {
		hook_init_functions();
		// ======================================
		setvbuf(stdout, NULL, _IONBF, 0);
		__hook_curdir = get_current_dir_name();
		// ======================================
		
		//hook_init_hookmode();
		int error_mutex = hook_init_mutex(first_run);
		ENABLE_LOG(true);
		//init_instrument(first_run);
		
		SET_INIT(true);
		
		//DEBUGMSG("hook_init - complete\n");
		// DEBUGMSG("hook_init [pid: %d]\n", getpid());
	}	
}


void inline hook_fini(void)
{
	// DEBUGMSG("hook_fini - begin \t\t\t\t\t << PROCESS TERMINATING >>\n");
	{
		for( int i = 0; i < 3; i++ ) {
			if( __hook_util_LOGFILE_FD ) {
				orgfn_fclose( __hook_util_LOGFILE );
				__hook_util_LOGFILE_FD = 0;
			}
		}
	}
	// DEBUGMSG("hook_fini - done \t\t\t\t\t << TERMINATED >>\n");
}


#include "hook_func_def.h"
#include "hook_func_fd.h"
// inputs
#include "hook_func_input.h"
#include "hook_func_input_sysrsrc.h"
// opers
#include "hook_func_oper.h"
#include "hook_func_oper_extlibs.h"
#include "hook_func_oper_filesys.h"
#include "hook_func_oper_network.h"
// outputs
#include "hook_func_output.h"
