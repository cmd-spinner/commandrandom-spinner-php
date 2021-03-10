
#ifndef _STACK_HEADER_FILE_
#define _STACK_HEADER_FILE_ 1

#define DEBUG_MODE 0

#define MAX_STACK_SIZE 250

typedef struct {
	int contents[MAX_STACK_SIZE+1];
} stackT;

#define STACK_TOP(__stackP) __stackP->contents[MAX_STACK_SIZE]

void inline StackInit(stackT *stackP)
{
	STACK_TOP(stackP) = -1;  /* I.e., empty */
}

int inline StackGetSize(stackT *stackP)
{
	return STACK_TOP(stackP);
}

int inline StackIsEmpty(stackT *stackP)
{
	return STACK_TOP(stackP) < 0;
}

int inline StackIsFull(stackT *stackP)
{
	return STACK_TOP(stackP) >= MAX_STACK_SIZE - 1;
}

#define FAST_StackPush(__stackP, __element) \
	__stackP->contents[++STACK_TOP(__stackP)] = __element;

void inline StackPush(stackT *stackP, int element)
{
#if DEBUG_MODE
	if (StackIsFull(stackP)) {
		fprintf(stderr, "Can't push element on stack: stack is full.\n");
		exit(1);  /* Exit, returning error code. */
	}
#endif
	/* Put information in array; update top. */
	
	stackP->contents[++STACK_TOP(stackP)] = element;
}


int inline StackPop(stackT *stackP)
{
#if DEBUG_MODE
	if (StackIsEmpty(stackP)) {
		fprintf(stderr, "Can't pop element from stack: stack is empty.\n");
		exit(1);  /* Exit, returning error code. */
	}
#endif
	return stackP->contents[STACK_TOP(stackP)--];
}

int inline copyStack( stackT* dest, stackT* src )
{
	memcpy( dest, src, sizeof( stackT ) );
}

int inline StackCompare( stackT* s1, stackT* s2 )
{
	if( STACK_TOP(s1) == STACK_TOP(s2) ) {
		if( STACK_TOP(s1) == -1 ) {
			return 0;
		}
		
		for( int i = 0; i < STACK_TOP(s1); i++ ) {
			if( s1->contents[i] == s2->contents[i] ) {
				continue;
			} else if( s1->contents[i] > s2->contents[i] ) {
				return 1;
			} else {
				return 2;
			} 
		}
		
		return 0;
	} else if( STACK_TOP(s1) > STACK_TOP(s2) ) {
		for( int i = 0; i < STACK_TOP(s2); i++ ) {
			if( s1->contents[i] == s2->contents[i] ) {
				continue;
			} else if( s1->contents[i] > s2->contents[i] ) {
				return 1;
			} else {
				return 2;
			} 
		}
		
		return 1;
	} else {
		for( int i = 0; i < STACK_TOP(s1); i++ ) {
			if( s1->contents[i] == s2->contents[i] ) {
				continue;
			} else if( s1->contents[i] > s2->contents[i] ) {
				return 1;
			} else {
				return 2;
			} 
		}
		
		return 2;
	} 
	
	return -1;
}

#endif // _STACK_HEADER_FILE_

