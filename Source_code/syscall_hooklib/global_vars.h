// ====================================================================================================
//
// Global Variables
//
int  				__INSTR__exec_index = 0;

//simplemu 			g_futexRingBuf[MAX_SYSCALL_NUM + FUTEX_OTHERS];

//unsigned char* 		__hook_util_sharedmem;
simplemu 			g_logmutex;
simplemu 			g_sharedmapmutex;
simplemu 			g_logrsrc_mutex;
bool 				g_bInit = false;
bool				g_bEnableLog = false;

PFNORG_OPEN 		orgfn_open = 0;
PFNORG_FOPEN 		orgfn_fopen = 0;
PFNORG_FWRITE 		orgfn_fwrite = 0;
PFNORG_FCLOSE 		orgfn_fclose = 0;
PFNORG_READLINK		orgfn_readlink = 0;
PFNORG_FREAD 		orgfn_fread = 0;
PFNORG_DLADDR		orgfn_dladdr = 0;
PFNORG_FTELL		orgfn_ftell = 0;
PFNORG_FSEEK		orgfn_fseek = 0;
PFNORG_REWIND		orgfn_rewind = 0;
PFNORG_GETENV		orgfn_getenv = 0;
PFNORG_FILENO		orgfn_fileno = 0;
PFNORG_FFLUSH		orgfn_fflush = 0;
PFNORG_FSTAT		orgfn_fstat = 0;
PFNORG_FEOF 		orgfn_feof = 0;

SHMEM_OBJ* 			g_pshm_logs = 0;
SHMEM_OBJ* 			g_pshm_map_meta = 0;
SHMEM_OBJ* 			g_pshm_map_data = 0;

// ====================================================================================================
