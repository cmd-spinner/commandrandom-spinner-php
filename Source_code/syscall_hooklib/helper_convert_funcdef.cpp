#define FUNCNAME 				pipe
#define SYSCALL_NUM				SC_PIPE
#define FUNCRETURN				int
#define FUNCPARAM 				int pipefd[2]
#define FUNCPARAM_INVOKE 		pipefd

#define FILENAME "hook_func_oper.h"

//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "commonlibs/readfile.h"

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
	//char* funcname = xstr(FUNCNAME);
	//char* funcreturn = xstr(FUNCRETURN);
	char funcname[1024];
	char funcreturn[1024];
	char* _funcparam  = xstr((FUNCPARAM));
	char funcparam[4096];
	char funcparam_invoke[4096];
	char* _funcparam_invoke  = xstr((FUNCPARAM_INVOKE));
	const char s[2] = ",";
	char* token;
	int i = 0;
	vector<string> vtlines;
	vector<string> vtnewfile;

	readfile( FILENAME, vtlines );
	/*
	#if (__ENABLE_HOOK_pread64 == 1)
	#define FUNCNAME 				pread64
	#define SYSCALL_NUM				SC_PREAD64
	#define FUNCRETURN				ssize_t
	#define FUNCPARAM 				int fd, void *buf, size_t count, off64_t offset
	#define FUNCPARAM_INVOKE 		fd, buf, count, offset

	extern "C" FUNCRETURN FUNCNAME( FUNCPARAM )
	{
		HOOK_FUNC_OPER_HEADER2
		{
			RSRC_VARS;
			string fd_str = GetFD(fd, fd_type);
		}
		LEAVE_FUNC;
		return ret;
	}

	*/  
	printf("%d\n", vtlines.size());
	string meta[5];
	string str_newFuncDef;
	int func_level = 0;
	bool skip;
	for( i = 0; i < vtlines.size(); i++ ) {
		skip = false;
		string line = vtlines[i];
		printf("%d.", i);
		//vtnewfile.push_back(line);
		
		//#define FUNCNAME
		if (line.find("#define FUNCNAME\t") == 0 || line.find("#define FUNCNAME ") == 0) { 
			//printf("[%d] <<%s>>\n", i, line.c_str()); 
			meta[0] = line; func_level++; skip = true; 
			
		}
		if (line.find("#define FUNCRETURN\t") == 0 || line.find("#define FUNCRETURN ") == 0) { 
			//printf("[%d] <<%s>>\n", i, line.c_str()); 
			meta[1] = line; func_level++; skip = true; 
			
		}
		if (line.find("#define FUNCPARAM\t") == 0 || line.find("#define FUNCPARAM ") == 0) {
			//printf("[%d] <<%s>>\n", i, line.c_str());
			meta[2] = line; func_level++; skip = true; 
			
		}
		if (line.find("#define FUNCPARAM_INVOKE\t") == 0 || line.find("#define FUNCPARAM_INVOKE ") == 0) { 
			//printf("[%d] <<%s>>\n", i, line.c_str());
			meta[3] = line; func_level++; skip = true; 
			
		}
		if (line.find("#define SYSCALL_NUM\t") == 0 || line.find("#define SYSCALL_NUM ") == 0) { 
			//printf("[%d] <<%s>>\n", i, line.c_str());
			meta[4] = line; func_level++; skip = true; 
			
		}
		
		if( line == string("#include \"hook_func_undef_macros.h\"\n") ) {
			skip = true;
		}
		
		
		
		if ( (  (line == string("extern \"C\" FUNCRETURN FUNCNAME( FUNCPARAM )\n")) ||
			    (line == string("extern \"C\" FUNCRETURN FUNCNAME(FUNCPARAM)\n")) ||
			    (line == string("extern FUNCRETURN FUNCNAME( FUNCPARAM )\n")) ||
			    (line == string("extern FUNCRETURN FUNCNAME(FUNCPARAM)\n")) ||
			    (line == string("extern \"C\" FUNCRETURN FUNCNAME( FUNCPARAM ) \n")) ||
			    (line == string("extern \"C\" FUNCRETURN FUNCNAME(FUNCPARAM) \n")) ||
			    (line == string("extern FUNCRETURN FUNCNAME( FUNCPARAM ) \n")) ||
			    (line == string("extern FUNCRETURN FUNCNAME(FUNCPARAM) \n"))
			 ) 
			&& func_level == 5) {
			printf("func start. [%d]\n", i+1);
			
			for( int j = 0; j < 5; j++ ) {
				meta[j] = meta[j].substr( meta[j].rfind("\t")+1 );
				meta[j] = meta[j].substr( 0, meta[j].length()-1 );
			//	printf("<%d> [[%s]]\n", j, meta[j].c_str());
			}
			if( meta[2] == string("/**/") || meta[2] == string("void") || meta[2] == string("/*void*/") ) meta[2] = "";
			if( meta[3] == string("/**/") || meta[3] == string("void") || meta[3] == string("/*void*/") ) meta[3] = "";
			for( int j = 0; j < 5; j++ ) {
			//	printf("<%d> [[%s]]\n", j, meta[j].c_str());
			}
				
			
			
			{
				strcpy( funcparam, meta[2].c_str() );
				strcpy( funcparam_invoke, meta[3].c_str() );
				strcpy( funcname, meta[0].c_str() );
				strcpy( funcreturn, meta[1].c_str() );
				
				param_def_cnt = 0;
				param_name_cnt = 0;
				
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

				int k;
				for( k = 0; k < param_def_cnt; k++ ) {
					//printf("[%s] -- [%s]\n", param_def[k], param_name[k]);
				}
				//printf("%d %d\n", param_def_cnt, param_name_cnt);

				
				//printf("%s\n", funcname);
				//printf("%s\n", funcparam);
				//printf("%s\n", funcparam_invoke);
				if( param_def_cnt != param_name_cnt )
				{
					printf("Error: param_def_cnt != param_name_cnt (%d != %d)\n", param_def_cnt, param_name_cnt);
					return 0;
				}
				
				char newdef[4096], tmp[4096];
				if( param_def_cnt == 0 ) {
					sprintf(newdef, "FUNC_P%d(%s, %s", param_def_cnt, funcreturn, funcname);
				} else {
					sprintf(newdef, "FUNC_P%d(%s, %s, ", param_def_cnt, funcreturn, funcname);
				}
				for( k = 0; k < param_def_cnt; k++ ) {
					char* p = strrstr(param_def[k], param_name[k]);
					char* type;
					char* def;
					char old_p;
					if( p ) {
					//p--;
					old_p = *p;
					*p = 0;
					}
					type = trim(param_def[k]);
					if( p ) {
					*p = old_p;
					}
					def = trim(p);
					sprintf(tmp, "%s, %s", type, def);
					strcat(newdef, tmp);
					if( k < param_def_cnt-1) strcat(newdef, ", ");
				}
				strcat(newdef, ")\n{\n");
				//printf(")\n{\n\n}\nFUNC_END();\n\n\n\n");
				printf("newdef: %s\n", newdef);
				str_newFuncDef = newdef;
			}
			
			meta[0] = meta[1] = meta[2] = meta[3] = meta[4] = string("");
			func_level++;
			skip = true;
			
			vtnewfile.push_back(str_newFuncDef);
		}
			
		if( func_level == 6 ) {
			if( line == string("{\n") ||
				line == string("\tHOOK_FUNC_OPER_HEADER2\n") ||
				line == string("\tHOOK_FUNC_OPER_HEADER3\n") ||
				line == string("\tHOOK_FUNC_OPER_HEADER_NOPARAM2\n") ||
				line == string("\tHOOK_FUNC_OPER_HEADER_NORETURN2\n") ||
				line == string("\t{\n") || 
				line == string("\tHOOK_FUNC_OPER_FOOTER\n") ||
				line == string("\t}\n") ||
				line == string("\t} \n")) {
				skip = true;			  
			}
			if( line == string("\t\tret = orgfn( FUNCPARAM_INVOKE );\n") ) {
				int k;
				string param, params;
				for( k = 0; k < param_def_cnt; k++ ) {
					param = param_name[k];
					params += param;
					if( k != param_def_cnt - 1 ) {
						params += string(", ");
					}
					//printf("[%s] -- [%s]\n", param_def[k], param_name[k]);
				}
				line = string("\t\tret = orgfn( ") + params + (" );\n");
			} else if( line == string("\t\torgfn( FUNCPARAM_INVOKE );\n") ) {
				int k;
				string param, params;
				for( k = 0; k < param_def_cnt; k++ ) {
					param = param_name[k];
					params += param;
					if( k != param_def_cnt - 1 ) {
						params += string(", ");
					}
					//printf("[%s] -- [%s]\n", param_def[k], param_name[k]);
				}
				line = string("\t\torgfn( ") + params + (" );\n");
			}

			
			if( line.find("\t\t") == 0 ) {
				//line = line.substr(1);
			}
		}
		if (line == string("}\n") && func_level == 6) {
			//printf("func end. [%d] \n", i+1);
			func_level = 0;
			skip = true;
			vtnewfile.push_back("}\n");
			vtnewfile.push_back("FUNC_END();\n");
		}
		
		if( skip == false ) {
			vtnewfile.push_back(line);
		}

		//printf("%s", vtlines[i].c_str());
	}
	//puts("aaaaaaaaaaaA");
	FILE* fo = fopen(FILENAME"_new", "wt");
	for( int i = 0; i < vtnewfile.size(); i++ ) {
		fprintf(fo, "%s", vtnewfile[i].c_str());
	}
	fclose(fo);
		

	return 0;
}

