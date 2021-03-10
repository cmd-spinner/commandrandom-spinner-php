#include <sys/ipc.h>
#include <sys/shm.h>

typedef struct tag_SHMEM_OBJ {
	int size;
	int curidx;
} SHMEM_OBJ;



inline void* sm_getptr( SHMEM_OBJ* pshmobj, unsigned int offset ) 
{
	// if( offset >= pshmobj->size ) {
	// 	return 0;
	// }
	
	// char* base = (char*) pshmobj;
	// char* buf  = (char*) &( base[sizeof(SHMEM_OBJ)] );
	
	// return (void*)&(buf[offset]);
}

inline void* sm_getbaseptr( SHMEM_OBJ* pshmobj ) 
{
	return sm_getptr(pshmobj, 0);
}


inline unsigned int smalloc( SHMEM_OBJ* pobj, int size, bool init_zero = false )
{
	if( size + pobj->curidx >= pobj->size ) {
		char tmp[1024]; 
		// PrintCallStack(0, 20);
		sprintf(tmp, "GetSharedMemBuffer returns 0 (%x + %x >= %x)\n", size, pobj->curidx, pobj->size );
		perror( tmp );
		return 0;
	}
	
	char* base = (char*) pobj;
	char* buf  = (char*) &( base[sizeof(SHMEM_OBJ)] );
	
	unsigned int retoffset = pobj->curidx;
	
	if(init_zero) {
		void* ret = (void*) &(buf[pobj->curidx]);
		memset( ret, 0, size );
	}
	pobj->curidx += size;
	
	return retoffset;
}

// shared memory...
inline SHMEM_OBJ* smcreate(int key, int size, bool doinit, int* pnretError)
{
#define _SET_ERR(__err_n) if( pnretError ) *pnretError = __err_n;
	
	int shmid;
	
	_SET_ERR( 0 );
	if ((shmid = shmget(key, size, IPC_CREAT | 0666)) < 0) {
		_SET_ERR( 1 );
        return 0;
    }	
	void* pshm_ret = 0;
	if ((pshm_ret = shmat(shmid, NULL, 0)) == (void *) -1) {
		_SET_ERR( 2 );
		return 0;
    }
    
    //////////////////////
    char* pshm_metadata = (char*) pshm_ret;
	char* pshm_buf      = (char*) &(pshm_metadata[sizeof(SHMEM_OBJ)]);
    
    SHMEM_OBJ* pobj = (SHMEM_OBJ*)pshm_metadata;
	
    if( doinit ) {
		pobj->size = size - sizeof(SHMEM_OBJ);
		pobj->curidx = 0;
	}
	
#undef _SET_ERR
    return pobj;
}

/*
char* ReadEntireFile(FILE* f, long& retsize)
{
	long lSize;
	orgfn_fseek (f, 0 , SEEK_END);
	lSize = orgfn_ftell (f);
	orgfn_rewind (f);
	
	char* buf = (char*)malloc( lSize );
	
	orgfn_fread( buf, 1, lSize, f );
	retsize = lSize;
	
	return buf;
}
*/


