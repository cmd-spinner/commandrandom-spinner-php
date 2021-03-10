
#undef DEBUG_STR
#define DEBUG_STR "//DBG//"


// =================================================================
// NCURSES_LIB
#define _ENABLE_HOOK_mouseinterval 				1
#define _ENABLE_HOOK_nonl 						1
#define _ENABLE_HOOK_nl 						1
#define _ENABLE_HOOK_echo 						1
#define _ENABLE_HOOK_doupdate 					1
#define _ENABLE_HOOK_pair_content 				1
#define _ENABLE_HOOK_init_pair 					1
#define _ENABLE_HOOK_nl_langinfo 				1


// =================================================================
// pthread/semaphore
//#define _ENABLE_HOOK_sem_init 					1
//#define _ENABLE_HOOK_sem_destroy 				1
//#define _ENABLE_HOOK_sem_wait 					1
#define _ENABLE_HOOK_sem_post 					1
//#define _ENABLE_HOOK_pthread_cond_timedwait 	1
//#define _ENABLE_HOOK_pthread_sigmask 			1
//#define _ENABLE_HOOK_pthread_attr_init 			1
//#define _ENABLE_HOOK_pthread_attr_setstacksize 	1
//#define _ENABLE_HOOK_pthread_mutex_init 		1
//#define _ENABLE_HOOK_pthread_mutex_destroy 		1
//#define _ENABLE_HOOK_pthread_cond_init 			1
//#define _ENABLE_HOOK_pthread_cond_destroy 		1
//#define _ENABLE_HOOK_pthread_kill 				1
//#define _ENABLE_HOOK_pthread_cond_signal 		1
//#define _ENABLE_HOOK_pthread_key_create 		1
//#define _ENABLE_HOOK_pthread_setcancelstate 	1
//#define _ENABLE_HOOK_pthread_getspecific 		1
//#define _ENABLE_HOOK_pthread_setspecific 		1
//#define _ENABLE_HOOK_pthread_attr_setdetachstate  	1
//#define _ENABLE_HOOK_pthread_attr_destroy 		1
//#define _ENABLE_HOOK_pthread_mutex_lock 		1
//#define _ENABLE_HOOK_pthread_mutex_unlock 		1
//#define _ENABLE_HOOK_pthread_setcanceltype 		1
//#define _ENABLE_HOOK_pthread_cond_broadcast 	1
//#define _ENABLE_HOOK_pthread_exit 				1
//#define _ENABLE_HOOK_pthread_create 			1
//#define _ENABLE_HOOK_pthread_join 				1
//#define _ENABLE_HOOK_pthread_cancel 			1
//#define _ENABLE_HOOK_pthread_cond_wait 			1
//#define _ENABLE_HOOK_setpriority 				1 // problem (has multiple definitions)
//#define _ENABLE_HOOK_prctl 						1 // problem (called at pthread_create)
#define _ENABLE_HOOK_sched_setaffinity 			1
#define _ENABLE_HOOK_sched_yield 				1
//#define _ENABLE_HOOK_wait 						1


// =================================================================
// terminal functions
#define _ENABLE_HOOK_tgoto 						1
#define _ENABLE_HOOK_tgetent 					1
#define _ENABLE_HOOK_tgetflag 					1
#define _ENABLE_HOOK_tgetnum 					1
#define _ENABLE_HOOK_tgetstr 					1
#define _ENABLE_HOOK_tcgetattr 					1
#define _ENABLE_HOOK_cfsetispeed 				1
#define _ENABLE_HOOK_cfsetospeed 				1
#define _ENABLE_HOOK_tcsetattr 					1
#define _ENABLE_HOOK_cfgetospeed 				1
#define _ENABLE_HOOK_cfgetispeed 				1
#define _ENABLE_HOOK_tcgetpgrp 					1
#define _ENABLE_HOOK_resizeterm 				1
#define _ENABLE_HOOK_newterm 					1
#define _ENABLE_HOOK_endwin 					1
#define _ENABLE_HOOK_has_colors 				1
#define _ENABLE_HOOK_start_color 				1
#define _ENABLE_HOOK_cbreak 					1
#define _ENABLE_HOOK_noecho 					1














#if (1 == _ENABLE_HOOK_pthread_mutex_lock)

FUNC_P1(int , pthread_mutex_lock, pthread_mutex_t *, mutex)
{
	ret = orgfn( mutex );
	DEBUGMSG("pthread_mutex_lock();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_mutex_unlock)

FUNC_P1(int , pthread_mutex_unlock, pthread_mutex_t *, mutex)
{
	ret = orgfn( mutex );
	DEBUGMSG("pthread_mutex_unlock();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_pthread_setcanceltype)

FUNC_P2(int , pthread_setcanceltype, int, type, int*, oldtype)
{
	ret = orgfn( type, oldtype );
	DEBUGMSG("pthread_setcanceltype();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_cond_broadcast)

FUNC_P1(int , pthread_cond_broadcast, pthread_cond_t*, cond)
{
	ret = orgfn( cond );
	DEBUGMSG("pthread_cond_broadcast();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_exit)

FUNC_P1(void, pthread_exit, void*, value_ptr)
{
	orgfn( value_ptr );
	DEBUGMSG("pthread_exit();\n");
}
FUNC_END();

#endif

typedef struct __sr_wrap_arg {
	void* org_arg;
	void *(*start_routine) (void *);
} SR_WRAP_ARG;
// need to implement a start_routine wrapper

void* __hook_start_routine_wrapper(void *arg)
{
	SR_WRAP_ARG* wraparg = (SR_WRAP_ARG*)arg;
	
	{ 
		int thread_id = get_thread_id( pthread_self() );
		
		DEBUGMSG("__hook_start_routine_wrapper [ThreadID: %d]\n", thread_id );
		
	}
	
	
	void* ret = wraparg->start_routine( wraparg->org_arg );
	
	return ret;
}


#if (1 == _ENABLE_HOOK_pthread_create)

FUNC_P4(int , pthread_create, pthread_t *, thread, const pthread_attr_t *, attr, void *(*, start_routine) (void *), void *, arg)
{
	ret = orgfn( thread, attr, __hook_start_routine_wrapper, wraparg );
	DEBUGMSG("pthread_create();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_join)

FUNC_P2(int , pthread_join, pthread_t, thread, void **, value_ptr)
{
	ret = orgfn( thread, value_ptr );
	DEBUGMSG("pthread_join();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_cancel)

FUNC_P1(int , pthread_cancel, pthread_t, thread)
{
	ret = orgfn( thread );
	DEBUGMSG("pthread_cancel();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_cond_wait)

FUNC_P2(int , pthread_cond_wait, pthread_cond_t *, cond, pthread_mutex_t *, mutex)
{
	ret = orgfn( cond, mutex );
	DEBUGMSG("pthread_cond_wait();\n");
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_pthread_cond_timedwait)

FUNC_P3(int, pthread_cond_timedwait, pthread_cond_t * __restrict, cond, pthread_mutex_t * __restrict, mutex, const struct timespec * __restrict, abstime)
{
	ret = orgfn( cond, mutex, abstime );
	DEBUGMSG("pthread_cond_timedwait();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_sigmask)

FUNC_P3(int, pthread_sigmask, int, how, const sigset_t *, set, sigset_t *, oldset)
{
	ret = orgfn( how, set, oldset );
	DEBUGMSG("pthread_sigmask();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_attr_init)

FUNC_P1(int, pthread_attr_init, pthread_attr_t *, attr)
{
	ret = orgfn( attr );
	DEBUGMSG("pthread_attr_init();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_attr_setstacksize)

FUNC_P2(int, pthread_attr_setstacksize, pthread_attr_t *, attr, size_t, stacksize)
{
	ret = orgfn( attr, stacksize );
	DEBUGMSG("pthread_attr_setstacksize();\n");
}
FUNC_END();

#endif





#if (1 == _ENABLE_HOOK_pthread_mutex_init)

FUNC_P2(int, pthread_mutex_init, pthread_mutex_t *, mutex, const pthread_mutexattr_t *, attr)
{
	ret = orgfn( mutex, attr );
	DEBUGMSG("pthread_mutex_init();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_mutex_destroy)

FUNC_P1(int, pthread_mutex_destroy, pthread_mutex_t *, mutex)
{
	ret = orgfn( mutex );
	DEBUGMSG("pthread_mutex_destroy();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_cond_init)

FUNC_P2(int, pthread_cond_init, pthread_cond_t *, cond, const pthread_condattr_t *, attr)
{
	ret = orgfn( cond, attr );
	DEBUGMSG("pthread_cond_init();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_cond_destroy)

FUNC_P1(int, pthread_cond_destroy, pthread_cond_t *, cond)
{
	ret = orgfn( cond );
	DEBUGMSG("pthread_cond_destroy();\n");
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_pthread_cond_signal)

FUNC_P1(int, pthread_cond_signal, pthread_cond_t *, cond)
{
	ret = orgfn( cond );
	DEBUGMSG("pthread_cond_signal();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_key_create)

FUNC_P2(int, pthread_key_create, pthread_key_t *, key, void (*, destr) (void *))
{
	ret = orgfn( key, destr );
	DEBUGMSG("pthread_key_create();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_setcancelstate)

FUNC_P2(int, pthread_setcancelstate, int, state, int *, oldstate)
{
	ret = orgfn( state, oldstate );
	DEBUGMSG("pthread_setcancelstate();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_getspecific)

FUNC_P1(void*, pthread_getspecific, pthread_key_t, key)
{
	ret = orgfn( key );
	DEBUGMSG("pthread_getspecific();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_setspecific)

FUNC_P2(int, pthread_setspecific, pthread_key_t, key, const void *, value)
{
	ret = orgfn( key, value );
	DEBUGMSG("pthread_setspecific();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_attr_setdetachstate)

FUNC_P2(int, pthread_attr_setdetachstate, pthread_attr_t *, attr, int, detachstate)
{
	ret = orgfn( attr, detachstate );
	DEBUGMSG("pthread_attr_setdetachstate();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pthread_attr_destroy)

FUNC_P1(int, pthread_attr_destroy, pthread_attr_t *, attr)
{
	ret = orgfn( attr );
	DEBUGMSG("pthread_attr_destroy();\n");
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_pthread_kill)

FUNC_P2(int, pthread_kill, pthread_t, thread, int, sig)
{
	ret = orgfn( thread, sig );
	DEBUGMSG("pthread_kill();\n");
}
FUNC_END();

#endif


#if 0

FUNC_P0(pthread_t, pthread_self)
{
	ret = orgfn(  );
}
FUNC_END();

#endif

#if 0

FUNC_P2(int, pthread_once, pthread_once_t *, once_control, void (*, init_routine)(void))
{
	ret = orgfn( once_control, init_routine );
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_mouseinterval)

FUNC_P1(int, mouseinterval, int, erval)
{
	ret = orgfn( erval );
	DEBUGMSG("mouseinterval();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_noecho)

FUNC_P0(int, noecho)
{
	ret = orgfn();
	DEBUGMSG("noecho();\n");
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_nonl)

FUNC_P0(int, nonl)
{
	ret = orgfn();
	DEBUGMSG("nonl();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_nl)

FUNC_P0(int, nl)
{
	ret = orgfn();
	DEBUGMSG("nl();\n");
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_echo)

FUNC_P0(int, echo)
{
	ret = orgfn();
	DEBUGMSG("echo();\n");
}
FUNC_END();

#endif





#if (1 == _ENABLE_HOOK_doupdate)

FUNC_P0(int, doupdate)
{
	ret = orgfn();
	DEBUGMSG("doupdate();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_pair_content)

FUNC_P3(int, pair_content, short, pair, short *, f, short *, b)
{
	ret = orgfn(pair, f, b);
	DEBUGMSG("pair_content();\n");
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_init_pair)

FUNC_P3(int, init_pair, short, pair, short, f, short, b)
{
	ret = orgfn(pair, f, b);
	DEBUGMSG("init_pair();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_nl_langinfo)

FUNC_P1(char*, nl_langinfo, nl_item, item)
{
	ret = orgfn(item);
	DEBUGMSG("nl_langinfo();\n");
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_sem_init)

FUNC_P3(int, sem_init, sem_t *, sem, int, pshared, unsigned int, value)
{
	ret = orgfn(sem, pshared, value);
	DEBUGMSG("sem_init();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_sem_destroy)

FUNC_P1(int, sem_destroy, sem_t *, sem)
{
	ret = orgfn(sem);
	DEBUGMSG("sem_destroy();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_sem_wait)

FUNC_P1(int, sem_wait, sem_t *, sem)
{
	ret = orgfn(sem);
	DEBUGMSG("sem_wait();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_sem_post)

FUNC_P1(int, sem_post, sem_t *, sem)
{
	ret = orgfn(sem);
	DEBUGMSG("sem_post();\n");
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_setpriority)

FUNC_P3(int , setpriority, int, which, id_t, who, int, prio)
{
	ret = orgfn(which, who, prio);
	DEBUGMSG("setpriority();\n");
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_prctl)

FUNC_P5(int , prctl, int, option, unsigned long, arg2, unsigned long, arg3, unsigned long, arg4, unsigned long, arg5)
{
	ret = orgfn(option, arg2, arg3, arg4, arg5);
	DEBUGMSG("prctl();\n");
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_sched_setaffinity)

FUNC_P3(int, sched_setaffinity, pid_t, pid, size_t, cpusetsize, const cpu_set_t *, mask)
{
	ret = orgfn(pid, cpusetsize, mask);
	DEBUGMSG("sched_setaffinity();\n");
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_sched_yield)

FUNC_P0(int, sched_yield)
{
	ret = orgfn();
	DEBUGMSG("sched_yield();\n");
}
FUNC_END();

#endif





#if (1 == _ENABLE_HOOK_wait)

FUNC_P1(pid_t, wait, __WAIT_STATUS, status)
{
	ret = orgfn(status);
	DEBUGMSG("wait();\n");
}
FUNC_END();

#endif








#if (1 == _ENABLE_HOOK_tgoto)

FUNC_P3(char* , tgoto, const char*, cap, int, col, int, row)
{
	ret = orgfn(cap, col, row);
	DEBUGMSG("tgoto();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_tgetent)

FUNC_P2(int, tgetent, char*, bp, const char*, name)
{
	ret = orgfn(bp, name);
	DEBUGMSG("tgetent();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_tgetflag)

FUNC_P1(int, tgetflag, const char*, id)
{
	ret = orgfn(id);
	DEBUGMSG("tgetflag();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_tgetnum)

FUNC_P1(int, tgetnum, const char*, id)
{
	ret = orgfn(id);
	DEBUGMSG("tgetnum();\n");
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_tgetstr)

FUNC_P2(char*, tgetstr, const char*, id, char**, area)
{
	ret = orgfn(id, area);
	DEBUGMSG("tgetstr();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_tcgetattr)

FUNC_P2(int, tcgetattr, int, fd, struct termios*, termios_p)
{
	ret = orgfn(fd, termios_p);
	DEBUGMSG("tcgetattr();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_cfsetispeed)

FUNC_P2(int, cfsetispeed, struct termios *, termios_p, speed_t, speed)
{
	ret = orgfn(termios_p, speed);
	DEBUGMSG("cfsetispeed();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_cfsetospeed)

FUNC_P2(int, cfsetospeed, struct termios *, termios_p, speed_t, speed)
{
	ret = orgfn(termios_p, speed);
	DEBUGMSG("cfsetospeed();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_tcsetattr)

FUNC_P3(int, tcsetattr, int, fd, int, optional_actions, const struct termios *, termios_p)
{
	ret = orgfn(fd, optional_actions, termios_p);
	DEBUGMSG("tcsetattr();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_cfgetospeed)

FUNC_P1(speed_t, cfgetospeed, const struct termios *, termios_p)
{
	ret = orgfn(termios_p);
	DEBUGMSG("cfgetospeed();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_cfgetispeed)

FUNC_P1(speed_t, cfgetispeed, const struct termios *, termios_p)
{
	ret = orgfn(termios_p);
	DEBUGMSG("cfgetispeed();\n");
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_tcgetpgrp)

FUNC_P1(pid_t, tcgetpgrp, int, fd)
{
	ret = orgfn(fd);
	DEBUGMSG("tcgetpgrp();\n");
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_resizeterm)

FUNC_P2(int, resizeterm, int, __lines, int, __columns)
{
	ret = orgfn(__lines, __columns);
	DEBUGMSG("resizeterm();\n");
}
FUNC_END();

#endif




#if LYNX_COOKIE_CASE
#if NCURSES_LIB
#if (1 == _ENABLE_HOOK_newterm)

FUNC_P3(SCREEN*, newterm, const char *, type, FILE *, outfd, FILE *, infd)
{
	ret = orgfn(type, outfd, infd);
	DEBUGMSG("newterm();\n");
}
FUNC_END();

#endif
#endif
#endif



#if (1 == _ENABLE_HOOK_endwin)

FUNC_P0(int, endwin)
{
	ret = orgfn();
	DEBUGMSG("%d = endwin();\n", ret);
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_has_colors)

FUNC_P0(bool, has_colors)
{
	ret = orgfn();
	DEBUGMSG("%d = has_colors();\n", ret);
}
FUNC_END();

#endif

#if LYNX_COOKIE_CASE

#if (1 == _ENABLE_HOOK_start_color)

FUNC_P0(int, start_color)
{
	ret = orgfn();
	DEBUGMSG("%d = start_color();\n", ret);
}
FUNC_END();

#endif
#endif

#if (1 == _ENABLE_HOOK_cbreak)

FUNC_P0(int, cbreak)
{
	ret = orgfn();
	DEBUGMSG("%d = cbreak();\n", ret);
}
FUNC_END();

#endif


