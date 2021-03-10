
//______________________________________________________________________________
//
// INSTRUMENTATION OPTIONS
//______________________________________________________________________________
//

#define CHECK_RETURN_ADDR								0
#define USE_DEBUG_CALLBACK								1 // for debugging: for release it must be DISABLED
#define PRINT_BBADDR_AT_DEBUG_CALLBACK					1

#define DEBUGCALLBACK_ROUTINE_SHOW_CS 					0
#define RET_BUFFEROVERFLOW_CASES 						0
#define INTEGER_OVERFLOW_CASE							1

// =====================================================================================
#define BASIC_INSTRUMENTATION							1
#define RECURSIVE_CALL_INSTRUMENTATION					1
#define FILLING_GAPS_INSTRUMENTATION					1
#define INDIRECT_CALL_INSTRUMENTATION					1


//______________________________________________________________________________
//
// HOOK LIBRARY OPTIONS
//______________________________________________________________________________
//

// 
// GLOBAL OPTIONS
//
#define USE_LOG_FILES 									1 // for debugging
#define FORK_PROCESS									1

#define DISABLE_SYNC_LOOPTAIL_ROUTINE					0 // this must be 0 for release
#define PRINT_CALLSTACK_AT_LOOPTAIL_ROUTINE				0
#define PRINT_BBADDR_AT_LOOPTAIL_ROUTINE				0
#define FILTER_LOOPTAIL_ROUTINE							0
	
#define PRINT_ONSYSCALL_RETURN							1


// =====================================================================================
// =====================================================================================
// =====================================================================================
//
// CASE SPECIFIC
//
#define LYNX_CASE 										0 // 1
#define LYNX_COOKIE_CASE 								0 // 0 
#define NCURSES_LIB 									0 // 1

#define NGINX_CASE 										0 // 1
#define GIF2PNG_CASE 									0
#define PROZILLA_CASE 									0
#define GOCR_CASE										0

#define SYSSTAT_IOSTAT_CASE								0

#define TNFTP_CASE										0

#define SPEC2006_SJENG_CASE 							0 /////////////
#define SPEC2006_BZIP2_CASE 							0 ///////////////
#define SPEC2006_H264REF_CASE 							0 ///////////////
#define SPEC2006_MCF_CASE								0
#define SPEC2006_GOBMK_CASE								0
#define SPEC2006_ASTAR_CASE								0
#define SPEC2006_HMMER_CASE								0
#define SPEC2006_LIBQUANTUM_CASE						0
#define SPEC2006_PERLBENCH_CASE							0
#define SPEC2006_OMNETPP_CASE							0
#define SPEC2006_GCC_CASE								0 
#define SPEC2006_XALAN_CASE								0

#define FIREFOX_CASE									0

#define UNRTF_CASE										0

// =====================================================================================
// =====================================================================================
// =====================================================================================

#if SPEC2006_BZIP2_CASE
	#define PRINTF_IS_OPER									1
	#define PUTCHAR_IS_OPER									1

	#define PRINTF_IS_DETERMINISTIC							1
	#define PUTS_IS_DETERMINISTIC							1
	#define FPRINTF_IS_DETERMINISTIC						1
	#define FWRITE_IS_DETERMINISTIC							1
#endif

#if SPEC2006_SJENG_CASE 
	#define PRINTF_IS_OPER									1
	#define PUTCHAR_IS_OPER									1

	#define PRINTF_IS_DETERMINISTIC							1
	#define PUTS_IS_DETERMINISTIC							1
	#define FPRINTF_IS_DETERMINISTIC						1
	#define FWRITE_IS_DETERMINISTIC							1
	#define PUTCHAR_IS_DETERMINISTIC						1
	#define FPUTC_IS_DETERMINISTIC							1
#endif

#if SPEC2006_H264REF_CASE
	#define PRINTF_IS_OPER									1
	#define PUTCHAR_IS_OPER									1

	#define PRINTF_IS_DETERMINISTIC							1
	#define PUTS_IS_DETERMINISTIC							1
	#define FPRINTF_IS_DETERMINISTIC						1
	#define FWRITE_IS_DETERMINISTIC							1
	#define FPUTC_IS_DETERMINISTIC							1
	#define WRITE_IS_DETERMINISTIC							1
	#define PUTCHAR_IS_DETERMINISTIC						0
#endif

#if SPEC2006_MCF_CASE
	#define PRINTF_IS_OPER									1
	#define PUTCHAR_IS_OPER									1

	#define PRINTF_IS_DETERMINISTIC							1
	#define PUTS_IS_DETERMINISTIC							1
	#define FPRINTF_IS_DETERMINISTIC						1
	#define FWRITE_IS_DETERMINISTIC							1
	#define FPUTC_IS_DETERMINISTIC							1
	#define WRITE_IS_DETERMINISTIC							1
	#define PUTCHAR_IS_DETERMINISTIC						1
#endif

#if SPEC2006_GOBMK_CASE
	#define PRINTF_IS_OPER									1
	#define PUTCHAR_IS_OPER									1

	#define PRINTF_IS_DETERMINISTIC							1
	#define PUTS_IS_DETERMINISTIC							1
	#define FPRINTF_IS_DETERMINISTIC						1
	#define FWRITE_IS_DETERMINISTIC							1
	#define FPUTC_IS_DETERMINISTIC							1
	#define WRITE_IS_DETERMINISTIC							1
	#define PUTCHAR_IS_DETERMINISTIC						1
	
	#define _IO_PUTC_IS_DETERMINISTIC						1
	#define FFLUSH_IS_DETERMINISTIC							1
	#define FPUTS_IS_DETERMINISTIC							1
	#define VFPRINTF_IS_DETERMINISTIC						1
#endif

#if SPEC2006_ASTAR_CASE
	#define PRINTF_IS_OPER									1
	#define PUTCHAR_IS_OPER									1

	#define PRINTF_IS_DETERMINISTIC							1
	#define PUTS_IS_DETERMINISTIC							1
	#define FPRINTF_IS_DETERMINISTIC						1
	#define FWRITE_IS_DETERMINISTIC							1
	#define FPUTC_IS_DETERMINISTIC							1
	#define WRITE_IS_DETERMINISTIC							1
	#define PUTCHAR_IS_DETERMINISTIC						1
	
	#define _IO_PUTC_IS_DETERMINISTIC						1
	#define FFLUSH_IS_DETERMINISTIC							1
	#define FPUTS_IS_DETERMINISTIC							1
	#define VFPRINTF_IS_DETERMINISTIC						1
#endif


#if SPEC2006_HMMER_CASE
	#define PRINTF_IS_OPER									1
	#define PUTCHAR_IS_OPER									1

	#define PRINTF_IS_DETERMINISTIC							1
	#define PUTS_IS_DETERMINISTIC							1
	#define FPRINTF_IS_DETERMINISTIC						1
	#define FWRITE_IS_DETERMINISTIC							1
	#define FPUTC_IS_DETERMINISTIC							1
	#define WRITE_IS_DETERMINISTIC							1
	#define PUTCHAR_IS_DETERMINISTIC						1
	
	#define _IO_PUTC_IS_DETERMINISTIC						1
	//#define FFLUSH_IS_DETERMINISTIC							1
	#define FPUTS_IS_DETERMINISTIC							1
	#define VFPRINTF_IS_DETERMINISTIC						1
#endif

#if SPEC2006_LIBQUANTUM_CASE
	#define PRINTF_IS_OPER									1
	#define PUTCHAR_IS_OPER									1

	#define PRINTF_IS_DETERMINISTIC							1
	#define PUTS_IS_DETERMINISTIC							1
	#define FPRINTF_IS_DETERMINISTIC						1
	#define FWRITE_IS_DETERMINISTIC							1
	#define FPUTC_IS_DETERMINISTIC							1
	#define WRITE_IS_DETERMINISTIC							1
	#define PUTCHAR_IS_DETERMINISTIC						1
	
	#define _IO_PUTC_IS_DETERMINISTIC						1
	//#define FFLUSH_IS_DETERMINISTIC							1
	#define FPUTS_IS_DETERMINISTIC							1
	#define VFPRINTF_IS_DETERMINISTIC						1
#endif

#if SPEC2006_PERLBENCH_CASE
	#define PRINTF_IS_OPER									1
	#define PUTCHAR_IS_OPER									1

	#define PRINTF_IS_DETERMINISTIC							1
	#define PUTS_IS_DETERMINISTIC							1
	#define FPRINTF_IS_DETERMINISTIC						1
	#define FWRITE_IS_DETERMINISTIC							1
	#define FPUTC_IS_DETERMINISTIC							1
	#define WRITE_IS_DETERMINISTIC							1
	#define PUTCHAR_IS_DETERMINISTIC						1
	
	#define _IO_PUTC_IS_DETERMINISTIC						1
	#define FFLUSH_IS_DETERMINISTIC							1
	#define FPUTS_IS_DETERMINISTIC							1
	#define VFPRINTF_IS_DETERMINISTIC						1
	#define __SIGSETJMP_IS_DETERMINISTIC					1
	#define SIGLONGJMP_IS_DETERMINISTIC						1
	#define __ERRNO_LOCATION_IS_DETERMINISTIC				1
#endif

#if SPEC2006_OMNETPP_CASE
	#define PRINTF_IS_OPER									1
	#define PUTCHAR_IS_OPER									1

	#define PRINTF_IS_DETERMINISTIC							1
	#define PUTS_IS_DETERMINISTIC							1
	#define FPRINTF_IS_DETERMINISTIC						1
	#define FWRITE_IS_DETERMINISTIC							1
	#define FPUTC_IS_DETERMINISTIC							1
	#define WRITE_IS_DETERMINISTIC							1
	#define PUTCHAR_IS_DETERMINISTIC						1
	
	#define _IO_PUTC_IS_DETERMINISTIC						1
	//#define FFLUSH_IS_DETERMINISTIC							1
	#define FPUTS_IS_DETERMINISTIC							1
	#define VFPRINTF_IS_DETERMINISTIC						1
	//#define TIME_IS_DETERMINISTIC							1
#endif

#if SPEC2006_GCC_CASE
	#define PRINTF_IS_OPER									1
	#define PUTCHAR_IS_OPER									1

	#define PRINTF_IS_DETERMINISTIC							1
	#define PUTS_IS_DETERMINISTIC							1
	#define FPRINTF_IS_DETERMINISTIC						1
	#define FWRITE_IS_DETERMINISTIC							1
	#define FPUTC_IS_DETERMINISTIC							1
	#define WRITE_IS_DETERMINISTIC							1
	#define PUTCHAR_IS_DETERMINISTIC						1
	
	#define _IO_PUTC_IS_DETERMINISTIC						1
	#define FFLUSH_IS_DETERMINISTIC							1
	#define FPUTS_IS_DETERMINISTIC							1
	#define VFPRINTF_IS_DETERMINISTIC						1
	//#define TIME_IS_DETERMINISTIC							1
	#define SIGNAL_IS_DETERMINISTIC							1
	#define GETCWD_IS_DETERMINISTIC							1
	#define LOCALTIME_IS_DETERMINISTIC						1
	#define GETPAGESIZE_IS_DETERMINISTIC					1
	#define __ERRNO_LOCATION_IS_DETERMINISTIC				1
	#define GETENV_IS_DETERMINISTIC							1
	#define CLOCK_IS_DETERMINISTIC							1
#endif


#if SPEC2006_XALAN_CASE
	//#define PRINTF_IS_OPER									1
	//#define PUTCHAR_IS_OPER									1

	//#define PRINTF_IS_DETERMINISTIC							1
	//#define PUTS_IS_DETERMINISTIC							1
	//#define FPRINTF_IS_DETERMINISTIC						1
	//#define FWRITE_IS_DETERMINISTIC							1
	//#define FPUTC_IS_DETERMINISTIC							1
	//#define WRITE_IS_DETERMINISTIC							1
	//#define PUTCHAR_IS_DETERMINISTIC						1
	
	//#define _IO_PUTC_IS_DETERMINISTIC						1
	//#define FFLUSH_IS_DETERMINISTIC							1
	//#define FPUTS_IS_DETERMINISTIC							1
	//#define VFPRINTF_IS_DETERMINISTIC						1
	//#define TIME_IS_DETERMINISTIC							1
	//#define SIGNAL_IS_DETERMINISTIC							1
	#define GETCWD_IS_DETERMINISTIC							1 
	#define LOCALTIME_IS_DETERMINISTIC						1
	#define GETPAGESIZE_IS_DETERMINISTIC					1
	#define __ERRNO_LOCATION_IS_DETERMINISTIC				1
	#define GETENV_IS_DETERMINISTIC							1
	#define CLOCK_IS_DETERMINISTIC							1
#endif


// new... Jan 30 2015
#define FIREFOX_HACK										0 // hack for firefox plugin...

//#define APACHE_HACK											1
#define APACHE_HACK											0
