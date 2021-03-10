#define FUNCNAME 				pipe
#define SYSCALL_NUM				SC_PIPE
#define FUNCRETURN				int
#define FUNCPARAM 				int pipefd[2]
#define FUNCPARAM_INVOKE 		pipefd


//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char* param_def[100];
char* param_name[100];
int param_def_cnt = 0;
int param_name_cnt = 0;

#define xstr(s) str(s)
#define str(s) #s

char *
strrstr(char *string, char *find)
{
	size_t stringlen, findlen;
	char *cp;

	findlen = strlen(find);
	stringlen = strlen(string);
	if (findlen > stringlen)
		return NULL;

	for (cp = string + stringlen - findlen; cp >= string; cp--)
		if (strncmp(cp, find, findlen) == 0)
			return cp;

	return NULL;
}


char * trim(char * s) {
    int l = strlen(s);

    while(isspace(s[l - 1])) --l;
    while(* s && isspace(* s)) ++s, --l;

    return strndup(s, l);
}

int main(int argc, char** argv)
{
  char* funcname = xstr(FUNCNAME);
  char* funcreturn = xstr(FUNCRETURN);
  char* _funcparam  = xstr((FUNCPARAM));
  char funcparam[4096];
  char funcparam_invoke[4096];
  char* _funcparam_invoke  = xstr((FUNCPARAM_INVOKE));
  const char s[2] = ",";
  char* token;
  int i = 0;

  strcpy( funcparam, &_funcparam[1] );
  strcpy( funcparam_invoke, &_funcparam_invoke[1] );
  funcparam[strlen(funcparam)-1] = 0;
  funcparam_invoke[strlen(funcparam_invoke)-1] = 0;
   
  
 
  token = strtok(funcparam, s);   
  while( token != NULL ) {
    param_def[param_def_cnt++] = trim(token);
    token = strtok(NULL, s);
  }

  token = strtok(funcparam_invoke, s);   
  while( token != NULL ) {
    param_name[param_name_cnt++] = trim(token);
    token = strtok(NULL, s);
  }

  for( i = 0; i < param_def_cnt; i++ ) {
    //printf("[%s] -- [%s]\n", param_def[i], param_name[i]);
  }
  //printf("%d %d\n", param_def_cnt, param_name_cnt);

  //printf("%s\n", funcname);
  //printf("%s\n", funcparam);
  //printf("%s\n", funcparam_invoke);
  if( param_def_cnt != param_name_cnt )
  {
    printf("Error!!!");
    return 0;
  }
  printf("\n\n\nFUNC_P%d(%s, %s, ", param_def_cnt, funcreturn, funcname);
  for( i = 0; i < param_def_cnt; i++ ) {
    char* p = strrstr(param_def[i], param_name[i]);
    char* type;
    char* def;
    char old_p;
    if( p ) {
      //p--;
      old_p = *p;
      *p = 0;
    }
    type = trim(param_def[i]);
    if( p ) {
      *p = old_p;
    }
    def = trim(p);
    printf("%s, %s", type, def);
    if( i < param_def_cnt-1) printf(", ");
  }
  printf(")\n{\n\n}\nFUNC_END();\n\n\n\n");



  

  return 0;
}

