
#include "string.h"
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY			116751
//
// FOLDER SETTINGS
//

#include "common.h"


#include "stack.h"

#include <errno.h>


#define O_RDONLY	0x0000
#define O_WRONLY	0x0001
#define O_RDWR		0x0002
#define O_ACCMODE	0x0003

#define O_BINARY	0x0004	/* must fit in char, reserved by dos */
#define O_TEXT		0x0008	/* must fit in char, reserved by dos */
#define O_NOINHERIT	0x0080	/* DOS-specific */

#define O_CREAT		0x0100	/* second byte, away from DOS bits */
#define O_EXCL		0x0200
#define O_NOCTTY	0x0400
#define O_TRUNC		0x0800
#define O_APPEND	0x1000
#define O_NONBLOCK	0x2000


#define MEGABYTE (1024*1024)
#define MAX_SHARED_FILE_COUNT 		10

char* 			__hook_curdir;

int 			__hook_util_CALLDEEP[8192];

#define IS_FORKED()  0
#define O_RDWR		0x0002

#define IS_HOOKLIB_FD(__filedesc) \
		(__hook_util_LOGFILE && (__filedesc == __hook_util_LOGFILE_FD)) || \
		(__hook_SHAREDFILE_FD && (__filedesc == __hook_SHAREDFILE_FD))

int __hook_util_funcs[512];

int reg_thread_id(pthread_t ptid);
int get_thread_id(pthread_t ptid);
int get_max_thread_id();
int gettid() ;

#define ENTER_USERFUNC __hook_util_CALLDEEP[get_thread_id(pthread_self())]++; 
#define LEAVE_USERFUNC __hook_util_CALLDEEP[get_thread_id(pthread_self())]--; 

#define ENTER_FUNC __hook_util_CALLDEEP[get_thread_id(pthread_self())]++; 
#define LEAVE_FUNC __hook_util_CALLDEEP[get_thread_id(pthread_self())]--; 
#define CALL_FROM_HOOK_SO (__hook_util_CALLDEEP[get_thread_id(pthread_self())] > 1 || g_bInit == false)

#define LOGDIR "/home/jiahao/CausalitySig"
#include "util_debugmsg.h"

extern PFNORG_OPEN orgfn_open;
extern PFNORG_FOPEN orgfn_fopen;
extern PFNORG_FWRITE orgfn_fwrite;
extern PFNORG_FCLOSE orgfn_fclose;
extern PFNORG_READLINK orgfn_readlink;
extern PFNORG_FREAD orgfn_fread;

//#include "../../llvm/src/lib/PRJ/models.h"

//void show_full_call_stack(int idx_begin, int cnt);
static inline uint64_t RDTSC();



//#include "ringbuf.h"
//#define RINGBUF_DEFINED 1

extern bool g_bInit;

#include <sys/time.h>

bool inline isInHookMod(void* addr) 
{
	static set<void*> addr_in_mod;
	static set<void*> addr_not_in_mod;
	
	if( addr_in_mod.find(addr) != addr_in_mod.end() ) return true;
	if( addr_not_in_mod.find(addr) != addr_not_in_mod.end() ) return false;
	
	Dl_info di;
	dladdr(addr, &di );
	if( strstr( di.dli_fname, "hook.so" ) ) {
		addr_in_mod.insert( addr );
		return true;
	} else {
		addr_not_in_mod.insert( addr );
		return false;
	} 
}

unsigned int GetTickCount()
{
	struct timeval tv;
	if(gettimeofday(&tv, NULL) != 0)
		return 0;
	
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}


pthread_t __hook_util_hash_pthread_t[0x1000];
//map <TID_MAP_KEY, int> __hook_util_map_tid;
int __hook_util_hash_pthread_t_counter = 0; // first thread will get 1 as it does ++cnt.



int reg_thread_id(pthread_t ptid)
{ 
	//printf("reg_thread_id (%d, sizeof(pthread_t) = %ld)\n", (unsigned int)ptid, sizeof(pthread_t));

	__hook_util_hash_pthread_t[ __hook_util_hash_pthread_t_counter++ ] = ptid;
	//printf("reg_thread_id - end\n");
	return (__hook_util_hash_pthread_t_counter-1);
}
int get_thread_id(pthread_t ptid)
{
	int ret = 0;
	for( int i = 0; i < __hook_util_hash_pthread_t_counter; i++ ) {
		if( __hook_util_hash_pthread_t[i] == ptid ) {
			ret = i;
			//printf("get_thread_id: ret = %d\n", ret);
			return i;
		}
	}
	
	// not found
	ret = reg_thread_id(ptid);
	
	//printf("get_thread_id: ret = %d\n", ret);
	
	return ret;
}
int get_max_thread_id()
{
	return (__hook_util_hash_pthread_t_counter-1);
}


int gettid() 
{
    pthread_t ptid = pthread_self();
    uint64_t threadId = 0;
    memcpy(&threadId, &ptid, std::min(sizeof(threadId), sizeof(ptid)));
    return (int)threadId;
}




#include <execinfo.h>
#include <dlfcn.h>


inline void* get_call_stack_raw(void* begin, int idx)
{
	void** ret;
	ret = (void**)begin;
	ret += sizeof(int);
	return ret[idx];
}


void get_call_stack_raw_debug(void* begin, int idx, int cnt)
{
	void** ret;
	ret = (void**)begin;
	ret += sizeof(int);
	for( int i = 0; ; i++ ) {
		if( i >= idx ) {
			for( int j = 0; j < cnt; j++, i++ ) {
				DEBUGMSG("--- raw[%d]: %p\n", i, (void*)ret[i]);
			}
			break;
		}
	}
}

int get_call_stack(void** pRet)
{
	//DEBUGMSG( "get_call_stack begin\n" );
	
	void* tracePtrs[90];
	int count = backtrace( tracePtrs, 90 );
	int retcount = 0;
	
	for( int ii = 0; ii < count; ii++ ) {
		if( isInHookMod(tracePtrs[ii]) == false ) 
		{
			pRet[retcount++] = (void*)tracePtrs[ii];			
		}
	}
		
	//DEBUGMSG( "get_call_stack end\n" );
	return retcount;
}

int is_same_callstack( unsigned int* pcs1, unsigned int s1, 
			 unsigned int* pcs2, unsigned int s2 )
{
	if( s1 != s2 ) return 0;
	if( memcmp( pcs1, pcs2, s1*sizeof(unsigned int) ) == 0 ) {
		return 1;
	}
	return 0;
}

inline void* get_call_stack_backtrace(int idx)
{
	void* tracePtrs[64];
	int count = backtrace( tracePtrs, idx+1 );
	
	return tracePtrs[idx];
}

inline string GetCallerRelativeAddr(int idx)
{
	void* tracePtrs[64];
	string addr;
	int count = backtrace( tracePtrs, idx+1 );
	static map<void*, string> mapAddr;
	
	
	//
	//
	//
	void* p = tracePtrs[idx];
	map<void*, string>::iterator it;
	it = mapAddr.find( p );
	if( it != mapAddr.end() ) {
		return it->second;
	}
	
	Dl_info info;
	if( orgfn_dladdr( p, &info ) ) { // nonzero == success
		//DEBUGMSG("dladdr:: %s:%p:%s\n", info.dli_fname, info.dli_fbase, info.dli_sname);
		u_int64_t st = (u_int64_t)p, fbase = (u_int64_t)info.dli_fbase;
		char tmp[100];
		u_int64_t offset = st - fbase;
		
		sprintf( tmp, "+%" PRIx64, offset );
		string fname = string(info.dli_fname);
		for( unsigned int i = 0; i < fname.length(); i++ ) {
			if( fname[i] == '/' ) {
				fname[i] = '\\';
			}
		}
		addr = fname + string(tmp);
		mapAddr.insert( pair<void*, string>(p, addr) );
	} else {
		//DEBUGMSG("dladdr fail\n");
	}
	
	return addr;
}

inline void show_full_call_stack(int idx_begin, int cnt)
{
	// 2 - 3
	// 0 1 2 3 4 5 6
	// - - * * * 
	void* tracePtrs[64];
	int count = backtrace( tracePtrs, cnt + idx_begin );
	
#define PRINT_NAMES 1
#if PRINT_NAMES
	char** funcNames = backtrace_symbols( tracePtrs, count);
#endif
	
	int idx = 0;
	int printed_cnt = 0;
	for( int ii = 0; ii < count; ii++ ) {
		//if( isInHookMod(tracePtrs[ii]) ) { continue; }
		//if( !strchr( funcNames[ii], '/' ) ) { continue; };
		//DEBUGMSG( "[%08X] %s\n", (unsigned int)tracePtrs[ii], funcNames[ii] );
		
		//DEBUGMSG( "\t\t<%d> [%08X] %s\n", cnt++, (unsigned int)tracePtrs[ii], funcNames[ii] );
		//DEBUGMSG("idx >= idx_begin : %d >= %d?, cnt = %d?\n", idx, idx_begin, cnt );
		if( idx >= idx_begin ) {
			
#if PRINT_NAMES
			DEBUGMSG( "\t\t<DBGLOG#%d> [%p] %s\n", idx, tracePtrs[ii], funcNames[ii] );
#else
			DEBUGMSG( "\t\t<DBGLOG#%d> [%p]\n", idx, tracePtrs[ii] );
#endif
			printed_cnt++;
		}
		
		idx++;
		if( printed_cnt == cnt ) {
			break;
		}
		//break;
	}

#if PRINT_NAMES	
	free(funcNames);
#endif
}


void show_stack(unsigned int* p, unsigned int size)
{
	//void* tracePtrs[100];
	int count = size;

	//char** funcNames = backtrace_symbols( (void* const*)p, size);
	
	char szOut[4096] = { 0, };
	char szTemp[4096];
	for( int ii = 0; ii < count; ii++ ) {		
		//sprintf( szTemp, "[%08X] %s\n", p[ii], funcNames[ii] );
		sprintf( szTemp, "[%08X]\n", p[ii] );
		strcat( szOut, szTemp );
		//DEBUGMSG("[%08X] %s\n", p[ii+1], funcNames[ii]);
	}
	DEBUGMSG( szOut );
	
	//free(funcNames);
}

static inline void wait_short()
{
	struct timespec req_short;
    req_short.tv_sec = 0;
    req_short.tv_nsec = 1000;
	//req_short.tv_nsec = 5000;
	//req_short.tv_nsec = 100;
    nanosleep(&req_short, 0);
}

static inline void wait_long()
{
	struct timespec req_long;
    req_long.tv_sec = 0;
    req_long.tv_nsec = 1000000;
    nanosleep(&req_long, 0);
}





#define getSyscallType(__nsyscall) __SYSCALL_TYPE[__nsyscall]
/**
int inline getSyscallType(int nSyscall)
{
	return __SYSCALL_TYPE[nSyscall];
}
**/

	
#define SYSCALL_RESULT_USE_SI							1
#define SYSCALL_RESULT_INDEPENDENT_SLAVE 				2
#define SYSCALL_RESULT_INDEPENDENT_MASTER				3
#define SYSCALL_RESULT_NO_LEAK			 				4
#define SYSCALL_RESULT_LEAK			 					5
#define SYSCALL_RESULT_ADVANCE_MASTER					6
#define SYSCALL_RESULT_ADVANCE_SLAVE					7
#define SYSCALL_LOOPTAIL_SYNCED							8
#define SYSCALL_RESULT_MATCHED			 				9
const char* getSyscallResultString( int n ) 
{
	switch( n ) {
		case SYSCALL_RESULT_USE_SI:
			return "SYSCALL_RESULT_USE_SI";
		case SYSCALL_RESULT_INDEPENDENT_SLAVE:
			return "SYSCALL_RESULT_INDEPENDENT_SLAVE";
		case SYSCALL_RESULT_INDEPENDENT_MASTER:
			return "SYSCALL_RESULT_INDEPENDENT_MASTER";
		case SYSCALL_RESULT_NO_LEAK:
			return "SYSCALL_RESULT_NO_LEAK";
		case SYSCALL_RESULT_LEAK:
			return "SYSCALL_RESULT_LEAK";
		case SYSCALL_RESULT_ADVANCE_MASTER:
			return "SYSCALL_RESULT_ADVANCE_MASTER";
		case SYSCALL_RESULT_ADVANCE_SLAVE:
			return "SYSCALL_RESULT_ADVANCE_SLAVE";
		case SYSCALL_LOOPTAIL_SYNCED:
			return "SYSCALL_LOOPTAIL_SYNCED";
		case SYSCALL_RESULT_MATCHED:
			return "SYSCALL_RESULT_MATCHED";
		default:
			break;
	}
	DEBUGMSG("=====================================\n");
	DEBUGMSG("[ERROR] getSyscallResultString(): NOT DEFINED: %d\n", n);
	DEBUGMSG("=====================================\n");
	
	return "NOT DEFINED";
}


void print_some_text(char* input)
{
	char out[40];
	memcpy( out, input, 30 );
	
	out[30] = 0;
	char output_text[4096], tmp[100];
	strcpy( output_text, "print_some_text(hex): ");
	for( int i = 0; i < 30; i++ ) {
		sprintf( tmp, " %02X", (unsigned char)out[i] );
		strcat( output_text, tmp );		
	}
	strcat( output_text, "\nprint_some_text(str): ");
	for( int i = 0; i < 30; i++ ) {
		sprintf( tmp, " %c", (unsigned char)out[i] );
		strcat( output_text, tmp );		
	}
	
	strcat( output_text, "\n=======================================\n");
	DEBUGMSG( output_text );	
}


int my_str_printf_va_list(char **psz, const char *format, va_list ap)
{
	int nLen;
	if ((nLen = vsnprintf(NULL, 0, format, ap)) >= 0)
	{
		char *szRes = (char*) malloc(nLen + 1);
		// ^ realloc does a fresh allocation is *psz == NULL
		if (szRes)
			vsnprintf(*psz = szRes, nLen+1, format, ap); // can't fail
			// ^ note the assignment....
		else
			nLen = -1;
	}
	return nLen;
} 

//struct iovec {
  //             void  *iov_base;    /* Starting address */
    //           size_t iov_len;     /* Number of bytes to transfer */
      //     };
#include  <sys/uio.h> 
string GetIOVec(const struct iovec* iov, int iovcnt)
{
	string output;
	
	char temp[100];
	for(unsigned int i = 0; i < iovcnt; i++ ) {
		char* pbuf = (char*)iov[i].iov_base;
		size_t len = iov[i].iov_len;
		
		for(unsigned int j = 0; j < len; j++ ) {
			sprintf(temp, "x%02X", (unsigned char)pbuf[j]);
			output += temp;
		}
	}
	return output;
}

inline int IsFirstRun()
{
	int first_run = 0;
	char fname[100];
	strcpy(fname,  "/tmp/ifldxinstance");
	if( IsFileExist(fname) ) {
		first_run = 0;
	} else {
		FILE* f = fopen(fname, "at");
		if( f ) {
			fprintf(f, ".");
			fclose(f);
		}
		first_run = 1;
	}
	if( first_run == 1 ) {
		char pszlog[1024];
		sprintf(pszlog, "[FIRST RUN (P:T=%x:%x)]", getpid(), gettid());
		puts(pszlog);
		
	}
	
	return first_run;
}


#ifdef RINGBUF_DEFINED
int HashSyscallIndex(int nSyscall, void* caller)
{
	int ret;
	char data[HT_KEYSIZE] = { 0, };
	//static int curindex = 0;
	
	if( g_pHashTable == 0 ) return -1;
	// ==========
	// Set
	memcpy( data, &nSyscall, sizeof(int) );
	memcpy( &data[sizeof(int)], &caller, sizeof(void*) ); 
	// ==========
	
	simplefu_enter( GET_HASHFUTEX() );
	ret = ht_get( g_pHashTable, data );
	if( ret == -1 ) {	
		if( !IS_MASTER_EXEC() ) {
			//DEBUGMSG("Slave issues a new system call! [%d:%p : %x]\n", nSyscall, caller, nSyscall);
			//exit(-1);
			simplefu_leave( GET_HASHFUTEX() );
			return -1;
		}
		if( g_pHashTable->global_index > MAX_SYSCALL_NUM ) {
			DEBUGMSG("//DBG// Registered Syscalls are more than MAX_SYSCALL_NUM(%d)\n", MAX_SYSCALL_NUM);
			simplefu_leave( GET_HASHFUTEX() );
			exit(-1);
			return -1;
		}
		// not exist yet
		ht_set( g_pHashTable, data, g_pHashTable->global_index );
		ret = g_pHashTable->global_index;
		//DEBUGMSG("assigned....RET = %d\n", ret);
		
		g_pHashTable->global_index++;
	}
	simplefu_leave( GET_HASHFUTEX() );
	
	return ret;
}
#endif




void increase_numbers_in_sda1_str(char* newin, int howmuch)
{
	char* num = strstr(newin, "sda1");
	char* num_start = &num[4];
	int arynum[20]; int numidx = 0;
		
	for( int i = 0; i < 11; i++ ) {
		num = strstr( num, " " );
		if( num ) {
			//printf( "%d; ", atoi( num ) );
			arynum[numidx++] = atoi(num);
			num++;
		} else {
			//printf( "%d; ", atoi( num ) );
			arynum[numidx++] = atoi(num);
		}
	}
	char new_num[100];
	*num_start = 0;
	for( int i = 0; i < numidx; i++ ) {
		arynum[i] += howmuch;
		sprintf(new_num, " %d", arynum[i]);
		strcat( newin, new_num );
	}
	strcat( newin, "\n" );
}

inline void print_two_buffer_comparison(const char* plog, const char* prep, unsigned int lenlog, unsigned int lenrep)
{
#define LINE_SIZE 32
	const char* pRead1 = (char*)plog;
	const char* pRead2 = (char*)prep;
	vector<unsigned int> vtidx;
	vector<unsigned char> vtval1;
	vector<unsigned char> vtval2;
	unsigned int i;
	unsigned int smaller_len = (lenlog > lenrep) ? lenrep : lenlog;
	//for( i = 0; i < smaller_len; i++ ) {
	//	if( pRead1[i] != pRead2[i] ) {
	//		vtidx.push_back( i );
	//		vtval1.push_back( (unsigned char)pRead1[i] );
	//		vtval2.push_back( (unsigned char)pRead2[i] );
	//	}
	//}
	
	//if( vtidx.size() ) {
	//	DEBUGMSG("[[DIFF-DETAIL]]\n   ");
	//}
	/*
	for( i = 0; i < vtidx.size(); i++ ) {
		DEBUGMSG("(@ %X) [%02X]!=[%02X] ", vtidx[i], (unsigned char)vtval1[i], (unsigned char)vtval2[i]);
		if( (i+1) % 16 == 0 ) {
			DEBUGMSG("\n   ");
		}
	}
	*/
	DEBUGMSG("____________________________________________________________________________________________________________________________\n");
	DEBUGMSG("   [TEXT]___________________________________________________________________________________________________________________\n");
	DEBUGMSG("   LOG: %s\n", GetPrintableBuf_Text(plog, lenlog).c_str());
	DEBUGMSG("   REP: %s\n", GetPrintableBuf_Text(prep, lenrep).c_str());
	
	DEBUGMSG("   [BINARY]_________________________________________________________________________________________________________________\n");
	DEBUGMSG("                 ");
	for( i = 0; i < LINE_SIZE; i++ ) DEBUGMSG("%02X ", (unsigned char)i);
	DEBUGMSG("\n"); 
	char output_p1[4096], output_p2[4096], tmp[255];
	output_p1[0] = 0;
	output_p2[0] = 0;
	unsigned int row_start_p1 = 0;
	unsigned int row_start_p2 = 0;
	int diff_cnt = 0;
	for( i = 0; i < smaller_len; i++ ) {
		
		bool bsame = false;
		if( plog[i] != prep[i] ) {
			diff_cnt++;
		} else {
			bsame = true;
		}
		
		
		sprintf(tmp, "%02X ", (unsigned char)plog[i]);
		strcat( output_p1, tmp );
		
		if( bsame ) {
			sprintf(tmp, "__ " );
			strcat( output_p2, tmp );
		} else {
			sprintf(tmp, "%02X ", (unsigned char)prep[i]);
			strcat( output_p2, tmp );
		} 
		
		
		//DEBUGMSG("%02X ", p1[i]);
		if( (i+1) % LINE_SIZE == 0 ) {
			if( diff_cnt > 0 ) {
				DEBUGMSG("   LOG %08X: %s\n", row_start_p1, output_p1);
				DEBUGMSG("   REP %08X: %s\n\n", row_start_p1, output_p2);
			} else {
				
			}
			row_start_p1 = i+1;
			row_start_p2 = i+1;
			output_p1[0] = 0;
			output_p2[0] = 0;
			diff_cnt = 0;
		}
	}
	if( diff_cnt > 0 ) {
		DEBUGMSG("   LOG %08X: %s\n", row_start_p1, output_p1);
		DEBUGMSG("   REP %08X: %s\n", row_start_p1, output_p2);
	}
	
	
	
	
	
	
	if( lenlog == lenrep ) {
	} else if( lenlog > lenrep ) {
		// p1 is longer
		DEBUGMSG("\n      Remaining Bytes from LOG @ %X\n\t", lenlog);
		for( i = smaller_len; i < lenlog; i++ ) {
			DEBUGMSG("%02X ", (unsigned char)plog[i]);
			if( (i+1) % LINE_SIZE == 0 ) {
				DEBUGMSG("\n   ");
			}
		}
	} else {
		// p2 is longer
		DEBUGMSG("\n      Remaining Bytes from REP @ %X\n", lenrep);
		for( i = smaller_len; i < lenrep; i++ ) {
			DEBUGMSG("%02X ", (unsigned char)prep[i]);
			if( (i+1) % LINE_SIZE == 0 ) {
				DEBUGMSG("\n   ");
			}
		}
		
	}
	DEBUGMSG("____________________________________________________________________________________________________________________________\n");
	
	
}

