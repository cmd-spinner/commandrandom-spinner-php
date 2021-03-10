


#ifndef __RINGBUF_HEADER__
#define __RINGBUF_HEADER__ 1
#include <unistd.h>

int inline pop_SYSCALLINFO(int execnum, PSCINFO pList, int* pListIndex, simplemu mutex, PSCINFO pRetItem, int debugnum);

PSCINFO_RINGBUF 	__hook_RINGBUF[3];
extern simplemu 	g_futexRingBuf[MAX_SYSCALL_NUM + FUTEX_OTHERS];


void inline print_ring_buf();


/*
 * 
typedef struct tag_SCINFO_RINGBUF {
	int 			sysinfoIdx[MAX_SYSCALL_NUM * MAX_SYSINFOIDX_COUNT];
	SCINFO 			sysinfo[MAX_RINGBUF_COUNT];
	int 			index[MAX_SYSCALL_NUM * 2 *MAX_EXECUTIONS];
	simplemu 		mutex[MAX_SYSCALL_NUM];
} SCINFO_RINGBUF, *PSCINFO_RINGBUF;
 */
#define GET_RINGBUF_INDEX(__syscallnum) (__syscallnum * RINGBUFFER_IDX_CNT)
#define MASTER 	0
#define SLAVE	1



void inline makeEmpty(int* pListIndex)
{
	/*
	DEBUGMSG("makeEmpty (%08X %08X)\n",
		&pListIndex[FIRST_INDEX], &pListIndex[LAST_INDEX]
	);
	show_full_call_stack(0,4);
	*/
	
	
	pListIndex[FIRST_INDEX] = -1;
	pListIndex[LAST_INDEX] = -1;
}


void inline get_RINGBUF_size(int &nFirstBuffer,
							 int &nSecondBuffer, 
							 int &nTotalSize)
{
	
	int nAvoidFalseSharing = 1024 * 512;
	int nOneBufferSize = 	sizeof(SCINFO_RINGBUF) + nAvoidFalseSharing;
	
	nFirstBuffer = 0;
	nSecondBuffer = nOneBufferSize;
	nTotalSize = nOneBufferSize * 2;
}


void inline set_RINGBUF_pointers(unsigned char* pBuffer)
{
	unsigned int i = 0;
	int nFirstBuffer, nSecondBuffer, nTotalSize;
	get_RINGBUF_size(nFirstBuffer, nSecondBuffer, nTotalSize);
	
	__hook_RINGBUF[MASTER] = (PSCINFO_RINGBUF)&(pBuffer[nFirstBuffer]);
	__hook_RINGBUF[SLAVE]  = (PSCINFO_RINGBUF)&(pBuffer[nSecondBuffer]);
}



void inline init_RINGBUF(unsigned char* pBuffer, int first_run)
{
	unsigned int i;
	set_RINGBUF_pointers( pBuffer );
	
	// init...
	if( IS_MASTER_EXEC() && first_run ) {
		//DEBUGMSG("init_SYSCALLINFO - makeEmpty\n");
		for( i = 0; i < MAX_SYSCALL_NUM; i++ ) {
			makeEmpty( &( __hook_RINGBUF[MASTER]->index[GET_RINGBUF_INDEX(i)] ) );
			makeEmpty( &( __hook_RINGBUF[SLAVE]->index[GET_RINGBUF_INDEX(i)] ) );
		}
		
		for( i = 0; i < MAX_SYSCALL_NUM; i++ ) {
			//simplefu_mutex_init(__hook_RINGBUF[MASTER]->mutex[GET_RINGBUF_MUTEX(i)]);
			//simplefu_mutex_init(__hook_RINGBUF[SLAVE]->mutex[GET_RINGBUF_MUTEX(i)]);		
		}
	}
}


void inline push_SYSCALLINFO(PSCINFO pList, int* pListIndex, simplemu mutex, PSCINFO pNewItem, int debugnum)
{
	//DEBUGMSG("push_SYSCALLINFO CALLED\n");
	//DEBUGMSG(" ---- (%d) [mutex=%x]\n", debugnum, mutex);
	while(1) {
		//DEBUGMSG("before mutex\n");
		simplefu_mutex_lock   (mutex);
		//DEBUGMSG("after mutex\n");
		
		if( pListIndex[LAST_INDEX] == pListIndex[FIRST_INDEX] && pListIndex[LAST_INDEX] == -1 ) { // empty
			pListIndex[FIRST_INDEX] = 0;
			pListIndex[LAST_INDEX] = 1;
			
			pNewItem->consumed = 0;
			memcpy( &pList[ pListIndex[FIRST_INDEX] ], pNewItem, sizeof( SCINFO ) );
			//void ReadParam( unsigned char* p, unsigned int offset, unsigned int size )
			//unsigned int aa[10];
			//pList[ pListIndex[FIRST_INDEX] ].ReadParam((unsigned char*)aa, 0, sizeof(unsigned int)*4);
			
			//DEBUGMSG("// dbg // push_SYSCALLINFO: (%d:%d)\n", 
			//		 debugnum, pListIndex[FIRST_INDEX]
			//	 	);
			
			simplefu_mutex_unlock (mutex);
			break;
		} else {
			// 0 - 99
			int curr = pListIndex[LAST_INDEX];
			int next = RINGBUF_NEXTITEM( curr );
			
			if( next == pListIndex[FIRST_INDEX] ) { // full
				//DEBUGMSG("ring buffer is full (%d)\n", debugnum);
				// ===== begin test =====
				//
				// just for test -- drop all messages from here.
				//
				//simplefu_mutex_unlock (mutex);
				//break;
				// ===== end test =====
				
				simplefu_mutex_unlock (mutex);
				wait_short();
				
				//{
				//	SCINFO dummy;
				//	pop_SYSCALLINFO( pList, pListIndex, mutex, &dummy, debugnum );
				//}
				
				continue;
			} else {
				//DEBUGMSG("// dbg // push_SYSCALLINFO: %d ~ %d [%08X %08X] (syscallnum = %d)\n", 
				//		 pListIndex[FIRST_INDEX], next,
				//		 &(pListIndex[FIRST_INDEX]), &(pListIndex[LAST_INDEX]), debugnum
				//		);
				
				pNewItem->consumed = 0;
				memcpy( &pList[curr], pNewItem, sizeof( SCINFO ) );
				pListIndex[LAST_INDEX] = next;
				
				//DEBUGMSG("// dbg // push_SYSCALLINFO[2]: (%d:%d)\n", 
				//	 debugnum, curr 
				 //	);
				
				
				simplefu_mutex_unlock (mutex);
				break;
			}
		}
	}
	//print_ring_buf();
}

int inline pop_SYSCALLINFO(int execnum, PSCINFO pList, int* pListIndex, simplemu mutex, PSCINFO pRetItem, int debugnum)
{
	simplefu_mutex_lock   (mutex);
	
	// 0 - 99	
	if( pListIndex[LAST_INDEX] == pListIndex[FIRST_INDEX] ) {
		if( pListIndex[FIRST_INDEX] == -1 ) { // empty
			simplefu_mutex_unlock (mutex);	
			// empty
			return 0; // no item to return 
		} else { // one item is there
			// this is not possible
			DEBUGMSG("pop_SYSCALLINFO -- error (not possible) [%d - %d] (%d)\n", pListIndex[FIRST_INDEX], pListIndex[LAST_INDEX], debugnum);
			simplefu_mutex_unlock (mutex);	
			exit(-1);
			return 1;
		}
	}
	
	int curr = pListIndex[FIRST_INDEX];
	while(1) {
		int next = RINGBUF_NEXTITEM( curr );
		if( next == pListIndex[LAST_INDEX] ) { // empty
			simplefu_mutex_unlock (mutex);	
			//DEBUGMSG("pop_SYSCALLINFO -- empty-2 [%d - %d] (syscallnum = %d)\n", curr, pListIndex[LAST_INDEX], debugnum);
			// empty
			return 0; // no item to return 
		}
		
		if( ((execnum == EI_SLAVE1) && (pList[ curr ].consumed & 0x1) == 0x1) ||
			((execnum == EI_SLAVE2) && (pList[ curr ].consumed & 0x2) == 0x2) ) {
			curr = next;
			continue;
		}
		
		if( execnum == EI_SLAVE1 ) {
			pList[ curr ].consumed |= 0x1;
		} else if( execnum == EI_SLAVE2 ) {
			pList[ curr ].consumed |= 0x2;
		}
		memcpy( pRetItem, &pList[ curr ], sizeof( SCINFO ) );
		
		//void ReadParam( unsigned char* p, unsigned int offset, unsigned int size )
		//unsigned int aa[4];
		//pRetItem->ReadParam( (unsigned char*)aa, 0, sizeof(unsigned int)*4 );
		if( pList[curr].consumed == 0x3 ) { // all consumed.
			pListIndex[FIRST_INDEX] = next;	
			DEBUGMSG("pop_SYSCALLINFO - all consumed (%d:%d) \n", debugnum, curr);
		} else {
			DEBUGMSG("pop_SYSCALLINFO - one consumed (%d:%d) \n", debugnum, curr);
		}
		
		break;
	}
		
	simplefu_mutex_unlock (mutex);
	return 1;
}


void inline check_SYSCALLINFO_begin(int syscallnum)
{
	simplefu_mutex_lock(g_futexRingBuf[syscallnum]); //__hook_RINGBUF[nExecIndex]->mutex[GET_RINGBUF_MUTEX(syscallnum)]);
}

void inline check_SYSCALLINFO_end(int syscallnum)
{
	simplefu_mutex_unlock(g_futexRingBuf[syscallnum]); //__hook_RINGBUF[nExecIndex]->mutex[GET_RINGBUF_MUTEX(syscallnum)]);
}


int inline pop_SYSCALLINFO(int execnum, PSCINFO pRetItem, int nRingBufIdx, int syscallnum)
{
	return pop_SYSCALLINFO( execnum, 
							&(__hook_RINGBUF[nRingBufIdx]->sysinfo[syscallnum * MAX_RINGBUF_COUNT]),
							&(__hook_RINGBUF[nRingBufIdx]->index[GET_RINGBUF_INDEX(syscallnum)]), 
							g_futexRingBuf[syscallnum],
							pRetItem, syscallnum );
}


void inline push_SYSCALLINFO(PSCINFO pNewItem, int nRingBufIdx, int syscallnum)
{
	push_SYSCALLINFO( &(__hook_RINGBUF[nRingBufIdx]->sysinfo[syscallnum * MAX_RINGBUF_COUNT]),
					  &(__hook_RINGBUF[nRingBufIdx]->index[GET_RINGBUF_INDEX(syscallnum)]),
					  g_futexRingBuf[syscallnum],
					  pNewItem, syscallnum );
}

void inline print_ring_buf()
{
	
			 
}

#endif


