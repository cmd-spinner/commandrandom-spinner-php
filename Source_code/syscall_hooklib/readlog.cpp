
#define DISABLE_DEBUGMSG2_MACRO 1
#define LEAVE_USERFUNC /**/
#define ENTER_USERFUNC /**/
#include <stdio.h>

#include <sys/stat.h>
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

using namespace std;


typedef int (*PFNORG_OPEN)( const char* pathname, int flags, mode_t mode );
typedef FILE* (*PFNORG_FOPEN)(const char *path, const char *mode);
typedef size_t (*PFNORG_FWRITE)(const void* ptr, size_t size, size_t nmemb, FILE* stream);
typedef int (*PFNORG_FCLOSE)( FILE* fp );
typedef size_t (*PFNORG_FREAD)(void * ptr, size_t size, size_t nitems, FILE * stream);
//ssize_t readlink(const char *path, char *buf, size_t bufsiz);
typedef ssize_t (*PFNORG_READLINK)(const char *path, char *buf, size_t bufsiz);



//PFNORG_OPEN orgfn_open = open;
PFNORG_FOPEN orgfn_fopen = fopen;
PFNORG_FWRITE orgfn_fwrite = fwrite;
PFNORG_FCLOSE orgfn_fclose = fclose;
PFNORG_FREAD orgfn_fread = fread;


#include "util_logio.h"




int main(int argc, char** argv)
{

	LoadResourceFiles( argv[1], g_mapRsrcMap );
	
	
	//int a;
	//scanf("%d", &a);
	// load resource
	
	
	return 0;
}
