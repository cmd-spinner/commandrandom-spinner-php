#include <signal.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#undef DEBUG_STR
#define DEBUG_STR "//DBG//"


// =================================================================
// process / fork
#define _ENABLE_HOOK_execv 						1
#define _ENABLE_HOOK_execvp 					1
#define _ENABLE_HOOK_execl 						1
#define _ENABLE_HOOK_execlp 					1
#define _ENABLE_HOOK_execve 					1
#define _ENABLE_HOOK_system 					1

// =================================================================
// log
#define _ENABLE_HOOK_syslog 					1
#define _ENABLE_HOOK_openlog 					1
#define _ENABLE_HOOK_closelog 					1



// =================================================================
// ids/groups/policies --- getters
#define _ENABLE_HOOK_getppid 					0
#define _ENABLE_HOOK_waitpid 					0
#define _ENABLE_HOOK_getegid 					1
#define _ENABLE_HOOK_getgid 					1
#define _ENABLE_HOOK_initgroups 				1
#define _ENABLE_HOOK_getgroups 					1
#define _ENABLE_HOOK_getuid 					1

// =================================================================
// sleep
#define _ENABLE_HOOK_sleep 						1
//#define _ENABLE_HOOK_usleep 					1

// =================================================================
// signals
#define _ENABLE_HOOK_signal 					0
#define _ENABLE_HOOK_sigemptyset 				0
//#define _ENABLE_HOOK_ioctl 						1
#define _ENABLE_HOOK_raise 						1
#define _ENABLE_HOOK_kill 						1
#define _ENABLE_HOOK_sigaddset 					0
#define _ENABLE_HOOK_sigprocmask 				0
#define _ENABLE_HOOK_sigsuspend 				0
#define _ENABLE_HOOK_siglongjmp 				0
#define _ENABLE_HOOK___sigsetjmp 				0
#define _ENABLE_HOOK_sigwait 					1
#define _ENABLE_HOOK_alarm 						1
#define _ENABLE_HOOK_pause 						1



// =================================================================
// epoll
#define _ENABLE_HOOK_epoll_create 				0
#define _ENABLE_HOOK_epoll_wait 				0
#define _ENABLE_HOOK_epoll_ctl 					0
#define _ENABLE_HOOK_poll 						0


// =================================================================
// timer
#define _ENABLE_HOOK_setitimer 					1
#define _ENABLE_HOOK_getrlimit64 				0//1 disabled dec27



// =================================================================
// environment -- output
#define _ENABLE_HOOK_putenv 					1


// =================================================================
// internal settings -- getter
#define _ENABLE_HOOK_getpagesize 				0//1 disabled dec 27


// =================================================================
// encryption
#define _ENABLE_HOOK_crypt_r 					1



// =================================================================
// memory release
#define _ENABLE_HOOK_freeaddrinfo 				0// 1 dec 27


// =================================================================
// etc
#define _ENABLE_HOOK_clearerr 					1
//#define _ENABLE_HOOK_mktime 					1
#define _ENABLE_HOOK_umask 						1
//#define _ENABLE_HOOK_sysconf 					1 // problem // not really need to be traced

#define _ENABLE_HOOK_realpath 					0
#define _ENABLE_HOOK_access 					0
//       isatty - test whether a file descriptor refers to a terminal
#define _ENABLE_HOOK_isatty 					0// 1 dec 27
#define _ENABLE_HOOK_define_key 				1
#define _ENABLE_HOOK_napms 						1

// terminal
#define _ENABLE_HOOK_ttyname 					1
#define _ENABLE_HOOK_ctermid 					1

// convert time
#define _ENABLE_HOOK_gmtime 					0// 1 disabled dec 27

// screen
#define _ENABLE_HOOK_initscr 					1
#define _ENABLE_HOOK_getloadavg 				1


// language translation
#define _ENABLE_HOOK_dcgettext 					0 // dont need this // not really interesting because it is mostly deterministic
#define _ENABLE_HOOK_bindtextdomain 			0// 1 disabled dec 27
#define _ENABLE_HOOK_textdomain 				0// 1 disabled dec 27


#define _ENABLE_HOOK_ungetc 					0// 1 dec 27


//*/
#define FOLLOWING_ONE_FORK_ALWAYS	1

#if 0
extern "C" int close(int fd)
{
#if 0
	ENTER_USERFUNC;
	typedef int (*PFNORG)( int ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, "close");
	int ret;
	
	ret = orgfn( fd );
	
	LEAVE_USERFUNC;
	return ret;
#else
	return 0;
#endif
}
#endif


///*
extern "C" pid_t fork(void)
{
	ENTER_USERFUNC;
	typedef pid_t (*PFNORG)( void ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, "fork");
	pid_t ret;

	//show_full_call_stack(0,10);
#if FOLLOWING_ONE_FORK_ALWAYS	
	if( CALL_FROM_HOOK_SO ) {
		// printf("CALL_FROM_HOOK_SO: fork...\n");
		ret = orgfn();
	} else if( IS_FORKED() ) {
		// printf("IS_FORKED: fork...\n");
		//simplefu master_futex = __hook_fork_completed_futex_master;
		//simplefu slave_futex = __hook_fork_completed_futex_slave;
		
		ret = orgfn();
		if( ret == 0 ) { // child
			// DEBUGMSG("[forked twice] fork() - returns %d: (curpid = %d) - (NOT FOLLOWED) child\n", (unsigned int)ret, (unsigned int)getpid());
			
			//&__hook_util_sharedmem_ptr, &__hook_util_localmem_ptr
			//set_not_forked( __hook_util_sharedmem_ptr, __hook_util_localmem_ptr, &__hook_util_sharedmem_ptr, &__hook_util_localmem_ptr );
			//int nResult = OnSyscall(SC_SYNCFORK_INTERNAL, &si);
			//DEBUGMSG( " OnSyscall(SC_SYNCFORK_INTERNAL) = %s\n", getSyscallResultString(nResult)  );
			//simplefu_up(slave_futex);
			//simplefu_down(master_futex);
		} else { // parent
			// DEBUGMSG("[forked twice] fork() - returns %d: (curpid = %d) - parent\n", (unsigned int)ret, (unsigned int)getpid());
			
			//simplefu_up(master_futex);
			//simplefu_down(slave_futex);
			//int nResult = OnSyscall(SC_SYNCFORK_INTERNAL, &si);
			//DEBUGMSG( " OnSyscall(SC_SYNCFORK_INTERNAL) = %s\n", getSyscallResultString(nResult)  );
		} 
		
	} else {
		//printf("!IS_FORKED: fork...\n");
		//DEBUGMSG("fork() - (curpid = %d)\n", (unsigned int)getpid(),);
		//simplefu master_futex = __hook_fork_completed_futex_master;
		//simplefu slave_futex = __hook_fork_completed_futex_slave;
		
		//printf("!IS_FORKED: fork 1...\n");
		ret = orgfn();
		//printf("!IS_FORKED: fork 2...\n");
		if( ret == 0 ) { // child
			//DEBUGMSG("fork() - returns %d: (curpid = %d) - (NOT FOLLOWED)  [%d] child\n", (unsigned int)ret, (unsigned int)getpid(), READ_COUNTER());
			
			//set_not_forked( __hook_util_sharedmem_ptr, __hook_util_localmem_ptr, &__hook_util_sharedmem_ptr, &__hook_util_localmem_ptr );
			//int nResult = OnSyscall(SC_SYNCFORK_INTERNAL, &si);
			//DEBUGMSG( " OnSyscall(SC_SYNCFORK_INTERNAL) = %s\n", getSyscallResultString(nResult)  );
			//simplefu_up(slave_futex);
			//simplefu_down(master_futex);
		} else { // parent
			//DEBUGMSG("fork() - returns %d: (curpid = %d) - [%d] parent\n", (unsigned int)ret, (unsigned int)getpid(), READ_COUNTER());
			
			//set_not_forked( __hook_util_sharedmem_ptr, __hook_util_localmem_ptr, &__hook_util_sharedmem_ptr, &__hook_util_localmem_ptr );
			
			//simplefu_up(master_futex);
			//simplefu_down(slave_futex);
			//int nResult = OnSyscall(SC_SYNCFORK_INTERNAL, &si);
			//DEBUGMSG( " OnSyscall(SC_SYNCFORK_INTERNAL) = %s\n", getSyscallResultString(nResult)  );
		} 
		//printf("!IS_FORKED: fork 3...\n");
	}
#else
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn();
	} else if( IS_FORKED() ) {
		ret = orgfn();
	} else {
		// DEBUGMSG("fork() - (curpid = %d)\n", (unsigned int)getpid());
		simplefu master_futex = __hook_fork_completed_futex_master;
		simplefu slave_futex = __hook_fork_completed_futex_slave;
		
		ret = orgfn();
		if( ret == 0 ) { // child
			__hook_util_sharedmem = duplicate_shared_memory();
			DEBUGMSG("fork() - returns %d: (curpid = %d ) - child\n", (unsigned int)ret, (unsigned int)getpid());
			
			//int nResult = OnSyscall(SC_SYNCFORK_INTERNAL, &si);
			//DEBUGMSG( " OnSyscall(SC_SYNCFORK_INTERNAL) = %s\n", getSyscallResultString(nResult)  );
			simplefu_up(slave_futex);
			simplefu_down(master_futex);
		} else { // parent
			DEBUGMSG("fork() - returns %d: (curpid = %d ) - parent\n", (unsigned int)ret, (unsigned int)getpid());
			
			simplefu_up(master_futex);
			simplefu_down(slave_futex);
			//int nResult = OnSyscall(SC_SYNCFORK_INTERNAL, &si);
			//DEBUGMSG( " OnSyscall(SC_SYNCFORK_INTERNAL) = %s\n", getSyscallResultString(nResult)  );
		} 
	}	
#endif
	DEBUGMSG("fork();\n");
	LEAVE_USERFUNC;
	return ret;
}
//*/


extern "C" void exit(int status) 
{
	ENTER_USERFUNC;
	typedef void (*PFNORG)( int ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, "exit");
	if( CALL_FROM_HOOK_SO ) {
		int ex;
		DEBUGMSG("exit() begin\n");
		puts("Type a number to continue.");
		scanf("%d", &ex);
		orgfn( status );
	} else {
		int ex;
		DEBUGMSG("exit() begin\n");
		puts("Type a number to continue.");
		scanf("%d", &ex);
		
		LEAVE_USERFUNC;
		orgfn( status );	
	} 
	LEAVE_USERFUNC;
	 
	while(1) { ; } // an infinite loop to fix noreturn warning.
}


#if (1 == _ENABLE_HOOK_syslog)
extern "C" void syslog( int priority, const char *format, ... )
{
	ENTER_FUNC;
	const int SYSCALL_NUM = SC_syslog;
	
	typedef void (*PFNORG)( int priority, const char *format, ... ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM] );
	if( CALL_FROM_HOOK_SO ) {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		if( outbuf ) {
			orgfn( priority, outbuf );
			free( outbuf );
		}
		
	} else {
		int buflen = 0;
		char* outbuf = 0;
		SAFE_VSNPRINTF( format, outbuf, buflen );
		
		if( outbuf ) {
			orgfn( priority, outbuf );
			// DEBUGMSG("%s(%s)\n", SYSCALL_NAMES[SYSCALL_NUM], outbuf);
			DEBUGMSG("syslog(%d, %s);\n", priority, format);
		
			free( outbuf );
		}
	} 
	DEBUGMSG("syslog();\n");
	
	LEAVE_FUNC;
}
#endif

#if (1 == _ENABLE_HOOK_execl)

extern "C" int execl( const char *path, const char *arg, ... )
{
	ENTER_FUNC;
	int ret;
	const int SYSCALL_NUM = SC_execl;
	
	typedef int (*PFNORG)( const char *path, const char *arg, ... ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM] );

	
	if(!CALL_FROM_HOOK_SO ) {
		// DEBUGMSG("%s()\n", SYSCALL_NAMES[SYSCALL_NUM]);
	}
	
	// original implementation
	#define INITIAL_ARGV_MAX 1024
	size_t argv_max = INITIAL_ARGV_MAX;
	const char *initial_argv[INITIAL_ARGV_MAX];
	const char **argv = initial_argv;
	va_list args;
	
	argv[0] = arg;
	
	va_start (args, arg);
	unsigned int i = 0;
	while (argv[i++] != NULL)
	{
		if (i == argv_max)
		{
			argv_max *= 2;
			const char **nptr = (const char**)realloc (argv == initial_argv ? NULL : argv,
										 argv_max * sizeof (const char *));
			if (nptr == NULL)
			{
				if (argv != initial_argv)
					free (argv);
				return -1;
			}
			if (argv == initial_argv)
				/* We have to copy the already filled-in data ourselves.  */
				memcpy (nptr, argv, i * sizeof (const char *));
			
			argv = nptr;
		}
		
		argv[i] = va_arg (args, const char *);
	}
	va_end (args);
	
	ret = execve (path, (char *const *) argv, __environ);
	if (argv != initial_argv)
		free (argv);
		
	LEAVE_FUNC;
	DEBUGMSG("%d = execl(%s, %s);\n", path, arg);
	return ret;
}
#endif

#if (1 == _ENABLE_HOOK_execlp)

extern "C" int execlp( const char *path, const char *arg, ... )
{
	ENTER_FUNC;
	const int SYSCALL_NUM = SC_execlp;
	int ret;
	
	typedef int (*PFNORG)( const char *path, const char *arg, ... ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM] );

	// original implementation
	#define INITIAL_ARGV_MAX 1024
	size_t argv_max = INITIAL_ARGV_MAX;
	const char *initial_argv[INITIAL_ARGV_MAX];
	const char **argv = initial_argv;
	va_list args;
	
	argv[0] = arg;
	
	va_start (args, arg);
	unsigned int i = 0;
	while (argv[i++] != NULL)
	{
		if (i == argv_max)
		{
			argv_max *= 2;
			const char **nptr = (const char**)realloc (argv == initial_argv ? NULL : argv,
										 argv_max * sizeof (const char *));
			if (nptr == NULL)
			{
				if (argv != initial_argv)
					free (argv);
				return -1;
			}
			if (argv == initial_argv)
				/* We have to copy the already filled-in data ourselves.  */
				memcpy (nptr, argv, i * sizeof (const char *));
			
			argv = nptr;
		}
		
		argv[i] = va_arg (args, const char *);
	}
	va_end (args);
	
	ret = execvpe (path, (char *const *) argv, __environ);
	if (argv != initial_argv)
		free (argv);
	
	if( CALL_FROM_HOOK_SO ) {
		// DEBUGMSG("%s()\n", SYSCALL_NAMES[SYSCALL_NUM]);
	}
	
	DEBUGMSG("%d = execlp(%s, %s);\n", path, arg);
	LEAVE_FUNC;
	return ret;
}
#endif




#if (1 == _ENABLE_HOOK_sleep)

FUNC_P1(unsigned int , sleep, unsigned int, seconds)
{
	DEBUGMSG("sleep(%u);\n", seconds);
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_signal)
#if !SIGNAL_IS_DETERMINISTIC

FUNC_P2(sighandler_t, signal, int, signum, sighandler_t, handler)
{
	DEBUGMSG("signal(%d);\n", signum);
}
FUNC_END();

#endif
#endif






#if (1 == _ENABLE_HOOK_mktime)

FUNC_P1(time_t, mktime, struct tm *, tm)
{
	DEBUGMSG("mktime();\n");
}
FUNC_END();
#endif




#if (1 == _ENABLE_HOOK_usleep)

FUNC_P1(int, usleep, useconds_t, usec)
{
	DEBUGMSG("usleep(%u);\n", usec);
}
FUNC_END();


#endif


#if 0 // disable this because it is actually a variable argument function.


FUNC_P3(int, fcntl, int, fd, int, cmd, int, extra)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &fd, sizeof( fd ) );
		COLLECT_ARGS( &cmd, sizeof( cmd ) );
		COLLECT_ARGS( &extra, sizeof( extra ) );
		GET_CRC();

		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int offset = 0;
			PUSH_RETURN_VAL( offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int offset = 0;
					POP_RETURN_VAL( offset );
			
					DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
				}
				break;
				
				case SYSCALL_RESULT_INDEPENDENT_SLAVE:
				default:
				{
					ret = orgfn( FUNCPARAM_INVOKE );
				}
				break;
			}
		}
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();


#endif 



//int rand(void);
// TODO: REMOVE //DBG// TAG

FUNC_P0(int , rand)
{
	//static int random_ret = 1;
	ret = orgfn();
	//ret = random_ret++;
	DEBUGMSG("%d = rand();\n", ret );
}
FUNC_END();


//int rand_r(unsigned int *seedp);


FUNC_P1(int , rand_r, unsigned int *, seedp)
{
	static int random_seed = 1;
	if( seedp ) {
		*seedp = random_seed;
	}
	ret = orgfn( seedp );
	DEBUGMSG("%d = rand_r(%d);\n", ret, random_seed );
}
FUNC_END();




#if (1 == _ENABLE_HOOK_epoll_create)

FUNC_P1(int, epoll_create, int, size)
{
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_sigemptyset)

FUNC_P1(int, sigemptyset, sigset_t *, set)
{
}
FUNC_END();

#endif
//
// sigaction is not handled here.
// signals will be processed in both executions independently.
// Only the system invocations in the signal handling functions will be synced if it is possible.
// If there is a complex signal processing in an application, 
//  determinisic signal handler must be implemented.
//



#if (1 == _ENABLE_HOOK_setitimer)

FUNC_P3(int, setitimer, int, which, const struct itimerval *, new_value, struct itimerval *, old_value)
{
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_getrlimit64)

FUNC_P2(int, getrlimit64, int, resource, struct rlimit64 *, rlp)
{
}
FUNC_END();

#endif



//
// SC_POSIX_MEMALIGN : same as malloc, so we don't need to handle this, just run it naturally.
//

//
//
// localtime_r and localtime : it is determinisic, so just run it.
//
//


//
// mmap64 
//  it doesn't read anything from the env. just running it independently is ok
//


//
// munmap
//  it doesn't read anything from the env. just running it independently is ok
//

//
// glob64, globfree64 = deterministic, run it 
//


#if (1 == _ENABLE_HOOK_ioctl)

FUNC_P3(int , ioctl, int, d, long unsigned int, request, void*, extra)
{
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_raise)


FUNC_P1(int, raise, int, sig)
{
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_kill)

FUNC_P2(int, kill, pid_t, pid, int, sig)
{
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_getppid)

FUNC_P0(int, getppid)
{
	HOOK_FUNC_OPER_HEADER_NOPARAM3
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_waitpid)

FUNC_P3(pid_t, waitpid, pid_t, pid, int*, status, int, options)
{
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_execve)

FUNC_P3_INV(int, execve, const char*, filename, char *const, argv[], char* const, envp[], /**/filename, argv, envp)
{
	ret = orgfn( filename, argv, envp );
	string strargv = GetPrintableStringArray(argv), strenvp = GetPrintableStringArray(envp);
	DEBUGMSG( "%d = execve(%s);\n", ret, strargv.c_str() );
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_umask)

FUNC_P1(mode_t, umask, mode_t, mask)
{
}
FUNC_END();

#endif

//#if 0 ///////////////// debug /////////////////////////?



#if !GETPAGESIZE_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK_getpagesize)


FUNC_P0(int, getpagesize)
{
	HOOK_FUNC_OPER_HEADER_NOPARAM3
}
FUNC_END();

#endif
#endif


#if (1 == _ENABLE_HOOK_sysconf)

FUNC_P1(long, sysconf, int, name)
{
}
FUNC_END();

#endif





#if (1 == _ENABLE_HOOK_crypt_r)

FUNC_P3(char* , crypt_r, const char*, key, const char*, salt, struct crypt_data *, data)
{
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_sigaddset)

FUNC_P2(int , sigaddset, sigset_t *, set, int, signum)
{
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_sigprocmask)

FUNC_P3(int , sigprocmask, int, how, const sigset_t *, set, sigset_t *, oldset)
{
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_sigsuspend)

FUNC_P1(int , sigsuspend, const sigset_t *, mask)
{
}
FUNC_END();

#endif





#if (1 == _ENABLE_HOOK_initgroups)

FUNC_P2(int , initgroups, const char*, user, gid_t, group)
{
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_epoll_wait)

FUNC_P4(int , epoll_wait, int, epfd, struct epoll_event*, events, int, maxevents, int, timeout)
{
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_epoll_ctl)

FUNC_P4(int , epoll_ctl, int, epfd, int, op, int, fd, struct epoll_event *, event)
{
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_getuid)

FUNC_P0(uid_t, getuid)
{
		ret = orgfn(  );
		//DEBUGMSG(" [FDRSRC:_SYSUIDINFO_] %d = "SYSCALL_NAMES[SYSCALL_NUM]"();\n", ret );
		DEBUGMSG("%u = getuid();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_realpath)

FUNC_P2(char* , realpath, const char*, path, char*, resolved_path)
{
}
FUNC_END();

#endif


#if LYNX_CASE
#if !__SIGSETJMP_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK___sigsetjmp)

FUNC_P2(int , __sigsetjmp, jmp_buf, env, int, savemask)
{
}
FUNC_END();

#endif
#endif
#endif


#if !SIGLONGJMP_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK_siglongjmp)

FUNC_P2(void, siglongjmp, sigjmp_buf, env, int, val)
{
	HOOK_FUNC_OPER_HEADER_NORETURN3
	HOOK_FUNC_OPER_FOOTER_NORETURN
	
	while(1) { ; }
}
FUNC_END();

#endif
#endif

#if (1 == _ENABLE_HOOK_system)

FUNC_P1(int, system, const char*, command)
{
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_freeaddrinfo)

FUNC_P1(void, freeaddrinfo, struct addrinfo*, res)
{
	//HOOK_FUNC_OPER_HEADER_NORETURN
	ENTER_FUNC;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);
	if( CALL_FROM_HOOK_SO ) {
		orgfn( res );
	} else {
		// DEBUGMSG(" [FDRSRC:_NETWORK_] "SYSCALL_NAMES[SYSCALL_NUM]"();\n");
		DEBUGMSG("freeaddrinfo();\n");
		orgfn( res );
	HOOK_FUNC_OPER_FOOTER_NORETURN
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_access)

FUNC_P2(int, access, const char*, pathname, int, mode)
{
	//HOOK_FUNC_OPER_HEADER
	ENTER_FUNC;
	FUNCRETURN ret;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( pathname, mode );
	} else {
		ret = orgfn( pathname, mode );
		DEBUGMSG(DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
		DEBUGMSG("%d = access(%s,%d);\n", ret, pathname, mode);
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_isatty)

FUNC_P1(int, isatty, int, fd)
{
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_clearerr)

FUNC_VOID_P1(void, clearerr, FILE*, stream)
{
	orgfn( stream );
	DEBUGMSG("clearerr(%s);\n", GET_FILE(stream).c_str());
}
FUNC_VOID_END();

#endif


#if (1 == _ENABLE_HOOK_poll)

FUNC_P3(int, poll, struct pollfd *, fds, nfds_t, nfds, int, timeout)
{
}
FUNC_END();

#endif


#if 0 // ----debug----- disable ungetmouse, getmouse functions

FUNC_P1(int, ungetmouse, MEVENT*, event)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS_PTR( event, sizeof( MEVENT ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			int exist = 0;
			PUSH_RETURN_VAL( __offset );
			exist = event ? 1 : 0 ;
			PUSH_PARAM( &exist, sizeof( int ), __offset );
			if( exist ) PUSH_PARAM( event, sizeof( MEVENT ), __offset );
			
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					int exist = 0;
					POP_RETURN_VAL( __offset );
					POP_PARAM( &exist, sizeof( int ), __offset );
					if( exist ) POP_PARAM( event, sizeof( MEVENT ), __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();




FUNC_P1(int, getmouse, MEVENT*, event)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS_PTR( event, sizeof( MEVENT ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			int exist = 0;
			PUSH_RETURN_VAL( __offset );
			exist = event ? 1 : 0 ;
			PUSH_PARAM( &exist, sizeof( int ), __offset );
			if( exist ) PUSH_PARAM( event, sizeof( MEVENT ), __offset );
			
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					int exist = 0;
					POP_RETURN_VAL( __offset );
					exist = event ? 1 : 0 ;
					POP_PARAM( &exist, sizeof( int ), __offset );
					if( exist ) POP_PARAM( event, sizeof( MEVENT ), __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_define_key)

FUNC_P2(int, define_key, const char*, sequence, int, key_symbol)
{
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();

#endif



#if 0 // ----- debug ---- not defined structure WINDOW*...

FUNC_P2(int, scrollok, WINDOW*, win, bool, bf)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		COLLECT_ARGS( &bf, sizeof( bf ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();



FUNC_P2(int, clearok, WINDOW*, win, bool, bf)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		COLLECT_ARGS( &bf, sizeof( bf ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();


#endif 

#if (1 == _ENABLE_HOOK_napms)

FUNC_P1(int, napms, int, ms)
{
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_putenv)

FUNC_P1(int, putenv, char*, str)
{
	ret = orgfn( str );
	// DEBUGMSG(" [FDRSRC:_ENV_] %d = putenv(%s);\n", ret, str );
	DEBUGMSG("%d = putenv(%s);\n", ret, str);
}
FUNC_END();

#endif



#if 0 // ---------- disabled ------- due to WINDOW*

FUNC_P3(int, wresize, WINDOW*, win, int, __lines, int, __columns)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		COLLECT_ARGS( &__lines, sizeof( __lines ) );
		COLLECT_ARGS( &__columns, sizeof( __columns ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();



FUNC_P1(int, wrefresh, WINDOW*, win)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();



FUNC_P3(int, wattr_on, WINDOW*, win, attr_t, attrs, void*, opts)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		COLLECT_ARGS( &attrs, sizeof( attrs ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();



FUNC_P3(int, wattr_off, WINDOW*, win, attr_t, attrs, void*, opts)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		COLLECT_ARGS( &attrs, sizeof( attrs ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();


#endif 



#if 0 // ----- debug ---- disabled due to WINDOW*

FUNC_P2(WINDOW*, newpad, int, nlines, int, ncols)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		//#define getyx(win,y,x)		y = win->_cur_y, x = win->_cur_x
		//WINDOW* w;
		//printf("jicase: %d\n", w->_curx);
		//printf("jicase: %d\n", w->_cury);
		
		COLLECT_ARGS_INIT();
		//COLLECT_ARGS( &nlines, sizeof( nlines ) );
		//COLLECT_ARGS( &ncols, sizeof( ncols ) );
		GET_CRC();
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"(%d, %d, %d)::: \n", nlines, ncols, si.syscallArgHash );
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();



FUNC_P2(int, keypad, WINDOW *, win, bool, bf)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		COLLECT_ARGS( &bf, sizeof( bf ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();


#endif 



#if 0 // ----- debug ---- disabled due to mmask_t

FUNC_P2(mmask_t, mousemask, mmask_t, newmask, mmask_t *, oldmask)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &newmask, sizeof( newmask ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			int exist = 0;
			PUSH_RETURN_VAL( __offset );
			exist = oldmask ? 1 : 0;
			PUSH_PARAM( &exist, sizeof( int ), __offset );
			if( exist ) PUSH_PARAM( oldmask, sizeof( mmask_t ), __offset );
			
			
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					int exist = 0;
					POP_RETURN_VAL( __offset );
					POP_PARAM( &exist, sizeof( int ), __offset );
					if( exist && oldmask ) POP_PARAM( oldmask, sizeof( mmask_t ), __offset );
			
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();

#endif





#if 0 // ---- debug --- disabled due to WINDOW* 

FUNC_P2(int, wbkgd, WINDOW *, win, chtype, ch)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		COLLECT_ARGS( &ch, sizeof( ch ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();




FUNC_P4(int, wtouchln, WINDOW *, win, int, y, int, n, int, changed)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		COLLECT_ARGS( &y, sizeof( y ) );
		COLLECT_ARGS( &n, sizeof( n ) );
		COLLECT_ARGS( &changed, sizeof( changed ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();



FUNC_P1(int, delwin, WINDOW *, win)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();



FUNC_P4(WINDOW*, newwin, int, nlines, int, ncols, int, begin_y, int, begin_x)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &nlines, sizeof( nlines ) );
		COLLECT_ARGS( &ncols, sizeof( ncols ) );
		COLLECT_ARGS( &begin_y, sizeof( begin_y ) );
		COLLECT_ARGS( &begin_x, sizeof( begin_x ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();




FUNC_P5(WINDOW*, derwin, WINDOW *, orig, int, nlines, int, ncols, int, begin_y, int, begin_x)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &orig, sizeof( orig ) );
		COLLECT_ARGS( &nlines, sizeof( nlines ) );
		COLLECT_ARGS( &ncols, sizeof( ncols ) );
		COLLECT_ARGS( &begin_y, sizeof( begin_y ) );
		COLLECT_ARGS( &begin_x, sizeof( begin_x ) );
		GET_CRC();
		
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();



FUNC_P1(int, wclear, WINDOW *, win)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();



FUNC_P1(int, wclrtoeol, WINDOW *, win)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();



FUNC_P1(int, werase, WINDOW *, win)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();



FUNC_P1(int, wnoutrefresh, WINDOW *, win)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();



FUNC_P7(int, pnoutrefresh, WINDOW *, pad, int, pminrow, int, pmincol, int, sminrow, int, smincol, int, smaxrow, int, smaxcol)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &pad, sizeof( pad ) );
		COLLECT_ARGS( &pminrow, sizeof( pminrow ) );
		COLLECT_ARGS( &pmincol, sizeof( pmincol ) );
		COLLECT_ARGS( &sminrow, sizeof( sminrow ) );
		COLLECT_ARGS( &smincol, sizeof( smincol ) );
		COLLECT_ARGS( &smaxrow, sizeof( smaxrow ) );
		COLLECT_ARGS( &smaxcol, sizeof( smaxcol ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();


#endif

#if 0 // --------- debug --------- due to WINDOW*

FUNC_P2(void, wbkgdset, WINDOW *, win, chtype, ch)
{
	HOOK_FUNC_OPER_HEADER_NORETURN
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;
		
		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof( win ) );
		COLLECT_ARGS( &ch, sizeof( ch ) );
		GET_CRC();
		
		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			orgfn( FUNCPARAM_INVOKE );			
			DEBUGMSG( DEBUGTEXTPARAM, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
										
					DEBUGMSG( DEBUGTEXTPARAM, FUNCPARAM_INVOKE );
					//ret = orgfn( FUNCPARAM_INVOKE );
				}
				break;
				
				case SYSCALL_RESULT_INDEPENDENT_SLAVE:
				default:
				{
					// 1. exec
					orgfn( FUNCPARAM_INVOKE );
				}
				break;
			}
		}
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
	HOOK_FUNC_OPER_FOOTER_NORETURN
}
FUNC_END();


#endif

#if (1 == _ENABLE_HOOK_ttyname)

FUNC_P1(char*, ttyname, int, fd)
{
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_ctermid)

FUNC_P1(char*, ctermid, char*, s)
{
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_ungetc)

FUNC_P2(int, ungetc, int, c, FILE*, stream)
{
	//HOOK_FUNC_OPER_HEADER
	ENTER_FUNC;
	FUNCRETURN ret;
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);
	if( CALL_FROM_HOOK_SO ) {
		ret = orgfn( c, stream );
	} else {
		ret = orgfn( c, stream );
		
		string fname = getFileNameFromFILE( stream );
		if( fname.length() ) {
			size_t found = fname.find("/tmp/.X11-unix/");
			if( found > 10 ) {
				// DEBUGMSG(" [FDRSRC:%s] %d = "SYSCALL_NAMES[SYSCALL_NUM]"( %d );\n", fname.c_str(), ret, c );
				DEBUGMSG("%d = ungetc(%d,%s);\n", ret, c, GET_FILE(stream).c_str());
			}
		} else {
			DEBUGMSG(DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
		}
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_execv)

FUNC_P2_INV(int, execv, const char*, path, char* const, argv[], /**/path, argv)
{
	ret = orgfn( path, argv );
	string strargv = GetPrintableStringArray(argv);
	DEBUGMSG( "%d = execv(%s,%s)\n", ret, path, strargv.c_str() );
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_getgroups)

FUNC_P2_INV(int, getgroups, int, size, gid_t, list[], /**/ size, list)
{
	ret = orgfn( size, list );
	DEBUGMSG("%d = getgroups(%d)\n", ret, size);
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_dcgettext)

FUNC_P3(char* , dcgettext, const char *, domainname, const char *, msgid, int, category)
{
	ret = orgfn(domainname, msgid, category);
	DEBUGMSG("%s = dcgettext(%s,%s,%d)\n", ret, domainname, msgid, category);
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_alarm)

FUNC_P1(unsigned int , alarm, unsigned int, seconds)
{
	ret = orgfn(seconds);
	DEBUGMSG("%u = alarm(%u)\n", ret, seconds);
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pause)

FUNC_P0(int, pause)
{
	ret = orgfn();
	DEBUGMSG("%d = pause()\n", ret);
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_gmtime)

// gmtime is determinisic
FUNC_P1(struct tm*, gmtime, const time_t *, timep)
{
		ret = orgfn( timep );
		//DEBUGMSG(" struct tm* = "SYSCALL_NAMES[SYSCALL_NUM]"( time_t* )\n");
		DEBUGMSG("gmtime()\n");
}
FUNC_END();
#endif

#if (1 == _ENABLE_HOOK_bindtextdomain)

FUNC_P2(char* , bindtextdomain, const char*, domainname, const char*, dirname)
{
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_textdomain)

FUNC_P1(char* , textdomain, const char*, domainname)
{
}
FUNC_END();

#endif


#if NCURSES_LIB
#if (1 == _ENABLE_HOOK_initscr)

FUNC_P0(WINDOW*, initscr)
{
	HOOK_FUNC_OPER_HEADER_NOPARAM3
	
}
FUNC_END();

#endif
#endif

#if 0 // ----- debug ----- disable due to WINDOW*


FUNC_P2(int , nodelay, WINDOW*, win, bool, bf)
{
	HOOK_FUNC_OPER_HEADER
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() begin\n");
		
		SCINFO si, siMatched;

		COLLECT_ARGS_INIT();
		COLLECT_ARGS( &win, sizeof(win) );
		COLLECT_ARGS( &bf, sizeof(bf) );
		GET_CRC();

		if( IS_MASTER_EXEC() ) { 
			// 1. exec
			ret = orgfn( FUNCPARAM_INVOKE );				
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
			
			int __offset = 0;
			PUSH_RETURN_VAL( __offset );
			
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
		} else {
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched);
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );
			switch( nResult ) {
				case SYSCALL_RESULT_USE_SI:
				{
					int __offset = 0;
					POP_RETURN_VAL( __offset );
										
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
		
		DEBUGMSG(SYSCALL_NAMES[SYSCALL_NUM]"() return \n");
}
FUNC_END();


#endif 




#if (1 == _ENABLE_HOOK_getgid)


FUNC_P0(gid_t, getgid)
{
	ret = orgfn();
	DEBUGMSG("%u = getgid()\n", ret);
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_openlog)

FUNC_VOID_P3(void, openlog, const char *, ident, int, option, int, facility)
{
	orgfn(ident, option, facility);
	DEBUGMSG("openlog(%s,%d,%d)\n", ident, option, facility);
}
FUNC_VOID_END();

#endif

#if (1 == _ENABLE_HOOK_closelog)

FUNC_VOID_P0(void, closelog)
{
	orgfn();
	DEBUGMSG("closelog()\n");
}
FUNC_VOID_END();

#endif







#if (1 == _ENABLE_HOOK_execvp)

FUNC_P2_INV(int, execvp, const char *, file, char *const, argv[], /**/file, argv)
{
		ret = orgfn( file, argv );
		string strargv = GetPrintableStringArray(argv);
		DEBUGMSG( "%d = execvp(%s,%s);\n", ret, file, strargv.c_str() );
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_getegid)


FUNC_P0(gid_t, getegid)
{
	ret = orgfn(  );
	//DEBUGMSG(" [FDRSRC:_SYSUIDINFO_]  %d = "SYSCALL_NAMES[SYSCALL_NUM]"();\n", ret );
	DEBUGMSG("%u = getegid()\n", ret);
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_sigwait)

FUNC_P2(int, sigwait, const sigset_t *, set, int *, sig)
{
	ret = orgfn( set, sig );
	DEBUGMSG("%d = sigwait()\n", ret);
}
FUNC_END();

#endif

#if 0
#if (1 == _ENABLE_HOOK_getloadavg)

FUNC_P2_INV(int, getloadavg, double, loadavg, int, nelem, /*invoke*/loadavg, nelem)
{
	//HOOK_FUNC_OPER_HEADER2_FILLSC(FILL_SCINFO_RETVAR_RETDBLARY_INTSIZE)
		ret = orgfn( loadavg, nelem );
}
FUNC_END();

#endif
#endif


//#endif // #if 0 ///////////////// debug /////////////////////////?
