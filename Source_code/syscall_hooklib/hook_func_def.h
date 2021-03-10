

#include <signal.h>

#include <dirent.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/resource.h>
#include <sys/uio.h>
#include <sys/statfs.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <setjmp.h>
#include <netdb.h>
#include <poll.h>
#include <termios.h>
#include <utility> // pari<>

#if LYNX_COOKIE_CASE
#include <curses.h>
#endif

//#include <term.h>
#include <langinfo.h>
#include <dirent.h>
//#include <sys/ioctl.h>

//#if NGINX_CASE
 #include <sys/epoll.h>
//#endif

#include <zlib.h>


//#include "../../llvm/src/lib/PRJ/models.h"

enum {
	NOTHING = 1,
  NAME_ONLY,
	PTR,
	DESCRIPTOR,
	BOTH, // have PTR and DESCRIPTOR
};

map<int, string> socket_record; // <fd, socket name>
int file_counter = 0;
int socket_counter = 0;

typedef struct file_info{
	FILE* f;
	int fd;
	string filename;
	int file_type;
	string id;
	file_info(int _fd, const char* _filename, string _id) {
    //DEBUGMSG("NEW file info: %s\n", _filename);
		file_type = DESCRIPTOR;
		fd = _fd;
		filename = _filename;
		id = _id;
	}
	file_info(FILE* _f, const char* _filename, string _id) {
    //DEBUGMSG("NEW file info: %s\n", _filename);
		file_type = PTR;
		f = _f;
		filename = _filename;
		id = _id;
	}
  file_info(const char* _filename, string _id) {
    file_type = NAME_ONLY;
    filename = _filename;
    id = _id;
  }

}file_info;
vector<file_info> file_record; 


inline string GET_FILE(int fd, const char* filename = NULL) {
	if (fd == -1 || filename == NULL) return "NULL";
	bool found = false;
	string ret;
	for (auto & f_info : file_record) {
		if (f_info.file_type == DESCRIPTOR || f_info.file_type == BOTH) {
			if (fd == f_info.fd) {
				ret = f_info.id;
				found = true;
				break;
			}
		} else if (f_info.file_type == NAME_ONLY && filename) {
      if (strcmp(filename, f_info.filename.c_str()) == 0) {
        ret = f_info.id;
        f_info.fd == fd;
        f_info.file_type = DESCRIPTOR;
        found = true;
        break;
      }
    }
	}
	if (!found) {
		string id = "file" + to_string(file_counter++);
		file_info f_info(fd, filename, id);
		file_record.push_back(f_info);
		ret = id;
	}
	return ret;
}

inline string GET_FILE(FILE* f, const char* filename = "") {
	if (f == NULL || filename == NULL) return "NULL";
	bool found = false;
	string ret;
	for (auto & f_info : file_record) {
		if (f_info.file_type == PTR || f_info.file_type == BOTH) {
			if (f == f_info.f) {
				ret = f_info.id;
				found = true;
				break;
			}
		} else if (f_info.file_type == NAME_ONLY && filename) {
      if (strcmp(filename, f_info.filename.c_str()) == 0) {
        ret = f_info.id;
        f_info.f = f;
        f_info.file_type = PTR;
        found = true;
        break;
      }
    }
	}
	if (!found) {
		string id = "file" + to_string(file_counter++);
		file_info f_info(f, filename, id);
		file_record.push_back(f_info);
		ret = id;
	}
	return ret;
}

inline string GET_FILE(const char* filename) {
	if (filename == NULL) return "NULL";
	bool found = false;
	string ret;
	for (auto & f_info : file_record) {
		if (strcmp(filename, f_info.filename.c_str()) == 0) {
			ret = f_info.id;
			found = true;
			break;
		}
	}
	if (!found) {
    string id = "file" + to_string(file_counter++);
    file_info f_info(filename, id);
    file_record.push_back(f_info);
    ret = id;
	}
	return ret;
}

// link FILE* and fd, called by *fileno*
inline void LINK_FILE(FILE* f, int fd) {
	string ret;
	for (auto & f_info : file_record) {
		if (f_info.file_type == PTR) {
			if (f == f_info.f) {
				f_info.fd = fd;
				f_info.file_type = BOTH;
				return;
			}
		} else if (f_info.file_type == DESCRIPTOR) {
			if (fd == f_info.fd) {
				f_info.f = f;
				f_info.file_type = BOTH;
				return;
			}
		} else if (f_info.file_type == BOTH) { // just update
			f_info.f = f;
			f_info.fd = fd;
		}
	}
}

inline void UPDATE_FD(int oldfd, int newfd) {
	for (auto & f_info : file_record) {
		if (oldfd == f_info.fd) {
			f_info.fd = newfd;
			break;
		}
	}
}

inline void UPDATE_FILENAME(string oldname, string newname) {
	for (auto & f_info : file_record) {
		if (oldname == f_info.filename) {
			f_info.filename = newname;
			break;
		}
	}
}

inline void DEL_FILE(FILE* f) {
  for (auto& f_info : file_record) {
    if (f_info.file_type == PTR || f_info.file_type == BOTH) {
      if (f_info.f == f)
        f_info.file_type = NAME_ONLY;
    }
  }
  //for (auto it = file_record.begin(); it != file_record.end(); it++) {
    //if (it->file_type == PTR || it->file_type == BOTH) {
      //if (f == it->f) {
        //file_record.erase(it);
        //return;
      //}
    //}
  //}
}

inline void DEL_FILE(int fd) {
  for (auto& f_info : file_record) {
    if (f_info.file_type == DESCRIPTOR || f_info.file_type == BOTH) {
      if (f_info.fd == fd)
        f_info.file_type = NAME_ONLY;
    }
  } 
	//for (auto it = file_record.begin(); it != file_record.end(); it++) {
		//if (it->file_type == DESCRIPTOR || it->file_type == BOTH) {
			//if (fd == it->fd) {
				//file_record.erase(it);
				//return;
			//}
		//}
	//}
}

extern map<int, string> socket_record; // <fd, socket name>
extern int socket_counter;
inline string GET_SOCKET(int fd) {
	auto it = socket_record.find(fd);
	if (it != socket_record.end()) {
		return it->second;
	} else {
		socket_record[fd] = "socket" + to_string(socket_counter++);
		return socket_record[fd];
	}
}

extern int CALL_FROM_DEBUGMSG; // it will be 1 when it is in the DEBUGMSG
#define DEBUGMSG_GUARD (!CALL_FROM_DEBUGMSG)  
// #define DEBUGMSG_GUARD do {} while (0);


struct group {
               char   *gr_name;        /* group name */
               char   *gr_passwd;      /* group password */
               gid_t   gr_gid;         /* group ID */
               char  **gr_mem;         /* NULL-terminated array of pointers
                                          to names of group members */
};



struct passwd {
    char   *pw_name;       /* username */
    char   *pw_passwd;     /* user password */
    uid_t   pw_uid;        /* user ID */
    gid_t   pw_gid;        /* group ID */
    char   *pw_gecos;      /* user information */
    char   *pw_dir;        /* home directory */
    char   *pw_shell;      /* shell program */
};

struct winsize
{
  unsigned short ws_row;	/* rows, in characters */
  unsigned short ws_col;	/* columns, in characters */
  unsigned short ws_xpixel;	/* horizontal size, pixels */
  unsigned short ws_ypixel;	/* vertical size, pixels */
};





// ================================================================================
// Most basic macros
// ================================================================================
#define PUSH_PARAM_SI( __param, __size, __offset, __si ) {\
			WriteParam( (unsigned char*)__param, __offset, __size, &__si );\
		}

#define POP_PARAM_SI( __param, __size, __offset, __si ) {\
			ReadParam( (unsigned char*)__param, __offset, __size, &__si );\
		}

#define PUSH_PARAM( __param, __size, __offset ) PUSH_PARAM_SI( __param, __size, __offset, si )
#define POP_PARAM( __param, __size, __offset ) POP_PARAM_SI( __param, __size, __offset, si )

#define PUSH_RETURN_VAL( __offset ) PUSH_PARAM_SI( &ret, sizeof(FUNCRETURN), __offset, si )
#define POP_RETURN_VAL( __offset ) POP_PARAM_SI( &ret, sizeof(FUNCRETURN), __offset, si )
// ================================================================================

// ================================================================================
// STRINGS
#define PUSH_PARAM_STR( __str, __offset ) {\
		int ______l = 0;\
		if( __str ) ______l = strlen( __str );\
		PUSH_PARAM( &______l, sizeof( int ), __offset );\
		if( ______l > 0 ) PUSH_PARAM( __str, ______l, __offset );\
	}
	
#define POP_PARAM_STR( __str, __offset ) {\
		int ______l = 0;\
		POP_PARAM( &______l, sizeof( int ), __offset );\
		if( ______l > 0 ) {\
			POP_PARAM( __str, ______l, __offset );\
			__str[______l] = 0;\
		}\
	}

#define POP_PARAM_STR_ALLOC( __str, __offset ) {\
		int ______l = 0;\
		POP_PARAM( &______l, sizeof( int ), __offset );\
		if( ______l > 0 ) {\
			__str = (char*)malloc( ______l + 1 );\
			POP_PARAM( __str, ______l, __offset );\
			__str[______l] = 0;\
		} else {\
			__str = 0;\
		}\
	}

#define POP_STR_PARAM_SI( __str, __offset, __si ) {\
		int ______l = 0;\
		POP_PARAM_SI( &______l, sizeof( int ), __offset, __si );\
		if( ______l > 0 ) {\
			MALLOC_VTMEM( __str, char*, ______l+1 );\
			POP_PARAM_SI( __str, ______l, __offset, __si );\
			__str[______l] = 0;\
		}\
	}

#define PUSH_STRPTR_PARAM(__ptr, __offset) {\
		int __slen__ = __ptr ? strlen(__ptr) : 0;\
		PUSH_PARAM( &__slen__, sizeof(int), __offset );\
		if( __slen__ >= 0 ) PUSH_PARAM(__ptr, __slen__, __offset );\
	}
	
#define POP_STRPTR_PARAM(__ptr, __offset) {\
		int __slen__;\
		POP_PARAM( &__slen__, sizeof(int), __offset );\
		if( __slen__ == 0 ) __ptr = 0;\
		else {\
			POP_PARAM(__ptr, __slen__, __offset );\
			__ptr[__slen__] = 0;\
		}\
	}

// ================================================================================
// BUFFERS
#define PUSH_PARAM_BUF( __buf, __size, __offset ) {\
		int __size__ = (int)__size;\
		if( !__buf ) {\
			__size__ = 0;\
		}\
		PUSH_PARAM( &__size__, sizeof( int ), __offset );\
		/*DEBUGMSG("push_param_buf: size: %d\n", __size__);*/\
		if( __size__ > 0 ) PUSH_PARAM( __buf, __size__, __offset );\
	}

	
#define POP_PARAM_BUF( __buf, __offset ) {\
		int __size__ = 0;\
		POP_PARAM( &__size__, sizeof( int ), __offset );\
		DEBUGMSG("pop_param_buf: size: %d\n", __size__);\
		if( __size__ > 0 ) {\
			POP_PARAM( __buf, __size__, __offset );\
		} else {\
			__buf = 0;\
		}\
	}

#define POP_PARAM_BUF_ALLOC( __buf, __type, __offset ) {\
		int __size__ = 0;\
		POP_PARAM( &__size__, sizeof( int ), __offset );\
		if( __size__ > 0 ) {\
			__buf = (__type) malloc( __size__ + 1 );\
			POP_PARAM( __buf, __size__, __offset );\
		} else {\
			__buf = 0;\
		}\
	}

	
#define PUSH_RETURN_STRPTR(__offset) PUSH_STRPTR_PARAM(ret, __offset)
#define POP_RETURN_STRPTR(__offset) {\
		int __slen__;\
		POP_PARAM( &__slen__, sizeof(int), __offset );\
		if( __slen__ == 0 ) ret = 0;\
		else {\
			ret = (char*)malloc( __slen__ + 1 );\
			POP_PARAM(ret, __slen__, __offset );\
			ret[__slen__] = 0;\
		}\
	}

#define PUSH_RETURN_BUF(__size, __offset) {\
		int __size__ = __size;\
		if( !ret ) {\
			__size__ = 0;\
			PUSH_PARAM(&__size__, sizeof(int), __offset);\
		} else {\
			PUSH_PARAM(&__size__, sizeof(int), __offset);\
			PUSH_PARAM(ret, __size__, __offset);\
		}\
	}
		
		
#define POP_RETURN_BUF(__size, __type, __offset) {\
		POP_PARAM( &__size, sizeof(int), __offset );\
		if( __size == 0 ) ret = 0;\
		else {\
			ret = (__type)malloc( __size );\
			POP_PARAM(ret, __size, __offset );\
		}\
	}


// FILLSC DEFINES

#define FILL_SCINFO_RETVAR_RETDBLARY_INTSIZE(__params) __FILL_SCINFO_RETVAR_RETDBLARY_INTSIZE(__params);
#define FILL_SCINFO_RETVAR(__params) __FILL_SCINFO_RETVAR(__params);
#define FILL_SCINFO_RETVAR_RETPTR(__params) __FILL_SCINFO_RETVAR_RETPTR(__params);
#define FILL_SCINFO_RETINT_VAR(__params) __FILL_SCINFO_RETINT_VAR(__params);
#define FILL_SCINFO_RETINT_FILE(__params) __FILL_SCINFO_RETINT_FILE(__params);
#define FILL_SCINFO_RETINT_INT_RETPTR(__params) __FILL_SCINFO_RETINT_INT_RETPTR(__params);
#define FILL_SCINFO_RETINT_RETPTR_RETPTR(__params) __FILL_SCINFO_RETINT_RETPTR_RETPTR(__params);
#define FILL_SCINFO_RETSTR_STR_STR(__params) __FILL_SCINFO_RETSTR_STR_STR(__params);
#define FILL_SCINFO_RETINT_CSTR_BUF(__params) __FILL_SCINFO_RETINT_CSTR_BUF(__params);
#define FILL_SCINFO_RETSTR_STR(__params) __FILL_SCINFO_RETSTR_STR(__params);
#define FILL_SCINFO_RETSTR_VAR_STR(__params) __FILL_SCINFO_RETSTR_VAR_STR(__params);

#define __FILL_SCINFO_DEFINE \
		void* caller = 0;\
		SCINFO si, siMatched;\
		int __offset = 0;\
		COLLECT_ARGS_INIT();
		
#define __FILL_SCINFO_MASTER_SYSCALL1 \
		/* CALL ORG FUNC */\
		ret = orgfn( FUNCPARAM_INVOKE );\
		DEBUGMSG(DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );

		// TESTING.
#define __FILL_SCINFO_MASTER_SYSCALL1_DT \
		/* CALL ORG FUNC */\
		ret = orgfn( FUNCPARAM_INVOKE );\
		/*DEBUGMSG(DEBUGTEXTPARAM, ret, FUNCPARAM_DEBUGTEXT );*/
		
#define __FILL_SCINFO_MASTER_SYSCALL1_NOPARAM \
		/* CALL ORG FUNC */\
		ret = orgfn( FUNCPARAM_INVOKE );\
		DEBUGMSG(DEBUGTEXTPARAM, ret);

#define __FILL_SCINFO_MASTER_SYSCALL2 \
		int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched, caller);\
		/*DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult)  );*/
		
#define __FILL_SCINFO_SLAVE_SYSCALL1 \
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched, caller);\
			int __offset = 0;\
			switch( nResult ) {\
				case SYSCALL_RESULT_USE_SI:
					
#define __FILL_SCINFO_SLAVE_SYSCALL2 \
					break;\
				case SYSCALL_RESULT_INDEPENDENT_SLAVE:\
				default:\
					ret = orgfn( FUNCPARAM_INVOKE );\
					break;\
			}\
			/*DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );*/\
			/*DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult) );*/

			
		// TESTING.
#define __FILL_SCINFO_SLAVE_SYSCALL2_DT \
					break;\
				case SYSCALL_RESULT_INDEPENDENT_SLAVE:\
				default:\
					ret = orgfn( FUNCPARAM_INVOKE );\
					break;\
			}\
			/*DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_DEBUGTEXT );*/\
			/*DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult) );*/
			
#define __FILL_SCINFO_SLAVE_SYSCALL2_NOPARAM \
					break;\
				case SYSCALL_RESULT_INDEPENDENT_SLAVE:\
				default:\
					ret = orgfn( FUNCPARAM_INVOKE );\
					break;\
			}\
			/*DEBUGMSG( DEBUGTEXTPARAM, ret );*/\
			/*DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult) );*/


// ====================================================================================================
// ====================================================================================================
// ====================================================================================================
// ====================================================================================================
// ====================================================================================================
#define __FILL_SCINFO_RETSTR_VAR_STR(__var, __str) \
		__FILL_SCINFO_DEFINE\
		COLLECT_ARGS( &__var, sizeof( __var ) );\
		COLLECT_ARGS_STR( __str );\
		GET_CRC();\
		if( IS_MASTER_EXEC() ) {\
			__FILL_SCINFO_MASTER_SYSCALL1\
			{\
				PUSH_RETURN_STRPTR( __offset );\
			}\
			__FILL_SCINFO_MASTER_SYSCALL2\
		} else {\
			__FILL_SCINFO_SLAVE_SYSCALL1\
			{\
				POP_RETURN_STRPTR( __offset );\
			}\
			__FILL_SCINFO_SLAVE_SYSCALL2\
		}
		
#define __FILL_SCINFO_RETINT_CSTR_RETBUF(__cstr, __buf) \
		__FILL_SCINFO_DEFINE\
		COLLECT_ARGS_STR( __cstr );\
		GET_CRC();\
		if( IS_MASTER_EXEC() ) {\
			__FILL_SCINFO_MASTER_SYSCALL1\
			{\
				PUSH_RETURN_VAL( __offset );\
				PUSH_PARAM_BUF( __buf, sizeof(*__buf), __offset );\
			}\
			__FILL_SCINFO_MASTER_SYSCALL2\
		} else {\
			__FILL_SCINFO_SLAVE_SYSCALL1\
			{\
				POP_RETURN_STRPTR( __offset );\
				POP_PARAM_BUF( __buf, __offset );\
			}\
			__FILL_SCINFO_SLAVE_SYSCALL2\
		}


#define __FILL_SCINFO_RETSTR_STR_STR(__str1, __str2) \
		__FILL_SCINFO_DEFINE\
		COLLECT_ARGS_STR( __str1 );\
		COLLECT_ARGS_STR( __str2 );\
		GET_CRC();\
		if( IS_MASTER_EXEC() ) {\
			__FILL_SCINFO_MASTER_SYSCALL1\
			{\
				PUSH_RETURN_STRPTR( __offset );\
			}\
			__FILL_SCINFO_MASTER_SYSCALL2\
		} else {\
			__FILL_SCINFO_SLAVE_SYSCALL1\
			{\
				POP_RETURN_STRPTR( __offset );\
			}\
			__FILL_SCINFO_SLAVE_SYSCALL2\
		}

#define __FILL_SCINFO_RETSTR_STR(__str1) \
		__FILL_SCINFO_DEFINE\
		COLLECT_ARGS_STR( __str1 );\
		GET_CRC();\
		if( IS_MASTER_EXEC() ) {\
			__FILL_SCINFO_MASTER_SYSCALL1\
			{\
				PUSH_RETURN_STRPTR( __offset );\
			}\
			__FILL_SCINFO_MASTER_SYSCALL2\
		} else {\
			__FILL_SCINFO_SLAVE_SYSCALL1\
			{\
				POP_RETURN_STRPTR( __offset );\
			}\
			__FILL_SCINFO_SLAVE_SYSCALL2\
		}
		
		
#define __FILL_SCINFO_RETINT_VAR(__var) \
		__FILL_SCINFO_DEFINE\
		COLLECT_ARGS( &__var, sizeof( __var ) );\
		GET_CRC();\
		if( IS_MASTER_EXEC() ) {\
			__FILL_SCINFO_MASTER_SYSCALL1\
			{\
				PUSH_RETURN_VAL( __offset );\
			}\
			__FILL_SCINFO_MASTER_SYSCALL2\
		} else {\
			__FILL_SCINFO_SLAVE_SYSCALL1\
			{\
				POP_RETURN_VAL( __offset );\
			}\
			__FILL_SCINFO_SLAVE_SYSCALL2\
		}
		
#define __FILL_SCINFO_RETVAR_RETPTR(__ptr) \
		__FILL_SCINFO_DEFINE\
		GET_CRC();\
		if( IS_MASTER_EXEC() ) {\
			__FILL_SCINFO_MASTER_SYSCALL1\
			{\
				PUSH_RETURN_VAL( __offset );\
			}\
			__FILL_SCINFO_MASTER_SYSCALL2\
		} else {\
			__FILL_SCINFO_SLAVE_SYSCALL1\
			{\
				POP_RETURN_VAL( __offset );\
				/* apply return to the argument if it is not a null ptr. */\
				if( __ptr ) *__ptr = ret;\
			}\
			__FILL_SCINFO_SLAVE_SYSCALL2\
		}
	
		
/*
#define FUNCNAME	 			getloadavg
#define SYSCALL_NUM				SC_GETLOADAVG
#define FUNCRETURN				int
#define FUNCPARAM 				double loadavg[], int nelem
#define FUNCPARAM_INVOKE 		loadavg, nelem

*/
#define __FILL_SCINFO_RETVAR_RETDBLARY_INTSIZE( __dblary, __intsize ) \
		__FILL_SCINFO_DEFINE\
		COLLECT_ARGS( &__intsize, sizeof( __intsize ) );\
		GET_CRC();\
		if( IS_MASTER_EXEC() ) {\
			__FILL_SCINFO_MASTER_SYSCALL1\
			{\
				PUSH_RETURN_VAL( __offset );\
				PUSH_PARAM_BUF( __dblary, sizeof( double ) * __intsize, __offset );\
			}\
			__FILL_SCINFO_MASTER_SYSCALL2\
		} else {\
			__FILL_SCINFO_SLAVE_SYSCALL1\
			{\
				POP_RETURN_VAL( __offset );\
				POP_PARAM_BUF( __dblary, __offset );\
			}\
			__FILL_SCINFO_SLAVE_SYSCALL2_NOPARAM\
		}
		
		
#define __FILL_SCINFO_RETVAR() \
		__FILL_SCINFO_DEFINE\
		GET_CRC();\
		if( IS_MASTER_EXEC() ) {\
			__FILL_SCINFO_MASTER_SYSCALL1_NOPARAM\
			{\
				PUSH_RETURN_VAL( __offset );\
			}\
			__FILL_SCINFO_MASTER_SYSCALL2\
		} else {\
			__FILL_SCINFO_SLAVE_SYSCALL1\
			{\
				POP_RETURN_VAL( __offset );\
			}\
			__FILL_SCINFO_SLAVE_SYSCALL2_NOPARAM\
		}

#define __FILL_SCINFO_RETINT_INT_RETPTR(__int1, __buf1) \
		__FILL_SCINFO_DEFINE\
		GET_CRC();\
		if( IS_MASTER_EXEC() ) {\
			__FILL_SCINFO_MASTER_SYSCALL1\
			{\
				PUSH_RETURN_VAL( __offset );\
				PUSH_PARAM( &__int1, sizeof( __int1 ), __offset );\
				if( __buf1 ) { PUSH_PARAM_BUF( __buf1, sizeof( *__buf1 ), __offset ); }\
			}\
			__FILL_SCINFO_MASTER_SYSCALL2\
		} else {\
			__FILL_SCINFO_SLAVE_SYSCALL1\
			{\
				POP_RETURN_VAL( __offset );\
				POP_PARAM( &__int1, sizeof(__int1), __offset );\
				if( __buf1 ) { POP_PARAM_BUF( __buf1, __offset ); }\
			}\
			__FILL_SCINFO_SLAVE_SYSCALL2\
		}
		
#define __FILL_SCINFO_RETINT_RETPTR_RETPTR(__ptr1, __ptr2) \
		__FILL_SCINFO_DEFINE\
		GET_CRC();\
		if( IS_MASTER_EXEC() ) {\
			__FILL_SCINFO_MASTER_SYSCALL1\
			{\
				PUSH_RETURN_VAL( __offset );\
				PUSH_PARAM_BUF( __ptr1, sizeof( *__ptr1 ), __offset );\
				PUSH_PARAM_BUF( __ptr2, sizeof( *__ptr2 ), __offset );\
			}\
			__FILL_SCINFO_MASTER_SYSCALL2\
		} else {\
			__FILL_SCINFO_SLAVE_SYSCALL1\
			{\
				POP_RETURN_VAL( __offset );\
				POP_PARAM_BUF( __ptr1, __offset );\
				POP_PARAM_BUF( __ptr2, __offset );\
			}\
			__FILL_SCINFO_SLAVE_SYSCALL2\
		}		
		
#define __FILL_SCINFO_RETINT_FILE(__file) \
		__FILL_SCINFO_DEFINE\
		COLLECT_ARGS( &__file, sizeof( __file ) );\
		GET_CRC();\
		if( IS_MASTER_EXEC() ) {\
			/* CALL ORG FUNC */\
			ret = orgfn( FUNCPARAM_INVOKE );\
			string fname = getFileNameFromFILE(__file);\
			if( fname.length() )  DEBUGMSG("[FDRSRC:%s] %d = "SYSCALL_NAMES[SYSCALL_NUM]"( %s );\n", fname.c_str(), ret, fname.c_str() );\
			else                  DEBUGMSG(DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );\
			/* STORE RETURN */\
			PUSH_RETURN_VAL( __offset );\
			/* PUT INTO SHAREDMEM */\
			__FILL_SCINFO_MASTER_SYSCALL2\
		} else {\
			int nResult = OnSyscall(SYSCALL_NUM, &si, &siMatched, caller);\
			int __offset = 0;\
			switch( nResult ) {\
				case SYSCALL_RESULT_USE_SI:\
					{\
						POP_RETURN_VAL( __offset );\
					}\
					break;\
				case SYSCALL_RESULT_INDEPENDENT_SLAVE: default:\
					ret = orgfn( FUNCPARAM_INVOKE );\
				break;\
			}\
			string fname = getFileNameFromFILE(__file);\
			if( fname.length() )  DEBUGMSG("[FDRSRC:%s] %d = "SYSCALL_NAMES[SYSCALL_NUM]"( %s );\n", fname.c_str(), ret, fname.c_str() );\
			else                  DEBUGMSG(DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );\
			DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );\
			DEBUGMSG( " OnSyscall("SYSCALL_NAMES[SYSCALL_NUM]") = %s\n", getSyscallResultString(nResult) );\
		}
		
//===================================================================================================================
//===================================================================================================================
//===================================================================================================================
//===================================================================================================================
//===================================================================================================================
//===================================================================================================================
//===================================================================================================================

#define HOOK_FUNC_OPER_HEADER_NOPARAM_NORETURN \
	ENTER_FUNC;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		orgfn( FUNCPARAM_INVOKE );\
	} else if( !IS_FORKED() ) {\
		DEBUGMSG(DEBUGTEXTPARAM);\
		orgfn( FUNCPARAM_INVOKE );\
	} else 


#define HOOK_FUNC_OPER_HEADER_NOPARAM_NORETURN2 \
	ENTER_FUNC;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		orgfn( FUNCPARAM_INVOKE );\
	} else 

#define HOOK_FUNC_OPER_HEADER_NOPARAM_NORETURN3 \
	ENTER_FUNC;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		orgfn( FUNCPARAM_INVOKE );\
	} else {\
		orgfn( FUNCPARAM_INVOKE );\
	}
		
#define HOOK_FUNC_OPER_HEADER_NORETURN \
	ENTER_FUNC;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		orgfn( FUNCPARAM_INVOKE );\
	} else if( !IS_FORKED() ) {\
		DEBUGMSG(DEBUGTEXTPARAM, FUNCPARAM_INVOKE );\
		orgfn( FUNCPARAM_INVOKE );\
	} else 

#define HOOK_FUNC_OPER_HEADER_NORETURN2 \
	ENTER_FUNC;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		orgfn( FUNCPARAM_INVOKE );\
	} else 

#define HOOK_FUNC_OPER_HEADER_NORETURN3 \
	ENTER_FUNC;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		orgfn( FUNCPARAM_INVOKE );\
	} else {\
		orgfn( FUNCPARAM_INVOKE );\
	}

		
#define HOOK_FUNC_OPER_FOOTER_NORETURN \
	LEAVE_FUNC;\

#define HOOK_FUNC_OPER_HEADER_NOPARAM \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else if( !IS_FORKED() ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
		DEBUGMSG(DEBUGTEXTPARAM, ret );\
	} else 
		
#define HOOK_FUNC_OPER_HEADER_NOPARAM2 \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else 
		
#define HOOK_FUNC_OPER_HEADER_NOPARAM3 \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	}

#define HOOK_FUNC_OPER_HEADER_NOLOG \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} 	
	
#define HOOK_FUNC_OPER_HEADER_NOPARAM_NOPTR \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else if( !IS_FORKED() ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
		DEBUGMSG(DEBUGTEXTPARAM_NOPTR);\
	} else 

		
#define HOOK_FUNC_OPER_HEADER \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else if( !IS_FORKED() ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
		DEBUGMSG(DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );\
	} else

#define HOOK_FUNC_OPER_HEADER_ND \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else if( !IS_FORKED() ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else 

#define HOOK_FUNC_OPER_HEADER2 \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else 

#define HOOK_FUNC_OPER_HEADER2_NORET \
	ENTER_FUNC;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		orgfn( FUNCPARAM_INVOKE );\
	} else 

		
#define HOOK_FUNC_OPER_HEADER3_DT \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else {\
		ret = orgfn( FUNCPARAM_INVOKE );\
		DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_DEBUGTEXT );\
	}


#define HOOK_FUNC_OPER_HEADER3_ND \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	}
#define HOOK_FUNC_OPER_HEADER3 \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else {\
		ret = orgfn( FUNCPARAM_INVOKE );\
		/*DEBUGMSG( DEBUGTEXTPARAM, ret, FUNCPARAM_INVOKE );*/\
	}

#define HOOK_FUNC_OPER_HEADER2_FILLSC(__fill_sc) \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else {\
		__fill_sc( FUNCPARAM_INVOKE );\
	}

#define HOOK_FUNC_OPER_FOOTER_NORET \
	LEAVE_FUNC;	
		
#define HOOK_FUNC_OPER_FOOTER \
	LEAVE_FUNC;\
	return ret;

#define HOOK_FUNC_OPER_HEADER_NOPTR \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else if( !IS_FORKED() ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
		DEBUGMSG(DEBUGTEXTPARAM_NOPTR, ret, FUNCPARAM_INVOKE_NOPTR );\
	} else 
	
#define HOOK_FUNC_OPER_HEADER_NOPTR2(__fill_scinfo) \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else {\
		__fill_scinfo( FUNCPARAM_INVOKE );\
	}

#define HOOK_FUNC_OPER_HEADER_NOPTR3 \
	ENTER_FUNC;\
	FUNCRETURN ret;\
	typedef FUNCRETURN (*PFNORG)( FUNCPARAM ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SYSCALL_NUM]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( FUNCPARAM_INVOKE );\
	} else {\
		ret = orgfn( FUNCPARAM_INVOKE );\
		DEBUGMSG(DEBUGTEXTPARAM_NOPTR, ret, FUNCPARAM_INVOKE_NOPTR );\
	}
		
#define HOOK_FUNC_INPUT_HEADER	HOOK_FUNC_OPER_HEADER
#define HOOK_FUNC_INPUT_FOOTER	HOOK_FUNC_OPER_FOOTER
	
	
#define COLLECT_ARGS_INIT() \
		vector<void*> vtArgPtr;\
		vector<int> vtArgSize;
		
#define COLLECT_ARGS(__ptr, __size) \
		vtArgPtr.push_back(__ptr);\
		vtArgSize.push_back(__size);

#define COLLECT_ARGS_STR(__ptr) \
		if( __ptr ) {\
			vtArgPtr.push_back((void*)__ptr);\
			vtArgSize.push_back(strlen( __ptr ));\
		}

#define COLLECT_ARGS_PTR(__ptr, __size) \
		if( __ptr ) {\
			vtArgPtr.push_back((void*)__ptr);\
			vtArgSize.push_back(__size);\
		}

		
		
		
/********************************************************************************************
 * FUNC P0
 ********************************************************************************************/
#define FUNC_VOID_P0(_fret, _fname ) \
extern "C" _fret _fname()\
{\
	ENTER_FUNC;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)(); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		orgfn();\
	} else 

		
		
#define FUNC_P0(_fret, _fname ) \
extern "C" _fret _fname()\
{\
	ENTER_FUNC;\
	_fret ret;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)(); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn();\
	} else 


/********************************************************************************************
 * FUNC P1
 ********************************************************************************************/
#define FUNC_VOID_P1(_fret, _fname, _fparam1_t, _fparam1 ) \
extern "C" _fret _fname( _fparam1_t  _fparam1 )\
{\
	ENTER_FUNC;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		orgfn( _fparam1 );\
	} else 

		
#define FUNC_P1(_fret, _fname, _fparam1_t, _fparam1 ) \
extern "C" _fret _fname( _fparam1_t  _fparam1 )\
{\
	ENTER_FUNC;\
	_fret ret;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( _fparam1 );\
	} else 

#define FUNC_P1_INV(_fret, _fname, _fparam1_t, _fparam1, _fparam1_invoke ) \
extern "C" _fret _fname( _fparam1_t  _fparam1 )\
{\
	ENTER_FUNC;\
	_fret ret;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( _fparam1_invoke );\
	} else 

		
/********************************************************************************************
 * FUNC P2
 ********************************************************************************************/
#define FUNC_P2_INV(_fret, _fname, _fparam1_t, _fparam1, _fparam2_t, _fparam2, _fparam1_inv, _fparam2_inv ) \
extern "C" _fret _fname( _fparam1_t  _fparam1, _fparam2_t  _fparam2 )\
{\
	ENTER_FUNC;\
	_fret ret;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1, _fparam2_t  _fparam2 ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( _fparam1_inv, _fparam2_inv );\
	} else

#define FUNC_P2(_fret, _fname, _fparam1_t, _fparam1, _fparam2_t, _fparam2 ) \
extern "C" _fret _fname( _fparam1_t  _fparam1, _fparam2_t  _fparam2 )\
{\
	ENTER_FUNC;\
	_fret ret;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1, _fparam2_t  _fparam2 ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( _fparam1, _fparam2 );\
	} else
		
#define FUNC_VOID_P2(_fret, _fname, _fparam1_t, _fparam1, _fparam2_t, _fparam2 ) \
extern "C" _fret _fname( _fparam1_t  _fparam1, _fparam2_t  _fparam2 )\
{\
	ENTER_FUNC;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1, _fparam2_t  _fparam2 ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		orgfn( _fparam1, _fparam2 );\
	} else 		


/********************************************************************************************
 * FUNC P3
 ********************************************************************************************/

#define FUNC_VOID_P3(_fret, _fname, _fparam1_t, _fparam1, _fparam2_t, _fparam2, _fparam3_t, _fparam3 ) \
extern "C" _fret _fname( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3 )\
{\
	ENTER_FUNC;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3 ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		orgfn( _fparam1, _fparam2, _fparam3 );\
	} else 

		
#define FUNC_P3(_fret, _fname, _fparam1_t, _fparam1, _fparam2_t, _fparam2, _fparam3_t, _fparam3 ) \
extern "C" _fret _fname( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3 )\
{\
	ENTER_FUNC;\
	_fret ret;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3 ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( _fparam1, _fparam2, _fparam3 );\
	} else 

#define FUNC_P3_INV(_fret, _fname, _fparam1_t, _fparam1, _fparam2_t, _fparam2, _fparam3_t, _fparam3, _fparam1_inv, _fparam2_inv, _fparam3_inv ) \
extern "C" _fret _fname( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3 )\
{\
	ENTER_FUNC;\
	_fret ret;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3 ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( _fparam1_inv, _fparam2_inv, _fparam3_inv );\
	} else 

		
/********************************************************************************************
 * FUNC P4
 ********************************************************************************************/

#define FUNC_P4(_fret, _fname, _fparam1_t, _fparam1, _fparam2_t, _fparam2, _fparam3_t, _fparam3, _fparam4_t, _fparam4  ) \
extern "C" _fret _fname( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3, _fparam4_t  _fparam4 )\
{\
	ENTER_FUNC;\
	_fret ret;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3, _fparam4_t  _fparam4 ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( _fparam1, _fparam2, _fparam3, _fparam4 );\
	} else 


#define FUNC_P4_INV(_fret, _fname, _fparam1_t, _fparam1, _fparam2_t, _fparam2, _fparam3_t, _fparam3, _fparam4_t, _fparam4, _fparam1_inv, _fparam2_inv, _fparam3_inv, _fparam4_inv  ) \
extern "C" _fret _fname( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3, _fparam4_t  _fparam4 )\
{\
	ENTER_FUNC;\
	_fret ret;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3, _fparam4_t  _fparam4 ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( _fparam1_inv, _fparam2_inv, _fparam3_inv, _fparam4_inv );\
	} else 


/********************************************************************************************
 * FUNC P5
 ********************************************************************************************/

#define FUNC_P5(_fret, _fname, _fparam1_t, _fparam1, _fparam2_t, _fparam2, _fparam3_t, _fparam3, _fparam4_t, _fparam4, _fparam5_t, _fparam5  ) \
extern "C" _fret _fname( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3, _fparam4_t  _fparam4, _fparam5_t  _fparam5 )\
{\
	ENTER_FUNC;\
	_fret ret;\
	const int SYSCALL_NUM = SC_ ## _fname;\
	typedef _fret (*PFNORG)( _fparam1_t  _fparam1, _fparam2_t  _fparam2, _fparam3_t  _fparam3, _fparam4_t  _fparam4, _fparam5_t  _fparam5 ); \
	static PFNORG orgfn = (PFNORG)dlsym(RTLD_NEXT, SYSCALL_NAMES[SC_ ## _fname]);\
	if( CALL_FROM_HOOK_SO ) {\
		ret = orgfn( _fparam1, _fparam2, _fparam3, _fparam4, _fparam5 );\
	} else 


/********************************************************************************************
 * FUNC ENDS
 ********************************************************************************************/

#define FUNC_END() \
	LEAVE_FUNC;\
	return ret;\
}

#define FUNC_VOID_END() \
	LEAVE_FUNC;\
}

/********************************************************************************************
 ********************************************************************************************/

#define GET_CRC() ;
//		CCrc32Static::BufferVtCrc32( vtArgPtr, vtArgSize, si.syscallArgHash );
			
#include "util_rsrc.h"

typedef struct tag_m_ngx_connection_s {
	char buf[108];
} _m_ngx_connection_t;





