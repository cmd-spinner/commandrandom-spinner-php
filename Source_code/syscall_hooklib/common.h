
#ifndef COMMON_HEADER_INCLUDED 
#define COMMON_HEADER_INCLUDED  1 


#define XSTR(xx) #xx
#define STR(yy) XSTR(yy)


#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <dirent.h>


#define P2S(___buffer, ___size) GetPrintableBuf(___buffer, ___size).c_str()


#define VT_COPY(__dst, __src) {\
			for( unsigned int __i__ = 0; __i__ < __src.size(); __i__++ ) {\
				__dst.push_back( __src[__i__] );\
			}\
		}

#define VT_COPY2(__dst, __src, __src_size) {\
			for( unsigned int __i__ = 0; __i__ < __src_size; __i__++ ) {\
				__dst.push_back( __src[__i__] );\
			}\
		}

#define POLY 0x8408


#define string_left( __strx__, len ) __strx__.substr(0, len)


/*
//                                      16   12   5
// this is the CCITT CRC 16 polynomial X  + X  + X  + 1.
// This works out to be 0x1021, but the way the algorithm works
// lets us use 0x8408 (the reverse of the bit pattern).  The high
// bit is always assumed to be set, thus we only use 16 bits to
// represent the 17 bit value.
*/

inline unsigned short crc16(const char *data_p, unsigned short length)
{
      unsigned char i;
      unsigned int data;
      unsigned int crc = 0xffff;

      if (length == 0)
            return (~crc);

      do
      {
            for (i=0, data=(unsigned int)0xff & *data_p++;
                 i < 8; 
                 i++, data >>= 1)
            {
                  if ((crc & 0x0001) ^ (data & 0x0001))
                        crc = (crc >> 1) ^ POLY;
                  else  crc >>= 1;
            }
      } while (--length);

      crc = ~crc;
      data = crc;
      crc = (crc << 8) | (data >> 8 & 0xff);

      return (crc);
}



inline int IsFileExist(char* fname)
{
	if( access( fname, F_OK ) != -1 ) {
		// file exists
		return 1;
	} else {
		// file doesn't exist
		return 0;
	}
	return 0;
}



/* assembly code to read the TSC */
static inline uint64_t RDTSC()
{
  unsigned int hi, lo;
  __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((uint64_t)hi << 32) | lo;
}



inline string GetPrintableBuf_Text(const void* buf, size_t len)
{
	string output;
	
	if( buf ) {
		char temp[10] = { 0, };
		const char* sbuf = (const char*)buf;
		for( unsigned int i = 0; i < len; i++ ) {
			if( 32 <= sbuf[i] && sbuf[i] <= 126 ) {
				output += sbuf[i];
			} else if( sbuf[i] == '\n' || sbuf[i] == '\r' || sbuf[i] == '\t' ) {
				output += '\\';
				switch( sbuf[i] ) {
					case '\n': output += 'n'; break;
					case '\r': output += 'r'; break;
					case '\t': output += 't'; break;
				}
			} else {
				output += ' ';
			}
		}
	} else {
		output = string("");
	}
	return output;
}



inline string GetPrintableStringArray(char* const buf[]) 
{
	string output;
	if( buf ) {
		string tmp;
		for( unsigned int i = 0; ; i++ ) {
			if( buf[i] == 0 ) break;
			
			tmp = buf[i];
			if( output.length() == 0 ) {
				output += tmp;
			} else {
				output += string("|") + tmp;
			} 
		}
	} else {
		output = string("");
	} 
	return output;
}

inline string GetPrintableBuf(const void* buf, size_t len)
{
	string output;
	
	if( buf ) {
		char temp[20];
		const char* sbuf = (const char*)buf;
		for( unsigned int i = 0; i < len; i++ ) {
			sprintf(temp, "x%02X", (unsigned char)sbuf[i]);
			output += temp;
		}
	} else {
		output = string("");
	}
	return output;
}

inline void EnumFiles(const char* path, vector<string> &vtfilename, vector<string> &vtdir, int deep = 0);
inline void EnumFiles(const char* path, vector<string> &vtfilename, vector<string> &vtdir, int deep)
{
	DIR           *d;
	struct dirent *dir;
	d = opendir(path);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if( strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 ) {
				continue;
			}
			if( dir->d_type == DT_DIR ) {
				char new_path[4096] = { 0, };
				int pathlen = strlen(path);
				strcpy(new_path, path);
				if( path[pathlen-1] != '/') {
					strcat(new_path, "/");
				}
				strcat(new_path, dir->d_name);
				
				
				//for( int i = 0; i < deep; i++ ) printf("\t");
				//printf("DIR: %s\n", dir->d_name);
				EnumFiles(new_path, vtfilename, vtdir, deep+1);
			} else {
				//for( int i = 0; i < deep; i++ ) printf("\t");
				//printf("FILE: %s\n", dir->d_name);
				vtfilename.push_back( dir->d_name );
				vtdir.push_back( path );
			}
		}
		closedir(d);
	}
}

inline bool startsWith(const string& fullstring, const string& starting) {
    return starting.length() <= fullstring.length() 
        && equal(starting.begin(), starting.end(), fullstring.begin());
}



inline bool endsWith(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}


inline int find_string_vt(vector<string>& vt, string &tofind) 
{
	for(int i = 0; i < (int)vt.size(); i++ ) {
		if( strcmp( vt[i].c_str(), tofind.c_str() ) == 0 ) { 
			return i;
		}
	}
	return -1;
}

inline const char* safe_strdup(const char* s)
{
	if( s ) {
		return strdup(s);
	}
	return "";
}

inline bool is_safe_index(size_t size, int index) 
{
	// size  1
	// index 0 1 ...
	//       O X
	
	// size  2
	// index 0 1 2 ...
	//       O O X
	
	if( size > index ) {
		return true;
	}
	return false;
}

#include <execinfo.h>
#include <dlfcn.h>


inline void PrintCallStack(int idx_begin, int cnt)
{
	// 2 - 3
	// 0 1 2 3 4 5 6
	// - - * * * 
	void* tracePtrs[64];
	int count = backtrace( tracePtrs, cnt + idx_begin );
	
	char** funcNames = backtrace_symbols( tracePtrs, count);
	
	int idx = 0;
	int printed_cnt = 0;
	for( int ii = 0; ii < count; ii++ ) {
		//if( isInHookMod(tracePtrs[ii]) ) { continue; }
		//if( !strchr( funcNames[ii], '/' ) ) { continue; };
		//DEBUGMSG( "[%08X] %s\n", (unsigned int)tracePtrs[ii], funcNames[ii] );
		
		//DEBUGMSG( "\t\t<%d> [%08X] %s\n", cnt++, (unsigned int)tracePtrs[ii], funcNames[ii] );
		//DEBUGMSG("idx >= idx_begin : %d >= %d?, cnt = %d?\n", idx, idx_begin, cnt );
		if( idx >= idx_begin ) {
			puts(funcNames[ii]);
			printed_cnt++;
		}
		
		idx++;
		if( printed_cnt == cnt ) {
			break;
		}
		//break;
	}

	free(funcNames);
}

#include "sharedmem.h"



#endif


