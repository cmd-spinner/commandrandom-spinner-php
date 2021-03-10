
#include "models.h"

#undef DEBUG_STR
#define DEBUG_STR "//DBG//"



// dir
#define __ENABLE_HOOK_getcwd					1

// 
#define __ENABLE_HOOK_geteuid					0//1 disabled dec 27
#define __ENABLE_HOOK_getgrnam 					1
#define __ENABLE_HOOK_getgrgid 					1
#define __ENABLE_HOOK_getpwnam					1
#define __ENABLE_HOOK_getpwuid					0


// process
#define __ENABLE_HOOK_getpid 					1

// time
#define __ENABLE_HOOK_time						1
#define __ENABLE_HOOK_gettimeofday 				1
#define _ENABLE_HOOK_clock 						1

// random
#define _ENABLE_HOOK_random 					1
#define _ENABLE_HOOK_srand 						1
#define _ENABLE_HOOK_srandom 					1



// ===================================================================================================================
// ===================================================================================================================
// ===================================================================================================================
// ===================================================================================================================
// ===================================================================================================================
// ===================================================================================================================
// ===================================================================================================================
// ===================================================================================================================


#if !TIME_IS_DETERMINISTIC
#if 1
#if (__ENABLE_HOOK_time == 1)

FUNC_P1(time_t, time, time_t *, t)
{
	ret = orgfn( t );
	
	/////////////////////////////////////////////////
	RSRC_VARS2(1, 1);
	
	string caller = GetCallerRelativeAddr(2);
	rsrcname = string("time:") + caller;
	
	// =====================
	APPEND_TIME(vtret, ret, "ret", 0);
	// =====================
	LogResource( CATEGORY_TIME, rsrcname, SYSCALL_NUM, vtret, vtarg );
	DEBUGMSG("time();\n");
	
}
FUNC_END();
#endif
#endif
#endif



#if (1 == __ENABLE_HOOK_gettimeofday)

FUNC_P2(int, gettimeofday, struct timeval *, tv, struct timezone *, tz)
{
		ret = orgfn( tv, tz );
		
		/////////////////////////////////////////////////
		RSRC_VARS2(1, 2);
		
		string caller = GetCallerRelativeAddr(2);
		rsrcname = string("gettimeofday:") + caller;
		DEBUGMSG("gettimeofday();\n");
}
FUNC_END();

#endif

#if !CLOCK_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK_clock)

FUNC_P0(clock_t , clock)
{
		ret = orgfn(  );
		
		RSRC_VARS2(1, 0)
		rsrcname = string("clock");
		DEBUGMSG("clock();\n");
}
FUNC_END();

#endif
#endif



#if (1 == _ENABLE_HOOK_random)

FUNC_P0(long int , random)
{
		ret = orgfn(  );
		
		RSRC_VARS2(1, 0)
		string caller = GetCallerRelativeAddr(2);
		rsrcname = string("/LIB/random:") + caller;

		DEBUGMSG("random();\n");
		
		//DEBUGMSG("%d = random()\n", ret);
		//DEBUGMSG(DEBUGTEXTPARAM, (int)ret );
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_srand)

FUNC_VOID_P1(void, srand, unsigned int, seed)
{
		orgfn( seed );
		
		RSRC_VARS2(0, 1)
		string caller = GetCallerRelativeAddr(2);
		rsrcname = string("/LIB/srand:") + caller;

		DEBUGMSG("srand();\n");
		
		APPEND_UINT(vtarg, seed, "seed", 0);
	HOOK_FUNC_OPER_FOOTER_NORETURN
}
FUNC_VOID_END();

#endif


#if (1 == _ENABLE_HOOK_srandom)

FUNC_VOID_P1(void, srandom, unsigned int, seed)
{
		orgfn( seed );
		RSRC_VARS2(0, 1)
		string caller = GetCallerRelativeAddr(2);
		rsrcname = string("/LIB/srandom:") + caller;
		DEBUGMSG("srandom();\n");
		
		APPEND_UINT(vtarg, seed, "seed", 0);
	HOOK_FUNC_OPER_FOOTER_NORETURN
}
FUNC_VOID_END();


#endif



#if (__ENABLE_HOOK_geteuid == 1)

FUNC_P0(uid_t, geteuid)
{
{ // [FDRSRC:_SYSUIDINFO_]
	//HOOK_FUNC_OPER_HEADER_NOPARAM
	ret = orgfn(  );

	DEBUGMSG("geteuid();\n");

	RSRC_VARS2(1,0)
	rsrcname = string("euid");
	// ================
	APPEND_UINT64(vtret, ret, "ret", 0);
	// ================
	LogResource( CATEGORY_SYSINFO, rsrcname, SYSCALL_NUM, vtret, vtarg );
	// ================
}
FUNC_END();

#endif


#if (__ENABLE_HOOK_getpwnam == 1)

FUNC_P1(struct passwd*, getpwnam, const char *, name)
{
	ret = orgfn( name );
	DEBUGMSG("getpwnam();\n");
}
FUNC_END();

#endif

#if (__ENABLE_HOOK_getgrnam == 1)

FUNC_P1(struct group*, getgrnam, const char *, name)
{
	ret = orgfn( name );
	DEBUGMSG("getgrnam();\n");
}
FUNC_END();

#endif

#if (__ENABLE_HOOK_getgrgid == 1)


FUNC_P1(struct group*, getgrgid, gid_t, gid)
{
	ret = orgfn( gid );
	DEBUGMSG("getgrgid();\n");
}
FUNC_END();

#endif


#if (1 == __ENABLE_HOOK_getpid)

FUNC_P0(pid_t, getpid)
{
	ret = orgfn(  );
	// pid_t ret_new = 0x0101;
	// DEBUGMSG("getpid = %X: returning %X\n", ret, ret_new);
	// ret = ret_new;

	DEBUGMSG("getpid();\n");
}
FUNC_END();

#endif


#if !GETCWD_IS_DETERMINISTIC
#if (__ENABLE_HOOK_getcwd == 1)

FUNC_P2(char* , getcwd, char*, buf, size_t, size)
{
	ret = orgfn( buf, size );
	DEBUGMSG("getcwd();\n");
}
FUNC_END();

#endif
#endif


#if (__ENABLE_HOOK_getpwuid == 1)

FUNC_P1(struct passwd*, getpwuid, uid_t, uid)
{
	ret = orgfn( uid );	
	DEBUGMSG("getpwuid();\n");
}
FUNC_END();

#endif
