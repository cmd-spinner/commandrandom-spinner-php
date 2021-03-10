
#ifndef __MODELS_HEADER_FILE__
#define __MODELS_HEADER_FILE__ 1 

const char* SYSCALL_NAMES[] = {
	"time", 
	"rand", "srand", "printf", "scanf", 
	"getpid", "write", "open", "close", "__INSTR__exit", 
	"read", "lseek", "sleep", "chdir", "fork",
	"signal", 
	"setpgrp", "socket", "bind", "listen", 
	"accept", 
	"__errno_location", 
	"rename", 
	"getenv", 
	"geteuid", "getpwnam", "getgrnam", 
	"open64", "dup2", "unlink", "gethostbyname", "mkdir", 
	"chown", "chmod", 
	"closedir", 
	"__fxstat64", "__xstat64", 
	"gettimeofday", "mktime", "sem_init", "sem_destroy", "sem_wait", 
	"sched_yield", "sem_post", "getsockname", "getsockopt", "setsockopt",
	"usleep", "gethostname", "fcntl", "connect", "random", 
	"openat64", "__fxstatat64", "epoll_create", "sigemptyset", "sigaction", 
	"setitimer", "getrlimit64", "accept4", 
	"localtime", 
	"posix_memalign",
	"localtime_r", "gmtime_r", "pread64", "pwrite64", "lseek64",
		
	"writev", "utimes", "ftruncate64", "mmap64", "munmap", 
	"opendir", 
	"readdir64", "glob64", "globfree64", "posix_fadvise64",
	"statfs64", "ioctl", "recv", "readv", "send", 
	"sendmsg", "recvmsg", "socketpair", "raise", "__DISABLED__abort",
	"kill", "getppid", "waitpid", "execve", "setsid",
	"umask", "sched_setaffinity", 
	"getpagesize",
	"sysconf", "srandom",
	"crypt_r", "sigaddset", "sigprocmask", "sigsuspend", "setpriority",
	"setrlimit64", "setgid", "initgroups", "setuid", "prctl",
	"uname", "epoll_wait", "epoll_ctl", "sendfile64", "getuid", 
	"shutdown", "realpath", "__lxstat64", 
	"fprintf", 
	"fputs", 
	"__sigsetjmp", 
	"fopen64", "fclose",  // 5
	"_IO_putc", "execl", "wait", "getpass", 
	"siglongjmp",    // 10
	"system", "_IO_getc", "fileno", 
	"fflush", 
	"fwrite",      // 15
	// ==================================
	"getaddrinfo", "getnameinfo", "popen", "pclose", "freeaddrinfo", // 20
	// ==================================
	"fputc",  // 21
	"fdopen", // 22
	"vfprintf",   // 23
	"feof", "fseeko64",      // 25
	// ==================================
	
	"ferror", "access", "tzset", "setlocale", "isatty",     // 30
	"getopt", "getlogin", "getpwuid", "getpgrp", "tcgetpgrp", // 35
	"mkstemp64", "fgets", 
	"getcwd",
	"dirname", "clearerr",      // 40
	"poll", "select", "fchmod", "tputs", "tgoto",              // 45
	"tgetent", "tgetflag", "tgetnum", "tgetstr", "tcgetattr",     // 50
	"cfsetispeed", "cfsetospeed", "tcsetattr", "cfgetispeed", "cfgetospeed", // 55
	"execlp", // 56
		
	"readlink", "remove", "ungetmouse", "getmouse", // 5
	"waddch", "wmove", "wgetch", "waddnstr", "define_key", // 10
	"perror", "scrollok", "fread", "clearok", "napms", // 15
	"putenv", "fgetc", "puts", 
	"putchar", "resizeterm", // 20	
	"wresize", "wborder", "wrefresh", "wattr_on", "wattr_off", // 25
	"newterm", "newpad", "keypad", "endwin", "has_colors",  // 30
	"start_color", "cbreak", "noecho", "mouseinterval", "mousemask", // 35
	"nonl", "nl", "echo", "wbkgd", "wtouchln", // 40
	"delwin", "newwin", "derwin", "wclear", "wclrtoeol", // 45
	"werase", "wnoutrefresh", "pnoutrefresh", "doupdate", "pair_content", // 50
	"wbkgdset", "ttyname", "ctermid", "freopen64", "mkdtemp",  // 55
	"truncate64", "rmdir", "ungetc", "init_pair", "nl_langinfo",  // 60
	"execv", "getgroups", "__h_errno_location", "inet_aton", "getgrgid", //65
	"__HOOK__getyx", "__HOOK__wattr_get", 
	
	"dcgettext", "alarm", "fopen", "pause", 
	"readdir", 
	"__xstat", "gmtime", "bindtextdomain", "textdomain", 
	
	"statfs", "pthread_mutex_lock", "pthread_mutex_unlock", "mvprintw", "printw",
	"pthread_setcanceltype", "pthread_cond_broadcast", "pthread_exit", 
	"pthread_create", "pthread_join", "pthread_cancel", "__pthread_unwind_next", "pthread_cond_wait", 
	"flushinp", "initscr", "nodelay", 
	
	"freopen", "pnm_readpaminit", "pnm_readpamrow", "fseek",
	"clock", 
	"dup", "ftell",
	
	"chroot", "getgid", "inflateEnd", "deflateEnd", "inflateInit_",
	"deflateInit_", "deflate", "inflate", "openlog", "closelog", 
	"syslog", "pipe", "gethostbyaddr",
	
	"__fxstat", "fscanf", "rewind",
	
	"fstat", "link", "stat", "execvp", "getegid", 
	"tmpfile", "ftruncate", "truncate", 
	
	// new ---
	"pthread_cond_timedwait", "pthread_sigmask", "pthread_attr_init", "pthread_attr_setstacksize", "stat64", 
	"sigwait", "pthread_mutex_init", "pthread_mutex_destroy", "pthread_cond_init", "pthread_cond_destroy", 
	"utime", "getloadavg", "fstat64", "pthread_kill", "pthread_cond_signal", 
	"__pthread_key_create", 
	
	"pthread_setcancelstate", "__isoc99_fscanf", "getservbyname", "__isoc99_scanf", 
	
	"pthread_getspecific", "pthread_setspecific", "pthread_attr_setdetachstate", "pthread_attr_destroy", "pthread_self",
	"pthread_once", "pthread_key_create", 
	"eventfd", "gethostbyname2",
	
	///////////////////////////////
	"malloc", "strncpy", "rand_r", 
	0
	};
	
/*
int main(int argc, char** argv)
{
	//const char* SYSCALL_NAMES[] = {
	int i;
	for( i = 0; ; i++ ){
		if( SYSCALL_NAMES[i] == 0 ) break;
		printf("#define SC_%s	%d\n", SYSCALL_NAMES[i], i);
	}
	printf("\n");
}
*/


#define SC_TIME		0	//time
#define SC_RAND		1	//rand
#define SC_SRAND		2	//srand
#define SC_PRINTF		3	//printf
#define SC_SCANF		4	//scanf
#define SC_GETPID		5	//getpid
#define SC_WRITE		6	//write
#define SC_OPEN		7	//open
#define SC_open		7	//open
#define SC_CLOSE		8	//close
#define SC___INSTR__EXIT		9	//__INSTR__exit
#define SC_READ		10	//read
#define SC_LSEEK		11	//lseek
#define SC_SLEEP		12	//sleep
#define SC_CHDIR		13	//chdir
#define SC_FORK		14	//fork
#define SC_SIGNAL		15	//signal
#define SC_SETPGRP		16	//setpgrp
#define SC_SOCKET		17	//socket
#define SC_BIND		18	//bind
#define SC_LISTEN		19	//listen
#define SC_ACCEPT		20	//accept
#define SC___ERRNO_LOCATION		21	//__errno_location
#define SC_RENAME		22	//rename
#define SC_GETENV		23	//getenv
#define SC_GETEUID		24	//geteuid
#define SC_GETPWNAM		25	//getpwnam
#define SC_GETGRNAM		26	//getgrnam
#define SC_OPEN64		27	//open64
#define SC_open64		27	//open64
#define SC_DUP2		28	//dup2
#define SC_UNLINK		29	//unlink
#define SC_GETHOSTBYNAME		30	//gethostbyname
#define SC_MKDIR		31	//mkdir
#define SC_CHOWN		32	//chown
#define SC_CHMOD		33	//chmod
#define SC_CLOSEDIR		34	//closedir
#define SC___FXSTAT64		35	//__fxstat64
#define SC___XSTAT64		36	//__xstat64
#define SC_GETTIMEOFDAY		37	//gettimeofday
#define SC_MKTIME		38	//mktime
#define SC_SEM_INIT		39	//sem_init
#define SC_SEM_DESTROY		40	//sem_destroy
#define SC_SEM_WAIT		41	//sem_wait
#define SC_SCHED_YIELD		42	//sched_yield
#define SC_SEM_POST		43	//sem_post
#define SC_GETSOCKNAME		44	//getsockname
#define SC_GETSOCKOPT		45	//getsockopt
#define SC_SETSOCKOPT		46	//setsockopt
#define SC_USLEEP		47	//usleep
#define SC_GETHOSTNAME		48	//gethostname
#define SC_FCNTL		49	//fcntl
#define SC_CONNECT		50	//connect
#define SC_RANDOM		51	//random
#define SC_OPENAT64		52	//openat64
#define SC___FXSTATAT64		53	//__fxstatat64
#define SC_EPOLL_CREATE		54	//epoll_create
#define SC_SIGEMPTYSET		55	//sigemptyset
#define SC_SIGACTION		56	//sigaction
#define SC_SETITIMER		57	//setitimer
#define SC_GETRLIMIT64		58	//getrlimit64
#define SC_ACCEPT4		59	//accept4
#define SC_LOCALTIME		60	//localtime
#define SC_POSIX_MEMALIGN		61	//posix_memalign
#define SC_LOCALTIME_R		62	//localtime_r
#define SC_GMTIME_R		63	//gmtime_r
#define SC_PREAD64		64	//pread64
#define SC_PWRITE64		65	//pwrite64
#define SC_LSEEK64		66	//lseek64
#define SC_WRITEV		67	//writev
#define SC_UTIMES		68	//utimes
#define SC_FTRUNCATE64		69	//ftruncate64
#define SC_MMAP64		70	//mmap64
#define SC_MUNMAP		71	//munmap
#define SC_OPENDIR		72	//opendir
#define SC_READDIR64		73	//readdir64
#define SC_GLOB64		74	//glob64
#define SC_GLOBFREE64		75	//globfree64
#define SC_POSIX_FADVISE64		76	//posix_fadvise64
#define SC_STATFS64		77	//statfs64
#define SC_IOCTL		78	//ioctl
#define SC_RECV		79	//recv
#define SC_READV		80	//readv
#define SC_SEND		81	//send
#define SC_SENDMSG		82	//sendmsg
#define SC_RECVMSG		83	//recvmsg
#define SC_SOCKETPAIR		84	//socketpair
#define SC_RAISE		85	//raise
#define SC___DISABLED__ABORT		86	//__DISABLED__abort
#define SC_KILL		87	//kill
#define SC_GETPPID		88	//getppid
#define SC_WAITPID		89	//waitpid
#define SC_EXECVE		90	//execve
#define SC_SETSID		91	//setsid
#define SC_UMASK		92	//umask
#define SC_SCHED_SETAFFINITY		93	//sched_setaffinity
#define SC_GETPAGESIZE		94	//getpagesize
#define SC_SYSCONF		95	//sysconf
#define SC_SRANDOM		96	//srandom
#define SC_CRYPT_R		97	//crypt_r
#define SC_SIGADDSET		98	//sigaddset
#define SC_SIGPROCMASK		99	//sigprocmask
#define SC_SIGSUSPEND		100	//sigsuspend
#define SC_SETPRIORITY		101	//setpriority
#define SC_SETRLIMIT64		102	//setrlimit64
#define SC_SETGID		103	//setgid
#define SC_INITGROUPS		104	//initgroups
#define SC_SETUID		105	//setuid
#define SC_PRCTL		106	//prctl
#define SC_UNAME		107	//uname
#define SC_EPOLL_WAIT		108	//epoll_wait
#define SC_EPOLL_CTL		109	//epoll_ctl
#define SC_SENDFILE64		110	//sendfile64
#define SC_GETUID		111	//getuid
#define SC_SHUTDOWN		112	//shutdown
#define SC_REALPATH		113	//realpath
#define SC___LXSTAT64		114	//__lxstat64
#define SC_FPRINTF		115	//fprintf
#define SC_FPUTS		116	//fputs
#define SC___SIGSETJMP		117	//__sigsetjmp
#define SC_FOPEN64		118	//fopen64
#define SC_fopen64		118	//fopen64	
#define SC_FCLOSE		119	//fclose
#define SC__IO_PUTC		120	//_IO_putc
#define SC_EXECL		121	//execl
#define SC_WAIT		122	//wait
#define SC_GETPASS		123	//getpass
#define SC_SIGLONGJMP		124	//siglongjmp
#define SC_SYSTEM		125	//system
#define SC__IO_GETC		126	//_IO_getc
#define SC_FILENO		127	//fileno
#define SC_FFLUSH		128	//fflush
#define SC_FWRITE		129	//fwrite
#define SC_GETADDRINFO		130	//getaddrinfo
#define SC_GETNAMEINFO		131	//getnameinfo
#define SC_POPEN		132	//popen
#define SC_PCLOSE		133	//pclose
#define SC_FREEADDRINFO		134	//freeaddrinfo
#define SC_FPUTC		135	//fputc
#define SC_FDOPEN		136	//fdopen
#define SC_VFPRINTF		137	//vfprintf
#define SC_FEOF		138	//feof
#define SC_FSEEKO64		139	//fseeko64
#define SC_FERROR		140	//ferror
#define SC_ACCESS		141	//access
#define SC_TZSET		142	//tzset
#define SC_SETLOCALE		143	//setlocale
#define SC_ISATTY		144	//isatty
#define SC_GETOPT		145	//getopt
#define SC_GETLOGIN		146	//getlogin
#define SC_GETPWUID		147	//getpwuid
#define SC_GETPGRP		148	//getpgrp
#define SC_TCGETPGRP		149	//tcgetpgrp
#define SC_MKSTEMP64		150	//mkstemp64
#define SC_FGETS		151	//fgets
#define SC_GETCWD		152	//getcwd
#define SC_DIRNAME		153	//dirname
#define SC_CLEARERR		154	//clearerr
#define SC_POLL		155	//poll
#define SC_SELECT		156	//select
#define SC_FCHMOD		157	//fchmod
#define SC_TPUTS		158	//tputs
#define SC_TGOTO		159	//tgoto
#define SC_TGETENT		160	//tgetent
#define SC_TGETFLAG		161	//tgetflag
#define SC_TGETNUM		162	//tgetnum
#define SC_TGETSTR		163	//tgetstr
#define SC_TCGETATTR		164	//tcgetattr
#define SC_CFSETISPEED		165	//cfsetispeed
#define SC_CFSETOSPEED		166	//cfsetospeed
#define SC_TCSETATTR		167	//tcsetattr
#define SC_CFGETISPEED		168	//cfgetispeed
#define SC_CFGETOSPEED		169	//cfgetospeed
#define SC_EXECLP		170	//execlp
#define SC_READLINK		171	//readlink
#define SC_REMOVE		172	//remove
#define SC_UNGETMOUSE		173	//ungetmouse
#define SC_GETMOUSE		174	//getmouse
#define SC_WADDCH		175	//waddch
#define SC_WMOVE		176	//wmove
#define SC_WGETCH		177	//wgetch
#define SC_WADDNSTR		178	//waddnstr
#define SC_DEFINE_KEY		179	//define_key
#define SC_PERROR		180	//perror
#define SC_SCROLLOK		181	//scrollok
#define SC_FREAD		182	//fread
#define SC_CLEAROK		183	//clearok
#define SC_NAPMS		184	//napms
#define SC_PUTENV		185	//putenv
#define SC_FGETC		186	//fgetc
#define SC_PUTS		187	//puts
#define SC_PUTCHAR		188	//putchar
#define SC_RESIZETERM		189	//resizeterm
#define SC_WRESIZE		190	//wresize
#define SC_WBORDER		191	//wborder
#define SC_WREFRESH		192	//wrefresh
#define SC_WATTR_ON		193	//wattr_on
#define SC_WATTR_OFF		194	//wattr_off
#define SC_NEWTERM		195	//newterm
#define SC_NEWPAD		196	//newpad
#define SC_KEYPAD		197	//keypad
#define SC_ENDWIN		198	//endwin
#define SC_HAS_COLORS		199	//has_colors
#define SC_START_COLOR		200	//start_color
#define SC_CBREAK		201	//cbreak
#define SC_NOECHO		202	//noecho
#define SC_MOUSEINTERVAL		203	//mouseinterval
#define SC_MOUSEMASK		204	//mousemask
#define SC_NONL		205	//nonl
#define SC_NL		206	//nl
#define SC_ECHO		207	//echo
#define SC_WBKGD		208	//wbkgd
#define SC_WTOUCHLN		209	//wtouchln
#define SC_DELWIN		210	//delwin
#define SC_NEWWIN		211	//newwin
#define SC_DERWIN		212	//derwin
#define SC_WCLEAR		213	//wclear
#define SC_WCLRTOEOL		214	//wclrtoeol
#define SC_WERASE		215	//werase
#define SC_WNOUTREFRESH		216	//wnoutrefresh
#define SC_PNOUTREFRESH		217	//pnoutrefresh
#define SC_DOUPDATE		218	//doupdate
#define SC_PAIR_CONTENT		219	//pair_content
#define SC_WBKGDSET		220	//wbkgdset
#define SC_TTYNAME		221	//ttyname
#define SC_CTERMID		222	//ctermid
#define SC_FREOPEN64		223	//freopen64
#define SC_freopen64		223	//freopen64
#define SC_MKDTEMP		224	//mkdtemp
#define SC_TRUNCATE64		225	//truncate64
#define SC_RMDIR		226	//rmdir
#define SC_UNGETC		227	//ungetc
#define SC_INIT_PAIR		228	//init_pair
#define SC_NL_LANGINFO		229	//nl_langinfo
#define SC_EXECV		230	//execv
#define SC_GETGROUPS		231	//getgroups
#define SC___H_ERRNO_LOCATION		232	//__h_errno_location
#define SC_INET_ATON		233	//inet_aton
#define SC_GETGRGID		234	//getgrgid
#define SC___HOOK__GETYX		235	//__HOOK__getyx
#define SC___HOOK__WATTR_GET		236	//__HOOK__wattr_get
#define SC_DCGETTEXT		237	//dcgettext
#define SC_ALARM		238	//alarm
#define SC_FOPEN		239	//fopen
#define SC_fopen		239	//fopen
#define SC_PAUSE		240	//pause
#define SC_READDIR		241	//readdir
#define SC___XSTAT		242	//__xstat
#define SC_GMTIME		243	//gmtime
#define SC_BINDTEXTDOMAIN		244	//bindtextdomain
#define SC_TEXTDOMAIN		245	//textdomain
#define SC_STATFS		246	//statfs
#define SC_PTHREAD_MUTEX_LOCK		247	//pthread_mutex_lock
#define SC_PTHREAD_MUTEX_UNLOCK		248	//pthread_mutex_unlock
#define SC_MVPRINTW		249	//mvprintw
#define SC_PRINTW		250	//printw
#define SC_PTHREAD_SETCANCELTYPE		251	//pthread_setcanceltype
#define SC_PTHREAD_COND_BROADCAST		252	//pthread_cond_broadcast
#define SC_PTHREAD_EXIT		253	//pthread_exit
#define SC_PTHREAD_CREATE		254	//pthread_create
#define SC_PTHREAD_JOIN		255	//pthread_join
#define SC_PTHREAD_CANCEL		256	//pthread_cancel
#define SC___PTHREAD_UNWIND_NEXT		257	//__pthread_unwind_next
#define SC_PTHREAD_COND_WAIT		258	//pthread_cond_wait
#define SC_FLUSHINP		259	//flushinp
#define SC_INITSCR		260	//initscr
#define SC_NODELAY		261	//nodelay
#define SC_FREOPEN		262	//freopen
#define SC_freopen		262	//freopen
#define SC_PNM_READPAMINIT		263	//pnm_readpaminit
#define SC_PNM_READPAMROW		264	//pnm_readpamrow
#define SC_FSEEK		265	//fseek
#define SC_CLOCK		266	//clock
#define SC_DUP		267	//dup
#define SC_FTELL		268	//ftell
#define SC_CHROOT		269	//chroot
#define SC_GETGID		270	//getgid
#define SC_INFLATEEND		271	//inflateEnd
#define SC_DEFLATEEND		272	//deflateEnd
#define SC_INFLATEINIT_		273	//inflateInit_
#define SC_DEFLATEINIT_		274	//deflateInit_
#define SC_DEFLATE		275	//deflate
#define SC_INFLATE		276	//inflate
#define SC_OPENLOG		277	//openlog
#define SC_CLOSELOG		278	//closelog
#define SC_SYSLOG		279	//syslog
#define SC_PIPE		280	//pipe
#define SC_GETHOSTBYADDR		281	//gethostbyaddr
#define SC___FXSTAT		282	//__fxstat
#define SC_FSCANF		283	//fscanf
#define SC_REWIND		284	//rewind
#define SC_FSTAT		285	//fstat
#define SC_LINK		286	//link
#define SC_STAT		287	//stat
#define SC_EXECVP		288	//execvp
#define SC_GETEGID		289	//getegid
#define SC_TMPFILE		290	//tmpfile
#define SC_FTRUNCATE		291	//ftruncate
#define SC_TRUNCATE		292	//truncate
#define SC_PTHREAD_COND_TIMEDWAIT		293	//pthread_cond_timedwait
#define SC_PTHREAD_SIGMASK		294	//pthread_sigmask
#define SC_PTHREAD_ATTR_INIT		295	//pthread_attr_init
#define SC_PTHREAD_ATTR_SETSTACKSIZE		296	//pthread_attr_setstacksize
#define SC_STAT64		297	//stat64
#define SC_SIGWAIT		298	//sigwait
#define SC_PTHREAD_MUTEX_INIT		299	//pthread_mutex_init
#define SC_PTHREAD_MUTEX_DESTROY		300	//pthread_mutex_destroy
#define SC_PTHREAD_COND_INIT		301	//pthread_cond_init
#define SC_PTHREAD_COND_DESTROY		302	//pthread_cond_destroy
#define SC_UTIME		303	//utime
#define SC_GETLOADAVG		304	//getloadavg
#define SC_FSTAT64		305	//fstat64
#define SC_PTHREAD_KILL		306	//pthread_kill
#define SC_PTHREAD_COND_SIGNAL		307	//pthread_cond_signal
#define SC___PTHREAD_KEY_CREATE		308	//__pthread_key_create
#define SC_PTHREAD_SETCANCELSTATE		309	//pthread_setcancelstate
#define SC___ISOC99_FSCANF		310	//__isoc99_fscanf
#define SC_GETSERVBYNAME		311	//getservbyname
#define SC___ISOC99_SCANF		312	//__isoc99_scanf
#define SC_PTHREAD_GETSPECIFIC		313	//pthread_getspecific
#define SC_PTHREAD_SETSPECIFIC		314	//pthread_setspecific
#define SC_PTHREAD_ATTR_SETDETACHSTATE		315	//pthread_attr_setdetachstate
#define SC_PTHREAD_ATTR_DESTROY		316	//pthread_attr_destroy
#define SC_PTHREAD_SELF		317	//pthread_self
#define SC_PTHREAD_ONCE		318	//pthread_once
#define SC_PTHREAD_KEY_CREATE		319	//pthread_key_create
#define SC_EVENTFD		320	//eventfd
#define SC_GETHOSTBYNAME2		321	//gethostbyname2
#define SC_MALLOC		322	//malloc
#define SC_STRNCPY		323	//strncpy
#define SC_RAND_R		324	//rand_r

#define SC_time	0
#define SC_rand	1
#define SC_srand	2
#define SC_printf	3
#define SC_scanf	4
#define SC_getpid	5
#define SC_write	6
#define SC_open	7
#define SC_close	8
#define SC___INSTR__exit	9
#define SC_read	10
#define SC_lseek	11
#define SC_sleep	12
#define SC_chdir	13
#define SC_fork	14
#define SC_signal	15
#define SC_setpgrp	16
#define SC_socket	17
#define SC_bind	18
#define SC_listen	19
#define SC_accept	20
#define SC___errno_location	21
#define SC_rename	22
#define SC_getenv	23
#define SC_geteuid	24
#define SC_getpwnam	25
#define SC_getgrnam	26
#define SC_open64	27
#define SC_dup2	28
#define SC_unlink	29
#define SC_gethostbyname	30
#define SC_mkdir	31
#define SC_chown	32
#define SC_chmod	33
#define SC_closedir	34
#define SC___fxstat64	35
#define SC___xstat64	36
#define SC_gettimeofday	37
#define SC_mktime	38
#define SC_sem_init	39
#define SC_sem_destroy	40
#define SC_sem_wait	41
#define SC_sched_yield	42
#define SC_sem_post	43
#define SC_getsockname	44
#define SC_getsockopt	45
#define SC_setsockopt	46
#define SC_usleep	47
#define SC_gethostname	48
#define SC_fcntl	49
#define SC_connect	50
#define SC_random	51
#define SC_openat64	52
#define SC___fxstatat64	53
#define SC_epoll_create	54
#define SC_sigemptyset	55
#define SC_sigaction	56
#define SC_setitimer	57
#define SC_getrlimit64	58
#define SC_accept4	59
#define SC_localtime	60
#define SC_posix_memalign	61
#define SC_localtime_r	62
#define SC_gmtime_r	63
#define SC_pread64	64
#define SC_pwrite64	65
#define SC_lseek64	66
#define SC_writev	67
#define SC_utimes	68
#define SC_ftruncate64	69
#define SC_mmap64	70
#define SC_munmap	71
#define SC_opendir	72
#define SC_readdir64	73
#define SC_glob64	74
#define SC_globfree64	75
#define SC_posix_fadvise64	76
#define SC_statfs64	77
#define SC_ioctl	78
#define SC_recv	79
#define SC_readv	80
#define SC_send	81
#define SC_sendmsg	82
#define SC_recvmsg	83
#define SC_socketpair	84
#define SC_raise	85
#define SC___DISABLED__abort	86
#define SC_kill	87
#define SC_getppid	88
#define SC_waitpid	89
#define SC_execve	90
#define SC_setsid	91
#define SC_umask	92
#define SC_sched_setaffinity	93
#define SC_getpagesize	94
#define SC_sysconf	95
#define SC_srandom	96
#define SC_crypt_r	97
#define SC_sigaddset	98
#define SC_sigprocmask	99
#define SC_sigsuspend	100
#define SC_setpriority	101
#define SC_setrlimit64	102
#define SC_setgid	103
#define SC_initgroups	104
#define SC_setuid	105
#define SC_prctl	106
#define SC_uname	107
#define SC_epoll_wait	108
#define SC_epoll_ctl	109
#define SC_sendfile64	110
#define SC_getuid	111
#define SC_shutdown	112
#define SC_realpath	113
#define SC___lxstat64	114
#define SC_fprintf	115
#define SC_fputs	116
#define SC___sigsetjmp	117
#define SC_fopen64	118
#define SC_fclose	119
#define SC__IO_putc	120
#define SC_execl	121
#define SC_wait	122
#define SC_getpass	123
#define SC_siglongjmp	124
#define SC_system	125
#define SC__IO_getc	126
#define SC_fileno	127
#define SC_fflush	128
#define SC_fwrite	129
#define SC_getaddrinfo	130
#define SC_getnameinfo	131
#define SC_popen	132
#define SC_pclose	133
#define SC_freeaddrinfo	134
#define SC_fputc	135
#define SC_fdopen	136
#define SC_vfprintf	137
#define SC_feof	138
#define SC_fseeko64	139
#define SC_ferror	140
#define SC_access	141
#define SC_tzset	142
#define SC_setlocale	143
#define SC_isatty	144
#define SC_getopt	145
#define SC_getlogin	146
#define SC_getpwuid	147
#define SC_getpgrp	148
#define SC_tcgetpgrp	149
#define SC_mkstemp64	150
#define SC_fgets	151
#define SC_getcwd	152
#define SC_dirname	153
#define SC_clearerr	154
#define SC_poll	155
#define SC_select	156
#define SC_fchmod	157
#define SC_tputs	158
#define SC_tgoto	159
#define SC_tgetent	160
#define SC_tgetflag	161
#define SC_tgetnum	162
#define SC_tgetstr	163
#define SC_tcgetattr	164
#define SC_cfsetispeed	165
#define SC_cfsetospeed	166
#define SC_tcsetattr	167
#define SC_cfgetispeed	168
#define SC_cfgetospeed	169
#define SC_execlp	170
#define SC_readlink	171
#define SC_remove	172
#define SC_ungetmouse	173
#define SC_getmouse	174
#define SC_waddch	175
#define SC_wmove	176
#define SC_wgetch	177
#define SC_waddnstr	178
#define SC_define_key	179
#define SC_perror	180
#define SC_scrollok	181
#define SC_fread	182
#define SC_clearok	183
#define SC_napms	184
#define SC_putenv	185
#define SC_fgetc	186
#define SC_puts	187
#define SC_putchar	188
#define SC_resizeterm	189
#define SC_wresize	190
#define SC_wborder	191
#define SC_wrefresh	192
#define SC_wattr_on	193
#define SC_wattr_off	194
#define SC_newterm	195
#define SC_newpad	196
#define SC_keypad	197
#define SC_endwin	198
#define SC_has_colors	199
#define SC_start_color	200
#define SC_cbreak	201
#define SC_noecho	202
#define SC_mouseinterval	203
#define SC_mousemask	204
#define SC_nonl	205
#define SC_nl	206
#define SC_echo	207
#define SC_wbkgd	208
#define SC_wtouchln	209
#define SC_delwin	210
#define SC_newwin	211
#define SC_derwin	212
#define SC_wclear	213
#define SC_wclrtoeol	214
#define SC_werase	215
#define SC_wnoutrefresh	216
#define SC_pnoutrefresh	217
#define SC_doupdate	218
#define SC_pair_content	219
#define SC_wbkgdset	220
#define SC_ttyname	221
#define SC_ctermid	222
#define SC_freopen64	223
#define SC_mkdtemp	224
#define SC_truncate64	225
#define SC_rmdir	226
#define SC_ungetc	227
#define SC_init_pair	228
#define SC_nl_langinfo	229
#define SC_execv	230
#define SC_getgroups	231
#define SC___h_errno_location	232
#define SC_inet_aton	233
#define SC_getgrgid	234
#define SC___HOOK__getyx	235
#define SC___HOOK__wattr_get	236
#define SC_dcgettext	237
#define SC_alarm	238
#define SC_fopen	239
#define SC_pause	240
#define SC_readdir	241
#define SC___xstat	242
#define SC_gmtime	243
#define SC_bindtextdomain	244
#define SC_textdomain	245
#define SC_statfs	246
#define SC_pthread_mutex_lock	247
#define SC_pthread_mutex_unlock	248
#define SC_mvprintw	249
#define SC_printw	250
#define SC_pthread_setcanceltype	251
#define SC_pthread_cond_broadcast	252
#define SC_pthread_exit	253
#define SC_pthread_create	254
#define SC_pthread_join	255
#define SC_pthread_cancel	256
#define SC___pthread_unwind_next	257
#define SC_pthread_cond_wait	258
#define SC_flushinp	259
#define SC_initscr	260
#define SC_nodelay	261
#define SC_freopen	262
#define SC_pnm_readpaminit	263
#define SC_pnm_readpamrow	264
#define SC_fseek	265
#define SC_clock	266
#define SC_dup	267
#define SC_ftell	268
#define SC_chroot	269
#define SC_getgid	270
#define SC_inflateEnd	271
#define SC_deflateEnd	272
#define SC_inflateInit_	273
#define SC_deflateInit_	274
#define SC_deflate	275
#define SC_inflate	276
#define SC_openlog	277
#define SC_closelog	278
#define SC_syslog	279
#define SC_pipe	280
#define SC_gethostbyaddr	281
#define SC___fxstat	282
#define SC_fscanf	283
#define SC_rewind	284
#define SC_fstat	285
#define SC_link	286
#define SC_stat	287
#define SC_execvp	288
#define SC_getegid	289
#define SC_tmpfile	290
#define SC_ftruncate	291
#define SC_truncate	292
#define SC_pthread_cond_timedwait	293
#define SC_pthread_sigmask	294
#define SC_pthread_attr_init	295
#define SC_pthread_attr_setstacksize	296
#define SC_stat64	297
#define SC_sigwait	298
#define SC_pthread_mutex_init	299
#define SC_pthread_mutex_destroy	300
#define SC_pthread_cond_init	301
#define SC_pthread_cond_destroy	302
#define SC_utime	303
#define SC_getloadavg	304
#define SC_fstat64	305
#define SC_pthread_kill	306
#define SC_pthread_cond_signal	307
#define SC___pthread_key_create	308
#define SC_pthread_setcancelstate	309
#define SC___isoc99_fscanf	310
#define SC_getservbyname	311
#define SC___isoc99_scanf	312
#define SC_pthread_getspecific	313
#define SC_pthread_setspecific	314
#define SC_pthread_attr_setdetachstate	315
#define SC_pthread_attr_destroy	316
#define SC_pthread_self	317
#define SC_pthread_once	318
#define SC_pthread_key_create	319
#define SC_eventfd	320
#define SC_gethostbyname2	321
#define SC_malloc	322
#define SC_strncpy	323
#define SC_rand_r	324

//====================================
#define SC_SYNCTAIL 	324
#define SC_RETURNCHECK 	325
#define COUNT_OF_SC 	326
#define _VR_MINUS1 		327
#define _VR_MINUS2 		327
#define _VR_MINUS3 		327
#define _VR_MINUS4 		327
#define _VR_MINUS5 		327
	
	
#endif

	
	
	
	
	
	

