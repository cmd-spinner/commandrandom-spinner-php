#include <stdio.h>
#include "models.h"

int main()
{
	int i = 0, j; 
	char temp[1024];
	for( i = 0; ; i++ ) {
		if( SYSCALL_NAMES[i] == 0 ) break;
		
		
		strcpy( temp, SYSCALL_NAMES[i] );
		for( j = 0; j < strlen(temp); j++ ) {
			temp[j] = toupper(temp[j]);
		}
		
		printf("#define SC_%s\t\t%d\t//%s\n", temp, i, SYSCALL_NAMES[i]);
	}
}