
#ifndef DEFINE_HEDAER_INCLUDED 
#define DEFINE_HEDAER_INCLUDED 1

const char* __home_dir = "/home/jiahao/log/hooklog";
const char* __replay_dir = "/home/jiahao/logs/e1";

#define DEF__INSTR__(_v1, _v2) int _v1;  int* _v2 = &_v1;


#define SET_INIT(__init) g_bInit = __init;
#define IS_INITED() (g_bInit)

#define ENABLE_LOG(__en) g_bEnableLog = __en;
#define IS_LOG_ENABLED() (g_bEnableLog)


#define FUTEX_SIGNATURE_SIZE 	20
#define FUTEX_OTHERS			20

//#define HASHFUTEX_INDEX	   	(MAX_SYSCALL_NUM + 1)
//#define GET_HASHFUTEX() 	g_futexRingBuf[HASHFUTEX_INDEX]

#define SHM_INST_KEY 116740



#define DECL_MALLOC_VTMEM() void* __vtMem[100]; int __vtMemCnt = 0;
#define MALLOC_VTMEM(__ptr, __type, __size) __ptr = (__type) malloc(__size);\
									__vtMem[__vtMemCnt++] = __ptr;\
									if( __vtMemCnt > 100 ) { DEBUGMSG("__vtMemCnt > limit\n"); exit(-1); }

#define FREE_VTMEM() { for( unsigned int __vtmem_i = 0; __vtmem_i < __vtMemCnt; __vtmem_i++ ) { free( __vtMem[__vtmem_i] ); } }


#define MAX_SYSCALL_NUM				COUNT_OF_SC

#ifdef __cplusplus
extern "C" {
#endif
typedef int (*PFNORG_OPEN)( const char* pathname, int flags, mode_t mode );
typedef FILE* (*PFNORG_FOPEN)(const char *path, const char *mode);
typedef size_t (*PFNORG_FWRITE)(const void* ptr, size_t size, size_t nmemb, FILE* stream);
typedef int (*PFNORG_FCLOSE)( FILE* fp );
//ssize_t readlink(const char *path, char *buf, size_t bufsiz);
typedef size_t (*PFNORG_FREAD)(void * ptr, size_t size, size_t nitems, FILE * stream);
typedef ssize_t (*PFNORG_READLINK)(const char *path, char *buf, size_t bufsiz);
//       int dladdr(void *addr, Dl_info *info);
typedef int (*PFNORG_DLADDR)(void* addr, Dl_info* info);
typedef int (*PFNORG_FSEEK)(FILE * stream, long int offset, int origin);
typedef long int (*PFNORG_FTELL)(FILE * stream);
typedef void (*PFNORG_REWIND)(FILE * stream);
typedef char* (*PFNORG_GETENV)(const char *name);
typedef int (*PFNORG_FILENO)(FILE *stream);
typedef int (*PFNORG_FFLUSH)(FILE *stream);
typedef int (*PFNORG_FSTAT)(int fd, struct stat* buf);
typedef int (*PFNORG_FEOF)(FILE* stream);

#ifdef __cplusplus
}
#endif
#endif 
