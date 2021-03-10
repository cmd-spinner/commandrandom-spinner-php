
#undef DEBUG_STR
#define DEBUG_STR "//DBG//"


#define _ENABLE_HOOK_readlink 					1 // doesn't work


// =================================================================
// file operations (fine grained)
#define _ENABLE_HOOK_pipe 						1
#define _ENABLE_HOOK_posix_fadvise64 			0
#define _ENABLE_HOOK_ftruncate64 				1
#define _ENABLE_HOOK_truncate64 				1
#define _ENABLE_HOOK_ftruncate 					1
#define _ENABLE_HOOK_truncate 					1
#define _ENABLE_HOOK_ftell 						0//1 disabled dec27

#define _ENABLE_HOOK_lseek 						0
#define _ENABLE_HOOK_lseek64 					0//1 disabled dec27
#define _ENABLE_HOOK_fseek64 					0
#define _ENABLE_HOOK_fseek 						0

#define _ENABLE_HOOK_rewind 					0

#define _ENABLE_HOOK_fclose 					1
#define _ENABLE_HOOK_closedir 					0
#define _ENABLE_HOOK_feof 						1// 1 dec 27
#define _ENABLE_HOOK_ferror 					0
#define _ENABLE_HOOK_fileno 					1// 1 dec 27
#define _ENABLE_HOOK_fflush 					0// 1 dec 27
#define _ENABLE_HOOK_pclose 					0// 1 dec 27

// =================================================================
// directory
#define _ENABLE_HOOK_opendir 					1//1 disabled dec27
#define _ENABLE_HOOK_readdir64 					1//1 disabled dec27
#define _ENABLE_HOOK_readdir 					1//1 disabled dec27



// =================================================================
// file system -- read states
#define _ENABLE_HOOK___fxstat64 				1//1 dec 27
#define _ENABLE_HOOK___fxstat 					1
#define _ENABLE_HOOK___xstat64 					1//1 dec 27
#define _ENABLE_HOOK___xstat 					1
#define _ENABLE_HOOK___fxstatat64 				1//1 dec 27
#define _ENABLE_HOOK_statfs64 					1
#define _ENABLE_HOOK_statfs 					1
#define _ENABLE_HOOK_fstat64 					1
#define _ENABLE_HOOK_fstat 						1
#define _ENABLE_HOOK_stat64 					1
#define _ENABLE_HOOK_stat 						1
/*
*/

// =================================================================
// temp
#define _ENABLE_HOOK_tmpfile 					1
#define _ENABLE_HOOK_mkdtemp 					0
#define _ENABLE_HOOK_mkstemp 					0
#define _ENABLE_HOOK_mkstemp64 					0




#if (1 == _ENABLE_HOOK_readlink)

FUNC_P3(ssize_t, readlink, const char*, path, char*, buf, size_t, bufsiz)
{
		ret = orgfn( path, buf, bufsiz );
		/*
		 * On success, these calls return the number of bytes placed in buf.
       (If the returned value equals bufsiz, then truncation may have
       occurred.)  On error, -1 is returned and errno is set to indicate the
       error.
		 */
		DEBUGMSG("readlink();\n");
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		//rsrcname = string(path);
		
		if( ret >= 0 ) {
			//char* tmp = (char*)malloc( ret+1 );
			//memcpy( tmp, buf, ret );
			//tmp[ret] = 0;
			
			//APPEND_STR( vtret, tmp, "buf", 0 );
			
			//free(tmp);
		} else {
			//APPEND_STR( vtret, "", "buf", 0 );
		} 
		//APPEND_INT( vtarg, bufsiz, "bufsiz", 0 );
		
		//LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg );
		//DEBUGMSG(DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_posix_fadvise64)

FUNC_P4(int , posix_fadvise64, int, fd, off64_t, offset, size_t, len, int, advice)
{
		ret = orgfn( fd, offset, len, advice );
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_ftruncate64)

FUNC_P2(int, ftruncate64, int, fd, off64_t, len)
{
		ret = orgfn( fd, len );
		DEBUGMSG("ftruncate64(%s);\n", GET_FILE(fd).c_str());
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_truncate64)

FUNC_P2(int, truncate64, const char*, pathname, off64_t, length)
{
		ret = orgfn( pathname, length );
		DEBUGMSG("truncate64(%s);\n", pathname);
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_ftruncate)

FUNC_P2(int, ftruncate, int, fd, off_t, len)
{
		ret = orgfn( fd, len );
		DEBUGMSG("ftruncate(%s);\n", GET_FILE(fd).c_str());
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_truncate)

FUNC_P2(int, truncate, const char*, path, off_t, len)
{
		ret = orgfn( path, len );
		DEBUGMSG("truncate(%s);\n", GET_FILE(path).c_str());
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_ftell)


FUNC_P1(long, ftell, FILE *, stream)
{
		ret = orgfn( stream );
    DEBUGMSG("ftell(%s);\n", GET_FILE(stream).c_str());
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_lseek)

FUNC_P3(off_t, lseek, int, fd, off_t, offset, int, whence)
{
		ret = orgfn( fd, offset, whence );
		DEBUGMSG("lseek(%s,%d);\n",GET_FILE(fd).c_str(), offset);
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// int fd_type = 0;
		// rsrcname = GetFD(fd, fd_type);
		// APPEND_INT( vtarg, offset, "offset", 0 );
		// APPEND_INT( vtarg, whence, "whence", 0 );
		// APPEND_INT( vtret, ret, "ret", 0 );
		// LogResource( GetCategoryFromFDType(fd_type, CATEGORY_FILE), rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_lseek64)

FUNC_P3(off64_t, lseek64, int, fd, off64_t, offset, int, whence)
{
		ret = orgfn( fd, offset, whence );
		DEBUGMSG("lseek64(%s,%lld);\n",GET_FILE(fd).c_str(),offset);
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// int fd_type = 0;
		// rsrcname = GetFD(fd, fd_type);
		// APPEND_UINT64( vtarg, offset, "offset", 0 );
		// APPEND_INT( vtarg, whence, "whence", 0 );
		// APPEND_UINT64( vtret, ret, "ret", 0 );
		// LogResource( GetCategoryFromFDType(fd_type, CATEGORY_FILE), rsrcname, SYSCALL_NUM, vtret, vtarg );
		
		//string fname = getRsrcNameFromFd(fd);
		//if( fname.length() ) {
		//	DEBUGMSG(" [FDRSRC:%s] %ld = "FUNCNAME_STR"( %ld, %d );\n", fname.c_str(), (unsigned int)ret, (unsigned int)offset, whence );
		//} else 
		//{
		//	DEBUGMSG(DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );
		//} 
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_fseek64)

FUNC_P3(int, fseek64, FILE*, stream, long long, offset, int, whence)
{
		ret = orgfn( stream, offset, whence );
		DEBUGMSG("fseek64(%s,%lld);\n",GET_FILE(stream).c_str(), offset);
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// rsrcname = GetFILE(stream);
		// APPEND_INT64( vtarg, offset, "offset", 0 );
		// APPEND_INT( vtarg, whence, "whence", 0 );
		// APPEND_INT( vtret, ret, "ret", 0 );
		// LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif


//#define _ENABLE_HOOK_fseek 1

#if (1 == _ENABLE_HOOK_fseek)

FUNC_P3(int, fseek, FILE*, stream, long int, offset, int, origin)
{
		ret = orgfn( stream, offset, origin );
		DEBUGMSG("fseek(%s,%ld);\n",GET_FILE(stream).c_str(), offset);
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_rewind)
// should be FUNC_VOID_P1
FUNC_P1(void, rewind, FILE*, stream)
{
	HOOK_FUNC_OPER_HEADER2_NORET
	{	
		orgfn( stream );
		DEBUGMSG("rewind(%s);\n", GET_FILE(stream).c_str());
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// rsrcname = GetFILE(stream);
		// LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg );
		//DEBUGMSG("%s("DEBUGTEXTPARAM") begin [%d]\n", FUNCNAME_STR, FUNCPARAM_INVOKE, READ_COUNTER());
		//DEBUGMSG(DEBUGTEXTPARAM, FUNCPARAM_INVOKE);		
	HOOK_FUNC_OPER_FOOTER_NORET;
}
FUNC_END();
#endif


#if (1 == _ENABLE_HOOK_fclose)

FUNC_P1(int, fclose, FILE*, stream)
{
		ret = orgfn( stream );
		DEBUGMSG("fclose(%s);\n", GET_FILE(stream).c_str());
		DEL_FILE(stream);
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_closedir)

FUNC_P1(int, closedir, DIR*, dirp)
{
	HOOK_FUNC_OPER_HEADER3
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_feof)

FUNC_P1(int, feof, FILE*, stream)
{
		ret = orgfn( stream );
		DEBUGMSG("feof(%s);\n", GET_FILE(stream).c_str());
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_ferror)

FUNC_P1(int, ferror, FILE*, stream)
{
	HOOK_FUNC_OPER_HEADER3
	DEBUGMSG("ferror(%s);\n", GET_FILE(stream).c_str());
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK_fileno)

FUNC_P1(int, fileno, FILE*, stream)
{
		ret = orgfn( stream );
		if (DEBUGMSG_GUARD) {
			LINK_FILE(stream, ret);
			DEBUGMSG("fileno(%s);\n", GET_FILE(stream).c_str());
		}
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_fflush)
#define FUNCPARAM_INVOKE_NOPTR 	"void"

FUNC_P1(int, fflush, FILE*, stream)
{
		ret = orgfn( stream );
		DEBUGMSG("fflush(%s);\n", GET_FILE(stream).c_str());
		
		string rsrcname;
		vector<LOG_DATA> vtret, vtarg;
		rsrcname = GetFILE(stream);
		APPEND_INT( vtret, ret, "ret", 0 );
		LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_pclose)

//
// The pclose() function waits for the associated process to terminate and returns 
// the exit status of the command as returned by wait4(2).
//
FUNC_P1(int, pclose, FILE*, stream)
{
	HOOK_FUNC_OPER_HEADER3
	DEBUGMSG("pclose(%s);\n", GET_FILE(stream).c_str());
}
FUNC_END();

#endif



#if !OPENDIR_IS_DETERMINISTIC
#if (1 == _ENABLE_HOOK_opendir)

FUNC_P1(DIR*, opendir, const char*, name)
{
		ret = orgfn( name );
		DEBUGMSG("opendir(%s);\n", name);
		
		SetDIR( ret, name );
}
FUNC_END();

#endif
#endif


#if (1 == _ENABLE_HOOK_readdir64)

FUNC_P1(struct dirent64* , readdir64, DIR*, dirp)
{
		ret = orgfn( dirp );		
		DEBUGMSG("readdir64();\n");
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// rsrcname = GetDIR(dirp);
		// dirent64_to_vector( ret, vtret );
		// LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_readdir)

FUNC_P1(struct dirent* , readdir, DIR*, dirp)
{
		ret = orgfn( dirp );
		DEBUGMSG("readdir();\n");
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// rsrcname = GetDIR(dirp);
		// dirent_to_vector( ret, vtret );
		// LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK___fxstat64)

FUNC_P3(int, __fxstat64, int, ver, int, fildes, struct stat64 *, stat_buf)
{
		ret = orgfn( ver, fildes, stat_buf );
		DEBUGMSG("__fxstat64(%s);\n", GET_FILE(fildes).c_str());
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// int fd_type = 0;
		// string rsrc = GetFD( fildes, fd_type );
		// APPEND_INT( vtarg, ver, "ver", 0 );
		// APPEND_INT( vtarg, fildes, "fildes", 0 );
		// APPEND_INT( vtret, ret, "ret", 0 );
		// APPEND_BUF( vtret, stat_buf, sizeof(struct stat64), "stat64", 0 );
		// LogResource( GetCategoryFromFDType(fd_type, CATEGORY_FILE), rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK___xstat64)

FUNC_P3(int, __xstat64, int, ver, const char*, path, struct stat64*, stat_buf)
{
		ret = orgfn( ver, path, stat_buf );
		DEBUGMSG("__xstat64(%s);\n", GET_FILE(path).c_str());
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// rsrcname = string(path);
		// APPEND_INT( vtarg, ver, "ver", 0 );
		// APPEND_BUF( vtret, stat_buf, sizeof(struct stat64), "stat64", 0 );
		// LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK___fxstatat64)

FUNC_P5(int, __fxstatat64, int, ver, int, dirfd, const char*, path, struct stat64 *, stat_buf, int, flags)
{
		ret = orgfn( ver, dirfd, path, stat_buf, flags );
		DEBUGMSG("__fxstatat64(%s);\n", GET_FILE(path));
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// int fd_type = 0;
		// rsrcname = string(path);
		// APPEND_INT( vtarg, ver, "ver", 0 );
		// APPEND_STR( vtarg, GetFD(dirfd, fd_type).c_str(), "dirfd", 0 );
		// APPEND_INT( vtarg, flags, "flags", 0 );
		// APPEND_INT( vtret, ret, "ret", 0 );
		// APPEND_BUF( vtret, stat_buf, sizeof(struct stat64), "stat64", 0 );
		// LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif



#if (1 == _ENABLE_HOOK___fxstat)

FUNC_P3(int, __fxstat, int, ver, int, fildes, struct stat *, stat_buf)
{
		ret = orgfn( ver, fildes, stat_buf );
		DEBUGMSG("__fxstat(%s);\n", GET_FILE(fildes).c_str());
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// int fd_type = 0;
		// rsrcname = GetFD(fildes, fd_type);
		// APPEND_INT( vtarg, ver, "ver", 0 );
		// APPEND_INT( vtret, ret, "ret", 0 );
		// APPEND_BUF( vtret, stat_buf, sizeof(struct stat), "stat", 0 );
		// LogResource( GetCategoryFromFDType(fd_type, CATEGORY_FILE), rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK___xstat)
#define FUNCPARAM_INVOKE_NOPTR	ver, path

FUNC_P3(int, __xstat, int, ver, const char*, path, struct stat*, stat_buf)
{
	//HOOK_FUNC_OPER_HEADER_NOPTR3
		ret = orgfn( ver, path, stat_buf );
		DEBUGMSG("__xstat(%s);\n", GET_FILE(path).c_str());
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// rsrcname = string(path);
		// APPEND_INT( vtarg, ver, "ver", 0 );
		// APPEND_INT( vtret, ret, "ret", 0 );
		// APPEND_BUF( vtret, stat_buf, sizeof(struct stat), "stat", 0 );
		// LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_statfs64)

FUNC_P2(int , statfs64, const char*, path, struct statfs64*, buf)
{
		ret = orgfn( path, buf );
		DEBUGMSG("statfs64();\n");
		
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		
		// rsrcname = string(path);
		// APPEND_INT( vtret, ret, "ret", 0 );
		// APPEND_BUF( vtret, buf, sizeof(struct stat64), "statfs64", 0 );
		// LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_statfs)

FUNC_P2(int , statfs, const char*, path, struct statfs*, buf)
{
		ret = orgfn( path, buf );
		DEBUGMSG("statfs();\n");
		//if( ret != -1 ) {
		//	string statfs_buf = GetPrintableBuf( (void*)buf, sizeof( struct statfs ) );
		//	DEBUGMSG(" [FDRSRC:_FILESYSTEM_]  %d = "FUNCNAME_STR"( %s, %s );\n", ret, path, statfs_buf.c_str() );
		//} else {
		//	DEBUGMSG(" [FDRSRC:_FILESYSTEM_]  %d = "FUNCNAME_STR"( %s, garbage );\n", ret, path );
		//}
		// string rsrcname;
		// vector<LOG_DATA> vtret, vtarg;
		// rsrcname = string(path);
		
		
		// APPEND_INT( vtret, ret, "ret", 0 );
		// APPEND_BUF( vtret, buf, sizeof(struct statfs), "statfs", 0 );
		// LogResource( CATEGORY_FILE, rsrcname, SYSCALL_NUM, vtret, vtarg );
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_fstat64)

FUNC_P2(int, fstat64, int, fildes, struct stat64 *, buf)
{
	//HOOK_FUNC_OPER_HEADER2_FILLSC(FILL_SCINFO_RETINT_INT_RETPTR)
		ret = orgfn( fildes, buf );
		DEBUGMSG("fstat64();\n");
}
FUNC_END();

#endif




#if (1 == _ENABLE_HOOK_fstat)

FUNC_P2(int, fstat, int, fd, struct stat *, buf)
{
	// HOOK_FUNC_OPER_HEADER2//_FILLSC(FILL_SCINFO_RETINT_INT_RETPTR)
		ret = orgfn( fd, buf );
		DEBUGMSG("fstat();\n");
}
FUNC_END();
#endif


#if (1 == _ENABLE_HOOK_stat)

FUNC_P2(int, stat, const char*, path, struct stat *, buf)
{
		ret = orgfn( path, buf );
		DEBUGMSG("stat();\n");
}
FUNC_END();
#endif



#if (1 == _ENABLE_HOOK_stat64)

FUNC_P2(int, stat64, const char *, pathname, struct stat64 *, buf)
{
		ret = orgfn( pathname, buf );
		DEBUGMSG("stat64();\n");	
}
FUNC_END();

#endif




//int mkstemp(char *template);


#if (1 == _ENABLE_HOOK_mkstemp)

FUNC_P1(int , mkstemp, char*, tmpl)
{
		ret = orgfn( tmpl );
		DEBUGMSG("mkstemp();\n");
		SetTmpFD( ret );
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_mkstemp64)

FUNC_P1(int, mkstemp64, char*, tmpl)
{
		ret = orgfn( tmpl );
		DEBUGMSG("mkstemp64();\n");
		SetTmpFD( ret );
}
FUNC_END();

#endif


#if (1 == _ENABLE_HOOK_tmpfile)


FUNC_P0(FILE*, tmpfile)
{
	//HOOK_FUNC_OPER_HEADER_NOPARAM3
		ret = orgfn(  );
		DEBUGMSG("tmpfile();\n");
		SetTmpFILE( ret );
	HOOK_FUNC_OPER_FOOTER	
}
FUNC_END();

#endif

#if (1 == _ENABLE_HOOK_mkdtemp)

FUNC_P1(char* , mkdtemp, char*, tmpl)
{
		ret = orgfn( tmpl );
		DEBUGMSG("mkdtemp();\n");
}
FUNC_END();

#endif
