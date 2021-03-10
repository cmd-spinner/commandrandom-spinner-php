
#ifndef UTIL_RSRC_HEADER
#define UTIL_RSRC_HEADER 1


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <ctype.h>
#include <sys/types.h>
#include <netdb.h>
       
#include "util_logio.h"


extern SHMEM_OBJ* 			g_pshm_map_meta;
extern SHMEM_OBJ* 			g_pshm_map_data;


string GetSysname( int fd );
string readlink_wrapper(int fd);
void UpdateSysnameFd(string fdname, unsigned char type, string alias);
bool GetSysnameInfo(int fd, string& alias, unsigned char& type);

/*
 * void addrinfo_to_vector( const struct addrinfo* pAI, vector<LOG_DATA>& vt )
{
	if (pAI) {
		APPEND_STRUCT_EXIST( vt, 1, "struct addrinfo",  STRUCT_TYPE_ADDRINFO );
		
		APPEND_INT( vt, pAI->ai_flags, "pAI->ai_flags", STRUCT_TYPE_ADDRINFO );
		APPEND_INT( vt, pAI->ai_family, "pAI->ai_family", STRUCT_TYPE_ADDRINFO );
		APPEND_INT( vt, pAI->ai_socktype, "pAI->ai_socktype", STRUCT_TYPE_ADDRINFO );
		APPEND_INT( vt, pAI->ai_protocol, "pAI->ai_protocol", STRUCT_TYPE_ADDRINFO );
		APPEND_STR( vt, pAI->ai_canonname, "pAI->ai_canonname", STRUCT_TYPE_ADDRINFO );
		
		string str_addr = sockaddr_to_string( (struct sockaddr*)pAI->ai_addr, pAI->ai_addrlen );
		APPEND_STR( vt, str_addr.c_str(), "pAI->ai_addr", STRUCT_TYPE_ADDRINFO );
	} else {
		APPEND_STRUCT_EXIST( vt, 0, "struct addrinfo",  STRUCT_TYPE_ADDRINFO );
	}
}

void addrinfos_to_vector( struct addrinfo **res, vector<LOG_DATA>& vt )
{
	if ( res ) {
		APPEND_STRUCT_EXIST( vt, 1, "struct addrinfo*n",  STRUCT_TYPE_ADDRINFOS );
		
		struct addrinfo* pAI = *res;
		while (pAI)
		{
			addrinfo_to_vector(pAI, vt);
			
			pAI = pAI->ai_next;
		}
	} else {
		APPEND_STRUCT_EXIST( vt, 0, "struct addrinfo*n",  STRUCT_TYPE_ADDRINFOS );
	}
}
 */
string GetSocketHostName( string ip ) 
{
#if 0
	map<string, FDINFO>::iterator it;
	it = g_mapIp2HostName.find( ip );
	if( it != g_mapIp2HostName.end() ) {
		FDINFO fdinfo = it->second;
		return fdinfo.name;
	}
	
	return ip;
#endif
}

void SetSocketHostIPs( const char* hostname, struct addrinfo **ips )
{
#if 0
	if ( ips ) {
		struct addrinfo* pAI = *ips;
		
		string str_hostname = string(hostname);
		while (pAI)
		{
			string str_ip = sockaddr_to_string( (struct sockaddr*)pAI->ai_addr, pAI->ai_addrlen, false, true );
			
			map<string, FDINFO>::iterator it;
			it = g_mapIp2HostName.find( str_ip );
			if( it != g_mapIp2HostName.end() ) {
				FDINFO fdinfo = it->second;
				fdinfo.name = str_hostname;
				it->second = fdinfo;
			} else {
				DECLARE_FDINFO( str_hostname, FD_TYPE_NETWORK, string("") );
				g_mapIp2HostName.insert( pair<string, FDINFO>(str_ip, fdinfo) );
			}
			DEBUGMSG("//DBG//SetSocketHostIPs: %s <-> %s\n", str_ip.c_str(), str_hostname.c_str());
			
			pAI = pAI->ai_next;
		}
	}
#endif
}


/*
void SetTmpFD(int fd)
{
	map<int, FDINFO>::iterator it;
	string name("[tempfile]");
	
	it = g_mapFd.find( fd );
	if( it != g_mapFd.end() ) { // found
		FDINFO fdinfo = it->second;
		DEBUGMSG("//DBG// SetFD: %s -> %s\n", fdinfo.name.c_str(), name.c_str());
		fdinfo.name = name;
		fdinfo.type = FD_TYPE_FILE;
		fdinfo.sysname = GetSysname(fd);
		it->second = fdinfo;
	} else { // not found
		DECLARE_FDINFO( name, FD_TYPE_FILE, GetSysname(fd) );
		g_mapFd.insert( pair<int, FDINFO>(fd, fdinfo) );
	}
}
*/

void SetTmpFD(int fd)
{
	map<int, FDINFO>::iterator it;
	string name("[tempfile]");
	
	string rsrcname = readlink_wrapper( fd );
	UpdateSysnameFd( rsrcname, FD_TYPE_FILE, name );
	/*
	it = g_mapFd.find( fd );
	if( it != g_mapFd.end() ) { // found
		FDINFO fdinfo = it->second;
		DEBUGMSG("//DBG// SetFD: %s -> %s\n", fdinfo.name.c_str(), name.c_str());
		fdinfo.name = name;
		fdinfo.type = FD_TYPE_FILE;
		fdinfo.sysname = GetSysname(fd);
		it->second = fdinfo;
	} else { // not found
		DECLARE_FDINFO( name, FD_TYPE_FILE, GetSysname(fd) );
		g_mapFd.insert( pair<int, FDINFO>(fd, fdinfo) );
	}
	*/
}

void LogFD(int fd, string addr, int type) 
{
	// DebugLog...
}

void SetFD(int fd, string addr, int type)
{
	//map<int, FDINFO>::iterator it;
	//map<int, int>::iterator itType;
	
	string rsrcname = readlink_wrapper( fd );
	//DEBUGMSG("SetFD: rsrcname = %s\n", rsrcname.c_str());
	//void UpdateSysnameFd(string fdname, unsigned char type, string alias)
	UpdateSysnameFd( rsrcname, type, addr );
	//it = g_mapFd.find( fd );
	/*
	if( it != g_mapFd.end() ) { // found
		FDINFO fdinfo = it->second;
		DEBUGMSG("+//DBG// SetFD: %s -> %s\n", fdinfo.name.c_str(), addr.c_str());
		fdinfo.name = addr;
		fdinfo.type = type;
		fdinfo.sysname = GetSysname(fd);
		it->second = fdinfo;
	} else { // not found
		DECLARE_FDINFO( addr, type, GetSysname(fd) );
		DEBUGMSG("+//DBG// SetFD: %d -> %s\n", fd, addr.c_str());
		g_mapFd.insert( pair<int, FDINFO>(fd, fdinfo) );
	}
	*/
}

/*
void SetFD(int fd, string addr, int type)
{
	map<int, FDINFO>::iterator it;
	map<int, int>::iterator itType;
	
	it = g_mapFd.find( fd );
	if( it != g_mapFd.end() ) { // found
		FDINFO fdinfo = it->second;
		DEBUGMSG("+//DBG// SetFD: %s -> %s\n", fdinfo.name.c_str(), addr.c_str());
		fdinfo.name = addr;
		fdinfo.type = type;
		fdinfo.sysname = GetSysname(fd);
		it->second = fdinfo;
	} else { // not found
		DECLARE_FDINFO( addr, type, GetSysname(fd) );
		DEBUGMSG("+//DBG// SetFD: %d -> %s\n", fd, addr.c_str());
		g_mapFd.insert( pair<int, FDINFO>(fd, fdinfo) );
	}
}
*/

string GetZStream(z_streamp stream) 
{
	//g_mapZStream
	map<z_streamp, FDINFO>::iterator it;
	it = g_mapZStream.find( stream );
	if( it != g_mapZStream.end() ) { // found
		FDINFO fdinfo = it->second;
		return fdinfo.name;
	}
	
	char tmp[50];
	sprintf(tmp, "%p", (void*)stream);
	return string("Raw:") + string(tmp);
}

void SetZStream(z_streamp stream, string src) 
{
	//g_mapZStream
	map<z_streamp, FDINFO>::iterator it;
	it = g_mapZStream.find( stream );
	if( it != g_mapZStream.end() ) { // found
		FDINFO fdinfo = it->second;
		DEBUGMSG("//DBG// SetZStream: %s -> %s\n", fdinfo.name.c_str(), src.c_str());
		fdinfo.name = src;
		fdinfo.type = FD_TYPE_FILE;
		it->second = fdinfo;
	} else { // not found
		DECLARE_FDINFO( src, FD_TYPE_FILE, string("") );
		g_mapZStream.insert( pair<z_streamp, FDINFO>(stream, fdinfo) );
	}
}

string readlink_wrapper(int fd)
{
	if( !IS_INITED() ) {
		return string("[!init]");
	}
	
	char buf[1024];
	char fd_path[255]; 
	string str_ret;
	int old_errno = errno;
	
	
	sprintf( fd_path, "/proc/self/fd/%d", fd );
	int ret = orgfn_readlink (fd_path, buf, 1023);
	if (ret == -1) {
		return string("[error]");
	}
	
	buf[ret] = 0;
	str_ret = string(buf);
	
	errno = old_errno;
	return str_ret;
}




int RecoverFileName(int fd, char* output, size_t outputsize) 
{ 
	if( IS_INITED() ) {
		//int fd; 
		char fd_path[255]; 
		output[0] = 0;

		memset( output, 0, outputsize );

		//fd = fileno(f); 
		sprintf( fd_path, "/proc/self/fd/%d", fd );
		if( orgfn_readlink( fd_path, output, outputsize-1 ) == -1 ) {
			return 0;
		}

		/*
		for( int i = 0; i < strlen( output ); i++ ) {
			if( output[i] == '[' ) {
				output[i] = '(';
			} else if ( output[i] == ']' ) {
				output[i] = ')';
			} 
		}
		*/

		return 1; 
	} else {
		return 0;
	} 
} 

typedef struct tag_SYSNAMEFD {
	unsigned int sysname_offset;
	unsigned int alias_offset;
	unsigned char type;
} SYSNAMEFD;

#define DYN_ARRAY(__var) 1

typedef struct tag_SYSNAMEFD_LIST {
	int cnt;
	SYSNAMEFD list[ DYN_ARRAY(cnt) ];
} SYSNAMEFD_LIST;

bool GetSysnameInfo(int fd, string& alias, unsigned char& type)
{
	bool ret = false;
	simplefu_mutex_lock( g_sharedmapmutex );
	
	string fdname = readlink_wrapper(fd);
	
	SYSNAMEFD_LIST* pList = (SYSNAMEFD_LIST*)sm_getptr( g_pshm_map_meta, 0 );
	
	for( int i = pList->cnt - 1; i >= 0; i-- ) { // search.
		unsigned int offset = pList->list[i].sysname_offset;
		char* str = (char*)sm_getptr( g_pshm_map_data, offset );
		//DEBUGMSG("compare? %s == %s\n", str, fdname.c_str());
		if( strcmp( str, fdname.c_str() ) == 0 ) { // same item found
			alias = string( (char*)sm_getptr( g_pshm_map_data, pList->list[i].alias_offset ) );
			type = pList->list[i].type;
			ret = true;
			break;
		}
	}
	simplefu_mutex_unlock( g_sharedmapmutex );
	
	return ret;
}

unsigned int map_data_string_storage(string s)
{
	static map<string, unsigned int> map_str;
	map<string, unsigned int>::iterator it;
	
	it = map_str.find( s );
	if( it != map_str.end() ) {
		return it->second;
	} else {
		unsigned int offset = smalloc( g_pshm_map_data, s.length()+1, true );
		strcpy( (char*)sm_getptr(g_pshm_map_data, offset), s.c_str() );
		
		map_str.insert( pair<string, unsigned int>(s, offset) );
		
		return offset;
	}
	
	DEBUGMSG("string_storage_failed\n");
	exit(-1);
	return -1;
}

void UpdateSysnameFd(string fdname, unsigned char type, string alias)
{
#if 0
	simplefu_mutex_lock( g_sharedmapmutex );
	
	SYSNAMEFD_LIST* pList = (SYSNAMEFD_LIST*)sm_getptr( g_pshm_map_meta, 0 );
	
	if( pList->cnt > 50000) {
		DEBUGMSG("[ERROR] UpdateSysnameFd: Too big\n");
		simplefu_mutex_unlock( g_sharedmapmutex );
		exit(-1);
	}
	
	int idx_to_update = -1;
	for( int i = pList->cnt - 1; i >= 0; i-- ) { // search.
		// pList->list[pList->cnt].sysname_offset = name_offset;
		unsigned int offset = pList->list[i].sysname_offset;
		char* str = (char*)sm_getptr( g_pshm_map_data, offset );
		if( strcmp( str, fdname.c_str() ) == 0 ) { // same item found
			// update here.
			idx_to_update = i;
			break;
		}
	}
	
	if( idx_to_update != -1 ) { // found - just update fd
		pList->list[idx_to_update].type = type;
		pList->list[idx_to_update].alias_offset = map_data_string_storage(alias);
		
		//DEBUGMSG("UpdateSysnameFd(1):: '%s' => [%d, %s] (cnt: %d)\n", fdname.c_str(), 
		//		 type, alias.c_str(),
		//		 pList->cnt);
	} else {
		// not found, new item
		// zero initialized  (last param)
		pList->list[pList->cnt].sysname_offset = map_data_string_storage( fdname );
		pList->list[pList->cnt].alias_offset = map_data_string_storage( alias );
		pList->list[pList->cnt].type = type;
		
		// update list.
		
		// add new cnt.
		pList->cnt++;
		
		//DEBUGMSG("UpdateSysnameFd(2):: '%s' => [%d, %s] (cnt: %d)\n", fdname.c_str(), 
		//		 type, alias.c_str(),
		//		 pList->cnt);
	}
	
	
	simplefu_mutex_unlock( g_sharedmapmutex );
#endif
}

string GetSysname( int fd ) 
{
	string ret;
	
	ret = readlink_wrapper(fd);
	//UpdateSysnameFd( fd, ret );
	
	return ret;
}
	

string GetCategoryFromFDType(int type, string strdef)
{
	switch(type)
	{
		case FD_TYPE_FILE:
			return CATEGORY_FILE;
			break;
		case FD_TYPE_NETWORK:
			return CATEGORY_NETWORK;
			break;
		case FD_TYPE_EVENT:
			return CATEGORY_EVENT;
			break;
	}
	
	return strdef;
}

/*
string GetFDSysname(int fd)
{
	char temp[20] = { 0, };
	map<int, FDINFO>::iterator it;
	
	it = g_mapFd.find( fd );
	if( it != g_mapFd.end() ) { // found
		FDINFO fdinfo = it->second;
		return fdinfo.sysname;
	}
	
	return string("unknown");
}
*/

bool GetFD(int fd, string& rsrcname, unsigned char& type)
{
#if 0
	//map<int, FDINFO>::iterator it;
	
	// bool GetSysnameInfo(int fd, string& alias, unsigned char& type)
	//string cur_sysname = readlink_wrapper(fd);
	bool ret = GetSysnameInfo( fd, rsrcname, type );
	if( ret == false ) {
		
		bool updated = false;
		string cur_sysname = readlink_wrapper(fd);
		// need to update the list here.
		// void UpdateSysnameFd(string fdname, unsigned char type, string alias);
		if( cur_sysname.length() > 1 ) {
			if( cur_sysname[0] == '/' ) {
				rsrcname = string("Recovered at GetFD:") + cur_sysname;
				type = FD_TYPE_FILE;
				
				UpdateSysnameFd( cur_sysname, type, rsrcname );
				updated = true;
			} else if( startsWith(cur_sysname, string("socket:")) || startsWith(cur_sysname, string("pipe:")) ) {
				rsrcname = string("Recovered at GetFD:") + cur_sysname;
				type = FD_TYPE_NETWORK;
				
				UpdateSysnameFd( cur_sysname, type, rsrcname );
				updated = true;
			} else if( cur_sysname.compare(string("anon_inode:[eventfd]")) == 0 ) {
				rsrcname = string("Recovered at GetFD:") + cur_sysname;
				type = FD_TYPE_EVENT;
				
				UpdateSysnameFd( cur_sysname, type, rsrcname );
				updated = true;
			} 
		}
		
		if( updated == false ) {
			DEBUGMSG("GetFD: GetSysnameInfo return false. (fd: %d, sysname: %s) \n", fd, cur_sysname.c_str());
		} else {
			ret = true;
		}
	}
	return ret;
	/*
	it = g_mapFd.find( fd );
	if( it != g_mapFd.end() ) { // found
		FDINFO fdinfo = it->second;
		
		sysname = fdinfo.sysname;
		
		type = fdinfo.type;
		rsrcname = fdinfo.name;
		
		if( sysname.compare( cur_sysname ) != 0 ) { // diff
			int newfd = GetFdFromSysname(cur_sysname);
			
			map<int, FDINFO>::iterator itnew;
			itnew = g_mapFd.find( newfd );
			if( itnew != g_mapFd.end() ) { // found
				FDINFO fdinfonew = itnew->second;
				
				type = fdinfonew.type;
				rsrcname = fdinfonew.name;
				sysname = fdinfonew.sysname;
				
				it->second = fdinfonew;
				return true;
			} else { // not found
				if( cur_sysname.length() > 1 ) {
					if( cur_sysname[0] == '/' ) { // its file. just update and go ahead.
						
						//type = fdinfonew.type;
						//rsrcname = fdinfonew.name;
						//sysname = fdinfonew.sysname;
				
						return true;
					}
				}
				DEBUGMSG("[ERROR] Serious Error! (%s, %s)\n", sysname.c_str(), cur_sysname.c_str());
				return false;
			}
		} else { // same
			type = fdinfo.type;
			rsrcname = fdinfo.name;
			return true;
		}
	}
	
	return false;
	*/
#endif
}

/*
bool GetFD(int fd, string& rsrcname, string &sysname, int& type)
{
	map<int, FDINFO>::iterator it;
	
	string cur_sysname = readlink_wrapper(fd);
	
	it = g_mapFd.find( fd );
	if( it != g_mapFd.end() ) { // found
		FDINFO fdinfo = it->second;
		
		sysname = fdinfo.sysname;
		
		type = fdinfo.type;
		rsrcname = fdinfo.name;
		
		if( sysname.compare( cur_sysname ) != 0 ) { // diff
			int newfd = GetFdFromSysname(cur_sysname);
			
			map<int, FDINFO>::iterator itnew;
			itnew = g_mapFd.find( newfd );
			if( itnew != g_mapFd.end() ) { // found
				FDINFO fdinfonew = itnew->second;
				
				type = fdinfonew.type;
				rsrcname = fdinfonew.name;
				sysname = fdinfonew.sysname;
				
				it->second = fdinfonew;
				return true;
			} else { // not found
				if( cur_sysname.length() > 1 ) {
					if( cur_sysname[0] == '/' ) { // its file. just update and go ahead.
						
						//type = fdinfonew.type;
						//rsrcname = fdinfonew.name;
						//sysname = fdinfonew.sysname;
				
						return true;
					}
				}
				DEBUGMSG("[ERROR] Serious Error! (%s, %s)\n", sysname.c_str(), cur_sysname.c_str());
				return false;
			}
		} else { // same
			type = fdinfo.type;
			rsrcname = fdinfo.name;
			return true;
		}
	}
	
	return false;
}
*/

/*
void ClearFD(int fd)
{
	map<int, FDINFO>::iterator it = g_mapFd.find( fd );
	if( it != g_mapFd.end() ) {
		g_mapFd.erase(it);
	}
}
*/

/*
void DupFD(int newfd, int oldfd)
{
	map<int, FDINFO>::iterator itold = g_mapFd.find( oldfd );
	if( itold != g_mapFd.end() ) {
		map<int, FDINFO>::iterator itnew = g_mapFd.find( newfd );
		if (itnew != g_mapFd.end() ) {
			itnew->second = itold->second; // copy
		} else {
			g_mapFd.insert( pair<int, FDINFO>(newfd, itold->second) ); // add
		}
	} else {
		return ;
	}
}
*/

void SetTempFileName(const char* tmpfilename)
{
}


void SetTmpFILE(FILE* f)
{
	map<FILE*, FDINFO>::iterator it;
	string name("[tempfile]");
	
	it = g_mapFILE.find( f );
	if( it != g_mapFILE.end() ) { // found
		FDINFO fdinfo = it->second;
		DEBUGMSG("//DBG// SetFile: %s -> %s\n", fdinfo.name.c_str(), name.c_str());
		fdinfo.name = name;
		it->second = fdinfo;
	} else { // not found
		DECLARE_FDINFO( name, FD_TYPE_FILE, string("") );
		g_mapFILE.insert( pair<FILE*, FDINFO>(f, fdinfo) );
	}
}

void LogFILE(FILE* f, string name)
{
}

void SetFILE(FILE* f, string name)
{
	map<FILE*, FDINFO>::iterator it;
	
	it = g_mapFILE.find( f );
	if( it != g_mapFILE.end() ) { // found
		FDINFO fdinfo = it->second;
		DEBUGMSG("//DBG// SetFile: %s -> %s\n", fdinfo.name.c_str(), name.c_str());
		fdinfo.name = name;
		it->second = fdinfo;
	} else { // not found
		DECLARE_FDINFO( name, FD_TYPE_FILE, string("") );
		g_mapFILE.insert( pair<FILE*, FDINFO>(f, fdinfo) );
	}
}

void ClearFILE(FILE* f)
{
	map<FILE*, FDINFO>::iterator it;
	
	it = g_mapFILE.find( f );
	if( it != g_mapFILE.end() ) { // found
		g_mapFILE.erase( it );
	}
}

string GetFILE(FILE* f)
{
	char temp[20] = { 0, };
	map<FILE*, FDINFO>::iterator it;
	
	it = g_mapFILE.find( f );
	if( it != g_mapFILE.end() ) { // found
		FDINFO fdinfo = it->second;
		return fdinfo.name;
	}
	
	// not found
	sprintf(temp, "Raw:%p", f);
	return string(temp);
}


void SetDIR(DIR* d, string addr)
{
	map<DIR*, FDINFO>::iterator it;
	
	it = g_mapDIR.find( d );
	if( it != g_mapDIR.end() ) { // found
		FDINFO fdinfo = it->second;
		DEBUGMSG("//DBG// SetFile: %s -> %s\n", fdinfo.name.c_str(), addr.c_str());
		fdinfo.name = addr;
		fdinfo.type = FD_TYPE_FILE;
		it->second = fdinfo;
	} else { // not found
		DECLARE_FDINFO( addr, FD_TYPE_FILE, string("") );
		g_mapDIR.insert( pair<DIR*, FDINFO>(d, fdinfo) );
	}
}

void ClearDIR(DIR* d)
{
	map<DIR*, FDINFO>::iterator it;
	
	it = g_mapDIR.find( d );
	if( it != g_mapDIR.end() ) { // found
		g_mapDIR.erase( it );
	}
}

string GetDIR(DIR* d)
{
	char temp[20] = { 0, };
	map<DIR*, FDINFO>::iterator it;
	
	it = g_mapDIR.find( d );
	if( it != g_mapDIR.end() ) { // found
		FDINFO fdinfo = it->second;
		return fdinfo.name;
	}
	
	// not found
	sprintf(temp, "Raw:%p", d);
	return string(temp);
}

string buf_to_readable_string( const char* buf, unsigned int bufsize ) 
{
	string ret;
	char tmpbuf[10] = { 0, };
	for( unsigned int i = 0; i < bufsize; i++ ) {
		if( isprint(buf[i]) ){
			sprintf(tmpbuf, "%c", (unsigned char)buf[i]);
		} else {
			sprintf(tmpbuf, "?");
		} 
		ret += tmpbuf;
	}
	return ret;
}


string buf_to_string( const char* buf, unsigned int bufsize ) 
{
	string ret;
	char tmpbuf[10] = { 0, };
	for( unsigned int i = 0; i < bufsize; i++ ) {
		sprintf(tmpbuf, "%02X", (unsigned char)buf[i]);
		ret += tmpbuf;
	}
	return ret;
}

string sockaddr_in_to_string2(const struct sockaddr_in *pSockIn, int lenData)
{
	char saddr[4096] = { 0, };
	if( lenData >= sizeof( struct sockaddr_in ) ) {
		/*
			struct sockaddr_in {
			short   sin_family;
			u_short sin_port;
			struct  in_addr sin_addr;
			char    sin_zero[8];
		};
			*/
		// char *inet_ntoa(struct in_addr in);
		
		
		sprintf( saddr, "%d|%d:%s", 
					pSockIn->sin_port, pSockIn->sin_family, inet_ntoa(pSockIn->sin_addr) );				
	} else {
		char* pBufRead = (char*)pSockIn;
		char tmpbuf[10] = { 0, };
		for( unsigned int i = 0; i < lenData; i++ ) {
			sprintf(tmpbuf, "%02X", (unsigned char)pBufRead[i]);
			strcat( saddr, tmpbuf );
		}
	}
	
	return string( saddr );
}


bool is_unix_socket(string &sockname) 
{
	return endsWith(sockname, string(":unix"));
}

struct sockaddr * string_to_sockaddr(string addr)
{
	struct sockaddr * ret = 0;
	if( startsWith( addr, string("x") ) ) { // binary
		// BINARY
		DEBUGMSG("addr-BIN = %s\n", addr.c_str());
	} else if( endsWith( addr, string(":unix") ) ) {
		// AF_UNIX
		DEBUGMSG("addr-UNIX = %s\n", addr.c_str());
		
		int len = addr.length() - 5;
		string path = string_left( addr, len );
		
		
		struct sockaddr_un* paddr = (struct sockaddr_un*)malloc( sizeof( struct sockaddr_un ) );
		paddr->sun_family = AF_UNIX;
		memset( paddr->sun_path, 0, sizeof(paddr->sun_path) );
		if( path.length() > 107 ) {
			memcpy( paddr->sun_path, path.c_str(), 107 );
		} else {
			strcpy( paddr->sun_path, path.c_str() );
		}
		ret = (struct sockaddr*) paddr;
	} else if( addr.find( ":" ) != string::npos )  {
		// AF_INET, AF_INET6
		if( addr.find(".") != string::npos ) { // ipv4
			struct sockaddr_in* paddr = (struct sockaddr_in*)malloc(sizeof(sockaddr_in)) ;
			
			int len = addr.length() - (addr.length() - addr.find(":"));
			string ipv4addr = string_left( addr, len );
			int port = atoi(addr.substr( ipv4addr.length() ).c_str());
			
			DEBUGMSG("ipv4: addr = [%s] port: %d\n", ipv4addr.c_str(), port );
		
			//inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
			inet_pton(AF_INET, ipv4addr.c_str(), &(paddr->sin_addr) );
			paddr->sin_family = AF_INET;
			paddr->sin_port = port;

			ret = (struct sockaddr*) paddr;
		} else { // ipv6
			struct sockaddr_in6* paddr = (struct sockaddr_in6*)malloc(sizeof(sockaddr_in6)) ;
			
			int len = addr.length() - (addr.length() - addr.rfind(":"));
			string ipv6addr = string_left( addr, len );
			int port = atoi(addr.substr( ipv6addr.length() ).c_str());
			
			DEBUGMSG("ipv6: addr = [%s] port: %d\n", ipv6addr.c_str(), port );
		
			//inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
			inet_pton(AF_INET6, ipv6addr.c_str(), &(paddr->sin6_addr) );
			
			paddr->sin6_family = AF_INET6;
			paddr->sin6_port = port;
			
			ret = (struct sockaddr*) paddr;
		} 
	} else if( addr.find(".") != string::npos ) { // ipv4
		struct sockaddr_in* paddr = (struct sockaddr_in*)malloc(sizeof(sockaddr_in)) ;
			
			string ipv4addr = addr;
			int port = 0; //atoi(addr.substr( ipv4addr.length() ).c_str());
			
			DEBUGMSG("ipv4: addr = [%s] port: %d (%s:%d)\n", ipv4addr.c_str(), port, __FILE__, __LINE__ );
		
			//inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
			inet_pton(AF_INET, ipv4addr.c_str(), &(paddr->sin_addr) );
			paddr->sin_family = AF_INET;
			paddr->sin_port = port;

			ret = (struct sockaddr*) paddr;
	}
	
	return ret;
	
}
string sockaddr_to_string(const struct sockaddr *sockaddr, int len, bool trans_ip_to_domain, bool remove_port)
{
	char tmp[INET_ADDRSTRLEN*2+1] = { 0, };
	char saddr[4096] = { 0, };
	if( sockaddr ) {
		bool done = false;
		struct sockaddr_in* psin = (struct sockaddr_in*)sockaddr;
		switch ( psin->sin_family ) {
			case AF_UNIX:
				{
					int i = 0, j = 0;
					struct sockaddr_un* paddr = (struct sockaddr_un*)sockaddr;
					char* sun_path = paddr->sun_path;
					int path_len = len - sizeof( psin->sin_family );
					if( path_len > 108 ) path_len = 108;
					for( i = 0; i < path_len; i++ ) {
						// find until its not a zero.
						if( sun_path[i] != 0 ) {
							break;
						}
					}
					for( ; i < path_len; i++ ) {
						saddr[j++] = sun_path[i];
						if( sun_path[i] == 0 ) {
							break;
						}
					}
					saddr[j] = 0;
					strcat( saddr, ":unix" );
					done = true;
				}
				break;
				
			case AF_INET:
				{
					struct sockaddr_in* paddr = (struct sockaddr_in*)sockaddr;
					if( len >= sizeof( struct sockaddr_in ) ) {
						inet_ntop( AF_INET, &(paddr->sin_addr), tmp, INET_ADDRSTRLEN*2 );
						string addr = string(tmp);
						if( trans_ip_to_domain ) {
							addr = GetSocketHostName( addr );
						}
						
						if( remove_port ) {
							sprintf( saddr, "%s", addr.c_str() );
						} else {
							sprintf( saddr, "%s:%d", addr.c_str(), paddr->sin_port );
						}
						done = true;
					}
				}
				break;
			case AF_INET6:
				{
					struct sockaddr_in6* paddr = (struct sockaddr_in6*)sockaddr;
					if( len >= sizeof( struct sockaddr_in6 ) ) {
						inet_ntop( AF_INET6, &(paddr->sin6_addr), tmp, INET_ADDRSTRLEN*2 );
						string addr = string(tmp);
						if( trans_ip_to_domain ) {
							addr = GetSocketHostName( addr );
						}
						
						if( remove_port ) {
							sprintf( saddr, "%s", addr.c_str() );
						} else {
							sprintf( saddr, "%s:%d", addr.c_str(), paddr->sin6_port );
						} 
						done = true;
					}
				}
				break;
		}
		if( done == false ) {
			char* pBufRead = (char*)sockaddr;
			char tmpbuf[10] = { 0, };
			for( unsigned int i = 0; i < len; i++ ) {
				if( i > 1024 ) {
					break;
				}
				sprintf(tmpbuf, "x%02X", (unsigned char)pBufRead[i]);
				strcat( saddr, tmpbuf );
			}
		}
	}
	
	return string( saddr );
}


/*
map<int, string> g_map_fd_sockname;
void setFdSockName(int fd, string sockname)
{
	map<int, string>::iterator it = g_map_fd_sockname.find( fd );
	if( it != g_map_fd_sockname.end() ) {
		it->second = sockname;
	} else {
		g_map_fd_sockname.insert( pair<int, string>(fd, sockname) );
	}
}

string getSockNameFromFd(int fd)
{
	map<int, string>::iterator it = g_map_fd_sockname.find( fd );
	if( it != g_map_fd_sockname.end() ) {
		return it->second;
	}
	return string("");	
}
*/


string getSocketName(int domain, int type, int protocol)
{
	static map<string, int> mapSocketName;
	
	string strsocket;
	char szSocket[400] = { 0, };

	sprintf( szSocket, "%x:%x:%x", domain, type, protocol );
	strsocket = szSocket;
	
	char socket_fd_name[400] = { 0, };
	int cnt = 0;
	
	map<string, int>::iterator it = mapSocketName.find( strsocket );
	if( it != mapSocketName.end() ) { // found
		cnt = it->second;
		it->second++;
	} else { // not exist
		mapSocketName.insert( pair<string, int>(strsocket, 1) );
		cnt = 0;
	}
	sprintf( socket_fd_name, "%s#%d", strsocket.c_str(), cnt);
	if( domain == AF_UNIX ) {
		strcat( socket_fd_name, ":unix" );
	}
	
	return string( socket_fd_name );
}




void hostent_to_vector(struct hostent* h, vector<LOG_DATA> &vt)
{
	//string ret = string("");
	
	if( h ) {
		APPEND_STRUCT_EXIST( vt, 1, "struct hostent",  STRUCT_TYPE_HOSTENT );
		/////////////////////////////
		APPEND_STR( vt, h->h_name, "h->h_name", STRUCT_TYPE_HOSTENT );
		APPEND_INT( vt, h->h_addrtype, "h->h_addrtype", STRUCT_TYPE_HOSTENT  );
		APPEND_INT( vt, h->h_length, "h->h_length", STRUCT_TYPE_HOSTENT  );
		
		/////////////////////////////
		int cnt = 0;
		COUNT_ARRAY_UNTIL_NULL( h->h_aliases, cnt );
		APPEND_INT( vt, cnt, "count of h->h_aliases", STRUCT_TYPE_HOSTENT );
		for( int i = 0; i < cnt; i++ ) {
			APPEND_STR( vt, h->h_aliases[i], "h->h_aliases[i]" , STRUCT_TYPE_HOSTENT );
		}
		
		/////////////////////////////
		COUNT_ARRAY_UNTIL_NULL( h->h_addr_list, cnt );
		
		APPEND_INT( vt, cnt, "count of h->h_addr_list", STRUCT_TYPE_HOSTENT );
		
		struct in_addr** addr_list = (struct in_addr **)h->h_addr_list;
		for( int i = 0; i < cnt; i++ ) {
			string s_addr_list = string( inet_ntoa( *addr_list[i] ) );
			APPEND_STR( vt, s_addr_list.c_str(), "h->h_addr_list[i]", STRUCT_TYPE_HOSTENT );
		}
	} else {
		APPEND_STRUCT_EXIST( vt, 0, "struct hostent", STRUCT_TYPE_HOSTENT );
	}
}

struct hostent* vector_to_hostent(vector<LOG_DATA> &vt, int& vtidx)
{
	//string ret = string("");
	hostent* ret = 0;
	bool exist = false;
	DOES_STRUCT_EXIST( vt[vtidx], STRUCT_TYPE_HOSTENT, exist ); vtidx++;
	
	if( exist ) {
		/////////////////////////////
		char* str_buf;
		size_t str_len;
		
		ret = (struct hostent*) malloc(sizeof(struct hostent));
		
		READ_STR( vt[vtidx], ret->h_name, str_len ); vtidx++;// h->h_name, "h->h_name", STRUCT_TYPE_HOSTENT );
		READ_INT( vt[vtidx], ret->h_addrtype ); vtidx++;//, "h->h_addrtype", STRUCT_TYPE_HOSTENT  );
		READ_INT( vt[vtidx], ret->h_length ); vtidx++;//"h->h_length", STRUCT_TYPE_HOSTENT  );
		
		/////////////////////////////
		int cnt = 0;
		READ_INT( vt[vtidx], cnt );
		//COUNT_ARRAY_UNTIL_NULL( h->h_aliases, cnt );
		//APPEND_INT( vt, cnt, "count of h->h_aliases", STRUCT_TYPE_HOSTENT );
		ret->h_aliases = (char**) malloc(sizeof(char*) * cnt);
		for( int i = 0; i < cnt; i++ ) {
			READ_STR( vt[vtidx], ret->h_aliases[i], str_len ); vtidx++;//"h->h_aliases[i]" , STRUCT_TYPE_HOSTENT );
		}
		
		/////////////////////////////
		//COUNT_ARRAY_UNTIL_NULL( h->h_addr_list, cnt );
		//APPEND_INT( vt, cnt, "count of h->h_addr_list", STRUCT_TYPE_HOSTENT );
		READ_INT( vt[vtidx], cnt ); vtidx++;
		 
		//struct in_addr** addr_list = (struct in_addr **)h->h_addr_list;
		ret->h_addr_list = (char** )malloc( sizeof( struct in_addr* ) * cnt );
		for( int i = 0; i < cnt; i++ ) {
			//int inet_aton(const char *cp, struct in_addr *inp);
			//inet_aton( 
			READ_STR( vt[vtidx], str_buf, str_len ); vtidx++;//, "h->h_addr_list[i]", STRUCT_TYPE_HOSTENT );
			ret->h_addr_list[i] = (char*)malloc( sizeof( struct in_addr ) );
			inet_aton( str_buf, (struct in_addr*)ret->h_addr_list[i] );
			//ret->h_addr_list = 
			//string s_addr_list = string( inet_ntoa( *addr_list[i] ) );
		}
	} else {
		//APPEND_STRUCT_EXIST( vt, 0, "struct hostent", STRUCT_TYPE_HOSTENT );
	}
	return ret;
}


int Compare_hostent( struct hostent* h1, struct hostent* h2 ) 
{
	if( h1 && h2 ) {
		if( h1->h_name && h2->h_name ) {
			if( strcmp( h1->h_name, h2->h_name ) != 0 ) {
				return -3;
			}
		} else if( !h1->h_name && !h2->h_name ) {
		} else {
			return -2;
		}
		
		if( h1->h_addrtype != h2->h_addrtype ) {
			return -3;
		}
		
		if( h1->h_length != h2->h_length ) {
			return -4;
		}
		
		int cnt1 = 0, cnt2 = 0;
		COUNT_ARRAY_UNTIL_NULL( h1->h_aliases, cnt1 );
		COUNT_ARRAY_UNTIL_NULL( h2->h_aliases, cnt2 );
		if( cnt1 != cnt2 ) {
			return -5;
		}
		for( int i = 0; i < cnt1; i++ ) {
			if( strcmp(h1->h_aliases[i], h2->h_aliases[i]) != 0 ) {
				return -6;
			}
		}
		
		COUNT_ARRAY_UNTIL_NULL( h1->h_addr_list, cnt1 );
		COUNT_ARRAY_UNTIL_NULL( h2->h_addr_list, cnt2 );
		if( cnt1 != cnt2 ) {
			return -7;
		}
		for( int i = 0; i < cnt1; i++ ) {
			if( h1->h_addr_list[i] != h2->h_addr_list[i] ) {
				return -8;
			}
		}
		
		
		return 1;
	} else if( !h1 && !h2 ) {
		return 1;
	} else {
		return -1;
	} 
	
	return 0;
}


int vector_to_iovecs(vector<LOG_DATA> &vt, int &vtidx, struct iovec* iov)
{
	int iovcnt = 0;
	bool exist = false;
	DOES_STRUCT_EXIST( vt[vtidx], STRUCT_TYPE_IOVEC, exist ); vtidx++;
	if( exist ) {
		READ_INT(vt[vtidx], iovcnt); vtidx++;
		DEBUGMSG("vector_to_iovecs: iovcnt: %d\n", iovcnt);
		
		for( int i = 0; i < iovcnt; i++ ) {
			void* __buf = 0;
			int __buf_size = 0;
			size_t __iov_len = 0;
			READ_SIZE( vt[vtidx], __iov_len ); vtidx++;
			DEBUGMSG("vector_to_iovecs: iov_len: %d\n", __iov_len);
			READ_BUF( vt[vtidx], __buf, __buf_size ); vtidx++;
			DEBUGMSG("vector_to_iovecs: __buf_size: %d\n", __buf_size);
			//iov[i].iov_len = __iov_len;
			if( __buf_size > 0 ) {
				//iov[i].iov_base = (void*)malloc( __buf_size );
				if( iov[i].iov_len < __buf_size ) {
					DEBUGMSG("vector_to_iovecs: bufsize: %d < %d\n", iov[i].iov_len, __buf_size);
					exit(-1);
				}
				
				memcpy( iov[i].iov_base, __buf, __buf_size ); //iov[i].iov_len );
			} else {
				iov[i].iov_base = 0;
			} 
		}
	}
	DEBUGMSG("vector_to_iovecs: return: %d\n", iovcnt);
	return iovcnt;
}

void iovecs_to_vector(const struct iovec* iov, int iovcnt, vector<LOG_DATA> &vt)
{
	string ret, str_iov;
	
	if( iov ) {
		APPEND_STRUCT_EXIST( vt, 1, "struct iov",  STRUCT_TYPE_IOVEC );
		
		APPEND_INT(vt, iovcnt, "iovcnt", STRUCT_TYPE_IOVEC);
		for( int i = 0; i < iovcnt; i++ ) {
			APPEND_SIZE( vt, iov[i].iov_len, "iov[i].iov_len", STRUCT_TYPE_IOVEC );
			APPEND_BUF( vt, iov[i].iov_base, iov[i].iov_len, "iov[i].iov_base", STRUCT_TYPE_IOVEC );
		}
		
	} else {
		APPEND_STRUCT_EXIST( vt, 0, "struct iov",  STRUCT_TYPE_IOVEC );
	}
}


bool vector_to_msghdr(vector<LOG_DATA> &vt, int &vtidx, struct msghdr* msg) 
{
	bool exist = false;
	DOES_STRUCT_EXIST( vt[vtidx], STRUCT_TYPE_MSGHDR, exist ); vtidx++;
	DEBUGMSG("vector_to_msghdr: struct_exist\n");
	if( exist ) {
		void* __buf = 0;
		int __buf_size = 0;
		int __uint = 0;
		
		//APPEND_BUF( vt, msg->msg_name, msg->msg_namelen, "msg->msg_name", STRUCT_TYPE_MSGHDR );
		READ_UINT( vt[vtidx], __uint ); vtidx++;
		msg->msg_namelen = __uint;
		DEBUGMSG("vector_to_msghdr: msg_namelen: %d\n", __uint);
		READ_BUF( vt[vtidx], __buf, __buf_size ); vtidx++;
		DEBUGMSG("vector_to_msghdr: read_buf: bufsize: %d (%p)\n", __buf_size, msg->msg_name);
		if( __buf_size > 0 ) {
			//msg->msg_name = (void*)malloc( __buf_size );
			if( msg->msg_namelen < __buf_size ) {
				DEBUGMSG("vector_to_msghdr: read_buf: bufsize: %d < %d\n", msg->msg_namelen, __buf_size);
				exit(-1);
			}
			memcpy( msg->msg_name, __buf, __buf_size );
			//msg->msg_namelen = __buf_size;
		} else {
			msg->msg_name = 0;
			//msg->msg_namelen  = 0;
		}
		
		//APPEND_BUF( vt, msg->msg_control, msg->msg_controllen, "msg->msg_controllen", STRUCT_TYPE_MSGHDR );
		READ_UINT( vt[vtidx], __uint ); vtidx++;
		msg->msg_controllen = __uint;
		DEBUGMSG("vector_to_msghdr: msg_controllen: %d\n", __uint);
		READ_BUF( vt[vtidx], __buf, __buf_size ); vtidx++;
		DEBUGMSG("vector_to_msghdr: read_buf: bufsize: %d (%p)\n", __buf_size, msg->msg_control);
		if( __buf_size > 0 ) {
			//msg->msg_control = (void*)malloc( __buf_size );
			if( msg->msg_controllen < __buf_size ) {
				DEBUGMSG("vector_to_msghdr: read_buf: bufsize: %d < %d\n", msg->msg_namelen, __buf_size);
				exit(-1);
			}
			
			memcpy( msg->msg_control, __buf, __buf_size );
			
		} else {
			msg->msg_control = 0;
		} 
		
		//APPEND_UINT( vt, msg->msg_flags, "msg->msg_flags", STRUCT_TYPE_MSGHDR );
		READ_UINT( vt[vtidx], __uint ); vtidx++;
		DEBUGMSG("vector_to_msghdr: read_uint: msg_flags: %d\n", __uint);
		msg->msg_flags = __uint;
		//if( msg->msg_flags != __uint ) {
		//	DEBUGMSG("vector_to_msghdr: return false\n");
		//	return false;
		//}
		
		//void vector_to_iovecs(vector<LOG_DATA> &vt, int &vtidx, const struct iovec* iov)
		if( msg->msg_iovlen != vector_to_iovecs(vt, vtidx, msg->msg_iov) ) {
			return false;
		}
	}
	return true;
}




struct iovec* dup_iovecs(const struct iovec* iov, int iovcnt)
{
	struct iovec* ret = 0;
	if( iov ) {
		ret = (struct iovec *)malloc( sizeof( struct iovec ) * iovcnt );
		
		for( int i = 0; i < iovcnt; i++ ) {
			ret[i].iov_len = iov[i].iov_len;
			
			ret[i].iov_base = malloc( ret[i].iov_len );
			memset( ret[i].iov_base, 0, ret[i].iov_len ); // clear
		}
	} 
	
	return ret;
}

void get_iovecs_signature(const struct iovec* iov, int iovcnt, string& strSig)
{
	if( iov ) {
		char elem[128];
		strSig += string("E|");
		
		for( int i = 0; i < iovcnt; i++ ) {
			sprintf(elem, "%lu|", iov[i].iov_len);
			strSig += string(elem);
		}
	} else {
		strSig += string("N|");
	}
}

void get_msghdr_signature( const struct msghdr* msg, string& strSig ) 
{
	if( msg ) {
		char elem[128];
		strSig += string("E|");
		//ret = (struct msghdr*) malloc( sizeof(struct msghdr) );
		//memset( ret, 0, sizeof(struct msghdr) ); // clear
		
		sprintf( elem, "%u|", msg->msg_namelen );
		strSig += string(elem);
		
		sprintf( elem, "%lu|", msg->msg_controllen );
		strSig += string(elem);
		
		sprintf( elem, "%lu|", msg->msg_iovlen );
		strSig += string(elem);
		get_iovecs_signature( msg->msg_iov, msg->msg_iovlen, strSig );
	} else {
		strSig += string("N|");
	}
}




struct msghdr* dup_msghdr( const struct msghdr* msg ) 
{
	struct msghdr* ret = 0;
	if( msg ) {
		ret = (struct msghdr*) malloc( sizeof(struct msghdr) );
		memset( ret, 0, sizeof(struct msghdr) ); // clear
		
		if( msg->msg_namelen > 0 ) {
			ret->msg_name = malloc(msg->msg_namelen);
		} else {
			ret->msg_name = 0;
		} 
		
		if( msg->msg_controllen > 0 ) {
			ret->msg_control = malloc(msg->msg_controllen);
		} else {
			ret->msg_control = 0;
		} 
		
		ret->msg_iovlen = msg->msg_iovlen;
		ret->msg_iov = dup_iovecs( msg->msg_iov, msg->msg_iovlen );
	}
	return ret;
}

void free_iovecs(struct iovec* iov, int iovcnt)
{
	if( iov ) {
		for( int i = 0; i < iovcnt; i++ ) {
			if( iov[i].iov_base ) {
				free( iov[i].iov_base );
			}
		}
		free( iov );
	} 
}


void free_msghdr(struct msghdr* msg)
{
	if( msg ) {
		if( msg->msg_namelen > 0 ) {
			free( msg->msg_name );
		} 
		
		if( msg->msg_controllen > 0 ) {
			free( msg->msg_control ); 
		} 
		
		free_iovecs( msg->msg_iov, msg->msg_iovlen );
		free( msg );
	}
}

void msghdr_to_vector(const struct msghdr* msg, vector<LOG_DATA> &vt) 
{
	if( msg ) {
		APPEND_STRUCT_EXIST( vt, 1, "struct msg",  STRUCT_TYPE_MSGHDR );
		
		APPEND_UINT( vt, msg->msg_namelen, "msg->msg_namelen", STRUCT_TYPE_MSGHDR );
		APPEND_BUF( vt, msg->msg_name, msg->msg_namelen, "msg->msg_name", STRUCT_TYPE_MSGHDR );
		APPEND_UINT( vt, msg->msg_controllen, "msg->msg_controllen", STRUCT_TYPE_MSGHDR );
		APPEND_BUF( vt, msg->msg_control, msg->msg_controllen, "msg->msg_control", STRUCT_TYPE_MSGHDR );
		APPEND_UINT( vt, msg->msg_flags, "msg->msg_flags", STRUCT_TYPE_MSGHDR );
		
		iovecs_to_vector( msg->msg_iov, msg->msg_iovlen, vt );
	} else {
		APPEND_STRUCT_EXIST( vt, 0, "struct msg",  STRUCT_TYPE_MSGHDR );
	}
}


#include <arpa/inet.h>

void addrinfo_to_vector( const struct addrinfo* pAI, vector<LOG_DATA>& vt )
{
	if (pAI) {
		APPEND_STRUCT_EXIST( vt, 1, "struct addrinfo",  STRUCT_TYPE_ADDRINFO );
		
		APPEND_INT( vt, pAI->ai_flags, "pAI->ai_flags", STRUCT_TYPE_ADDRINFO );
		APPEND_INT( vt, pAI->ai_family, "pAI->ai_family", STRUCT_TYPE_ADDRINFO );
		APPEND_INT( vt, pAI->ai_socktype, "pAI->ai_socktype", STRUCT_TYPE_ADDRINFO );
		APPEND_INT( vt, pAI->ai_protocol, "pAI->ai_protocol", STRUCT_TYPE_ADDRINFO );
		APPEND_STR( vt, pAI->ai_canonname, "pAI->ai_canonname", STRUCT_TYPE_ADDRINFO );
		
		DEBUGMSG("addrinfo_to_vector: (%s:%d)\n", __FILE__, __LINE__);
		string str_addr = sockaddr_to_string( (struct sockaddr*)pAI->ai_addr, pAI->ai_addrlen, true );
		DEBUGMSG("addrinfo_to_vector: (%s:%d)\n", __FILE__, __LINE__);
		APPEND_STR( vt, str_addr.c_str(), "pAI->ai_addr", STRUCT_TYPE_ADDRINFO );
	} else {
		APPEND_STRUCT_EXIST( vt, 0, "struct addrinfo",  STRUCT_TYPE_ADDRINFO );
	}
}

void addrinfos_to_vector( struct addrinfo **res, vector<LOG_DATA>& vt )
{
	DEBUGMSG("addrinfos_to_vector: (%s:%d)\n", __FILE__, __LINE__);
	if ( res ) {
		DEBUGMSG("addrinfos_to_vector: (%s:%d)\n", __FILE__, __LINE__);
		APPEND_STRUCT_EXIST( vt, 1, "struct addrinfo*n",  STRUCT_TYPE_ADDRINFOS );
		
		struct addrinfo* pAI = *res;
		while (pAI)
		{
			DEBUGMSG("addrinfos_to_vector: (%s:%d)\n", __FILE__, __LINE__);
			addrinfo_to_vector(pAI, vt);
			DEBUGMSG("addrinfos_to_vector: (%s:%d)\n", __FILE__, __LINE__);
			
			pAI = pAI->ai_next;
		}
		DEBUGMSG("addrinfos_to_vector: (%s:%d)\n", __FILE__, __LINE__);
	} else {
		APPEND_STRUCT_EXIST( vt, 0, "struct addrinfo*n",  STRUCT_TYPE_ADDRINFOS );
	}
	DEBUGMSG("addrinfos_to_vector: (%s:%d)\n", __FILE__, __LINE__);
}


bool vector_to_addrinfo( vector<LOG_DATA>& vt, int& vtidx, vector<struct addrinfo*>& vtRet )
{
	bool exist = false;
	DOES_STRUCT_EXIST( vt[vtidx], STRUCT_TYPE_ADDRINFO, exist ); vtidx++;
	if (exist) {
		//APPEND_STRUCT_EXIST( vt, 1, "struct addrinfo",  STRUCT_TYPE_ADDRINFO );
		char* str;
		size_t str_size;
		struct addrinfo* pAI = (struct addrinfo*)malloc( sizeof(struct addrinfo) );
		
		READ_INT( vt[vtidx], pAI->ai_flags ); vtidx++;//, "pAI->ai_flags", STRUCT_TYPE_ADDRINFO );
		READ_INT( vt[vtidx], pAI->ai_family ); vtidx++;//, "pAI->ai_family", STRUCT_TYPE_ADDRINFO );
		READ_INT( vt[vtidx], pAI->ai_socktype ); vtidx++;//, "pAI->ai_socktype", STRUCT_TYPE_ADDRINFO );
		READ_INT( vt[vtidx], pAI->ai_protocol ); vtidx++;// , "pAI->ai_protocol", STRUCT_TYPE_ADDRINFO );
		READ_STR( vt[vtidx], pAI->ai_canonname, str_size); vtidx++;//"pAI->ai_canonname", STRUCT_TYPE_ADDRINFO );
		
		DEBUGMSG("pAI->ai_flags = %X\n", pAI->ai_flags);
		DEBUGMSG("pAI->ai_family = %X\n", pAI->ai_family);
		DEBUGMSG("pAI->ai_canonname = %s (str_size = %d)\n", pAI->ai_canonname, str_size);
		//string str_addr = sockaddr_to_string( (struct sockaddr*)pAI->ai_addr, pAI->ai_addrlen, true );
		
		READ_STR( vt[vtidx], str, str_size ); vtidx++;//str_addr.c_str(), "pAI->ai_addr", STRUCT_TYPE_ADDRINFO );
		pAI->ai_addr = string_to_sockaddr( str );
		DEBUGMSG("pAI->ai_addr = %s (str_size = %d)\n", str, str_size);
		
		
		vtRet.push_back( pAI );
		
		return true;
	} else {
		//APPEND_STRUCT_EXIST( vt, 0, "struct addrinfo",  STRUCT_TYPE_ADDRINFO );
		return false;
	}
	
	return false;
}

struct addrinfo ** vector_to_addrinfos( vector<LOG_DATA>& vt, int& vtidx )
{
	bool exist = false;
	struct addrinfo ** ret = 0;
	DOES_STRUCT_EXIST( vt[vtidx], STRUCT_TYPE_ADDRINFOS, exist ); vtidx++;
	
	if ( exist ) {
		//APPEND_STRUCT_EXIST( vt, 1, "struct addrinfo*n",  STRUCT_TYPE_ADDRINFOS );
		DEBUGMSG("vector_to_addrinfos 1\n");
		
		vector<struct addrinfo*> vtRet;
		while( vector_to_addrinfo(vt, vtidx, vtRet) ) {
			;
		}
		DEBUGMSG("vector_to_addrinfos 1.5 (%d)\n", vtRet.size());
		
		ret = (struct addrinfo **)malloc( sizeof( struct addrinfo** ) );
		*ret = (struct addrinfo*)malloc( sizeof( struct addrinfo ) );
		struct addrinfo* pAI = *ret;
		
		for( unsigned int i = 0; i < vtRet.size(); i++ ) {
			DEBUGMSG("vector_to_addrinfos 2\n");
			memcpy( pAI, vtRet[i], sizeof( struct addrinfo ) );
			
			DEBUGMSG("pAI->ai_addr = %X\n", pAI->ai_addr );
			DEBUGMSG("pAI->ai_canonname = %s\n", pAI->ai_canonname );
			
			if( i == vtRet.size() - 1 ) {
				pAI->ai_next = 0;
			} else {
				pAI->ai_next = (struct addrinfo*)malloc( sizeof( struct addrinfo ) );
				pAI = pAI->ai_next;
			}
		}
		DEBUGMSG("vector_to_addrinfos 3\n");
	} else {
		DEBUGMSG("vector_to_addrinfos 4\n");
	}
	return ret;
}

/*
 * void addrinfos_to_vector( struct addrinfo **res, vector<LOG_DATA>& vt )
{
	if ( res ) {
		APPEND_STRUCT_EXIST( vt, 1, "struct addrinfo*n",  STRUCT_TYPE_ADDRINFOS );
		
		struct addrinfo* pAI = *res;
		while (pAI)
		{
			addrinfo_to_vector(pAI, vt);
			
			pAI = pAI->ai_next;
		}
	} else {
		APPEND_STRUCT_EXIST( vt, 0, "struct addrinfo*n",  STRUCT_TYPE_ADDRINFOS );
	}
}

void addrinfo_to_vector( const struct addrinfo* pAI, vector<LOG_DATA>& vt )
{
	if (pAI) {
		APPEND_STRUCT_EXIST( vt, 1, "struct addrinfo",  STRUCT_TYPE_ADDRINFO );
		
		APPEND_INT( vt, pAI->ai_flags, "pAI->ai_flags", STRUCT_TYPE_ADDRINFO );
		APPEND_INT( vt, pAI->ai_family, "pAI->ai_family", STRUCT_TYPE_ADDRINFO );
		APPEND_INT( vt, pAI->ai_socktype, "pAI->ai_socktype", STRUCT_TYPE_ADDRINFO );
		APPEND_INT( vt, pAI->ai_protocol, "pAI->ai_protocol", STRUCT_TYPE_ADDRINFO );
		APPEND_STR( vt, pAI->ai_canonname, "pAI->ai_canonname", STRUCT_TYPE_ADDRINFO );
		
		string str_addr = sockaddr_to_string( (struct sockaddr*)pAI->ai_addr, pAI->ai_addrlen, true );
		APPEND_STR( vt, str_addr.c_str(), "pAI->ai_addr", STRUCT_TYPE_ADDRINFO );
	} else {
		APPEND_STRUCT_EXIST( vt, 0, "struct addrinfo",  STRUCT_TYPE_ADDRINFO );
	}
}

 */
int Compare_addrinfo( struct addrinfo* ai1, struct addrinfo* ai2 ) 
{
	if( ai1 && ai2 ) {
		if( ai1->ai_flags != ai2->ai_flags ) {
			return -10;
		}
		if( ai1->ai_family != ai2->ai_family ) {
			return -11;
		}
		if( ai1->ai_socktype != ai2->ai_socktype ) {
			return -12;
		}
		if( ai1->ai_protocol != ai2->ai_protocol ) {
			return -13;
		}
		if( ai1->ai_canonname && ai2->ai_canonname ) {
			if( strcmp(ai1->ai_canonname, ai2->ai_canonname) != 0 ) {
				return -14;
			}
		} else if( ai1->ai_canonname == 0 && ai2->ai_canonname == 0 ) { 
		} else {
			return -15;
		}
		
		
		if( ai1->ai_addr && ai2->ai_addr ) {
			string str_addr1 = sockaddr_to_string( (struct sockaddr*)ai1->ai_addr, ai1->ai_addrlen, true );
			string str_addr2 = sockaddr_to_string( (struct sockaddr*)ai1->ai_addr, ai1->ai_addrlen, true );
			if( strcmp(str_addr1.c_str(), str_addr2.c_str()) != 0 ) {
				return -16;
			}
		} else if( ai1->ai_addr == 0 && ai2->ai_addr == 0 ) {
		} else {
			return -17;
		}
		return 1;
	} else if( ai1 == 0 && ai2 == 0 ) {
		return 1;
	} else {
		return -18;
	} 
	return 0;
}

int Compare_addrinfos( struct addrinfo** ai1, struct addrinfo** ai2 ) 
{
	if( ai1 && ai2 ) {
		struct addrinfo* pAI1 = *ai1;
		struct addrinfo* pAI2 = *ai2;
		
		while (1)
		{
			if( pAI1 && pAI2 ) {
				int ret = Compare_addrinfo( pAI1, pAI2 );
				if( ret != 1 ) {
					return ret;
					break;
				}
			} else if( pAI1 == 0 && pAI2 == 0 ) {
				break;
			} else {
				return -1;
			}
			//addrinfo_to_vector(pAI, vt);
			
			pAI1 = pAI1->ai_next;
			pAI2 = pAI2->ai_next;
		}
		return 1;
	} else if( ai1 == 0 && ai2 == 0 ) {
		return 1;
	}
	return 0;
}


void dirent_to_vector(struct dirent* dir, vector<LOG_DATA>& vt)
{
	
	if( dir ) {
		APPEND_STRUCT_EXIST( vt, 1, "struct dirent", STRUCT_TYPE_DIRENT );
		
		APPEND_UINT64( vt, dir->d_ino, "dir->d_ino", STRUCT_TYPE_DIRENT );
		APPEND_UINT64( vt, dir->d_off, "dir->d_off", STRUCT_TYPE_DIRENT ); 
		APPEND_INT( vt, dir->d_reclen, "dir->d_reclen", STRUCT_TYPE_DIRENT ); 
		APPEND_INT( vt, dir->d_type, "dir->d_type", STRUCT_TYPE_DIRENT );
		APPEND_STR( vt, dir->d_name, "dir->d_name", STRUCT_TYPE_DIRENT ); 
	} else {
		APPEND_STRUCT_EXIST( vt, 1, "struct dirent",  STRUCT_TYPE_DIRENT );
	}
	
}

void dirent64_to_vector(struct dirent64* dir, vector<LOG_DATA>& vt)
{
	if( dir ) {
		APPEND_STRUCT_EXIST( vt, 1, "struct dirent64",  STRUCT_TYPE_DIRENT64 );
		
		APPEND_UINT64( vt, dir->d_ino, "dir->d_ino", STRUCT_TYPE_DIRENT64 );
		APPEND_UINT64( vt, dir->d_off, "dir->d_off", STRUCT_TYPE_DIRENT64 ); 
		APPEND_INT( vt, dir->d_reclen, "dir->d_reclen", STRUCT_TYPE_DIRENT64 ); 
		APPEND_INT( vt, dir->d_type, "dir->d_type", STRUCT_TYPE_DIRENT64 );
		APPEND_STR( vt, dir->d_name, "dir->d_name", STRUCT_TYPE_DIRENT64 ); 
	} else {
		APPEND_STRUCT_EXIST( vt, 1, "struct dirent64",  STRUCT_TYPE_DIRENT64 );
	}
}


string PointerToString(void* ptr)
{
	char tmp[50];
	sprintf(tmp, "%p", ptr);
	return string(tmp);
}



//////////////////////
// old
/*
void dupFdRsrcName(int newfd, int oldfd)
{
	map<int, string>::iterator itold = g_map_fd_name.find( oldfd );
	if( itold != g_map_fd_name.end() ) {
		map<int, string>::iterator itnew = g_map_fd_name.find( newfd );
		if (itnew != g_map_fd_name.end() ) {
			itnew->second = itold->second; // copy
		} else {
			g_map_fd_name.insert( pair<int, string>(newfd, itold->second) ); // add
		}
	} else {
		return ;
	}
}

void setFdRsrcName(int fd, string filename)
{
	map<int, string>::iterator it = g_map_fd_name.find( fd );
	if( it != g_map_fd_name.end() ) {
		it->second = filename;
	} else {
		g_map_fd_name.insert( pair<int, string>(fd, filename) );
	}
}

int RsrcNameExist(int fd)
{
	map<int, string>::iterator it = g_map_fd_name.find( fd );
	if( it != g_map_fd_name.end() ) {
		return 1;
	}
	
	return 0;	
}

string getRsrcNameFromFd(int fd)
{
	map<int, string>::iterator it = g_map_fd_name.find( fd );
	if( it != g_map_fd_name.end() ) {
		return it->second;
	}
	
	//
	// try to figure this out
	//
	//string ret = string("");
	//char filename[4096] = { 0, };
	//if( recover_filename( fd, filename, 4096 ) ) {
	//	ret = string(filename);
		//setFdRsrcName( fd, ret );
	//}
	return string("");
		
}
*/




string z_streamp_to_string(z_streamp stream)
{
	string ret;
	if( stream ) {
		ret = string("zstream:") + P2S( stream->next_in, stream->avail_in );
	} else {
		char tmp[50];
		sprintf(tmp, "Raw:%p", stream );
		ret = string(tmp);
	} 
	return ret;
}












#endif
