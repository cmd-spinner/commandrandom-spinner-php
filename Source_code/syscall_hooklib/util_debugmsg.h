
#ifndef __UTIL_DEBUGMSG_HEADER__ //////////////////////////
#define __UTIL_DEBUGMSG_HEADER__ 1


#define DO_NOT_PRINT_DBG_MSG		1


#define SAFE_VSNPRINTF(__fmt, __pstr, __return_len) {\
		va_list __safe_vnprintf_args;\
		va_start(__safe_vnprintf_args, __fmt);\
		if ((__return_len = vsnprintf(NULL, 0, __fmt, __safe_vnprintf_args)) >= 0) {\
			va_end(__safe_vnprintf_args);\
			va_start(__safe_vnprintf_args, __fmt);\
			__pstr = (char*) malloc(__return_len + 10);\
			if (__pstr)\
				vsnprintf(__pstr, __return_len + 1, __fmt, __safe_vnprintf_args);\
			else \
				__return_len = -1;\
		}\
		va_end(__safe_vnprintf_args);\
	}

#define USE_LOG_FILES 1

#if USE_LOG_FILES
FILE* 			__hook_util_LOGFILE;
int 			__hook_util_LOGFILE_FD;
#endif
FILE* 			__hook_util_LOGFILE_FILE = 0;


#if USE_LOG_FILES
void inline initFiles()
{
	char filename[1024];
	
	if( __hook_util_LOGFILE_FD == 0 ) {
		sprintf( filename, "%s/debugmsg.txt", LOGDIR );
		__hook_util_LOGFILE = orgfn_fopen( filename, "w+" );
		__hook_util_LOGFILE_FD = orgfn_fileno(__hook_util_LOGFILE);
	}
}
#else
#define initFiles() /**/
#endif

extern int inst_counter;
extern int line_counter;
char delimiter[] = ":";

int digits(int x) {
  x < 0 ? x = -x : 0;
  return x < 10 ? 1 :
      x < 100 ? 2 :
      x < 1000 ? 3 :
      x < 10000 ? 4 :
      x < 100000 ? 5 :
      x < 1000000 ? 6 :
      x < 10000000 ? 7 :
      x < 100000000 ? 8 :
      x < 1000000000 ? 9 : 0;
}

#if USE_LOG_FILES
int DEBUGMSG( const char *fmt, ... ) 
{
	ENTER_USERFUNC;		
#ifdef DO_NOT_PRINT_DBG_MSG
	if( strlen(fmt) > 7 ) {
		if( memcmp( fmt, "//DBG//", 7 ) == 0 ) {
			LEAVE_USERFUNC;
			return 0;
		}
	}
#endif

#if !PRINT_ONSYSCALL_RETURN
	if( strlen(fmt) > 11 ) {
		if( memcmp( fmt, " OnSyscall(", 11 ) == 0 ) {
			LEAVE_USERFUNC;
			return 0;
		}
	}
#endif

	int buflen = 0;
	char* outbuf = 0;
	SAFE_VSNPRINTF( fmt, outbuf, buflen );
	if ( buflen != -1 )
	{	
		initFiles();
		
		if( !IS_LOG_ENABLED() ) {
			int len_outbuf = strlen(outbuf);
			if( len_outbuf > 0 ) {
				if( outbuf[len_outbuf-1] == '\n' ) outbuf[len_outbuf-1] = 0;
				puts(outbuf);
			}
		} else {
			simplefu_mutex_lock( g_logmutex );
			char counter[10] = {0,};
			sprintf(counter, "%d", inst_counter);
			int counter_len = digits(inst_counter);
			orgfn_fwrite(counter, 1, counter_len, __hook_util_LOGFILE);
			orgfn_fwrite(delimiter, 1, sizeof(delimiter) - 1, __hook_util_LOGFILE);
			
			sprintf(counter, "%d", line_counter);
			counter_len = digits(line_counter);
			orgfn_fwrite(counter, 1, counter_len, __hook_util_LOGFILE);
			orgfn_fwrite(delimiter, 1, sizeof(delimiter) - 1, __hook_util_LOGFILE);

			orgfn_fwrite( outbuf, 1, strlen(outbuf), __hook_util_LOGFILE );
			fflush( __hook_util_LOGFILE );
			simplefu_mutex_unlock( g_logmutex );
		}
		//pthread_mutex_unlock(&g_debugmsg_lock);
		free( outbuf );
	}
	LEAVE_USERFUNC;
	return buflen;
}

int DEBUGMSG2( FILE* fpout, const char *fmt, ... ) 
{
	ENTER_USERFUNC;
	if( !IS_LOG_ENABLED() ) {
		LEAVE_USERFUNC;
		return 0;
	}
	
	int buflen = 0;
	char* outbuf = 0;
	SAFE_VSNPRINTF( fmt, outbuf, buflen );
	//printf("E: OUTBUF = %s\n", outbuf);
	if ( buflen != -1 )
	{
		while( 1 ) {
			bool found = false;
			char* pos = strchr(outbuf, '\r');
			if( pos ) {
				char* pos_next = pos;
				if( *(++pos_next) != '\0' ) {
					*pos = ' ';
					found = true;
				}
			}
			
			pos = strchr(outbuf, '\n');
			if( pos ) {
				char* pos_next = pos;
				if( *(++pos_next) != '\0' ) {
					*pos = ' ';
					found = true;
				}
			}
			
			if( found == false ) break;
		}
		
		if( !IS_LOG_ENABLED() ) {
			int len_outbuf = strlen(outbuf);
			if( len_outbuf > 0 ) {
				if( outbuf[len_outbuf-1] == '\n' ) outbuf[len_outbuf-1] = 0;
				puts(outbuf);
			}
		} else {
			simplefu_mutex_lock( g_logmutex );
			orgfn_fwrite( outbuf, 1, strlen(outbuf), fpout );
			fflush( fpout );
			simplefu_mutex_unlock( g_logmutex );
			//puts( szTotal );
		}
		//sem_post(g_psem);
		//pthread_mutex_unlock(&g_debugmsg_lock);
		
		//printf("E 5\n");
		free( outbuf );
	}
	//printf("F\n");
	LEAVE_USERFUNC;
	return buflen;
}
#else
#define DEBUGMSG(...) ((void)0)

#endif

//============================== new log files ================================
void inline LogBin_InitFiles()
{
	char filename[1024];
	
	if( __hook_util_LOGFILE_FILE == 0 ) {
		sprintf( filename, "%s/file.bin", LOGDIR );
		__hook_util_LOGFILE_FILE = orgfn_fopen( filename, "wb" );
	}
}


void LogBin_File( unsigned char* pbuf, int size ) 
{
	ENTER_USERFUNC;		

	if ( pbuf )
	{
		LogBin_InitFiles();
		
		simplefu_mutex_lock( g_logmutex );
		orgfn_fwrite( pbuf, 1, size, __hook_util_LOGFILE_FILE );
		simplefu_mutex_unlock( g_logmutex );
	}
	LEAVE_USERFUNC;
}


#endif // __UTIL_DEBUGMSG_HEADER__ ////////////////////////
