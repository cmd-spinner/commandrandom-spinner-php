
#ifndef UTIL_LOG_IO_HEADER
#define UTIL_LOG_IO_HEADER 1


#include "define.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <ctype.h>
#include <stdlib.h>
#include <inttypes.h>
#include <zlib.h>



#include "common.h"

#define RSRC_VARS \
	string rsrcname;\
	vector<LOG_DATA> vtret, vtarg;\
	vtret.reserve(1);\
	vtarg.reserve(5);\
	unsigned char fd_type = 0;

#define RSRC_VARS2(__n_ret, __n_arg) \
	string rsrcname;\
	vector<LOG_DATA> vtret, vtarg;\
	vtret.reserve(__n_ret);\
	vtarg.reserve(__n_arg);\
	unsigned char fd_type = 0;

#define CATEGORY_NETWORK 		string("NETWORK")
#define CATEGORY_FILE 			string("FILE")
#define CATEGORY_CONSOLE 		string("CONSOLE")
#define CATEGORY_USERINFO		string("USERINFO")
#define CATEGORY_TIME			string("TIME")
#define CATEGORY_SYSINFO		string("SYSINFO")
#define CATEGORY_EVENT			string("EVENT")
#define CATEGORY_ZLIB 			string("ZLIB")

#define COUNT_ARRAY_UNTIL_NULL(_elem, _cnt) \
		_cnt = 0;\
		if( _elem ) {\
			for( int __i = 0; ; __i++ ) {\
				if( _elem[__i] == 0 ) break;\
				else _cnt++;\
			}\
		}
		
#define STRUCT_TYPE_NONE	   				0
#define STRUCT_TYPE_HOSTENT   				1
#define STRUCT_TYPE_IOVEC   				2
#define STRUCT_TYPE_MSGHDR   				3
#define STRUCT_TYPE_ADDRINFO				4
#define STRUCT_TYPE_ADDRINFOS				5
#define STRUCT_TYPE_DIRENT					6
#define STRUCT_TYPE_DIRENT64				7

inline const char* GetStructTypeName(int struct_type)
{
	switch( struct_type ) {
	case STRUCT_TYPE_NONE:
		return "STRUCT_TYPE_NONE";
		break;
	case STRUCT_TYPE_HOSTENT:
		return "STRUCT_TYPE_HOSTENT";
		break;
	case STRUCT_TYPE_IOVEC:
		return "STRUCT_TYPE_IOVEC";
		break;
	case STRUCT_TYPE_MSGHDR:
		return "STRUCT_TYPE_MSGHDR";
		break;
	case STRUCT_TYPE_ADDRINFO:
		return "STRUCT_TYPE_ADDRINFO";
		break;
	case STRUCT_TYPE_ADDRINFOS:
		return "STRUCT_TYPE_ADDRINFOS";
		break;
	case STRUCT_TYPE_DIRENT:
		return "STRUCT_TYPE_DIRENT";
		break;
	case STRUCT_TYPE_DIRENT64:
		return "STRUCT_TYPE_DIRENT64";
		break;
	}
	return "";
}


#define TYPE_CHAR 				1
#define TYPE_INT 				2
#define TYPE_UINT    			3
#define TYPE_INT64 				4
#define TYPE_UINT64 			5
#define TYPE_SIZE				6
#define TYPE_SSIZE				7
#define TYPE_TIME				8
#define TYPE_CLOCK				9
#define TYPE_STR				10
#define TYPE_BUF				11
#define TYPE_STRUCT_EXIST		12

#define APPEND_STRUCT_EXIST(__vt, __bool, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_STRUCT_EXIST;\
		__ld.struct_type = __st_type;\
		__ld.data.ch = __bool;\
		__vt.push_back( __ld );\
	}
	
#define DOES_STRUCT_EXIST(__ld, __st_type, __does_exist) {\
		if( __ld.type == TYPE_STRUCT_EXIST && __ld.struct_type == __st_type && \
			__ld.data.ch ) {\
			__does_exist = true;\
		} else {\
			__does_exist = false;\
		}\
	}

#define APPEND_INT(__vt, __int, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_INT;\
		__ld.struct_type = __st_type;\
		__ld.data.sint = __int;\
		__vt.push_back( __ld );\
	}

#define READ_INT(__ld, __ret_int) {\
		if( __ld.type == TYPE_INT ) {\
			__ret_int = __ld.data.sint;\
		}\
	}

#define APPEND_SIZE(__vt, __size, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_SIZE;\
		__ld.struct_type = __st_type;\
		__ld.data.size = __size;\
		__vt.push_back( __ld );\
	}

#define READ_SIZE(__ld, __ret_size) {\
		if( __ld.type == TYPE_SIZE ) {\
			__ret_size = __ld.data.size;\
		}\
	}

#define APPEND_SSIZE(__vt, __size, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_SSIZE;\
		__ld.struct_type = __st_type;\
		__ld.data.ssize = __size;\
		__vt.push_back( __ld );\
	}

#define READ_SSIZE(__ld, __ret_size) {\
		if( __ld.type == TYPE_SSIZE ) {\
			__ret_size = __ld.data.ssize;\
		}\
	}

#define APPEND_UINT(__vt, __uint, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_UINT;\
		__ld.struct_type = __st_type;\
		__ld.data.uint = __uint;\
		__vt.push_back( __ld );\
	}

#define READ_UINT(__ld, __ret_uint) {\
		if( __ld.type == TYPE_UINT ) {\
			__ret_uint = __ld.data.uint;\
		}\
	}
		

#define APPEND_CHAR(__vt, __ch, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_CHAR;\
		__ld.struct_type = __st_type;\
		__ld.data.ch = __ch;\
		__vt.push_back( __ld );\
	}
	
#define READ_CHAR(__ld, __ret_ch) {\
		if( __ld.type == TYPE_CHAR ) {\
			__ret_ch = __ld.data.ch;\
		}\
	}

#define APPEND_STR(__vt, __pchar, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_STR;\
		__ld.struct_type = __st_type;\
		if( __pchar ) {\
			__ld.size = strlen(__pchar);\
			__ld.buf_smalloc = false;\
			__ld.data.str = (char*)malloc(__ld.size + 1);\
			memcpy( __ld.data.str, __pchar, __ld.size+1 );\
		} else {\
			__ld.buf_smalloc = false;\
			__ld.data.str = 0;\
			__ld.size = 0;\
		}\
		__vt.push_back( __ld );\
	}

#define READ_STR(__ld, __ret_str, __ret_len) {\
		if( __ld.type == TYPE_STR ) {\
			if( __ld.buf_smalloc ) {\
				if( __ld.data.str_offset == 0 ) {\
					__ret_str = 0;\
					__ret_len = 0;\
				} else {\
					__ret_str = (char*)sm_getptr( g_pshm_logs, __ld.data.str_offset );\
					__ret_len = __ld.size;\
				}\
			} else {\
				if( __ld.data.str == 0 ) {\
					__ret_str = 0;\
					__ret_len = 0;\
				} else {\
					__ret_str = (char*) __ld.data.str;\
					__ret_len = __ld.size;\
				}\
			}\
		}\
	}
	
#define APPEND_UINT64(__vt, __uint64, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_UINT64;\
		__ld.struct_type = __st_type;\
		__ld.data.uint64 = __uint64;\
		__vt.push_back( __ld );\
	}

#define READ_UINT64(__ld, __ret_uint64) {\
		if( __ld.type == TYPE_UINT64 ) {\
			__ret_uint64 = __ld.data.uint64;\
		}\
	}


#define APPEND_INT64(__vt, __int64, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_INT64;\
		__ld.struct_type = __st_type;\
		__ld.data.int64 = __int64;\
		__vt.push_back( __ld );\
	}
	   
	   
#define READ_INT64(__ld, __ret_int64) {\
		if( __ld.type == TYPE_INT64 ) {\
			__ret_int64 = __ld.data.int64data;\
		}\
	}	

#define APPEND_TIME(__vt, __time, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_TIME;\
		__ld.struct_type = __st_type;\
		__ld.data.time = __time;\
		__vt.push_back( __ld );\
	}
	   
	   
#define READ_TIME(__ld, __ret_time ) {\
		if( __ld.type == TYPE_TIME ) {\
			__ret_time = __ld.data.time;\
		}\
	}	


#define APPEND_CLOCK(__vt, __clock, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_CLOCK;\
		__ld.struct_type = __st_type;\
		__ld.data.clock = __clock;\
		__vt.push_back( __ld );\
	}
	   
	   
#define READ_CLOCK(__ld, __ret_clock ) {\
		if( __ld.type == TYPE_CLOCK ) {\
			__ret_clock = __ld.data.clock ;\
		}\
	}	
	
#define APPEND_BUF(__vt, __ptr, __size, __dbgname, __st_type) {\
		LOG_DATA __ld;\
		__ld.type = TYPE_BUF;\
		__ld.struct_type = __st_type;\
 		if( __ptr && __size > 0 ) {\
			__ld.size = __size;\
			__ld.buf_smalloc = false;\
			__ld.data.buf = malloc(__size);\
			memcpy( __ld.data.buf, __ptr, __size );\
		} else {\
			__ld.buf_smalloc = false;\
			__ld.data.buf = 0;\
			__ld.size = 0;\
		}\
		__vt.push_back( __ld );\
	}

	
#define READ_BUF(__ld, __ret_buf, __ret_len) {\
		if( __ld.type == TYPE_BUF ) {\
			if( __ld.buf_smalloc ) {\
				__ret_buf = sm_getptr( g_pshm_logs, __ld.data.buf_offset );\
				__ret_len = __ld.size;\
			} else {\
				__ret_buf = __ld.data.buf;\
				__ret_len = __ld.size;\
			}\
		}\
	}
	

typedef struct tag_FDINFO {
	string 			name;
	unsigned char 	type;
	string 			sysname;
} FDINFO;

#define DECLARE_FDINFO(__name, __type, __sysname) \
	FDINFO fdinfo;\
	fdinfo.name = __name;\
	fdinfo.type = __type;\
	fdinfo.sysname = __sysname;

#define DECLARE_FDINFO1(__name) \
	FDINFO fdinfo;\
	fdinfo.name = __name;
	
#define FD_TYPE_FILE		0
#define FD_TYPE_NETWORK		1
#define FD_TYPE_EVENT		2

//map<int, FDINFO> g_mapFd;
map<FILE*, FDINFO> g_mapFILE;
map<DIR*, FDINFO> g_mapDIR;
map<string, FDINFO> g_mapIp2HostName;

map<z_streamp, FDINFO> g_mapZStream;


typedef struct tag_LOG_DATA {
	unsigned char 	type;
	unsigned char 	struct_type;
	ssize_t 		size;
	bool 			buf_smalloc;
	union tag_u_data {
		ssize_t 		ssize;
		size_t 			size;
		char 			ch;
		int 			sint;
		unsigned int 	uint;
		long long int 	int64;
		uint64_t 		uint64;
		unsigned int 	buf_offset;
		unsigned int 	str_offset;
		time_t 			time;
		clock_t 		clock;
		void* 			buf;
		char* 			str;
	} data;
} LOG_DATA;

typedef struct tag_LOG_DATA_ARY {
	int 				cnt;
	int 				idx;
	unsigned int 		ary_offset;
	//LOG_DATA 		*ary;
} LOG_DATA_ARY;

typedef struct tag_RSRC_LOG_ELEM {
	bool 			 	used;
	int  	  		 	syscall;
	LOG_DATA_ARY 		ret;
	LOG_DATA_ARY 		arg;
} RSRC_LOG_ELEM;

typedef struct tag_RSRC_LOG_ELEM_ARY {
	int 				used_cnt;
	int 				cnt;
	int 				idx;
	unsigned int 		ary_offset;
	//RSRC_LOG_ELEM *ary;
} RSRC_LOG_ELEM_ARY;

typedef struct tag_RSRC_DATA {
	RSRC_LOG_ELEM_ARY 	logInput;
	RSRC_LOG_ELEM_ARY 	logOutput;
} RSRC_DATA;

/*
typedef struct tag_RSRC_DATA_DELAY {
	string filename;
	string filename_output;
	bool loaded;
	string key;
	map<string, struct tag_RSRC_DATA_DELAY*>* pmapRsrcData;
	
	RSRC_LOG_ELEM_ARY logInput;
	RSRC_LOG_ELEM_ARY logOutput;
	
	tag_RSRC_DATA_DELAY()
	{
		loaded = false;
	}
} RSRC_DATA_DELAY;
*/

map<string, string> 			*g_pmapRsrcMap = 0;
map<string, unsigned int> 		*g_pmapResource = 0;

//map<int, string> g_map_fd_name;
inline string GetSocketHostName( string ip );
inline string sockaddr_to_string(const struct sockaddr *sockaddr, int len, bool trans_ip_to_domain, bool remove_port = false);
inline void LogResource(string category, string rsrc, int syscall, vector<LOG_DATA> &vtret, vector<LOG_DATA> &vtargs, SHMEM_OBJ* shmem = g_pshm_logs, bool output = false);
inline int LoadResource(string category, string rsrc, int syscall, vector<LOG_DATA> &vtret, vector<LOG_DATA> &vtargs, SHMEM_OBJ* shmem = g_pshm_logs, bool output = false);
inline int LoadResourceTest(string category, string rsrc, int syscall, vector<LOG_DATA> &vtret, vector<LOG_DATA> &vtargs, SHMEM_OBJ* shmem = g_pshm_logs, bool output = false);
inline void LoadEachResource(FILE* f, string key, bool isOutput, map<string, unsigned int> &mapRsrcData, SHMEM_OBJ* shmem);
inline void LoadEachResourceShmem(FILE* f, string key, bool isOutput, map<string, unsigned int> &mapRsrcData, SHMEM_OBJ* shmem);

bool do_not_print_log_rsrc_key(string& rsrc_key);

inline string GetFileName(string path)
{
	string name = path;
	size_t found = path.rfind( '/' );
	if (found!=string::npos) {
		name = path.substr( found + 1 );
	}
	return name;
}

inline void GetRsrcName(string category, string s, string& retkey, string& retvalue)
{	
	static map<string, string> g_mapRsrcName;
	map<string, string>::iterator it;
	
	retkey = category + string("/") + s;
	it = g_mapRsrcName.find( retkey );
	if( it != g_mapRsrcName.end() ) { // found
		retvalue = it->second;
		return ;
	}
	
	string name = GetFileName(s);
	unsigned int crcval = crc16( s.c_str(), s.length() );
	
	
	if( s.length() > 1024 ) {
		char* out = (char*)malloc( s.length() + 40 ) ;	
		sprintf( out, "%s:%d", name.c_str(), crcval );
		retvalue = category + string("/") + string(out);
		free(out);
	} else {
		char out[1024 + 40];
		sprintf( out, "%s:%d", name.c_str(), crcval );
		retvalue = category + string("/") + string(out);
	}
	//
	// log the map.
	//
	if(orgfn_fopen && orgfn_fwrite && orgfn_fclose) {
		char path[1024];
		sprintf( path, "%s/rsrc_map", __home_dir );
		
		
		FILE* f = orgfn_fopen(path, "at");
		if( f ) {
			const char* sep = "<->";
			const char* newline = "\n";
			//orgfn_fwrite( &vt[i].type, sizeof(vt[i].type), 1, f );
			orgfn_fwrite( retkey.c_str(), retkey.length(), 1, f );
			orgfn_fwrite( sep, 3, 1, f );
			orgfn_fwrite( retvalue.c_str(), retvalue.length(), 1, f );
			orgfn_fwrite( newline, 1, 1, f );
			orgfn_fclose(f);
		}
	}
	
	g_mapRsrcName.insert( pair<string, string>(retkey, retvalue) );
}

/*
 * 
typedef struct tag_LOG_DATA {
	int type;
	int size;
	int struct_type;
	char* dbg_name;
	
	union data {
		char chdata;
		int intdata;
		unsigned int uintdata;
		long long int int64data;
		long long unsigned int uint64;
		void* buf;
		char* str;
	}
} LOG_DATA;
 */
inline string print_log_data( vector<LOG_DATA> &vt, SHMEM_OBJ* shm ) 
{
	string ret;
	char tmp[128];
	for( unsigned int i = 0;i < vt.size(); i++ ) {
		string cur;
		switch( vt[i].type ) {
			case TYPE_CHAR:
				if( vt[i].data.ch == 0 ) { cur = '\\'; cur += '0'; }
				else if( vt[i].data.ch == '\r' ) { cur = '\\'; cur += 'r'; }
				else if( vt[i].data.ch == '\n' ) { cur = '\\'; cur += 'n'; }
				else cur = vt[i].data.ch;
				break;
			case TYPE_INT:
				sprintf( tmp, "INT:%d", vt[i].data.sint ); 
				cur = string(tmp);
				break;
			case TYPE_SIZE:
				sprintf( tmp, "SIZE:%lu", vt[i].data.size ); 
				cur = string(tmp);
				break;
			case TYPE_SSIZE:
				sprintf( tmp, "SSIZE:%ld", vt[i].data.ssize ); 
				cur = string(tmp);
				break;
			case TYPE_TIME:
				sprintf( tmp, "TIME:%ld", vt[i].data.time ); 
				cur = string(tmp);
				break;
			case TYPE_CLOCK:
				sprintf( tmp, "CLOCK:%ld", vt[i].data.clock ); 
				cur = string(tmp);
				break;
			case TYPE_UINT:
				sprintf( tmp, "UINT:%u", vt[i].data.uint ); 
				cur = string(tmp);
				break;
			case TYPE_INT64:
				sprintf( tmp, "INT64:%lld", vt[i].data.int64 ); 
				cur = string(tmp);
				break;
			case TYPE_UINT64:
				sprintf( tmp, "UINT64:%" PRIu64, vt[i].data.uint64 ); 
				cur = string(tmp);
				break;
			case TYPE_STR:
				if( vt[i].buf_smalloc ) {
					if( vt[i].data.str_offset ) cur = string("STR:") + string((const char*)sm_getptr(shm, vt[i].data.str_offset));
					else cur = string("STR:");
				} else {
					if( vt[i].data.str ) cur = string("STR:") + string(vt[i].data.str);
					else cur = string("STR:");
				}
				break;
			case TYPE_BUF:
				if( vt[i].buf_smalloc ) {
					cur = string("BUF:") + P2S( sm_getptr(shm, vt[i].data.buf_offset), vt[i].size );
				} else {
					cur = string("BUF:") + P2S( vt[i].data.buf, vt[i].size );
				}
				break;
			case TYPE_STRUCT_EXIST:
				{
					const char* name = GetStructTypeName( vt[i].struct_type );
					if( name[0] != 0 ) {
						cur = string(name);
						if( vt[i].data.ch == 1 ) {
							cur += string(" exist:");
						} else {
							cur += string(" not exist:");
						} 
					}
				}
				break;
		}
		
		ret += cur + '\n';
	}
	return ret;
}

#define __DEBUG_FILE_LINE	(__debug_fname ? __debug_fname : __FILE__), (__debug_line >= 0 ? __debug_line : __LINE__)


inline bool CompareLogData( vector<LOG_DATA> &vt1, vector<LOG_DATA> &vt2, SHMEM_OBJ* shm, const char* __debug_fname = 0, int __debug_line = -1  ) 
{
	string ret;
	char tmp[128];
	
	if( vt1.size() != vt2.size() ) {
		DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
		return false;
	}
	
	for( unsigned int i = 0;i < vt1.size(); i++ ) {
		string cur;
		if( vt1[i].type != vt2[i].type ) {
			DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
			return false;
		}
		switch( vt1[i].type ) {
			case TYPE_CHAR:
				if( vt1[i].data.ch != vt2[i].data.ch ) {
					DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_INT:
				if( vt1[i].data.sint != vt2[i].data.sint ) {
					DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_SIZE:
				if( vt1[i].data.size != vt2[i].data.size ) {
					DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_SSIZE:
				if( vt1[i].data.ssize != vt2[i].data.ssize ) {
					DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_TIME:
				if( vt1[i].data.time != vt2[i].data.time ) {
					DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_CLOCK:
				if( vt1[i].data.clock != vt2[i].data.clock ) {
					DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_UINT:
				if( vt1[i].data.uint != vt2[i].data.uint ) {
					DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_INT64:
				if( vt1[i].data.int64 != vt2[i].data.int64 ) {
					DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_UINT64:
				if( vt1[i].data.uint64 != vt2[i].data.uint64 ) {
					DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_STR:
				{
					char* str1 = 0;
					char* str2 = 0;
					
					if( vt1[i].buf_smalloc ) {
						if( vt1[i].data.str_offset ) str1 = (char*)sm_getptr(shm, vt1[i].data.str_offset);
						else str1 = 0;
					} else {
						str1 = vt1[i].data.str;
					}
					if( vt2[i].buf_smalloc ) {
						if( vt2[i].data.str_offset ) str2 = (char*)sm_getptr(shm, vt2[i].data.str_offset);
						else str2 = 0;
					} else {
						str2 = vt2[i].data.str;
					}
					
					
					if( (str1 == 0 && str2 != 0) ||
						(str1 != 0 && str2 == 0) ) {
						DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
						return false;
					}
					if( str1 && str2 ) {
						if( strcmp( str1, str2 ) != 0 ) {
							DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
							return false;
						}
					}
				}
				break;
			case TYPE_BUF:
				{
					void* buf1 = 0;
					void* buf2 = 0;
					
					if( vt1[i].buf_smalloc ) {
						if( vt1[i].data.buf_offset ) buf1 = sm_getptr(shm, vt1[i].data.buf_offset);
						else buf1 = 0;
					} else {
						buf1 = vt1[i].data.buf;
					}
					if( vt2[i].buf_smalloc ) {
						if( vt2[i].data.buf_offset ) buf2 = sm_getptr(shm, vt2[i].data.buf_offset);
						else buf2 = 0;
					} else {
						buf2 = vt2[i].data.buf;
					}
					
					if( (buf1 == 0 && buf2 != 0) ||
						(buf1 != 0 && buf2 == 0) ) {
						DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
						return false;
					}
					if( vt1[i].size != vt2[i].size ) {
						DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
						return false;
					}
					if( buf1 && buf2 ) {
						if( memcmp( buf1, buf2, vt1[i].size ) != 0 ) {
							DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
							return false;
						}
					}

					
				}
				break;
			case TYPE_STRUCT_EXIST:
				if( vt1[i].struct_type != vt2[i].struct_type ) {
					DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				if( vt1[i].data.ch != vt2[i].data.ch ) {
					DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
		}
	}
	return true;
}


inline bool CompareLogData( vector<LOG_DATA> &vt1, LOG_DATA* vt2, int vt2_size, SHMEM_OBJ* shm, const char* __debug_fname = 0, int __debug_line = -1 ) 
{
	string ret;
	char tmp[128];
	
	if( vt1.size() != vt2_size ) {
		DEBUGMSG("CompareLogData-size: (%d != %d) %s:%d\n", vt1.size(), vt2_size, __DEBUG_FILE_LINE);
		return false;
	}
	
	for( unsigned int i = 0;i < vt1.size(); i++ ) {
		string cur;
		if( vt1[i].type != vt2[i].type ) {
			DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
			return false;
		}
		switch( vt1[i].type ) {
			case TYPE_CHAR:
				if( vt1[i].data.ch != vt2[i].data.ch ) {
					//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_INT:
				if( vt1[i].data.sint != vt2[i].data.sint ) {
					//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_SIZE:
				if( vt1[i].data.size != vt2[i].data.size ) {
					//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_SSIZE:
				if( vt1[i].data.ssize != vt2[i].data.ssize ) {
					//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_TIME:
				if( vt1[i].data.time != vt2[i].data.time ) {
					//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_CLOCK:
				if( vt1[i].data.clock != vt2[i].data.clock ) {
					//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_UINT:
				if( vt1[i].data.uint != vt2[i].data.uint ) {
					//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_INT64:
				if( vt1[i].data.int64 != vt2[i].data.int64 ) {
					//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_UINT64:
				if( vt1[i].data.uint64 != vt2[i].data.uint64 ) {
					//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
			case TYPE_STR:
				{
					char* str1 = 0;
					char* str2 = 0;
					
					if( vt1[i].buf_smalloc ) {
						if( vt1[i].data.str_offset ) str1 = (char*)sm_getptr(shm, vt1[i].data.str_offset);
						else str1 = 0;
					} else {
						str1 = vt1[i].data.str;
					}
					if( vt2[i].buf_smalloc ) {
						if( vt2[i].data.str_offset ) str2 = (char*)sm_getptr(shm, vt2[i].data.str_offset);
						else str2 = 0;
					} else {
						str2 = vt2[i].data.str;
					}
					
					
					if( (str1 == 0 && str2 != 0) ||
						(str1 != 0 && str2 == 0) ) {
						//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
						return false;
					}
					if( str1 && str2 ) {
						if( strcmp( str1, str2 ) != 0 ) {
							//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
							return false;
						}
					}
				}
				break;
			case TYPE_BUF:
				{
					void* buf1 = 0;
					void* buf2 = 0;
					
					if( vt1[i].buf_smalloc ) {
						if( vt1[i].data.buf_offset ) buf1 = sm_getptr(shm, vt1[i].data.buf_offset);
						else buf1 = 0;
					} else {
						buf1 = vt1[i].data.buf;
					}
					if( vt2[i].buf_smalloc ) {
						if( vt2[i].data.buf_offset ) buf2 = sm_getptr(shm, vt2[i].data.buf_offset);
						else buf2 = 0;
					} else {
						buf2 = vt2[i].data.buf;
					}
					
					if( (buf1 == 0 && buf2 != 0) ||
						(buf1 != 0 && buf2 == 0) ) {
						//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
						return false;
					}
					if( vt1[i].size != vt2[i].size ) {
						//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
						return false;
					}
					if( buf1 && buf2 ) {
						if( memcmp( buf1, buf2, vt1[i].size ) != 0 ) {
							//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
							return false;
						}
					}
				}
				break;
			case TYPE_STRUCT_EXIST:
				if( vt1[i].struct_type != vt2[i].struct_type ) {
					//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				if( vt1[i].data.ch != vt2[i].data.ch ) {
					//DEBUGMSG("CompareLogData: %s:%d\n", __DEBUG_FILE_LINE);
					return false;
				}
				break;
		}
	}
	
	//DEBUGMSG("same!!\n");
	return true;
}


inline void WriteBinLogData( vector<LOG_DATA> &vt, FILE* f, SHMEM_OBJ* shm ) 
{
	//string ret;
	//char tmp[128];
	unsigned int cnt = vt.size();
	orgfn_fwrite( &cnt, sizeof(cnt), 1, f );
	
	for( unsigned int i = 0;i < vt.size(); i++ ) {
		orgfn_fwrite( &vt[i].type, sizeof(vt[i].type), 1, f );
		//string cur;
		switch( vt[i].type ) {
			case TYPE_CHAR:
				orgfn_fwrite( &vt[i].data.ch, sizeof(vt[i].data.ch), 1, f );
				break;
			case TYPE_INT:
				orgfn_fwrite( &vt[i].data.sint, sizeof(vt[i].data.sint), 1, f );
				break;
			case TYPE_SIZE:
				orgfn_fwrite( &vt[i].data.size, sizeof(vt[i].data.size), 1, f );
				break;
			case TYPE_SSIZE:
				orgfn_fwrite( &vt[i].data.ssize, sizeof(vt[i].data.ssize), 1, f );
				break;
			case TYPE_TIME:
				orgfn_fwrite( &vt[i].data.time, sizeof(vt[i].data.time), 1, f );
				break;
			case TYPE_CLOCK:
				orgfn_fwrite( &vt[i].data.clock, sizeof(vt[i].data.clock), 1, f );
				break;
			case TYPE_UINT:
				orgfn_fwrite( &vt[i].data.uint, sizeof(vt[i].data.uint), 1, f );
				break;
			case TYPE_INT64:
				orgfn_fwrite( &vt[i].data.int64, sizeof(vt[i].data.int64), 1, f );
				break;
			case TYPE_UINT64:
				orgfn_fwrite( &vt[i].data.uint64, sizeof(vt[i].data.uint64), 1, f );
				break;
			case TYPE_STR:
				{
					char* str = 0;
					if( vt[i].buf_smalloc ) {
						str = (char*)sm_getptr(shm, vt[i].data.str_offset);
					} else {
						str = vt[i].data.str;
					}
					if( str ) {
						ssize_t len = strlen(str);
						orgfn_fwrite( &len, sizeof(len), 1, f );
						orgfn_fwrite( str, 1, len, f );
					} else {
						ssize_t len = -1;
						orgfn_fwrite( &len, sizeof(len), 1, f );
					}
				}
				break;
			case TYPE_BUF:
				{
					void* buf = 0;
					if( vt[i].buf_smalloc ) {
						buf = (void*)sm_getptr(shm, vt[i].data.buf_offset);
					} else {
						buf = vt[i].data.buf;
					}
					
					if( buf ) {
						ssize_t len = vt[i].size;
						orgfn_fwrite( &len, sizeof(len), 1, f );
						orgfn_fwrite( buf, 1, len, f );
					} else {
						ssize_t len = -1;
						orgfn_fwrite( &len, sizeof(len), 1, f );
					}
					
				}
				break;
			case TYPE_STRUCT_EXIST:
				{
					orgfn_fwrite( &vt[i].struct_type, sizeof(vt[i].struct_type), 1, f );
					orgfn_fwrite( &vt[i].data.ch, sizeof(vt[i].data.ch), 1, f );
					/*
					const char* name = GetStructTypeName( vt[i].struct_type );
					if( name[0] != 0 ) {
						cur = string(name);
						if( vt[i].data.intdata == 1 ) {
							cur += string(" exist:");
						} else {
							cur += string(" not exist:");
						} 
					}
					*/
				}
				break;
		}
		//ret += cur + '\n';
	}
	//return ret;
}

inline LOG_DATA_ARY ReadBinLogData( FILE* f, int &ret_cnt, SHMEM_OBJ* shmem ) 
{
	//string ret;
	//char tmp[128];
	unsigned int cnt = 0;//vt.size();
	orgfn_fread( &cnt, sizeof(cnt), 1, f );
	//DEBUGMSG("DEBUGMSG: cnt: %d %X\n", cnt, ftell(f) );
	if( cnt < 0) {
		exit(-1);
	}
	
	vector<LOG_DATA> reglogs;
	for( unsigned int i = 0;i < cnt; i++ ) {
		LOG_DATA elem;
		ssize_t len = 0;
		//orgfn_fread( &vt[i].type, sizeof(vt[i].type), 1, f );
		orgfn_fread( &elem.type, sizeof(elem.type), 1, f );
		//DEBUGMSG("ReadBinLogData: type: %d\n", elem.type );
		
		//string cur;
		switch( elem.type ) {
			case TYPE_CHAR:
				orgfn_fread( &elem.data.ch, sizeof(elem.data.ch), 1, f );
				break;
			case TYPE_INT:
				orgfn_fread( &elem.data.sint, sizeof(elem.data.sint), 1, f );
				break;
			case TYPE_SIZE:
				orgfn_fread( &elem.data.size, sizeof(elem.data.size), 1, f );
				break;
			case TYPE_SSIZE:
				orgfn_fread( &elem.data.ssize, sizeof(elem.data.ssize), 1, f );
				break;
			case TYPE_TIME:
				orgfn_fread( &elem.data.time, sizeof(elem.data.time), 1, f );
				break;
			case TYPE_CLOCK:
				orgfn_fread( &elem.data.clock, sizeof(elem.data.clock), 1, f );
				break;
			case TYPE_UINT:
				orgfn_fread( &elem.data.uint, sizeof(elem.data.uint), 1, f );
				break;
			case TYPE_INT64:
				orgfn_fread( &elem.data.int64, sizeof(elem.data.int64), 1, f );
				break;
			case TYPE_UINT64:
				orgfn_fread( &elem.data.uint64, sizeof(elem.data.uint64), 1, f );
				break;
			case TYPE_STR:
				orgfn_fread( &len, sizeof(len), 1, f );
				if( len == -1 ) {
					elem.size = len;
					elem.data.str_offset = 0;
					elem.buf_smalloc = true;
				} else {
					elem.size = len;
					elem.data.str_offset = smalloc(shmem, len+1, true);
					elem.buf_smalloc = true;
					orgfn_fread( sm_getptr(shmem, elem.data.str_offset), 1, len, f );
				}
				break;
			case TYPE_BUF:
				orgfn_fread( &len, sizeof(len), 1, f );
				//printf("ReadBinLogData: buf.len: %d\n", len );
				if( len == -1 ) {
					elem.data.buf_offset = 0;
					elem.buf_smalloc = true;
					elem.size = len;
				} else {
					elem.data.buf_offset = smalloc(shmem, len, false);//malloc(len);
					elem.buf_smalloc = true;
					elem.size = len;
					orgfn_fread( sm_getptr(shmem, elem.data.buf_offset), 1, len, f );
					//int len = -1;
					//orgfn_fwrite( &len, sizeof(len), 1, f );
				}
				break;
			case TYPE_STRUCT_EXIST:
				{
					orgfn_fread( &elem.struct_type, sizeof(elem.struct_type), 1, f );
					orgfn_fread( &elem.data.ch, sizeof(elem.data.ch), 1, f );
					/*
					const char* name = GetStructTypeName( vt[i].struct_type );
					if( name[0] != 0 ) {
						cur = string(name);
						if( vt[i].data.intdata == 1 ) {
							cur += string(" exist:");
						} else {
							cur += string(" not exist:");
						} 
					}
					*/
				}
				break;
		}
		reglogs.push_back( elem );
		//ret += cur + '\n';
	}
	//return ret;
	
	LOG_DATA_ARY retary;
	ret_cnt = reglogs.size();
	//DEBUGMSG("ret_cnt = %x * sizeof(LOG_DATA) = %x\n", ret_cnt, sizeof(LOG_DATA));
	retary.cnt = ret_cnt;
	retary.ary_offset = smalloc(shmem, sizeof(LOG_DATA)*ret_cnt, false); //new LOG_DATA[ret_cnt];
	LOG_DATA* pLD = (LOG_DATA*)sm_getptr(shmem, retary.ary_offset);
	for( unsigned int i = 0; i < reglogs.size(); i++ ) {
		pLD[i] = reglogs[i];
	}
	
	return retary;
}

inline LOG_DATA_ARY ReadBinLogDataTest( FILE* f, int &ret_cnt, SHMEM_OBJ* shmem ) 
{
	//string ret;
	//char tmp[128];
	unsigned int cnt = 0;//vt.size();
	orgfn_fread( &cnt, sizeof(cnt), 1, f );
	DEBUGMSG("DEBUGMSG: cnt: %d %X\n", cnt, ftell(f) );
	if( cnt < 0) {
		exit(-1);
	}
	
	vector<LOG_DATA> reglogs;
	for( unsigned int i = 0;i < cnt; i++ ) {
		LOG_DATA elem;
		ssize_t len = 0;
		//orgfn_fread( &vt[i].type, sizeof(vt[i].type), 1, f );
		orgfn_fread( &elem.type, sizeof(elem.type), 1, f );
		DEBUGMSG("ReadBinLogData: type: %d (fpos = %X)\n", elem.type, ftell(f) );
		
		//string cur;
		switch( elem.type ) {
			case TYPE_CHAR:
				orgfn_fread( &elem.data.ch, sizeof(elem.data.ch), 1, f );
				break;
			case TYPE_INT:
				orgfn_fread( &elem.data.sint, sizeof(elem.data.sint), 1, f );
				break;
			case TYPE_SIZE:
				orgfn_fread( &elem.data.size, sizeof(elem.data.size), 1, f );
				break;
			case TYPE_SSIZE:
				orgfn_fread( &elem.data.ssize, sizeof(elem.data.ssize), 1, f );
				break;
			case TYPE_TIME:
				orgfn_fread( &elem.data.time, sizeof(elem.data.time), 1, f );
				break;
			case TYPE_CLOCK:
				orgfn_fread( &elem.data.clock, sizeof(elem.data.clock), 1, f );
				break;
			case TYPE_UINT:
				orgfn_fread( &elem.data.uint, sizeof(elem.data.uint), 1, f );
				break;
			case TYPE_INT64:
				orgfn_fread( &elem.data.int64, sizeof(elem.data.int64), 1, f );
				break;
			case TYPE_UINT64:
				orgfn_fread( &elem.data.uint64, sizeof(elem.data.uint64), 1, f );
				break;
			case TYPE_STR:
				orgfn_fread( &len, sizeof(len), 1, f );
				if( len <= 0 ) {
					elem.size = len;
					elem.data.str_offset = 0;
					elem.buf_smalloc = true;
				} else {
					elem.size = len;
					elem.data.str_offset = smalloc(shmem, len+1, true);
					elem.buf_smalloc = true;
					orgfn_fread( sm_getptr(shmem, elem.data.str_offset), 1, len, f );
				}
				DEBUGMSG("TYPE_STR: len = %d\n", len);
				
				break;
			case TYPE_BUF:
				orgfn_fread( &len, sizeof(len), 1, f );
				//printf("ReadBinLogData: buf.len: %d\n", len );
				if( len <= 0 ) {
					elem.data.buf_offset = 0;
					elem.buf_smalloc = true;
					elem.size = len;
				} else {
					elem.data.buf_offset = smalloc(shmem, len, false);//malloc(len);
					elem.buf_smalloc = true;
					elem.size = len;
					orgfn_fread( sm_getptr(shmem, elem.data.buf_offset), 1, len, f );
					//int len = -1;
					//orgfn_fwrite( &len, sizeof(len), 1, f );
				}
				DEBUGMSG("TYPE_BUF: len = %d (fpos = %X)\n", len, ftell(f));
				break;
			case TYPE_STRUCT_EXIST:
				{
					orgfn_fread( &elem.struct_type, sizeof(elem.struct_type), 1, f );
					orgfn_fread( &elem.data.ch, sizeof(elem.data.ch), 1, f );
					/*
					const char* name = GetStructTypeName( vt[i].struct_type );
					if( name[0] != 0 ) {
						cur = string(name);
						if( vt[i].data.intdata == 1 ) {
							cur += string(" exist:");
						} else {
							cur += string(" not exist:");
						} 
					}
					*/
				}
				break;
		}
		reglogs.push_back( elem );
		//ret += cur + '\n';
	}
	//return ret;
	
	LOG_DATA_ARY retary;
	ret_cnt = reglogs.size();
	//DEBUGMSG("ret_cnt = %x * sizeof(LOG_DATA) = %x\n", ret_cnt, sizeof(LOG_DATA));
	retary.cnt = ret_cnt;
	retary.ary_offset = smalloc(shmem, sizeof(LOG_DATA)*ret_cnt, false); //new LOG_DATA[ret_cnt];
	LOG_DATA* pLD = (LOG_DATA*)sm_getptr(shmem, retary.ary_offset);
	for( unsigned int i = 0; i < reglogs.size(); i++ ) {
		pLD[i] = reglogs[i];
	}
	
	return retary;
}


inline void WriteBinLog(FILE* f, int syscall, vector<LOG_DATA> &vtret, vector<LOG_DATA> &vtargs, SHMEM_OBJ* shmem, string& rsrc)
{
	//long int pos = ftell(f);
	orgfn_fwrite( &syscall, sizeof(syscall), 1, f );
	
	if( syscall == 83 ) {  
		if( strcmp( rsrc.c_str(), "/tmp/.X11-unix/X0:unix" ) == 0 ) {
			static int n = 0;
			WriteBinLogData( vtret, f, shmem );
			//DEBUGMSG("WriteBinLog............: (starts@ %X) [%d] after ret %X [size = %d] ", pos, n++, ftell(f), vtret.size() );
			WriteBinLogData( vtargs, f, shmem );
			//DEBUGMSG(" after args %X [size = %d]\n", ftell(f), vtargs.size() );
		} else {
			WriteBinLogData( vtret, f, shmem );
			WriteBinLogData( vtargs, f, shmem );
		}
	} else {
		WriteBinLogData( vtret, f, shmem );
		WriteBinLogData( vtargs, f, shmem );
	}
	
//	if( syscall == 83 && vtargs.size() != 1 ) {  
//		DEBUGMSG("WriteBinLog............: arg.cnt = %d\n", vtargs.size() );
//	}
}

inline bool ReadBinLog(FILE* f, RSRC_LOG_ELEM& elem, SHMEM_OBJ* shmem, string& rsrc)
{
	size_t ret = orgfn_fread( &(elem.syscall), sizeof(elem.syscall), 1, f );
	//DEBUGMSG("elem.syscall = %d\n", elem.syscall);
	if( ret <= 0 ) {
		return false;
	}
	if( elem.syscall == 83 ) { 
		if( strcmp( rsrc.c_str(), "NETWORK//tmp/.X11-unix/X0:unix" ) == 0 ) {
			static int n = 0;
			if( n++ == 62 ) {
				elem.ret = ReadBinLogDataTest( f, elem.ret.cnt, shmem );
				//DEBUGMSG("================= after ret fpos = %X\n",ftell(f));
				elem.arg = ReadBinLogDataTest( f, elem.arg.cnt, shmem );
				//DEBUGMSG("================= after arg fpos = %X\n",ftell(f));
			} else {
				elem.ret = ReadBinLogData( f, elem.ret.cnt, shmem );
				//DEBUGMSG("================= after ret fpos = %X\n",ftell(f));
				elem.arg = ReadBinLogData( f, elem.arg.cnt, shmem );
				//DEBUGMSG("================= after arg fpos = %X\n",ftell(f));
			}
		} else {
			elem.ret = ReadBinLogData( f, elem.ret.cnt, shmem );
			elem.arg = ReadBinLogData( f, elem.arg.cnt, shmem );
		}
	} else {
		elem.ret = ReadBinLogData( f, elem.ret.cnt, shmem );
		elem.arg = ReadBinLogData( f, elem.arg.cnt, shmem );
	}
	//printf("ret_syscall:%d\n", ret_syscall);
	//elem.ret = ReadBinLogData( f, elem.ret.cnt, shmem );
	//DEBUGMSG("ReadBinLogData - ret\n", elem.syscall);
	//elem.arg = ReadBinLogData( f, elem.arg.cnt, shmem );
	//DEBUGMSG("ReadBinLogData - arg\n", elem.syscall);
	
	if( elem.syscall == 83 ) { 
		if( strcmp( rsrc.c_str(), "NETWORK//tmp/.X11-unix/X0:unix" ) == 0 ) {
			static int n = 0;
			if( elem.arg.cnt != 2 ) {
				DEBUGMSG("cnt: %d [ZERO??] ReadBinLog............: [%s] arg.cnt = %d, ret.cnt = %d (fpos %X)\n", n++, rsrc.c_str(), elem.arg.cnt, elem.ret.cnt, ftell(f) );
			} else {
				//DEBUGMSG("cnt: %d ReadBinLog............: [%s] arg.cnt = %d, ret.cnt = %d (fpos = %X)\n", n++, rsrc.c_str(), elem.arg.cnt, elem.ret.cnt, ftell(f) );
			}
		}
	}
	return true;
}

inline int ReadMap(FILE* f, map<string, string> &ret, const char* debugmsg)
{
	map<string, string>::iterator it;
	char* line = 0;
	size_t len = 0;
	ssize_t read;
	//static map<const char*, const char*, cmp_str> temp;
	
	while ((read = getline(&line, &len, f)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);
		char* psep = strstr(line, "<->");
		char* key = line;
		char* val = psep + 3;
		*psep = 0;
		if( val [ strlen(val) -1 ] == '\n' ) {
			val [ strlen(val) -1 ] = 0;
		}
		//printf("K'%s'\n", key);
		//printf("V'%s'\n", val);
		
		it = ret.find( key );
		if( it != ret.end() ) {
			if( strcmp(it->second.c_str(), val) != 0 ) {
				printf("[%s] Conflicts!!\n", debugmsg);
				exit(-1);
			}
		} else {
			ret.insert( pair<string, string>( key, val ) );
		}
    }
    if( line ) {
		free(line);
	}
}

inline int PrintMap(map<string, string> &rsrcmap) 
{
	map<string, string>::iterator it;
	for( it = rsrcmap.begin(); it != rsrcmap.end(); it++ ) {
		printf("K[%s] : V[%s]\n", it->first.c_str(), it->second.c_str());
	}
	return 1;
}


#define USE_OLD_DEBUGFILE		1
#define USE_TEXT_FORMAT_DUMP	1

inline void LogResource(string category, string rsrc, int syscall, vector<LOG_DATA> &vtret, vector<LOG_DATA> &vtargs, SHMEM_OBJ* shmem, bool output)
{
#if 0
	ENTER_USERFUNC;
	char path[4096];
	
	if(orgfn_fopen && orgfn_fwrite && orgfn_fclose) {
		string rsrc_key, rsrc_value;
		GetRsrcName(category, rsrc, rsrc_key, rsrc_value);
		FILE* f;
		if( output ) {
			sprintf( path, "%s/%s.output", __home_dir, rsrc_value.c_str() );
		} else {
			sprintf( path, "%s/%s", __home_dir, rsrc_value.c_str() );
		}
		
		f = orgfn_fopen( path, "at" );
		if( f ) {
			//static set<string> rsrcset;
			//string strpath = rsrc_value;
			//bool bExist = false;
			//if( rsrcset.find( strpath ) == rsrcset.end() ) {
			//	bExist = false;
			//	rsrcset.insert( strpath );
			//} else {
			//	bExist = true;
			//}// 
			
			//unsigned tnum = time(NULL);
			//if( strstr( path, "X0:unix:2201" ) ) {
				//DEBUGMSG("path: %s, pos = %X, tnum = %X\n", path, ftell(f), tnum);
			//}
			
			//if( bExist == false ) {
#ifndef DISABLE_DEBUGMSG2_MACRO
			//	DEBUGMSG2( f, "Resource: [%s]\n", rsrc.c_str());
#endif
			//}
#ifndef DISABLE_DEBUGMSG2_MACRO
			//printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			DEBUGMSG2( f, "[%s] R[%s] A[%s]\n", 
					    SYSCALL_NAMES[syscall], 
						print_log_data(vtret, shmem).c_str(), 
						print_log_data(vtargs, shmem).c_str() );
#endif
			//orgfn_fflush( f ) ;
			orgfn_fclose( f ) ;
		}
	
		// add bin for binary format
		strcat(path, ".bin");
		f = orgfn_fopen( path, "ab" );
		if( f ) {
			//if( strstr( path, "X0:unix:2201.bin" ) ) {
			//	DEBUGMSG("path: %s, pos = %X\n", path, ftell(f));
			//}
			simplefu_mutex_lock( g_logrsrc_mutex );
			WriteBinLog( f, syscall, vtret, vtargs, shmem, rsrc );
			simplefu_mutex_unlock( g_logrsrc_mutex );
			
			orgfn_fclose( f ) ;
		}
	}
#if USE_OLD_DEBUGFILE
	//DEBUGMSG( "[%s|%s] [%s] R[%s] A[%s]\n", category.c_str(), rsrc.c_str(), SYSCALL_NAMES[syscall], 
	//		  print_log_data(vtret, shmem).c_str(), 
	//		  print_log_data(vtargs, shmem).c_str() );
#endif
	LEAVE_USERFUNC;
#endif
}

inline const char* get_LoadResource_errorcode(int e) {
	switch( e ) {
		case -1:
			return "ERROR_ALL_USED_UP";
		case -2:
			return "ERROR_NO_DATA_AVAILABLE";
		case -3:
			return "ERROR_NOT_FOUND_IN_MAP";
		case -4:
			return "ERROR_NO_MAP_EXIST";
	}
	return "ERROR_UNKNOWN";
}

inline int LoadResourceTest(string category, string rsrc, int syscall, vector<LOG_DATA> &vtret, vector<LOG_DATA> &vtargs, SHMEM_OBJ* shmem, bool output)
{
	int nResult = 0;
	ENTER_USERFUNC;
	char path[4096];
	
	if( g_pmapResource ) {
		string rsrc_key, rsrc_value;
		GetRsrcName(category, rsrc, rsrc_key, rsrc_value);
		//FILE* f;
		//if( output ) {
		//	sprintf( path, "%s/%s/%s.output", __home_dir, category.c_str(), rsrc_name.c_str() );
		//} else {
			//sprintf( path, "%s/%s/%s", __home_dir, category.c_str(), rsrc_name.c_str() );
		//}
		
		map<string, unsigned int>::iterator it = g_pmapResource->find( rsrc_key );
		if( it != g_pmapResource->end() ) {
			RSRC_DATA* pRsrcData = (RSRC_DATA*)sm_getptr(shmem,  it->second );
			
			//vector<RSRC_LOG_ELEM>* pvt = 0;
			RSRC_LOG_ELEM_ARY* plogary;
			int cnt_logs = 0;
			if( output ) { // output
				plogary = &(pRsrcData->logOutput);
			} else {
				plogary = &(pRsrcData->logInput);
			}
			
			bool bFound = false;
			RSRC_LOG_ELEM* pelem = (RSRC_LOG_ELEM*)sm_getptr(shmem, plogary->ary_offset);
			if( plogary->used_cnt == plogary->cnt ) {
				// all used up
				//if( strcmp( rsrc_key.c_str(), "TIME/gettimeofday:\\usr\\lib\\firefox\\libnspr4.so+21910")  == 0 ) {
				//	DEBUGMSG("gettimeofday:: all used up\n");
				//}
				DEBUGMSG("[ERROR] [%s] [%d] No Logged Data is available for this request -- all used up? [%d/%d] (%s:%d)\n", 
						 rsrc_key.c_str(), output, 
						 plogary->used_cnt, plogary->cnt, __FILE__, __LINE__);
				nResult = -1;
			} else {
				for( int i = 0; i < plogary->cnt; i++ ) {
					if( pelem[i].used == false ) {
						if( pelem[i].syscall == syscall ) {
							if( CompareLogData( vtargs, 
												(LOG_DATA*) sm_getptr(shmem,  pelem[i].arg.ary_offset ), 
												pelem[i].arg.cnt, shmem, __FILE__, __LINE__ ) == true ) {
								LOG_DATA* pLDRet = (LOG_DATA*)(sm_getptr(shmem, pelem[i].ret.ary_offset));
								
							
								if( strcmp( rsrc_key.c_str(), "FILE//usr/bin/firefox" ) == 0 ) {
									DEBUGMSG(" --consumed-- [%s] :: [%d] ; %d\n", SYSCALL_NAMES[ pelem[i].syscall ], pelem[i].used, plogary->used_cnt );
								}
							
								// same...
								plogary->used_cnt++;
								pelem[i].used = true;
								//DEBUGMSG("[%d] Matched!! (elem.vtret.size() = %d, i = %d / pvtsize = %d)\n", 
								//		 (int)get_thread_id(pthread_self()), 
								//		 (int)pelem[i].ret.cnt, 
								//		 (int)i, 
								//		 (int)logary.cnt);
								
								// copy returns
								VT_COPY2( vtret, pLDRet, pelem[i].ret.cnt );
								bFound = true;
								nResult = 1;
								
								if( strstr( rsrc_key.c_str(), "addrinfo" ) ) {
									//DEBUGMSG(" --consumed-- [%s] :: [%d] ; %d -- vtargs (%s) vtret(%s)\n", SYSCALL_NAMES[ pelem[i].syscall ], pelem[i].used, plogary->used_cnt, 
									//		 print_log_data(vtargs, shmem).c_str(), print_log_data(vtret, shmem).c_str() );
								}
								
								
								break;
							} else {
								//DEBUGMSG("CompareLogData Failed: <<%d>>\n", __LINE__);
							}
						} else {
							//DEBUGMSG("<<diff syscall: log[%d].syscall = %d vs. syscall = %d: %d>>\n", i, pelem[i].syscall, syscall, __LINE__);
						}
					} else {
						//DEBUGMSG("<<%d>>\n", __LINE__);
					}
				}
				
				if( !bFound ) {
					if( !do_not_print_log_rsrc_key(rsrc_key) ) 
					{
						DEBUGMSG("[ERROR] [%s] No Logged Data for this request (%s) -- no data avail (%s, used_cnt/cnt = %d/%d) (%s:%d)\n", 
								rsrc_key.c_str(),
								print_log_data(vtargs, shmem).c_str(),
								SYSCALL_NAMES[syscall], 
								plogary->used_cnt, plogary->cnt, __FILE__, __LINE__);
						//for( int i = 0; i < plogary->cnt; i++ ) {
							//DEBUGMSG(" --- [%s] :: [%d] ; %d\n", SYSCALL_NAMES[ pelem[i].syscall ], pelem[i].used, plogary->used_cnt );
						//}
					}
					nResult = -2;
				}
			}
		} else {
			// not found...
			if( !do_not_print_log_rsrc_key(rsrc_key) ) {
				DEBUGMSG("[ERROR] [%s] No Logged Data for this request (%s) -- not found in the map (%s:%d)\n", rsrc_key.c_str(), print_log_data(vtargs, shmem).c_str(), __FILE__, __LINE__);
			}
			
			/*
			{
				map<string, unsigned int>::iterator itemp = g_pmapResource->begin();
				for( ; itemp != g_pmapResource->end(); itemp++ ) {
					if( strstr( itemp->first.c_str(), "test-block-simple.pset" ) ) {
						DEBUGMSG("itemp ->first == %s\n", itemp->first.c_str());
						DEBUGMSG("COMPARE:\n");
						DEBUGMSG("%s\n", rsrc_key.c_str());
						DEBUGMSG("%s\n", itemp->first.c_str());
						DEBUGMSG("---\n");
						
						//break;
					}
				}
			}
			*/
			
			nResult = -3;
		}
	} else {
		DEBUGMSG("[ERROR] g_pmapResource is not loaded...(%s:%d)\n", __FILE__, __LINE__);
		nResult = -4;
	}
	LEAVE_USERFUNC;
	return nResult;
}

inline int LoadResource(string category, string rsrc, int syscall, vector<LOG_DATA> &vtret, vector<LOG_DATA> &vtargs, SHMEM_OBJ* shmem, bool output)
{
	int nResult = 0;
	ENTER_USERFUNC;
	char path[4096];
	
	if( g_pmapResource ) {
		string rsrc_key, rsrc_value;
		GetRsrcName(category, rsrc, rsrc_key, rsrc_value);
		//FILE* f;
		//if( output ) {
		//	sprintf( path, "%s/%s/%s.output", __home_dir, category.c_str(), rsrc_name.c_str() );
		//} else {
			//sprintf( path, "%s/%s/%s", __home_dir, category.c_str(), rsrc_name.c_str() );
		//}
		map<string, unsigned int>::iterator it = g_pmapResource->find( rsrc_key );
		if( it != g_pmapResource->end() ) {
			RSRC_DATA* pRsrcData = (RSRC_DATA*) sm_getptr(shmem,  it->second );
			
			//vector<RSRC_LOG_ELEM>* pvt = 0;
			RSRC_LOG_ELEM_ARY* plogary;
			int cnt_logs = 0;
			if( output ) { // output
				//pvt = &pRsrcData->vtLogOutput;
				plogary = &(pRsrcData->logOutput);
			} else {
				//pvt = &pRsrcData->vtLogInput;
				plogary = &(pRsrcData->logInput);
			}
			
			bool bFound = false;
			RSRC_LOG_ELEM* pelem = (RSRC_LOG_ELEM*)sm_getptr(shmem, plogary->ary_offset);
			
			if( plogary->used_cnt == plogary->cnt ) {
				// all used up
				DEBUGMSG("[ERROR] [%s] No Logged Data is available for this request -- all used up? (%s:%d)\n", rsrc_key.c_str(), __FILE__, __LINE__);
				nResult = -1;
			} else {
				for( int i = 0; i < plogary->cnt; i++ ) {
					if( pelem[i].used == false ) {
						if( pelem[i].syscall == syscall ) {
							if( CompareLogData( vtargs, 
												(LOG_DATA*) sm_getptr(shmem,  pelem[i].arg.ary_offset ), 
												pelem[i].arg.cnt, shmem, __FILE__, __LINE__ ) == true ) {
								// same...
								pelem[i].used = true;
								plogary->used_cnt++;
								//DEBUGMSG("[%d] Matched!! (elem.vtret.size() = %d, i = %d / pvtsize = %d)\n", 
								//		 (int)get_thread_id(pthread_self()), 
								//		 (int)elem.vtret.size(), 
								//		 (int)i, 
								//		 (int)pvt->size());
								
								// copy returns
								LOG_DATA* pLDRet = (LOG_DATA*)(sm_getptr(shmem, pelem[i].ret.ary_offset));
								VT_COPY2( vtret, pLDRet, pelem[i].ret.cnt );
								bFound = true;
								nResult = 1;
								break;
							} else {
								//DEBUGMSG("<<i = %d, syscall = %d, rsrc = %s>>\n", i, syscall, rsrc.c_str() );
							}
						} else {
							//DEBUGMSG("<<%d>>\n", __LINE__);
						}
					} else {
						//DEBUGMSG("<<%d>>\n", __LINE__);
					}
				}
				
				if( !bFound ) {
					if( !do_not_print_log_rsrc_key(rsrc_key) ) {
						DEBUGMSG("[ERROR] [%s] No Logged Data is available for this request -- no data available (%s:%d)\n", rsrc_key.c_str(), __FILE__, __LINE__);
					}
					nResult = -2;
				}
			}
			
		} else {
			// not found...
			if( !do_not_print_log_rsrc_key(rsrc_key) ) {
				DEBUGMSG("[ERROR] [%s] No Logged Data is available for this request -- not found in the map (%s:%d)\n", rsrc_key.c_str(), __FILE__, __LINE__);
			}
			nResult = -3;
		}
	} else {
		DEBUGMSG("[ERROR] g_pmapResource is not loaded... (%s:%d)\n", __FILE__, __LINE__);
		nResult = -4;
	}
	LEAVE_USERFUNC;
	return nResult;
}

/*
inline void LoadEachResourceDelay(string filename, string key, bool isOutput, map<string, RSRC_DATA*> &mapRsrcData)
{
	map<string, RSRC_DATA*>::iterator it;
	RSRC_DATA* pRsrcData = 0;
	it = mapRsrcData.find( key );
	if( it != mapRsrcData.end() ) { // found
		pRsrcData = it->second;
	} else {
		pRsrcData = new RSRC_DATA;
		mapRsrcData.insert( pair<string, RSRC_DATA*>(key, pRsrcData) );
	}
	
	if( isOutput ) {
		pRsrcData->filename_output = filename;
	} else {
		pRsrcData->filename = filename;
	} 
	pRsrcData->loaded = false;
	pRsrcData->key = key;
	pRsrcData->pmapRsrcData = &mapRsrcData;
	
}
*/

/*
inline void LoadEachResource(FILE* f, string key, bool isOutput, map<string, RSRC_DATA*> &mapRsrcData)
{
	map<string, RSRC_DATA*>::iterator it;
	RSRC_DATA* pRsrcData = 0;
	it = mapRsrcData.find( key );
	if( it != mapRsrcData.end() ) { // found
		pRsrcData = it->second;
	} else {
		pRsrcData = new RSRC_DATA;
		mapRsrcData.insert( pair<string, RSRC_DATA*>(key, pRsrcData) );
	}
	
	vector<RSRC_LOG_ELEM> vtinput;
	vector<RSRC_LOG_ELEM> vtoutput;
	pRsrcData->loaded = true;
	while( true ) {
		RSRC_LOG_ELEM elem;
		bool ret = ReadBinLog(f, elem );
		if( ret == false ) {
			break;
		}
		//printf("elem.syscall: %d\n", elem.syscall);
		
		if( isOutput ) {
			//pRsrcData->vtLogOutput.push_back(elem);
			vtoutput.push_back(elem);
		} else {
			//pRsrcData->vtLogInput.push_back(elem);
			vtinput.push_back(elem);
		}
	}
	
	pRsrcData->logInput.cnt  = vtinput.size();
	pRsrcData->logInput.ary = new RSRC_LOG_ELEM[vtinput.size()];
	pRsrcData->logOutput.cnt = vtoutput.size();
	pRsrcData->logOutput.ary = new RSRC_LOG_ELEM[vtoutput.size()];
	unsigned int i;
	for( i = 0; i < vtinput.size(); i++ ) {
		pRsrcData->logInput.ary[i] = vtinput[i];
	}
	for( i = 0; i < vtoutput.size(); i++ ) {
		pRsrcData->logOutput.ary[i] = vtoutput[i];
	}
	
}
*/

inline void LoadEachResourceShmem(FILE* f, string key, bool isOutput, map<string, unsigned int> &mapRsrcData, SHMEM_OBJ* shmem)
{
	RSRC_DATA* pRsrcData = 0;
	
	//DEBUGMSG("LoadEachResourceShmem: %s\n", key.c_str());
	map<string, unsigned int>::iterator it;
	it = mapRsrcData.find( key );
	if( it != mapRsrcData.end() ) { // found
		pRsrcData = (RSRC_DATA*)sm_getptr(shmem, it->second);
	} else {
		unsigned int offset = smalloc(shmem, sizeof( RSRC_DATA ), true);
		pRsrcData = (RSRC_DATA*)sm_getptr(shmem, offset);
		mapRsrcData.insert( pair<string, unsigned int>(key, offset) );
	}
	
	int ncnt = 0;
	vector<RSRC_LOG_ELEM> vtinput;
	vector<RSRC_LOG_ELEM> vtoutput;
	while( true ) {
		RSRC_LOG_ELEM elem;
		memset(&elem, 0, sizeof(RSRC_LOG_ELEM));
	
		//DEBUGMSG("cnt: %d\n", ncnt++);
		bool ret = ReadBinLog(f, elem, shmem, key);
		if( ret == false ) {
			break;
		}
		//printf("elem.syscall: %d\n", elem.syscall);
		
		if( isOutput ) {
			vtoutput.push_back(elem);
		} else {
			vtinput.push_back(elem);
		}
	}
	
	
	unsigned int i;
	
	if( isOutput ) {
		pRsrcData->logOutput.cnt = vtoutput.size();
		pRsrcData->logOutput.ary_offset = smalloc(shmem, sizeof( RSRC_LOG_ELEM ) * vtoutput.size(), true);
		
		RSRC_LOG_ELEM* aryoutput = (RSRC_LOG_ELEM*)sm_getptr(shmem, pRsrcData->logOutput.ary_offset);
		for( i = 0; i < vtoutput.size(); i++ ) {
			aryoutput[i] = vtoutput[i];
		}
	} else {
		pRsrcData->logInput.cnt  = vtinput.size();
		pRsrcData->logInput.ary_offset = smalloc(shmem, sizeof( RSRC_LOG_ELEM ) * vtinput.size(), true);
		
		RSRC_LOG_ELEM* aryinput = (RSRC_LOG_ELEM*)sm_getptr(shmem, pRsrcData->logInput.ary_offset);
		for( i = 0; i < vtinput.size(); i++ ) {
			aryinput[i] = vtinput[i];
		}
	}
	
	//DEBUGMSG("LoadEachResourceShmem: %s - end\n", key.c_str());
}


inline map<string, unsigned int> *LoadResourceFiles(const char* rsrc_folder, map<string, string> &rsrcmap, SHMEM_OBJ* shmem, bool bFirstLoad)
{
	char map_file_name[4096];
	FILE* f;
	map<string, unsigned int> *mapRsrcData = new map<string, unsigned int>;
	
	//
	// 1. read map
	//
	sprintf( map_file_name, "%s/rsrc_map", rsrc_folder );
	f = orgfn_fopen(map_file_name, "rt");
	if(f) {
		ReadMap(f, rsrcmap, "g_map");
		orgfn_fclose(f);
	}
	
	DEBUGMSG("Cnts: [rsrc map: %lu]\n", rsrcmap.size());
	
	if( bFirstLoad == true ) {
		// sanity check and loading individual resource.
		map<string, string>::iterator it;
		for( it = rsrcmap.begin(); it != rsrcmap.end(); it++ ) {
			// this includes directory and file name;
			string rsrcname = it->second;
			string rsrcname_output = rsrcname + string(".output");
			
			string fullpath;
			int found = 0;
			FILE* f = 0;
			// load this.
			fullpath = string(rsrc_folder) + string("/") + rsrcname + string(".bin");
			////////
			f = orgfn_fopen(fullpath.c_str(), "rb");
			if( f ) { // no output
				//LoadEachResourceDelay( fullpath, it->first, false, *mapRsrcData );
				LoadEachResourceShmem( f, it->first, false, *mapRsrcData, shmem );
				orgfn_fclose(f);
				found++;
				
			}
			
			// load this
			fullpath = string(rsrc_folder) + string("/") + rsrcname_output + string(".bin");
			f = orgfn_fopen(fullpath.c_str(), "rb");
			if( f ) { // output
				LoadEachResourceShmem( f, it->first, true, *mapRsrcData, shmem );
				orgfn_fclose(f);
				found++;
			}
			
			if( found == 0 ) {
				DEBUGMSG("Not Found: RsrcName: %s (tried %s too) [debug: %s]\n", 
					rsrcname.c_str(), rsrcname_output.c_str(), fullpath.c_str());
				delete mapRsrcData;
				exit(-1);
				return 0;
			}
		}
	} else {
	}
	//DEBUGMSG("LoadResourceFiles - done");
	
	return mapRsrcData;
}



#endif 