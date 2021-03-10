
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "stack.h"
#include <unistd.h>

int DEBUGMSG( const char *fmt, ... );

#define GET_PARAM_BUF_SIZE 						50 // needs a protection here
#define MAX_RINGBUF_COUNT						50000



#define FIRST_INDEX 				0
#define LAST_INDEX 					1
#define RINGBUFFER_IDX_CNT			2
#define MAX_EXECUTIONS  			3
#define RINGBUF_NEXTITEM(__val) 	((__val + 1) % (MAX_RINGBUF_COUNT))
//#define MAX_SYSCALL_NUM				COUNT_OF_SC



#define CHECK_REMAINED_BUF_SIZE(__val, __used) 		__val > (GET_PARAM_BUF_SIZE - (sizeof(int)) - (__used))
void show_full_call_stack(int idx_begin, int cnt);

typedef struct tag_SCINFO {
	unsigned char param[GET_PARAM_BUF_SIZE];
	unsigned int argHash;
	unsigned char consumed;
} SCINFO, *PSCINFO;
// 400 + 400 + 12 = 812

typedef struct tag_SCINFO_RINGBUF {
	SCINFO 		 	sysinfo[MAX_SYSCALL_NUM * MAX_RINGBUF_COUNT];
	int 			index[MAX_SYSCALL_NUM * RINGBUFFER_IDX_CNT];
} SCINFO_RINGBUF, *PSCINFO_RINGBUF;


// ====================================
//
// functions
//
int OnSyscall(int nSyscall, PSCINFO pSI, PSCINFO psiMatched, void* caller = 0);
const char* getSyscallResultString( int n ) ;
int DEBUGMSG( const char *fmt, ... ) ;
static inline void wait_short();
static inline void wait_long();

void inline hook_init(int first_run);
void inline hook_fini(void);
//unsigned char* init_shared_memory(int first_run);

//extern simplemu 	g_futexRingBuf[MAX_SYSCALL_NUM + FUTEX_OTHERS];
// ====================================


inline void WriteParam( unsigned char* p, int &offset, int size, PSCINFO pInfo )
{
	if( offset + size > GET_PARAM_BUF_SIZE) {
		char log[1024];
		sprintf(log, "[ERROR] SCINFO::WriteParam Error\n Out of range (offset: %d, size: %d)\n", offset, size);
		puts(log);
		show_full_call_stack(1, 10);
		exit(-1);
	}
		
	if( size <= 0 ) {
		return ;
	} else {
		offset += size;
	}
		
	
#if 0
	char sslog[1024] = { 0, }, sstmp[10];
	for( unsigned int j = 0; j < size; j++ ) {
		sprintf(sstmp, "%02x", (unsigned char)p[j]);
		strcat( sslog, sstmp );
	}
	DEBUGMSG("WriteParam: %s\n", sslog );
#endif
	
	memcpy(&(pInfo->param[offset]), p, size);
}

inline void ReadParam( unsigned char* p, int &offset, int size, PSCINFO pInfo )
{
	// 0+100
	// 100
	if( offset + size > GET_PARAM_BUF_SIZE) {
		char log[1024];
		sprintf(log, "[ERROR] SCINFO::ReadParam Error\n Out of range (offset: %d, size: %d)\n", offset, size);
		puts(log);
		show_full_call_stack(1, 10);
		exit(-1);
	} 
	
	if( size <= 0 ) {
		return ;
	} else {
		offset += size;
	}
	
	
	memcpy(p, &(pInfo->param[offset]), size);
}	






