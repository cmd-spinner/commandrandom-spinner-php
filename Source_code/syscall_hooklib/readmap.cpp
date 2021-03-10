#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int ReadMap(FILE* f, map<string, string> &ret, const char* debugmsg)
{
	map<string, string>::iterator it;
	char* line = 0;
	size_t len = 0;
	ssize_t read;
	
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
			ret.insert( pair<string,string>( key, val ) );
		}
    }
    if( line ) {
		free(line);
	}
}

void CompareMaps(map<string, string> &m1, map<string, string> &m2)
{
	if( m1.size() != m2.size() ) {
		printf("[beginning] Size Diff: %lu vs. %lu\n", m1.size(), m2.size());
	}
	map<string, string>::iterator it;
	map<string, string>::iterator it2;
	
	// remove identical
	vector<string> key_to_remove;
	for( it = m1.begin(); it != m1.end(); it++ ) {
		it2 = m2.find(it->first);
		if( it2 != m2.end() ) {
			if( strcmp( it->second.c_str(), it2->second.c_str() ) == 0 ) {
				// same
				// delete
				key_to_remove.push_back(it->first);
			}
		}
	}
	
	
	for( unsigned int i = 0; i < key_to_remove.size(); i++ ) {
		it = m1.find( key_to_remove[i] );
		if( it != m1.end() ) {
			m1.erase(it);
		}
		
		it = m2.find( key_to_remove[i] );
		if( it != m2.end() ) {
			m2.erase(it);
		}
	}
	
	if( m1.size() != m2.size() ) {
		printf("[a1] Size Diff: %lu vs. %lu\n", m1.size(), m2.size());
	}
	
	
	// with same key diff vals
	printf("Same Key Diff Values\n");
	for( it = m1.begin(); it != m1.end(); it++ ) {
		it2 = m2.find(it->first);
		if( it2 != m2.end() ) {
			if( strcmp( it->second.c_str(), it2->second.c_str() ) != 0 ) {
				printf("K[%s] V[%s][%s]\n", it->first.c_str(), it->second.c_str(), it2->second.c_str());
			}
		}
	}
	
#define DUMP_FILTER_POLICY \
		if( strstr( it->first.c_str(), "/home/jiahao/.cache/mozilla/firefox/") ) {\
			continue;\
		} else if( strstr( it->first.c_str(), "Recovered:pipe:") ) {\
			continue;\
		}
		
	printf("Dumps\n");
	for( it = m1.begin(); it != m1.end(); it++ ) {
		DUMP_FILTER_POLICY;
		
		printf("M1:K[%s]V[%s]\n", it->first.c_str(), it->second.c_str());
	}
	
	printf("-----------------------------------------------\n");
	
	for( it = m2.begin(); it != m2.end(); it++ ) {
		DUMP_FILTER_POLICY;;
		
		printf("M2:K[%s]V[%s]\n", it->first.c_str(), it->second.c_str());
	}
	
	
}

int main(int argc, char** argv)
{
	map<string, string> map1, map2;
	FILE* f;
	
	if( argc >= 2 ) {
		f = fopen(argv[1], "rt");
		if(f) {
			ReadMap(f, map1, "map1");
			fclose(f);
		}
	}
	if( argc >= 3 ) {
		f = fopen(argv[2], "rt");
		if(f) {
			ReadMap(f, map2, "map2");
			fclose(f);
		}
	}
	
	CompareMaps(map1, map2);
	return 1;
}

