#include <signal.h>

#undef DEBUG_STR
#define DEBUG_STR "//DBG//"


#define _ENABLE_HOOK_strncpy				1
#define _ENABLE_HOOK_malloc					1
#define _ENABLE_HOOK_mvprintw				1
#define _ENABLE_HOOK_printw					1
#define _ENABLE_HOOK_write					1
#define _ENABLE_HOOK_pwrite64				1
#define _ENABLE_HOOK_writev					1
#define _ENABLE_HOOK_send					1
#define _ENABLE_HOOK_sendmsg				1
#define _ENABLE_HOOK_sendfile64				1
#define _ENABLE_HOOK_fprintf				1
#define _ENABLE_HOOK_fputs					1
//#if 0
#define _ENABLE_HOOK__IO_putc				1
#define _ENABLE_HOOK_fwrite					1
#define _ENABLE_HOOK_fputc					1
#define _ENABLE_HOOK_vfprintf				1
#define _ENABLE_HOOK_tputs					1
//#define _ENABLE_HOOK_waddch					1
//#define _ENABLE_HOOK_waddnstr				1
#define _ENABLE_HOOK_perror					1
#define _ENABLE_HOOK_puts					1


// =================================================================
// file system operations
#define _ENABLE_HOOK_remove 					1
#define _ENABLE_HOOK_unlink 					1
#define _ENABLE_HOOK_link 						1
#define _ENABLE_HOOK_rmdir 						1
#define _ENABLE_HOOK_mkdir 						1
#define _ENABLE_HOOK_chown 						1
#define _ENABLE_HOOK_chmod 						1
#define _ENABLE_HOOK_chroot 					1
#define _ENABLE_HOOK_fchmod 					1
// change file time
#define _ENABLE_HOOK_utime 						1
#define _ENABLE_HOOK_utimes 					1

#define _ENABLE_HOOK_chdir 						1
#define _ENABLE_HOOK_rename 					1




// =================================================================
// internal settings
#define _ENABLE_HOOK_setrlimit64 				1
#define _ENABLE_HOOK_setgid 					1
#define _ENABLE_HOOK_setuid 					1
#define _ENABLE_HOOK_setsid 					1
#define _ENABLE_HOOK_setlocale 					1


// =================================================================
// console output
#define _ENABLE_HOOK_printf 					0
#define _ENABLE_HOOK_putchar 					0
///////////////////////////////////////////////////////////////////////////////////




#if INTEGER_OVERFLOW_CASE
//
// this is here for integer overflow cases
//
#if (1 == _ENABLE_HOOK_strncpy)
#define FUNCNAME	 			strncpy
#define FUNCRETURN				char*
#define FUNCPARAM 				char* dst, const char* src, size_t size
#define FUNCPARAM_INVOKE 		dst, src, size
#define SYSCALL_NUM				SC_STRNCPY

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		
		DEBUGMSG("strncpy();\n");
	} 
	HOOK_FUNC_OPER_FOOTER
}
#include "hook_func_undef_macros.h"
#endif

//
// this is here for integer overflow cases
//
#if (1 == _ENABLE_HOOK_malloc)
#define FUNCNAME	 			malloc
#define FUNCRETURN				void*
#define FUNCPARAM 				size_t size
#define FUNCPARAM_INVOKE 		size
#define SYSCALL_NUM SC_MALLOC

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		if (DEBUGMSG_GUARD) {
			// DEBUGMSG("%s();\n", SYSCALL_NAMES[SYSCALL_NUM]);
		}
	}
	HOOK_FUNC_OPER_FOOTER
}
#include "hook_func_undef_macros.h"
#endif

#endif

#if (1 == _ENABLE_HOOK_mvprintw)
#define FUNCNAME	 			mvprintw
#define FUNCPARAM 				int y, int x, const char *format, ...
#define FUNCRETURN				int
#define FUNCPARAM_INVOKE 		/**/
#define SYSCALL_NUM				SC_MVPRINTW

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	ENTER_FUNC;
		
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM] );
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		if( outbuf ) {
			ret = orgfn( y, x, outbuf );
			free( outbuf );
		}
	
	} else {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		if( outbuf ) {
			ret = orgfn( y, x, outbuf );
			DEBUGMSG("%s();\n", SYSCALL_NAMES[SYSCALL_NUM]);
		
			free( outbuf );
		}
	} 
	
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"
#endif

#if (1 == _ENABLE_HOOK_printw)
#define FUNCNAME	 			printw
#define FUNCPARAM 				const char *format, ...
#define FUNCRETURN				int
#define FUNCPARAM_INVOKE 		/**/
#define SYSCALL_NUM				SC_PRINTW

//
// OUTGOING SYSTEM CALL: WILL COMPARE PARAMETERS RATHER THAN COPY.
//
extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM] );
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		va_list args;
		va_start(args, format);
		ret = vprintf(format, args);
		va_end(args);
	} else {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		ret = orgfn( outbuf );
		
		DEBUGMSG("%s();\n", SYSCALL_NAMES[SYSCALL_NUM]);
		
		if(outbuf) free( outbuf );
	} 
	
	
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"
#endif

#if !PRINTF_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK_printf)
#define FUNCNAME	 			printf
#define FUNCPARAM 				const char *format, ...
#define FUNCRETURN				int
#define FUNCPARAM_INVOKE 		/**/
#define SYSCALL_NUM				SC_PRINTF

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		va_list args;
		va_start(args, format);
		ret = vprintf(format, args);
		va_end(args);
	} else {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		if( outbuf ) {
			ret = orgfn( "%s", outbuf );
		} else {
			ret = orgfn( "" );
		}
		
		if( outbuf ) {
			DEBUGMSG("%s();\n", SYSCALL_NAMES[SYSCALL_NUM]) ;
			
			string rsrcname;
			vector<LOG_DATA> vtret, vtarg;
			rsrcname = string("printf");
			APPEND_STR(vtarg, outbuf, "outbuf", 0);
			LogResource( CATEGORY_CONSOLE, rsrcname, SYSCALL_NUM, vtret, vtarg, g_pshm_logs, true );
		}
		
		if(outbuf) free( outbuf );
	} 
	
	
	LEAVE_FUNC;
	return ret;
}
#include "hook_func_undef_macros.h"
#endif 

#endif

#if !WRITE_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK_write)
#define FUNCNAME	 			write
#define FUNCPARAM 				int fd, const void *buf, size_t count
#define FUNCRETURN				ssize_t
#define FUNCPARAM_INVOKE 		fd, buf, count
#define SYSCALL_NUM				SC_WRITE

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );

		RSRC_VARS;
		GetFD( fd, rsrcname, fd_type );
		APPEND_SIZE(vtarg, count, "count", 0);
		DEBUGMSG("write(%s);\n", GET_FILE(fd).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif

#endif
#if (1 == _ENABLE_HOOK_pwrite64)
#define FUNCNAME	 			pwrite64
#define FUNCPARAM 				int fd, const void *buf, size_t count, off64_t offset
#define FUNCPARAM_INVOKE 		fd, buf, count, offset
#define SYSCALL_NUM				SC_PWRITE64
#define FUNCRETURN				ssize_t

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("pwrite64(%s);\n", GET_FILE(fd).c_str());
	}
	HOOK_FUNC_OPER_FOOTER	
}

#include "hook_func_undef_macros.h"
#endif

#if (1 == _ENABLE_HOOK_writev)
#define FUNCNAME	 			writev
#define FUNCPARAM 				int fd, const struct iovec *iov, int iovcnt
#define FUNCPARAM_INVOKE 		fd, iov, iovcnt
#define SYSCALL_NUM				SC_WRITEV
#define FUNCRETURN				ssize_t

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("writev(%s);\n", GET_FILE(fd).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif

#if (1 == _ENABLE_HOOK_send)
#define FUNCNAME	 			send
#define FUNCPARAM 				int fd, const void* buf, size_t len, int flags
#define FUNCPARAM_INVOKE 		fd, buf, len, flags
#define SYSCALL_NUM				SC_SEND
#define FUNCRETURN				ssize_t

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("send(%s);\n", GET_SOCKET(fd).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif


#if (1 == _ENABLE_HOOK_sendmsg)
#define FUNCNAME	 			sendmsg
#define FUNCPARAM 				int fd, const struct msghdr *msg, int flags
#define FUNCPARAM_INVOKE 		fd, msg, flags
#define SYSCALL_NUM				SC_SENDMSG
#define FUNCRETURN				ssize_t

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("sendmsg(%s);\n", GET_SOCKET(fd).c_str());
	}
	HOOK_FUNC_OPER_FOOTER	
}

#include "hook_func_undef_macros.h"
#endif

#if (1 == _ENABLE_HOOK_sendfile64)
#define FUNCNAME	 			sendfile64
#define FUNCRETURN				ssize_t
#define FUNCPARAM 				int s, int fd, off64_t *offset, size_t count
#define FUNCPARAM_INVOKE 		s, fd, offset, count
#define SYSCALL_NUM				SC_SENDFILE64

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("sendfile64(%s,%s);\n", GET_FILE(s).c_str(), GET_FILE(fd).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif

#if !FPRINTF_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK_fprintf)
#define FUNCNAME	 			fprintf
#define FUNCPARAM 				FILE* stream, const char *format, ...
#define FUNCRETURN				int
#define FUNCPARAM_INVOKE 		/**/
#define SYSCALL_NUM				SC_FPRINTF

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM] );
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		if( outbuf ) {
			ret = orgfn( stream, outbuf );
			DEBUGMSG("fprintf(%s);\n", GET_FILE(stream).c_str());
			free( outbuf );
		}
	} else {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		if( outbuf ) {
			ret = orgfn( stream, outbuf );
			DEBUGMSG("fprintf(%s);\n", GET_FILE(stream).c_str());
			//DEBUGMSG("%s(%s)\n", FUNCNAME_STR, outbuf);
			
			string rsrcname;
			vector<LOG_DATA> vtret, vtarg;
			rsrcname = GetFILE( stream );
			APPEND_INT64(vtret, ret, "ret", 0);
			APPEND_STR(vtarg, outbuf, "buf", 0);
			LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg, g_pshm_logs, true );
		
			free( outbuf );
		}
	} 
	
	
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"
#endif
#endif

#if !FPUTS_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK_fputs)
#define FUNCPARAM 				const char * str, FILE * stream
#define FUNCRETURN				int
#define FUNCPARAM_INVOKE 		str, stream
#define SYSCALL_NUM				SC_FPUTS

extern "C" FUNCRETURN fputs( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("fputs(%s);\n", GET_FILE(stream).c_str());		
		string rsrcname;
		vector<LOG_DATA> vtret, vtarg;
		rsrcname = GetFILE( stream );
		APPEND_INT64(vtret, ret, "ret", 0);
		APPEND_STR(vtarg, str, "str", 0);
		LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg, g_pshm_logs, true );
		
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif
#endif

#if !_IO_PUTC_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK__IO_putc)
#define FUNCPARAM 				int __c, _IO_FILE * __fp
#define FUNCRETURN				int
#define FUNCPARAM_INVOKE 		__c, __fp
#define SYSCALL_NUM				SC__IO_PUTC

extern "C" FUNCRETURN _IO_putc( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("_IO_putc(%s);\n", GET_FILE(__fp).c_str());
		//DEBUGMSG("%s("DEBUGTEXTPARAM") begin [%d]\n", FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());
		//DEBUGMSG("[%d] "FUNCNAME_STR"("DEBUGTEXTPARAM")\n",
		//		  READ_COUNTER(), FUNCPARAM_INVOKE);
		//DEBUGMSG(FUNCNAME_STR"(%c, FILE*)\n",
		//		  __c);
		string rsrcname;
		vector<LOG_DATA> vtret, vtarg;
		int fd_type = 0;
		rsrcname = GetFILE( __fp );
		APPEND_INT64(vtret, ret, "ret", 0);
		APPEND_CHAR(vtarg, __c, "buf", 0);
		LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg, g_pshm_logs, true );
	}
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"
#endif
#endif

#if !FWRITE_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK_fwrite)
#define FUNCNAME	 			fwrite
#define FUNCRETURN				size_t
#define FUNCPARAM 				const void* ptr, size_t size, size_t nmemb, FILE* stream
#define FUNCPARAM_INVOKE 		ptr, size, nmemb, stream
#define SYSCALL_NUM				SC_FWRITE

//[5] write			[library]
extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		if (DEBUGMSG_GUARD) {
			DEBUGMSG("fwrite(%s);\n", GET_FILE(stream).c_str());
		}
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif
#endif


#if !FPUTC_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK_fputc)
#define FUNCRETURN				int
#define FUNCPARAM 				int c, FILE* stream
#define FUNCPARAM_INVOKE 		c, stream
#define SYSCALL_NUM				SC_FPUTC

extern "C" FUNCRETURN fputc( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("fputc(%s);\n", GET_FILE(stream).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif
#endif

#if !VFPRINTF_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK_vfprintf)
#define FUNCRETURN				int
#define FUNCPARAM 				FILE *stream, const char *format, va_list ap
#define FUNCPARAM_INVOKE 		stream, format, ap
#define SYSCALL_NUM				SC_VFPRINTF

extern "C" FUNCRETURN vfprintf(FUNCPARAM)
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("vfprintf();\n");
		{
			char output[4096];
			vsprintf( output, format, ap );
			
			string rsrcname;
			vector<LOG_DATA> vtret, vtarg;
			int fd_type = 0;
			rsrcname = GetFILE( stream );
			APPEND_INT(vtret, ret, "ret", 0);
			APPEND_STR(vtarg, output, "output", 0);
			LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg, g_pshm_logs, true );
		
		}
		
		
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif
#endif

#if (1 == _ENABLE_HOOK_tputs)
#define FUNCRETURN				int
#define FUNCPARAM 				const char* str, int affcnt, int (*fnputc)(int)
#define FUNCPARAM_INVOKE 		str, affcnt, fnputc
#define SYSCALL_NUM				SC_TPUTS

extern "C" FUNCRETURN tputs( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("tputs();\n");		
		
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif

#if (1 == _ENABLE_HOOK_waddch)
#define FUNCRETURN				int
#define FUNCPARAM 				WINDOW* win, const chtype ch
#define FUNCPARAM_INVOKE 		win, ch
#define SYSCALL_NUM				SC_WADDCH

extern "C" FUNCRETURN waddch( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("waddch();\n");		
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif

#if (1 == _ENABLE_HOOK_waddnstr)
#define FUNCRETURN				int
#define FUNCPARAM 				WINDOW* win, const char* str, int n
#define FUNCPARAM_INVOKE 		win, str, n
#define SYSCALL_NUM				SC_WADDNSTR

extern "C" FUNCRETURN waddnstr( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("%d = waddnstr(%s,%d);\n", ret, str, n);
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif

#if (1 == _ENABLE_HOOK_perror)
#define FUNCRETURN				void 
#define FUNCPARAM 				const char* s
#define FUNCPARAM_INVOKE 		s
#define SYSCALL_NUM				SC_PERROR

extern "C" FUNCRETURN perror( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2_NORET
	{
		orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("perror();\n");
	}
	LEAVE_FUNC;
}

#include "hook_func_undef_macros.h"
#endif

#if 1

#if !PUTS_IS_DETERMINISTIC	
#if (1 == _ENABLE_HOOK_puts)
#define FUNCPARAM 				const char * str
#define FUNCRETURN				int
#define FUNCPARAM_INVOKE 		str
#define SYSCALL_NUM				SC_PUTS

extern "C" FUNCRETURN puts( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("puts();\n");
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif

#endif
#endif



#if !PUTCHAR_IS_DETERMINITIC
#if (1 == _ENABLE_HOOK_putchar)
#define FUNCRETURN				int
#define FUNCPARAM 				int c
#define FUNCPARAM_INVOKE 		c
#define SYSCALL_NUM				SC_PUTCHAR

extern "C" FUNCRETURN putchar( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("putchar();\n");	
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif
#endif






#if (1 == _ENABLE_HOOK_remove)
#define FUNCNAME	 			remove
#define FUNCRETURN				int
#define FUNCPARAM 				const char* pathname
#define FUNCPARAM_INVOKE 		pathname
#define SYSCALL_NUM				SC_REMOVE

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("remove(%s);\n", GET_FILE(pathname).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif




#if (1 == _ENABLE_HOOK_unlink)
#define FUNCNAME 				unlink
#define SYSCALL_NUM				SC_UNLINK
#define FUNCRETURN				int
#define FUNCPARAM 				const char* pathname
#define FUNCPARAM_INVOKE 		pathname

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	//HOOK_FUNC_OPER_HEADER
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("unlink(%s);\n", GET_FILE(pathname).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif

#if (1 == _ENABLE_HOOK_link)
#define FUNCNAME	 			link
#define FUNCRETURN				int
#define FUNCPARAM 				const char *oldpath, const char *newpath
#define FUNCPARAM_INVOKE 		oldpath, newpath
#define SYSCALL_NUM				SC_LINK

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("link(%s,%s);\n", GET_FILE(oldpath).c_str() , GET_FILE(newpath).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}
#include "hook_func_undef_macros.h"
#endif



#if (1 == _ENABLE_HOOK_rmdir)
#define FUNCNAME 				rmdir
#define SYSCALL_NUM				SC_RMDIR
#define FUNCRETURN				int
#define FUNCPARAM 				const char* pathname
#define FUNCPARAM_INVOKE 		pathname

extern "C" FUNCRETURN FUNCNAME(FUNCPARAM)
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("rmdir(%s);\n", GET_FILE(pathname).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif



#if (1 == _ENABLE_HOOK_mkdir)
#define FUNCNAME 				mkdir
#define SYSCALL_NUM				SC_MKDIR
#define FUNCRETURN				int 
#define FUNCPARAM 				const char* path, mode_t mode
#define FUNCPARAM_INVOKE 		path, mode


extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("mkdir(%s);\n", GET_FILE(path).c_str());
	} 
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif



#if (1 == _ENABLE_HOOK_chown)
#define FUNCNAME 				chown
#define SYSCALL_NUM				SC_CHOWN
#define FUNCRETURN				int
#define FUNCPARAM 				const char* path, uid_t owner, gid_t group
#define FUNCPARAM_INVOKE 		path, owner, group

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("chown(%s);\n", GET_FILE(path).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}
#include "hook_func_undef_macros.h"
#endif




#if (1 == _ENABLE_HOOK_chmod)
#define FUNCNAME 				chmod
#define SYSCALL_NUM				SC_CHMOD
#define FUNCRETURN				int
#define FUNCPARAM 				const char* path, mode_t mode
#define FUNCPARAM_INVOKE 		path, mode

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("chmod(%s);\n", GET_FILE(path).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif





#if (1 == _ENABLE_HOOK_chroot)
#define FUNCNAME 				chroot
#define SYSCALL_NUM				SC_CHROOT
#define FUNCRETURN				int
#define FUNCPARAM 				const char *path
#define FUNCPARAM_INVOKE 		path


extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("chroot(%s);\n", GET_FILE(path).c_str());
	}
	HOOK_FUNC_OPER_FOOTER	
}

#include "hook_func_undef_macros.h"
#endif


#if (1 == _ENABLE_HOOK_fchmod)
#define FUNCNAME	 			fchmod
#define FUNCRETURN				int
#define FUNCPARAM 				int fd, mode_t mode
#define FUNCPARAM_INVOKE 		fd, mode
#define SYSCALL_NUM				SC_FCHMOD

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("fchmod(%s);\n", GET_FILE(fd).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif





#if (1 == _ENABLE_HOOK_chdir)
#define FUNCNAME 				chdir
#define SYSCALL_NUM				SC_CHDIR
#define FUNCRETURN				int
#define FUNCPARAM 				const char *path
#define FUNCPARAM_INVOKE 		path

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("chdir(%s);\n", GET_FILE(path).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif


#if (1 == _ENABLE_HOOK_rename)
#define FUNCNAME 				rename
#define SYSCALL_NUM				SC_RENAME
#define FUNCRETURN				int
#define FUNCPARAM 				const char *olddir, const char *newdir
#define FUNCPARAM_INVOKE 		olddir, newdir

extern "C" FUNCRETURN FUNCNAME(FUNCPARAM)
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		UPDATE_FILENAME(olddir, newdir);
		DEBUGMSG("rename(%s);\n", GET_FILE(newdir).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}
#include "hook_func_undef_macros.h"
#endif




#if (1 == _ENABLE_HOOK_utimes)
#define FUNCNAME	 			utimes
#define SYSCALL_NUM				SC_UTIMES
#define FUNCRETURN				int
#define FUNCPARAM 				const char* filename, const struct timeval times[2]
#define FUNCPARAM_INVOKE 		filename, times

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("utimes(%s);\n", GET_FILE(filename).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif


#if (1 == _ENABLE_HOOK_utime)
#include <utime.h>
#define FUNCNAME	 			utime
#define SYSCALL_NUM				SC_UTIME
#define FUNCRETURN				int
#define FUNCPARAM 				const char *filename, const struct utimbuf *times
#define FUNCPARAM_INVOKE 		filename, times

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("utime(%s);\n", GET_FILE(filename).c_str());
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif



#if (1 == _ENABLE_HOOK_setrlimit64)
#define FUNCNAME	 			setrlimit64
#define FUNCRETURN				int 
#define FUNCPARAM 				int res, const struct rlimit64* rlp
#define FUNCPARAM_INVOKE 		res, rlp
#define SYSCALL_NUM				SC_SETRLIMIT64

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER3
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif



#if (1 == _ENABLE_HOOK_setgid)
#define FUNCNAME	 			setgid
#define FUNCRETURN				int 
#define FUNCPARAM 				gid_t gid
#define FUNCPARAM_INVOKE 		gid
#define SYSCALL_NUM				SC_SETGID

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER3
	DEBUGMSG("setgid();\n");
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif



#if (1 == _ENABLE_HOOK_setuid)
#define FUNCNAME	 			setuid
#define FUNCRETURN				int 
#define FUNCPARAM 				uid_t uid
#define FUNCPARAM_INVOKE 		uid
#define SYSCALL_NUM				SC_SETUID

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER3
	DEBUGMSG("setuid();\n");
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif




#if (1 == _ENABLE_HOOK_setsid)
#define FUNCNAME	 			setsid
#define FUNCRETURN				pid_t
#define FUNCPARAM 				/**/
#define FUNCPARAM_INVOKE 		/**/
#define SYSCALL_NUM				SC_SETSID

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER_NOPARAM3
	DEBUGMSG("setsid();\n");
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif




		
#if (1 == _ENABLE_HOOK_setlocale)
#define FUNCNAME	 			setlocale
#define FUNCRETURN				char*
#define FUNCPARAM 				int category, const char* locale
#define FUNCPARAM_INVOKE 		category, locale
#define SYSCALL_NUM				SC_SETLOCALE

	
extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER2
	{
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("setlocale();\n");
	} 
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif
