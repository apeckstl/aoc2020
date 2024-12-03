#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>

/* struct definitions */
typedef struct hashtable hashtable;
typedef struct bucket bucket;

struct bucket {
	int key;
	char *value;
	bucket *next;
};

struct hashtable {
	int size;
	int count;
	bucket **buckets;
};

/* function prototypes */
void	   ht_init(hashtable*,int);
int        ht_hash(int,int);
char*      ht_get(hashtable*,int);
void       ht_put(hashtable*,int,char*);
void       ht_del(hashtable*,int);
void       ht_free(hashtable*);
bucket    *ht_free_bucket(bucket*);

#endif
