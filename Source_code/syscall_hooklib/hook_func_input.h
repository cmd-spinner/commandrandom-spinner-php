//#include "../common/models.h"
#include "models.h"

#undef DEBUG_STR
#define DEBUG_STR "//DBG//"

// getaddrinfo 
#define __ENABLE_HOOK_getaddrinfo			1



// read
#define __ENABLE_HOOK_pread64				0
#define __ENABLE_HOOK_read					1
#define __ENABLE_HOOK_readv					1
#define __ENABLE_HOOK_fgets					1
#define __ENABLE_HOOK_fread					1
#define __ENABLE_HOOK_fgetc					1 // too slow
#define __ENABLE_HOOK__IO_getc				1 // too slow
#define __ENABLE_HOOK_feof					1


// recv
#define __ENABLE_HOOK_recvmsg				1
#define __ENABLE_HOOK_recv 					1


#define __ENABLE_HOOK_getlogin				1


// =================================================================
// inflate/deflate
#define _ENABLE_HOOK_inflateEnd 				1
#define _ENABLE_HOOK_deflateEnd 				1
#define _ENABLE_HOOK_inflateInit_ 				1
#define _ENABLE_HOOK_deflateInit_ 				1
#define _ENABLE_HOOK_deflate 					1
#define _ENABLE_HOOK_inflate 					1


// etc
#define __ENABLE_HOOK_getpass				1


/*
#define HANDLE_URANDOM_FILE(__length, __fname, __buf, __size) \
			if( __length > 0 ) {\
				if( strcmp( __fname, "/dev/urandom" ) == 0 || strcmp( __fname, "/dev/random" ) == 0 ) {\
					char* ptmpbuf = (char*)__buf;\
					for( int i = 0; i < __size; i++ ) {\
						ptmpbuf[i] = (char)i+1;\
					}\
				}\
			}
*/


	

bool white_list_rsrc(string& rsrcname, int type)
{
	if( type == FD_TYPE_EVENT ) 
	{
		return true;
	}
	
	if( type == FD_TYPE_FILE ) 
	{
		if( startsWith(rsrcname, string("Recovered:") ) == true ||
			rsrcname.find( string("places.sqlite") ) != string::npos ||
			rsrcname.find( string("/U1_Profile/") ) != string::npos ||
			rsrcname.find( string("/a15sania.U1/") ) != string::npos ||
			rsrcname.find( string("/.Xauthority") ) != string::npos ||
			rsrcname.find( string("/.ICEauthority") ) != string::npos )
		{
			return true;
		}
	}
	
	if( type == FD_TYPE_NETWORK ) 
	{
		if( endsWith(rsrcname, string(":unix") ) ) 
		{
			return true;
		}
	}
	
	return false;
}

bool do_not_print_log_rsrc(string& rsrcname)
{
	if( rsrcname == string("/usr/share/themes/Default/gtk-3.0/gtk-keys.css") ||
		rsrcname == string("/dev/urandom") ||
		rsrcname == string("/usr/lib/firefox/browser/chrome.manifest") ||
		rsrcname == string("/proc/self/stat") ||
		rsrcname.find(string(".mozilla/firefox/Crash Reports/LastCrash")) != string::npos ||
		rsrcname.find(string("/dev/pts/")) != string::npos ||
		rsrcname.find(string("/.viminfo")) != string::npos 
	)
	{
		return true;
	}
	return false;
}

bool do_not_print_log_rsrc_key(string& rsrc_key)
{
	if( rsrc_key.find(string("/usr/share/themes/Default/gtk-3.0/gtk-keys.css")) ||
		rsrc_key.find(string("/dev/urandom")) ||
		rsrc_key.find(string("/usr/lib/firefox/browser/chrome.manifest")) ||
		rsrc_key.find(string("/proc/self/stat")) ||
		rsrc_key.find(string(".mozilla/firefox/Crash Reports/LastCrash")) != string::npos ||
		rsrc_key.find(string("FILE//usr/share/X11/")) != string::npos ||
		startsWith(rsrc_key, string("FILE//usr/lib/python3.4/encodings/")) ||
		rsrc_key.find(string("FILE//usr/share/X11/")) != string::npos ||
		rsrc_key == string("FILE//usr/lib/firefox/browser/components/components.manifest")
	)
	{
		return true;
	}
	return false;
}


#if (__ENABLE_HOOK_getaddrinfo == 1)

FUNC_P4(int, getaddrinfo, const char*, node, const char*, service, const struct addrinfo*, hints, struct addrinfo **, res)
{
	ret = orgfn( node, service, hints, res );
	
	DEBUGMSG("getaddrinfo();\n");
}
FUNC_END();	

#endif

#if (__ENABLE_HOOK_pread64 == 1)

FUNC_P4(ssize_t, pread64, int, fd, void *, buf, size_t, count, off64_t, offset)
{
	RSRC_VARS;
	string fd_str = GetFD(fd, fd_type);
	
	APPEND_UINT64( vtarg, count, "count", 0 );
	APPEND_UINT64( vtarg, offset, "offset", 0 );			

	ret = orgfn( FUNCPARAM_INVOKE );

	DEBUGMSG("pread64(%s);\n"GET_FILE(fd).c_str());
}
FUNC_END();

#endif

#if (__ENABLE_HOOK_fread == 1)


FUNC_P4(size_t, fread, void*, ptr, size_t, size, size_t, cnt, FILE*, stream)
{
	ret = orgfn( ptr, size, cnt, stream );
	DEBUGMSG("fread(%s,%u);\n", GET_FILE(stream).c_str(), size*cnt);
}
FUNC_END();
#endif


#if (__ENABLE_HOOK_read == 1)

FUNC_P3(ssize_t, read, int, fd, void *, buf, size_t, count)
{
	ret = orgfn( fd, buf, count );
	DEBUGMSG("read(%s,%u);\n", GET_FILE(fd).c_str(), count);	
}
FUNC_END();
#endif


#if (__ENABLE_HOOK_readv == 1)

FUNC_P3(ssize_t, readv, int, fd, const struct iovec *, iov, int, iovcnt)
{
	RSRC_VARS
	GetFD( fd, rsrcname, fd_type );
	APPEND_INT( vtarg, iovcnt, "iovcnt", 0 );
	/*
		*  On success, readv(), preadv() and preadv2() return the number of
		bytes read; writev(), pwritev() and pwritev2() return the number of
		bytes written.
	* */
	ret = orgfn( fd, iov, iovcnt );

	DEBUGMSG("readv(%s);\n", GET_FILE(fd).c_str());
}
FUNC_END();
#endif

#if (__ENABLE_HOOK_recv == 1)

FUNC_P4(ssize_t, recv, int, sockfd, void*, buf, size_t, len, int, flags)
{
	ret = orgfn( sockfd, buf, len, flags );
	
	DEBUGMSG("recv(%s);\n", GET_SOCKET(sockfd).c_str());
}
FUNC_END();
#endif


#if (__ENABLE_HOOK_recvmsg == 1)

FUNC_P3(ssize_t, recvmsg, int, sockfd, struct msghdr*, msg, int, flags)
{
	ret = orgfn( sockfd, msg, flags );

	string strSig; get_msghdr_signature( msg, strSig );
	DEBUGMSG("recvmsg(%s);\n", GET_SOCKET(sockfd).c_str());
}
FUNC_END();
#endif

#if (__ENABLE_HOOK_getpass == 1)

FUNC_P1(char*, getpass, const char*, prompt)
{
	ret = orgfn( prompt );
	DEBUGMSG("getpass();\n");
}
FUNC_END();
#endif


#if (__ENABLE_HOOK__IO_getc == 1)
//#define FUNCPARAM_INVOKE_NOPTR 	"void"
//#define DEBUGTEXTPARAM_NOPTR	" %d = "FUNCNAME_STR"(%s);\n"

FUNC_P1(int, _IO_getc, _IO_FILE*, __fp)
{
	ret = orgfn( __fp );
	//DEBUGMSG("%d = "FUNCNAME_STR"( __fp );\n", ret );
}
FUNC_END();
#endif



#if (__ENABLE_HOOK_getlogin == 1)

FUNC_P0(char*, getlogin)
{
	ret = orgfn();
	
	DEBUGMSG("getlogin();\n");
}
FUNC_END();
#endif


#if (__ENABLE_HOOK_fgets == 1)


FUNC_P3(char* , fgets, char*, s, int, size, FILE*, stream)
{
	ret = orgfn( s, size, stream );

	DEBUGMSG("fgets(%s);\n", GET_FILE(stream).c_str());
}
FUNC_END();
#endif







#if (__ENABLE_HOOK_fgetc == 1)

FUNC_P1(int, fgetc, FILE*, stream)
{
	ret = orgfn( stream );
	
	DEBUGMSG("fgetc(%s);\n", GET_FILE(stream).c_str());
}
FUNC_END();
#endif



#if 0 // ---- finding blocking api
#endif // #if 0 // ---- finding blocking api

#if (1 == _ENABLE_HOOK_inflateEnd)


FUNC_P1(int, inflateEnd, z_streamp, stream)
{
	ret = orgfn( stream );
	DEBUGMSG("inflateEnd();\n");
}
FUNC_END();
#endif

#if (1 == _ENABLE_HOOK_deflateEnd)

FUNC_P1(int, deflateEnd, z_streamp, stream)
{
	ret = orgfn( stream  );
	DEBUGMSG("deflateEnd();\n");
}
FUNC_END();
#endif


#if (1 == _ENABLE_HOOK_inflateInit_)

FUNC_P3(int, inflateInit_, z_streamp, stream, const char *, version, int, stream_size)
{
	ret = orgfn( stream, version, stream_size );
	DEBUGMSG("inflateInit_();\n");
}
FUNC_END();
#endif

#if (1 == _ENABLE_HOOK_deflateInit_)

FUNC_P4(int, deflateInit_, z_streamp, stream, int, level, const char *, version, int, stream_size)
{
	ret = orgfn( stream, level, version, stream_size );
	DEBUGMSG("deflateInit_();\n");
}
FUNC_END();
#endif


#if (1 == _ENABLE_HOOK_deflate)

FUNC_P2(int, deflate, z_streamp, stream, int, flush)
{
	ret = orgfn( stream, flush );
	DEBUGMSG("deflate();\n");
}
FUNC_END();
#endif

#if (1 == _ENABLE_HOOK_inflate)

FUNC_P2(int, inflate, z_streamp, stream, int, flush)
{
	ret = orgfn( stream, flush );
	DEBUGMSG("inflate();\n");
}
FUNC_END();
#endif











#if 0
#if !__ERRNO_LOCATION_IS_DETERMINISTIC

FUNC_P0(int*, __errno_location, )
{
	
	//HOOK_FUNC_OPER_HEADER_NOPARAM_NOPTR
	//ENTER_FUNC;
	FUNCRETURN ret;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR);

	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( FUNCPARAM_INVOKE );
		return ret;
	} else {
		ret = orgfn( FUNCPARAM_INVOKE );
		//printf("orgfn = %p, __errno_location = %p, dlsym(...) = %p\n", orgfn, __errno_location, (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR));
		//printf("__errno_location returns %p (thread: %d);\n", ret, get_thread_id(pthread_self()));
		//DEBUGMSG(" [%d] "TEXT_SINGLE_EXEC" "DEBUGTEXTPARAM_NOPTR, READ_COUNTER() );
		//printf("done?\n");
		//LEAVE_FUNC;
		//printf("returns %p\n", ret);
		return ret;
	} 
	LEAVE_FUNC;
	//printf("returns %p\n", ret);
	return ret;
}
#endif



FUNC_P0(int*, __h_errno_location, )
{
	HOOK_FUNC_OPER_HEADER_NOPARAM3
	HOOK_FUNC_OPER_FOOTER
}

string fname = getRsrcNameFromFd(fd);


#if !TNFTP_CASE

FUNC_P1(char* , getenv, const char *, name)
{
	HOOK_FUNC_OPER_HEADER3
	HOOK_FUNC_OPER_FOOTER
}
#endif

#endif



#if 0

FUNC_P1(int , wgetch, WINDOW*, win)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(FUNCNAME_STR"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );				
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int offset = 0;
			PUSH_PARAM( &ret, sizeof( FUNCRETURN ), offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
#if PRINT_ONSYSCALL_RETURN			
			DEBUGMSG( " OnSyscall("FUNCNAME_STR") = %s\n",  getSyscallResultString(nResult)  );
#endif			
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
#if PRINT_ONSYSCALL_RETURN			
			DEBUGMSG( " OnSyscall("FUNCNAME_STR") = %s\n",  getSyscallResultString(nResult)  );
#endif			
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int offset = 0;
					
					POP_PARAM( &ret, sizeof( FUNCRETURN ), offset );
					DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
					//ret = orgfn( FUNCPARAM_INVOKE );
				}
				break;
				
				case SYSCALL_RESULT_INDEPENDENT_SLAVE:
				default:
				{
					// 1. exec
					ret = orgfn( FUNCPARAM_INVOKE );
				}
				break;
			}
		}
		
		DEBUGMSG(FUNCNAME_STR"() return \n");
	} 
	HOOK_FUNC_OPER_FOOTER
}
#endif






//=============================================================
//=============================================================
//=============================================================
//=============================================================
//=============================================================

#define ORG_FSCANF() va_list args;\
		va_start(args, format);\
		ret = vfscanf(stream, format, args);\
		va_end(args);
	
	
extern "C" int __isoc99_fscanf( FILE* stream, const char *format, ... )
{
	ENTER_USERFUNC;
	
	//typedef int (*PFNORG)( FILE* stream, const char *format, ... ); 
	//static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, "__isoc99_fscanf");
	int ret;

	ORG_FSCANF();
	DEBUGMSG("%d = __isoc99_fscanf();\n", ret);
	
	LEAVE_USERFUNC;
	return ret;
	
}

extern "C" int fscanf( FILE* stream, const char *format, ... )
{
	ENTER_USERFUNC;
	
	//typedef int (*PFNORG)( FILE* stream, const char *format, ... ); 
	//static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, "__isoc99_fscanf");
	int ret;

	ORG_FSCANF();
	DEBUGMSG("%d = fscanf(%s);\n", ret, GetPrintableBuf_Text(format, strlen(format)).c_str());
	
	LEAVE_USERFUNC;
	return ret;
	
}
#undef ORG_FSCANF




