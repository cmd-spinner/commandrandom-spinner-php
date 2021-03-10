
#include "models.h"

#undef DEBUG_STR
// #define DEBUG_STR "//DBG//"
#define DEBUG_STR ""


// files
#define __ENABLE_HOOK_open 					1
#define __ENABLE_HOOK_open64 				1
#define __ENABLE_HOOK_fopen					1
#define __ENABLE_HOOK_fopen64				1	
#define __ENABLE_HOOK_freopen64 			1
#define __ENABLE_HOOK_freopen 				1

#define __ENABLE_HOOK_openat64				1
#define __ENABLE_HOOK_popen					1
#define __ENABLE_HOOK_fdopen				1
#define __ENABLE_HOOK_eventfd				1

// socket
#define _ENABLE_HOOK_connect 					1

#define _ENABLE_HOOK_socketpair 				1

#define __ENABLE_HOOK_accept 				1
#define __ENABLE_HOOK_accept4				1
#define __ENABLE_HOOK_socket 				1

// =================================================================
// duplicate
#define _ENABLE_HOOK_dup 						1
#define _ENABLE_HOOK_dup2 						1


#define _ENABLE_HOOK_pipe 						1

#if (__ENABLE_HOOK_open == 1)	

FUNC_P3(int, open, const char *, pathname, int, flags, mode_t, mode)
{
	ret = orgfn_open(pathname, flags, mode);
	if( ret != -1 ) {
		DEBUGMSG("open(%s);\n", GET_FILE(ret, pathname).c_str());
	}
}
FUNC_END();

#endif 

#if (__ENABLE_HOOK_open64 == 1)	

FUNC_P3(int, open64, const char *, pathname, int, flags, mode_t, mode)
{
	ret = orgfn( pathname, flags, mode );

	if( ret != -1 ) {
		DEBUGMSG("open64(%s);\n", GET_FILE(ret, pathname).c_str());
	}
}
FUNC_END();

#endif

#if (__ENABLE_HOOK_fopen64 == 1)

FUNC_P2( FILE*, fopen64, const char*, filename, const char*, type )
{
	ret = orgfn( filename, type );
	if(DEBUGMSG_GUARD) {
		DEBUGMSG("fopen64(%s);\n", GET_FILE(ret, filename).c_str());
	}
}
FUNC_END();

#endif





#if (__ENABLE_HOOK_freopen64 == 1)
FUNC_P3(FILE*, freopen64, const char *, filename, const char*, type, FILE*, stream)
{
	ret = orgfn( filename, type, stream );
	
	if( ret ) {
		DEBUGMSG("fopen64(%s);\n", GET_FILE(ret, filename).c_str());
	}
}
FUNC_END();

#endif

#if (__ENABLE_HOOK_freopen == 1)

FUNC_P3(FILE*, freopen, const char *, filename, const char*, type, FILE*, stream)
{
	ret = orgfn( filename, type, stream );
	
	if( ret ) {
		DEBUGMSG("freopen64(%s);\n", GET_FILE(ret, filename).c_str());
	}
}
FUNC_END();


#endif

#if (__ENABLE_HOOK_fopen == 1)

FUNC_P2(FILE*, fopen, const char *, path, const char*, mode)
{
	ret = orgfn( path, mode );
	if( DEBUGMSG_GUARD && ret ) {
		string filename = path; 
		DEBUGMSG("fopen(%s);\n", GET_FILE(ret, path).c_str());
	}
}
FUNC_END();

#endif

#if (__ENABLE_HOOK_openat64 == 1)

FUNC_P4(int, openat64, int, dirfd, const char*, pathname, int, flags, mode_t, mode)
{
	ret = orgfn( dirfd, pathname, flags, mode );
	if( ret != -1 ) {
		DEBUGMSG("openat64(%s);\n", GET_FILE(ret, pathname).c_str());
	}
}
FUNC_END();

#endif

#if (__ENABLE_HOOK_popen == 1)

FUNC_P2(FILE*, popen, const char *, command, const char*, type)
{
	ret = orgfn( command, type );
	if( ret != 0 ) {
		DEBUGMSG("popen(%s);\n", GET_FILE(ret).c_str());
		LogFILE( ret, command );
	}
}
FUNC_END();

#endif

#if (__ENABLE_HOOK_fdopen == 1)

FUNC_P2(FILE*, fdopen, int, fd, const char*, mode)
{
	ret = orgfn( fd, mode );
	if( ret ) {
		LINK_FILE(ret, fd);
		DEBUGMSG("fdopen(%s);\n", GET_FILE(fd).c_str());
	}
}
FUNC_END();


#endif

#if (__ENABLE_HOOK_eventfd == 1)

FUNC_P2(int, eventfd, unsigned int, initval, int, flags)
{
	ret = orgfn( initval, flags );
	if( ret != -1 ) {
		DEBUGMSG("eventfd();\n");
	}
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_connect)

FUNC_P3(int, connect, int, sockfd, const struct sockaddr*, addr, socklen_t, addrlen)
{
	// If the connection or binding succeeds, zero is returned.  On error,
	// -1 is returned, and errno is set appropriately.
	ret = orgfn( sockfd, addr, addrlen );
	
	if( addr && addrlen ) {
		
		// sockaddr_in
		string str_sockAddr = sockaddr_to_string( (struct sockaddr*)addr, addrlen, true );
		
		//str_sockAddr = sockaddr_in_to_string2( (struct sockaddr_in*)addr, addrlen );
		DEBUGMSG("connect(%s);\n", GET_SOCKET(sockfd).c_str());
	} else {
		DEBUGMSG("connect(%s);\n", GET_SOCKET(sockfd).c_str());
	}
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_socketpair)
FUNC_P4_INV(int, socketpair, int, domain, int, type, int, protocol, int, socket_vector[2], /*invokes*/domain, type, protocol, socket_vector)
{
	ret = orgfn( domain, type, protocol, socket_vector );
	
	for( int i = 0; i < 2; i++ ) {
		string strsocket = getSocketName( domain, type, protocol );
		SetFD( socket_vector[i], strsocket, FD_TYPE_NETWORK );
		// DEBUGMSG(DEBUG_STR" socket_vector[%d] = %s\n", i, strsocket.c_str());
		
		if( 0 ) {
			char filename[4096] = { 0, };
			string rec;
			if( RecoverFileName( socket_vector[i], filename, 4096 ) ) {
				rec = string("Recovered:") + string(filename);
				// DEBUGMSG("socketpair fd recovered: %s\n", rec.c_str());
			}
		}
	}
	// DEBUGMSG(DEBUG_STR"%d = socketpair(%d, %d, %d, {%d:%d});\n", ret, domain, type, protocol, socket_vector[0], socket_vector[1] );
}
FUNC_END();

#endif


#if (1 == __ENABLE_HOOK_bind)
FUNC_P3(int, bind, int, sockfd, const struct sockaddr *, addr, socklen_t, addrlen)
{
	//
	// On success, zero is returned.  On error, -1 is returned, and errno is 
	// set appropriately.
	// 
	ret = orgfn( sockfd, addr, addrlen );
	// sockfd <- addr 
	string rsrcname = sockaddr_to_string( (struct sockaddr*)addr, addrlen, true );
		
	if( ret == 0 ) {
		SetFD( sockfd, rsrcname, FD_TYPE_NETWORK );
	}
	DEBUGMSG("bind(%s);\n", GET_SOCKET(sock_fd).c_str());

}
FUNC_END();

#endif




#if (__ENABLE_HOOK_accept == 1)
FUNC_P3(int, accept, int, socket, struct sockaddr *, address, socklen_t *, address_len)
{
	ret = orgfn( socket, address, address_len );
	// On success, accept() returns a non-negative integer that is a descriptor for 
	// the accepted socket. On error, -1 is returned, and errno is set appropriately.

	if( ret != -1 ) {
		string rsrcname = sockaddr_to_string( (struct sockaddr*)address, *address_len, true );
		DEBUGMSG("accept(%s);\n", GET_SOCKET(socket).c_str());
	}
}
FUNC_END();

#endif


#if (__ENABLE_HOOK_accept4 == 1)
FUNC_P4(int, accept4, int, sockfd, struct sockaddr *, addr, socklen_t *, addrlen, int, flags)
{
	ret = orgfn( sockfd, addr, addrlen, flags );
	// On success, accept() returns a non-negative integer that is a descriptor for 
	// the accepted socket. On error, -1 is returned, and errno is set appropriately.
	
	// Its the same except the *flags*

	if( ret != -1 ) {
		string rsrcname = sockaddr_to_string( (struct sockaddr*)addr, *addrlen, true );
		DEBUGMSG("accept4(%s);\n", GET_SOCKET(sockfd).c_str());
	}
	// DEBUGMSG(DEBUG_STR" %d = accept4(%d);\n", ret, sockfd );
}
FUNC_END();

#endif


#if (__ENABLE_HOOK_socket == 1)
FUNC_P3(int, socket, int, domain, int, type, int, protocol)
{
	ret = orgfn( domain, type, protocol );
	//
	// make a signature of ...
	// 
	//if( RsrcNameExist( ret ) == 0 ) 
	if( ret != -1 ) {
		string rsrcname = getSocketName( domain, type, protocol );
		
		// ======================================================
		//setFdRsrcName( ret, string(strsocket) );
		//DEBUGMSG(" [FDRSRC:_NETWORK_] %s = "FUNCNAME_STR"( %d, %d, %d );\n", strsocket.c_str(), FUNCPARAM_INVOKE );
		
		SetFD( ret, rsrcname, FD_TYPE_NETWORK );
		DEBUGMSG("socket(%s);\n", GET_SOCKET(ret).c_str());
	}
}
FUNC_END();

#endif


// TODO: update fd
#if (1 == _ENABLE_HOOK_dup)
FUNC_P1(int, dup, int, oldfd)
{
	ret = orgfn( oldfd );
	UPDATE_FD(oldfd, ret);
	DEBUGMSG("dup(%s);\n", GET_FILE(ret).c_str());
	//DupFD( ret, oldfd );

}
FUNC_END();
#endif

#if (1 == _ENABLE_HOOK_dup2)

FUNC_P2(int, dup2, int, oldfd, int, newfd)
{
	ret = orgfn( oldfd, newfd );
	UPDATE_FD(oldfd, newfd);
	DEBUGMSG("dup2(%s);\n", GET_FILE(newfd).c_str());
}
FUNC_END();


#endif


#if (1 == _ENABLE_HOOK_pipe)

FUNC_P1_INV(int, pipe, int, pipefd[2], pipefd)
{
	ret = orgfn( pipefd );
	{
		string rsrcname;
		//rsrcname
		string caller = GetCallerRelativeAddr(2);
		char tmp[30];
		
		for( int i = 0; i < 2; i++ ) {
			sprintf(tmp, ":%d", pipefd[i]);
			rsrcname = string("pipe:") + caller + string(tmp);
			SetFD( pipefd[i], rsrcname, FD_TYPE_NETWORK );
		}
	}
	DEBUGMSG("pipe(%s,%s);\n", ret, GET_FILE(pipefd[0]).c_str(), GET_FILE(pipefd[1]).c_str());
}
FUNC_END();




#endif



/*
extern "C" int close(int fd)
{
	ENTER_USERFUNC;
	
	typedef int (*PFNORG)( int ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, "close");
	int ret;
	
	ret = orgfn( fd );
	//DEBUGMSG("close(%d);\n", fd);
	//ClearFD(fd);
	
	LEAVE_USERFUNC;
	return ret;
}
*/

/*
extern "C" int fcntl(int fd, int cmd, int arg )
{
	ENTER_USERFUNC;
	
	typedef int (*PFNORG)( int, int, int ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, "fcntl");
	int ret;
	
	ret = orgfn(fd, cmd, arg);
    
	if( cmd == 0 ) {
		DEBUGMSG("%d = fcntl(%d, %d, %d); [F_DUPFD = 0]\n", ret, fd, cmd, arg);
	}
	
	LEAVE_USERFUNC;
	return ret;
}
*/
	   
/*
extern "C" int dup3(int oldfd, int newfd, int flags)
{
	ENTER_USERFUNC;
	
	typedef int (*PFNORG)( int, int, int ); 
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, "dup3");
	int ret;
	
	ret = orgfn( oldfd, newfd, flags );
	DEBUGMSG("%d = dup3(%d, %d, %d);\n", ret, oldfd, newfd, flags);
	
	LEAVE_USERFUNC;
	return ret;
}
*/
