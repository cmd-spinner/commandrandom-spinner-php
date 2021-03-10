#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#define HT_DEBUGMSG 0

//#define debug_printf printf
#define debug_printf 

#define HT_HASHTABLE_SIZE 			COUNT_OF_SC
#define HT_NEXT_UNDEF 	 			0xffffffff
#define HT_KEYSIZE 					(sizeof(int) + sizeof(void*))

typedef struct tag_ht_entry {
	char key[HT_KEYSIZE];
	int  value; // new index
	unsigned int   next;
	unsigned char  inuse;
	unsigned int   hash;
} ht_entry;

typedef struct tag_ht_hashtable {
	int global_index;
	ht_entry elem[HT_HASHTABLE_SIZE];
} ht_hashtable;

//ht_hashtable g_hashTable;
ht_hashtable* g_pHashTable = 0;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ht_init_hashtable(ht_hashtable* ptable);
void ht_set( ht_hashtable *hashtable, const char *key, int value );
int ht_get( ht_hashtable *hashtable, const char *key );
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////





void ht_init_hashtable(ht_hashtable* ptable)
{
	memset(ptable, 0, sizeof(ht_hashtable));
}

int ht_is_same_key(const char* k1, const char* k2)
{
	if( memcmp(k1, k2, HT_KEYSIZE) == 0 ) return 1;
	return 0;
}
/* Hash a string for a particular hash table. */
unsigned int ht_hash( ht_hashtable *hashtable, const char *key, int keysize ) 
{
	unsigned int hashval = 0;
	int i = 0;

	/* Convert our string to an integer */
	//while( hashval < ULONG_MAX && i < keysize ) {
	while( hashval < UINT_MAX && i < keysize ) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}

	return hashval % HT_HASHTABLE_SIZE;
}

int ht_find_hash_near( ht_hashtable* hashtable, int cur, int hash )
{
	for( int i = cur+1; i < HT_HASHTABLE_SIZE; i++ ) {
		if( hashtable->elem[ i ].hash == hash ) {
			return i;
		}
	}
	
	for( int i = cur-1; i >= 0; i-- ) {
		if( hashtable->elem[ i ].hash == hash ) {
			return i;
		}
	}
	return -1;
}


int ht_find_empty_near( ht_hashtable* hashtable, int hash)
{
	for( int i = hash+1; i < HT_HASHTABLE_SIZE; i++ ) {
		if( hashtable->elem[ i ].inuse == 0 ) { // empty! found!
			return i;
		}
	}
	
	for( int i = hash-1; i >= 0; i-- ) {
		if( hashtable->elem[ i ].inuse == 0 ) { // empty! found!
			return i;
		}
	}
	return -1;
}

void ht_set_new_elem( ht_hashtable* hashtable, int idx, unsigned int hash, const char* key, int value)
{
	memcpy( hashtable->elem[ idx ].key, key, HT_KEYSIZE );
	hashtable->elem[ idx ].value = value;
	hashtable->elem[ idx ].inuse = 1;
	hashtable->elem[ idx ].hash = hash;
	hashtable->elem[ idx ].next = HT_NEXT_UNDEF;
}

/* Insert a key-value pair into a hash table. */
void ht_set( ht_hashtable *hashtable, const char *key, int value ) 
{
	int bin = 0, hash = 0;
	
	hash = bin = ht_hash( hashtable, key, HT_KEYSIZE );
	debug_printf("bin: %x // key: %s // value: %s\n", bin, key, value);
	if( hashtable->elem[ bin ].inuse ) {
		if( hashtable->elem[ bin ].hash != hash ) {
			int newbin = ht_find_hash_near( hashtable, bin, hash );
			if( newbin == -1 ) {
				newbin = ht_find_empty_near( hashtable, hash );
			}

			debug_printf("[!] already used with other items;; orghash = %x  newbin = %x\n", hash, newbin); 
			
			
			bin = newbin;
		}
		
		if( hashtable->elem[ bin ].inuse == 0 ) {
			ht_set_new_elem( hashtable, bin, hash, key, value );
		} else {
			while(1) {
				if( hashtable->elem[ bin ].next == HT_NEXT_UNDEF && hashtable->elem[ bin ].inuse ) {
					// have no next...
					// find a new empty slot.
					int newbin = ht_find_empty_near( hashtable, hash );
					debug_printf("newbin: %x // key: %s // value: %s\n", newbin, key, value);
					
					ht_set_new_elem( hashtable, newbin, hash, key, value );
					hashtable->elem[ bin ].next = newbin; // connect it.
					break;
				} else {
					// jumping to the next one
					bin = hashtable->elem[ bin ].next;
					continue;
				}	
			}
		}
	} else {
		ht_set_new_elem( hashtable, bin, hash, key, value );
	}
}

/* Retrieve a key-value pair from a hash table. */
int ht_get( ht_hashtable *hashtable, const char *key )
{
	const int reterr = -1;
	int bin = 0, hash = 0;
	int found = 0;
	ht_entry *pair;

	//debug_printf("ht_get begins [%x]\n", HT_HASHTABLE_SIZE );
	hash = bin = ht_hash( hashtable, key, HT_KEYSIZE );	
	debug_printf("htget: hash = %x [key: %s]\n", bin, key);
	if( hashtable->elem[bin].inuse ) {
		if( hashtable->elem[bin].hash != hash ) {
			int newbin = ht_find_hash_near(hashtable, bin, hash);
			if( newbin != -1 ) {
				debug_printf("htget: hash --newbin-corrected = %x [key: %s]\n", bin, key);
				bin = newbin;
			} else {
				return reterr;
			}
		}
	}
	
	if( hashtable->elem[bin].inuse ) {
		while(1) {
			if( ht_is_same_key(hashtable->elem[bin].key, key) == 1 ) {
				found = 1;
				break;
			} else {
				if( hashtable->elem[bin].inuse && hashtable->elem[bin].next != HT_NEXT_UNDEF ) {
					bin = hashtable->elem[bin].next;
					debug_printf("\thtget: hash = %x [key: %s]\n", bin, key);
				} else {
					break;
				}
			}
		}
	}
	
	if( !found ) {
		return reterr;
	}
	
	return hashtable->elem[ bin ].value;
}

/*
int main( int argc, char **argv ) 
{

	ht_hashtable *hashtable = &g_hashTable;
	
	ht_init_hashtable( hashtable );
	const char* keys[] = {
		"kex1", "kex11", "kex12", "key2", "key3", "kez4", "kez12", 0
	};
	const char* values[] = {
		"first", "second", "third", "forth", "fifth", "sixth", "seventh", 0 
	};

	for ( int i = 0; ; i++ ) {
		if( keys[i] == 0 ) break;
		ht_set( hashtable, keys[i], i*100 );
	}

	int ret;
	
#define HT_HASHGET(__key) {\
		ret = ht_get( hashtable, __key );\
		printf( "%s -> %d\n", __key, ret );\
	}
	
	for ( int i = 0; ; i++ ) {
		if( keys[i] == 0 ) break;
		HT_HASHGET( keys[i] );
	}
	
	return 0;
}
*/

