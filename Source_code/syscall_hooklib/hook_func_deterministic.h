
#if PRINTF_IS_DETERMINISTIC
extern "C" int printf( const char *format, ... )
{
	ENTER_FUNC;
		
	typedef int (*PFNORG)( const char *format, ... ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, "printf");
	int ret;
	if( CALL_FROM_HOOK_SO ) {
		va_list args;
		va_start(args, format);
		ret = vprintf(format, args);
		va_end(args);
	} else if( IS_FORKED() ) {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		
		if( IS_MASTER_EXEC() ) { 
			if( outbuf ) {
				ret = orgfn( "%s", outbuf );
			} else {
				ret = orgfn( "" );
			} 
		
			DEBUGMSG("%d = printf(%s)\n", ret, outbuf);
		} else {
			ret = buflen;
			DEBUGMSG("%d = printf(%s)\n", ret, outbuf);
		}
		free(outbuf);
	} else {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		if( outbuf ) {
			ret = orgfn( "%s", outbuf );
		} else {
			ret = orgfn( "" );
		} 
				
		DEBUGMSG(" [%d] printf(%s) - "TEXT_SINGLE_EXEC"\n", READ_COUNTER(), outbuf );
		
		if(outbuf) free( outbuf );
	} 
	LEAVE_FUNC;
	
	
	return ret;
}
#endif

#if PUTS_IS_DETERMINISTIC

#define FUNCPARAM 				const char * str
#define FUNCNAME 				puts
#define FUNCNAME_STR 			"puts"
#define FUNCRETURN				int
#define FUNCPARAM_INVOKE 		str
#define DEBUGTEXTPARAM			"%s"
#define SYSCALL_NUM				SC_PUTS

extern "C" FUNCRETURN puts( FUNCPARAM )
{
	ENTER_FUNC;
	
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR);
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( FUNCPARAM_INVOKE );
	} else if( IS_FORKED() ) {
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );
			
			static int exec_counter = 0;
			if( exec_counter == 33 ) {
				g_print_basicblock = 1;
			}
			DEBUGMSG(FUNCNAME_STR"("DEBUGTEXTPARAM"); [cnt = %d]\n", FUNCPARAM_INVOKE, exec_counter++ );			
		} else { // slave
			if( str ) {
				ret = strlen( str );
			} else {
				ret = -1;
			} 
			
			static int exec_counter = 0;
			if( exec_counter == 33 ) {
				g_print_basicblock = 1;
			}
			DEBUGMSG(FUNCNAME_STR"("DEBUGTEXTPARAM"); [cnt = %d]\n", FUNCPARAM_INVOKE, exec_counter++ );
		}
	} else {
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG(FUNCNAME_STR"("DEBUGTEXTPARAM") - "TEXT_SINGLE_EXEC"\n",
				 FUNCPARAM_INVOKE );
	}
	
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"


#endif

#if FPRINTF_IS_DETERMINISTIC

#define FUNCNAME	 			fprintf
#define FUNCNAME_STR 			"fprintf"
#define FUNCPARAM 				FILE* stream, const char *format, ...
#define FUNCRETURN				int
#define FUNCPARAM_INVOKE 		/**/
#define DEBUGTEXTPARAM			/**/
#define SYSCALL_NUM				SC_FPRINTF

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	ENTER_FUNC;
		
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR );
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		if( outbuf ) {
			ret = orgfn( stream, outbuf );
			free( outbuf );
		}
		
	} else if( IS_FORKED() ) {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( stream, outbuf );
			DEBUGMSG("%d = "FUNCNAME_STR"(%s);\n", buflen, outbuf, READ_COUNTER());
		} else {
			ret = buflen;
			DEBUGMSG("%d = "FUNCNAME_STR"(%s);\n", buflen, outbuf, READ_COUNTER());
		}
		free(outbuf);
	} else {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		if( outbuf ) {
			ret = orgfn( stream, outbuf );
			DEBUGMSG("%s(%s) - "TEXT_SINGLE_EXEC" [%d]\n", FUNCNAME_STR, outbuf, READ_COUNTER());
			free( outbuf );
		}
	} 
	
	
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"


#endif

#if FPUTC_IS_DETERMINISTIC

#define FUNCNAME 				fputc
#define FUNCNAME_STR 			"fputc"
#define FUNCRETURN				int
#define FUNCPARAM 				int c, FILE* stream
#define FUNCPARAM_INVOKE 		c, stream
#define DEBUGTEXTPARAM			"%c, %p"
#define SYSCALL_NUM				SC_FPUTC

extern "C" FUNCRETURN fputc( FUNCPARAM )
{
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR);
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( FUNCPARAM_INVOKE );
	} else if( IS_FORKED() ) {
		DEBUGMSG("%s("DEBUGTEXTPARAM") begin [%d]\n", FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());
		
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );
		} else { // slave
			ret = c;
		}
		DEBUGMSG("%d = %s("DEBUGTEXTPARAM") return [%d]\n", (int)ret, FUNCNAME_STR, 
				 FUNCPARAM_INVOKE, READ_COUNTER());
	} else {
		ret = orgfn( FUNCPARAM_INVOKE );
		//DEBUGMSG("%s("DEBUGTEXTPARAM") begin [%d]\n", FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());
		DEBUGMSG("%s("DEBUGTEXTPARAM") - "TEXT_SINGLE_EXEC" [%d]\n",
				 FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());		
	}
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"

#endif

#if FWRITE_IS_DETERMINISTIC
#define FUNCNAME	 			fwrite
#define FUNCNAME_STR 			"fwrite"
#define FUNCRETURN				size_t
#define FUNCPARAM 				const void* ptr, size_t size, size_t nmemb, FILE* stream
#define FUNCPARAM_INVOKE 		ptr, size, nmemb, stream
#define DEBUGTEXTPARAM			"%p, %d, %d, %p"
#define SYSCALL_NUM				SC_FWRITE

//[5] write			[library]
extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR);
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( FUNCPARAM_INVOKE );
	} else if( IS_FORKED() ) {
		
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );
			DEBUGMSG(FUNCNAME_STR"("DEBUGTEXTPARAM") - [%d]\n",
				 FUNCPARAM_INVOKE, READ_COUNTER());		
		} else { // slave
			ret = nmemb;
			DEBUGMSG(FUNCNAME_STR"("DEBUGTEXTPARAM") - [%d]\n",
				 FUNCPARAM_INVOKE, READ_COUNTER());		
		}
	} else {
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG("[%d] %s("DEBUGTEXTPARAM") - "TEXT_SINGLE_EXEC"\n",
				 READ_COUNTER(), FUNCNAME_STR, FUNCPARAM_INVOKE );		
	}
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"
#endif

#if WRITE_IS_DETERMINISTIC
#define FUNCNAME	 			write
#define FUNCNAME_STR 			"write"
#define FUNCPARAM 				int fd, const void *buf, size_t count
#define FUNCRETURN				ssize_t
#define FUNCPARAM_INVOKE 		fd, buf, count
#define DEBUGTEXTPARAM			" %d = "FUNCNAME_STR"( %d, %p, %d );\n"
#define FUNCPARAM_INVOKE_NOPTR 	fd, count
#define DEBUGTEXTPARAM_NOPTR	" %d = "FUNCNAME_STR"( %d, %d );\n"
#define SYSCALL_NUM				SC_WRITE

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER_NOPTR
	{
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );
		} else { // slave
			ret = count;
		}
		DEBUGMSG(FUNCNAME_STR"(%d, %d, %s) - [%d]\n",
				 fd, count, buf, READ_COUNTER());		
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif


#if PUTCHAR_IS_DETERMINITIC

#define FUNCNAME 				putchar
#define FUNCNAME_STR 			"putchar"
#define FUNCRETURN				int
#define FUNCPARAM 				int c
#define FUNCPARAM_INVOKE 		c
#define DEBUGTEXTPARAM			"%c"
#define SYSCALL_NUM				SC_PUTCHAR

extern "C" FUNCRETURN putchar( FUNCPARAM )
{
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR);
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( FUNCPARAM_INVOKE );
	} else if( IS_FORKED() ) {
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );
		} else { // slave
			ret = c;
		}
		DEBUGMSG("%s("DEBUGTEXTPARAM") - [%d]\n",
				 FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());		
	} else {
		ret = orgfn( FUNCPARAM_INVOKE );
		//DEBUGMSG("%s("DEBUGTEXTPARAM") begin [%d]\n", FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());
		DEBUGMSG("%s("DEBUGTEXTPARAM") - "TEXT_SINGLE_EXEC" [%d]\n",
				 FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());		
	}
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"
#endif

#if _IO_PUTC_IS_DETERMINISTIC

#define FUNCNAME 				_IO_putc
#define FUNCNAME_STR 			"_IO_putc"
#define FUNCPARAM 				int __c, _IO_FILE * __fp
#define FUNCRETURN				int
#define FUNCPARAM_INVOKE 		__c, __fp
#define DEBUGTEXTPARAM			"%c"
#define SYSCALL_NUM				SC__IO_PUTC

extern "C" FUNCRETURN _IO_putc( FUNCPARAM )
{
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR);
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( FUNCPARAM_INVOKE );
	} else if( IS_FORKED() ) {
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );
		} else { // slave
			ret = __c;
		}
	} else {
		ret = orgfn( FUNCPARAM_INVOKE );
		//DEBUGMSG("%s("DEBUGTEXTPARAM") begin [%d]\n", FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());
		DEBUGMSG(" [%d] "TEXT_SINGLE_EXEC" "FUNCNAME_STR"("DEBUGTEXTPARAM") \n", READ_COUNTER(), __c );
	}
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"

#endif

#if FPUTS_IS_DETERMINISTIC

#define FUNCNAME	 			fputs
#define FUNCNAME_STR 			"fputs"
#define FUNCPARAM 				const char * str, FILE * stream
#define FUNCRETURN				int
#define FUNCPARAM_INVOKE 		str, stream
#define DEBUGTEXTPARAM			"%s, %p"
#define SYSCALL_NUM				SC_FPUTS

extern "C" FUNCRETURN fputs( FUNCPARAM )
{
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR);
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( FUNCPARAM_INVOKE );
	} else if( IS_FORKED() ) {
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );
		} else { // slave
			ret = strlen( str );
		}
	} else {
		ret = orgfn( FUNCPARAM_INVOKE );
		//DEBUGMSG("%s("DEBUGTEXTPARAM") begin [%d]\n", FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());
		DEBUGMSG("%s("DEBUGTEXTPARAM") - "TEXT_SINGLE_EXEC" [%d]\n",
				 FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());		
	}
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"

#endif

#if VFPRINTF_IS_DETERMINISTIC

#define FUNCNAME 				vfprintf
#define FUNCNAME_STR 			"vfprintf"
#define FUNCRETURN				int
#define FUNCPARAM 				FILE *stream, const char *format, va_list ap
#define FUNCPARAM_INVOKE 		stream, format, ap
#define DEBUGTEXTPARAM			"%p, %p, %p"
#define SYSCALL_NUM				SC_VFPRINTF

extern "C" FUNCRETURN vfprintf(FUNCPARAM)
{
	ENTER_FUNC;
	
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR);
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( FUNCPARAM_INVOKE );
	} else if( IS_FORKED() ) {
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );
		} else { // slave
			char __output[4096];
			ret = vsprintf( __output, format, ap );			
		}
		DEBUGMSG(" [%d] "TEXT_SINGLE_EXEC" "FUNCNAME_STR"() \n", READ_COUNTER() );
	} else {
		ret = orgfn( FUNCPARAM_INVOKE );
		//DEBUGMSG("%s("DEBUGTEXTPARAM") begin [%d]\n", FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());
		DEBUGMSG(" [%d] "TEXT_SINGLE_EXEC" "FUNCNAME_STR"() \n", READ_COUNTER() );		
	}
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"
#endif


#if TIME_IS_DETERMINISTIC

#define FUNCNAME 				time
#define FUNCNAME_STR 			"time"
#define SYSCALL_NUM				SC_TIME
#define FUNCRETURN				time_t
#define FUNCPARAM 				time_t *t
#define FUNCPARAM_INVOKE 		t
#define DEBUGTEXTPARAM			" %d = "FUNCNAME_STR"( %p );\n"

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR);
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( FUNCPARAM_INVOKE );
	} else if( IS_FORKED() ) {
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );
		} else { // slave
			ret = orgfn( FUNCPARAM_INVOKE );
		}
		DEBUGMSG("%s() - [%d]\n",
				 FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());		
	} else {
		ret = orgfn( FUNCPARAM_INVOKE );
		//DEBUGMSG("%s("DEBUGTEXTPARAM") begin [%d]\n", FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());
		DEBUGMSG("%s("DEBUGTEXTPARAM") - "TEXT_SINGLE_EXEC" [%d]\n",
				 FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());		
	}
	LEAVE_FUNC;
	return ret;
}
#include "hook_func_undef_macros.h"
#endif

///*
#if __SIGSETJMP_IS_DETERMINISTIC
#define FUNCNAME	 			__sigsetjmp
#define FUNCNAME_STR 			"__sigsetjmp"
#define FUNCRETURN				int 
#define FUNCPARAM 				jmp_buf env, int savemask
#define FUNCPARAM_INVOKE 		env, savemask
#define DEBUGTEXTPARAM			" %d = "FUNCNAME_STR"( %p, %d );\n"
#define SYSCALL_NUM				SC___SIGSETJMP

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR);
	FUNCRETURN ret;
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( FUNCPARAM_INVOKE );
	} else if( IS_FORKED() ) {
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );
		} else { // slave
			ret = orgfn( FUNCPARAM_INVOKE );
		}
	} else {
		ret = orgfn( FUNCPARAM_INVOKE );
		DEBUGMSG(" [%d] "TEXT_SINGLE_EXEC" "DEBUGTEXTPARAM, READ_COUNTER(), ret, FUNCPARAM_INVOKE );
	}
	LEAVE_FUNC;
	return ret;
}

#include "hook_func_undef_macros.h"
#endif
//*/


#if SIGLONGJMP_IS_DETERMINISTIC

#define FUNCNAME	 			siglongjmp
#define FUNCNAME_STR 			"siglongjmp"
#define FUNCRETURN				void
#define FUNCPARAM 				sigjmp_buf env, int val
#define FUNCPARAM_INVOKE 		env, val
#define DEBUGTEXTPARAM			FUNCNAME_STR"( %p, %d );\n"
#define SYSCALL_NUM				SC_SIGLONGJMP

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, FUNCNAME_STR);
	if( CALL_FROM_HOOK_SO ) {
		orgfn( FUNCPARAM_INVOKE );
	} else if( IS_FORKED() ) {
		if( IS_MASTER_EXEC() ) { 
			orgfn( FUNCPARAM_INVOKE );
		} else { // slave
			orgfn( FUNCPARAM_INVOKE );
		}
	} else {
		DEBUGMSG("<%d> "TEXT_SINGLE_EXEC" - "DEBUGTEXTPARAM,
				 READ_COUNTER(), FUNCPARAM_INVOKE );		
		
		orgfn( FUNCPARAM_INVOKE );
		//DEBUGMSG("%s("DEBUGTEXTPARAM") begin [%d]\n", FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());
	}
	LEAVE_FUNC;
	
	while(1) { ; }
}

#include "hook_func_undef_macros.h"

#endif


#if OPENDIR_IS_DETERMINISTIC
#define FUNCNAME 				opendir
#define FUNCNAME_STR 			"opendir"
#define FUNCRETURN				DIR*
#define FUNCPARAM 				const char* name
#define FUNCPARAM_INVOKE 		name
#define DEBUGTEXTPARAM			" %p = "FUNCNAME_STR"( %s );\n"
#define SYSCALL_NUM				SC_OPENDIR

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER
	{
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );				
		} else {
			ret = orgfn( FUNCPARAM_INVOKE );					
		}
		DEBUGMSG(FUNCNAME_STR"() return \n");
	} 
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif


#if __ERRNO_LOCATION_IS_DETERMINISTIC
#define FUNCNAME_STR 			"__errno_location"
#define FUNCNAME 				__errno_location
#define SYSCALL_NUM				SC___ERRNO_LOCATION
#define FUNCRETURN				int*
#define FUNCPARAM 				void
#define FUNCPARAM_INVOKE 		/**/
#define DEBUGTEXTPARAM			"%p = "FUNCNAME_STR"( void );\n"
#define DEBUGTEXTPARAM_NOPTR	FUNCNAME_STR"( void );\n"

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER_NOPARAM_NOPTR
	{
		DEBUGMSG(FUNCNAME_STR"() begin\n");
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );
		} else {
			ret = orgfn( FUNCPARAM_INVOKE );
		}
		{
			//show_full_call_stack(0, 1);
		}
		DEBUGMSG(DEBUGTEXTPARAM, ret);
		DEBUGMSG(FUNCNAME_STR"() return\n");
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif

#if SIGNAL_IS_DETERMINISTIC
#define FUNCNAME 				signal
#define FUNCNAME_STR 			"signal"
#define SYSCALL_NUM				SC_SIGNAL
#define FUNCRETURN				sighandler_t
#define FUNCPARAM 				int signum, sighandler_t handler
#define FUNCPARAM_INVOKE 		signum, handler
#define DEBUGTEXTPARAM			" %p = "FUNCNAME_STR"( %d, %p );\n"

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER
	{
		DEBUGMSG(FUNCNAME_STR"() begin\n");
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );
		} else {
			ret = orgfn( FUNCPARAM_INVOKE );
		}
		DEBUGMSG(FUNCNAME_STR"() return \n");
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif


#if GETCWD_IS_DETERMINISTIC
#define FUNCNAME	 			getcwd
#define FUNCNAME_STR 			"getcwd"
#define FUNCRETURN				char* 
#define FUNCPARAM 				char* buf, size_t size
#define FUNCPARAM_INVOKE 		buf, size
#define DEBUGTEXTPARAM			" %s = "FUNCNAME_STR"( %p, %d );\n"
#define SYSCALL_NUM				SC_GETCWD

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER
	{
		DEBUGMSG(FUNCNAME_STR"() begin\n");
		
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );				
		} else {
			ret = orgfn( FUNCPARAM_INVOKE );
		}
		DEBUGMSG(FUNCNAME_STR"() return \n");
	} 
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif

#if GETPAGESIZE_IS_DETERMINISTIC
#define FUNCNAME_STR 			"getpagesize"
#define FUNCNAME	 			getpagesize
#define FUNCRETURN				int
#define FUNCPARAM 				/**/
#define FUNCPARAM_INVOKE 		/**/
#define DEBUGTEXTPARAM			" %d = "FUNCNAME_STR"();\n"
#define SYSCALL_NUM				SC_GETPAGESIZE

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER_NOPARAM
	{
		DEBUGMSG(FUNCNAME_STR"() begin\n");
		
		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );				
		} else {
			ret = orgfn( FUNCPARAM_INVOKE );
		}
		DEBUGMSG(FUNCNAME_STR"() return \n");
	} 
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif

#if CLOCK_IS_DETERMINISTIC
#define FUNCNAME	 			clock
#define FUNCNAME_STR 			"clock"
#define SYSCALL_NUM				SC_CLOCK
#define FUNCRETURN				clock_t 
#define FUNCPARAM 				/**/
#define FUNCPARAM_INVOKE 		/**/
#define DEBUGTEXTPARAM			" %d = "FUNCNAME_STR"( );\n"

extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
{
	HOOK_FUNC_OPER_HEADER_NOPARAM
	{
		DEBUGMSG(FUNCNAME_STR"() begin\n");

		if( IS_MASTER_EXEC() ) { 
			ret = orgfn( FUNCPARAM_INVOKE );				
		} else {
			ret = orgfn( FUNCPARAM_INVOKE );
		}
		
		DEBUGMSG(FUNCNAME_STR"() return \n");
	}
	HOOK_FUNC_OPER_FOOTER
}

#include "hook_func_undef_macros.h"
#endif




