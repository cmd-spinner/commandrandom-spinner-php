
#undef DEBUG_STR
#define DEBUG_STR "//DBG//"


// =================================================================
// socket
#define _ENABLE_HOOK_getsockname 				0

#define _ENABLE_HOOK_setsockopt 				0 // DISABLED -- not much interesting
#define _ENABLE_HOOK_getsockopt 				0
#define _ENABLE_HOOK_gethostname 				1
#define _ENABLE_HOOK_gethostbyname 				1
#define _ENABLE_HOOK_gethostbyname2				1
#define _ENABLE_HOOK_gethostbyaddr 				1
#define __ENABLE_HOOK_bind 						1
#define _ENABLE_HOOK_select 					1                                              

/*

#define __ENABLE_HOOK_getnameinfo			1


#define __ENABLE_HOOK_listen 				1

// flush dns cache
#define _ENABLE_HOOK_flushinp 					1
*/







#if (1 == _ENABLE_HOOK_setsockopt)


FUNC_P5(int, setsockopt, int, sockfd, int, level, int, optname, const void*, optval, socklen_t, optlen)
{
	ret = orgfn( sockfd, level, optname, optval, optlen );
	
	//string rsrcname = getRsrcNameFromFd( sockfd );
	//if( rsrcname.length() ) {
		//DEBUGMSG(" [FDRSRC:_NETWORK_] %d = "FUNCNAME_STR"( %s, %d, %d, %s, %d );\n", ret, rsrcname.c_str(), level, optname, P2S(optval, optlen), optlen );
	//} else {
		//DEBUGMSG(" [FDRSRC:_NETWORK_] %d = "FUNCNAME_STR"( %d, %d, %d, %s, %d );\n", ret, sockfd, level, optname, P2S(optval, optlen), optlen );
	//}
	RSRC_VARS2(1, 3);
	rsrcname = GetFD(sockfd, fd_type);
	
	APPEND_INT( vtarg, level, "level", 0 );
	APPEND_INT( vtarg, optname, "optname", 0 );
	APPEND_BUF( vtarg, optval, optlen, "optval", 0 );
	
	APPEND_INT( vtret, ret, "ret", 0 );
		
	LogResource( GetCategoryFromFDType(fd_type, CATEGORY_NETWORK), rsrcname, SYSCALL_NUM, vtret, vtarg, true );
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_gethostbyname)

FUNC_P1(struct hostent *, gethostbyname, const char *, name)
{
	ret = orgfn( name );

	RSRC_VARS2(30, 1);
	rsrcname = string(name);
	
	hostent_to_vector( ret, vtret );
	
	LogResource( CATEGORY_NETWORK, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_gethostbyname2)

FUNC_P2(struct hostent *, gethostbyname2, const char *, name, int, af)
{
	ret = orgfn( name, af );

	//DEBUGMSG(DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
	
	RSRC_VARS2(30, 2);
	rsrcname = string(name);
	
	APPEND_INT( vtarg, af, "af", 0 );
	
	hostent_to_vector( ret, vtret );
		
	LogResource( CATEGORY_NETWORK, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_gethostbyaddr)

FUNC_P3(struct hostent *, gethostbyaddr, const void*, addr, socklen_t, len, int, type)
{
	ret = orgfn( addr, len, type );			

	//DEBUGMSG(DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
	
	RSRC_VARS2(30, 5);
	rsrcname = sockaddr_to_string( (struct sockaddr*)addr, len, true );
		
	APPEND_INT( vtarg, type, "type", 0 );
	hostent_to_vector( ret, vtret );
	
	//APPEND_STR( args, string(name) );
	LogResource( CATEGORY_NETWORK, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif





#if (__ENABLE_HOOK_getnameinfo == 1)

FUNC_P7(int, getnameinfo, const struct sockaddr*, sa, socklen_t, salen, char*, host, size_t, hostlen, char*, serv, size_t, servlen, int, flags)
{
	ret = orgfn( sa, salen, host, hostlen, serv, servlen, flags );
	
	RSRC_VARS2(3, 10);
	rsrcname = string("getnameinfo");
	
	APPEND_STR( vtarg, sockaddr_to_string( (const struct sockaddr*) sa, salen, true ).c_str(), "sa", 0 );
	APPEND_INT( vtarg, flags, "flags", 0 );
	
	APPEND_INT( vtret, ret, "ret", 0 );
	if( ret == 0 ) {
		APPEND_STR( vtret, host, "host", 0 );
		APPEND_STR( vtret, serv, "serv", 0 );
	}
	
	LogResource( CATEGORY_NETWORK, rsrcname,  SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_select)

FUNC_P5(int, select, int, nfds, fd_set *, readfds, fd_set*, writefds, fd_set *, exceptfds, struct timeval *, timeout)
{
	ret = orgfn( nfds, readfds, writefds, exceptfds, timeout );
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_flushinp)

FUNC_P0(int , flushinp)
{
	HOOK_FUNC_OPER_HEADER_NOPARAM3
}
FUNC_END();

#endif



#if (__ENABLE_HOOK_listen == 1)

FUNC_P2(int, listen, int, socket, int, backlog)
{
	HOOK_FUNC_OPER_HEADER3_ND
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_gethostname)

// Deterministic
FUNC_P2(int, gethostname, char*, name, size_t, len)
{
	//[FDRSRC:_NETWORK_]
	ret = orgfn( name, len );
	DEBUGMSG("%d = gethostname(%s,%u);\n", ret, name, len);
	//DEBUGMSG(" [FDRSRC:_NETWORK_]  %d = "FUNCNAME_STR"( %s, %d );\n", ret, FUNCPARAM_INVOKE );
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_getsockopt)

FUNC_P5(int, getsockopt, int, sockfd, int, level, int, optname, void*, optval, socklen_t *, optlen)
{
	ret = orgfn( sockfd, level, optname, optval, optlen );
	DEBUGMSG("%d = getsockopt(%s);\n", ret, GET_SOCKET(sockfd).c_str());
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_getsockname)

FUNC_P3(int, getsockname, int, sockfd, struct sockaddr *, addr, socklen_t *, addrlen)
{
	HOOK_FUNC_OPER_HEADER3
}
FUNC_END();

#endif