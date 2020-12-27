#include <stdio.h>
#include "cubetable.h"
#include <math.h>

/*
 *	ht_hash - Knuth's integer hash function.
 */
int ht_hash(int x, int y, int z, int w, int ht_size)
{
	return (abs(x * 31) + abs(y * 37) + abs(z * 41) + abs(w * 43)) % ht_size;
}

/*
 *	ht_init - creates empty hashtable with 'size' buckets.
 */
void ht_init(hashtable *ht, int size)
{

	// assign attributes
	ht->size    = size;
	ht->count   = 0;
	ht->buckets = calloc(size, sizeof(bucket *));
	return;
}

/*
 *	ht_put - creates or updates an entry in hashtable
 */
void ht_put(hashtable *ht, int x, int y, int z, int w)
{
	// hash key
	int bidx = ht_hash(x, y, z, w, ht->size);
	//printf("bidx: %d\n", bidx);

	// traverse bucket chain
	bucket *i = ht->buckets[bidx];
	while (i) {
		if (i->x == x && i->y == y && i->z == z && i->w == w) {
			return;
		}
		if (i->next == NULL)
			break;
		i = i->next;
	}

	// key not in hashtable, create new bucket
	bucket *b = malloc(sizeof(bucket));
	b->x    = x;
	b->y	= y;
	b->z	= z;
	b->w	= w;
	b->next   = NULL;
	
	// set as either head or next chain link
	if (i == NULL)
		ht->buckets[bidx] = b;
	else
		i->next = b;

	// update entries count
	ht->count += 1;
}

/*
 *	ht_get - retrieves the value for a given key.
 */
int ht_get(hashtable *ht, int x, int y, int z, int w)
{
	// ht_hash key
	unsigned long bidx = ht_hash(x, y, z, w, ht->size);

	// search for key
	bucket *i = ht->buckets[bidx];
	while (i) {
		if (i->x == x && i->y == y && i->z == z && i->w == w)
			return 1;
		i = i->next;
	}
	return 0;
}

/*
 *	ht_del - deletes entry for 'key' if it exists, else no panic.
 */
void ht_del(hashtable *ht, int x, int y, int z, int w)
{
	// hash key
	int bidx = ht_hash(x, y, z, w, ht->size);

	// index to correct bucket
	bucket *i = ht->buckets[bidx];

	// check if head is target
	if (i->x == x && i->y == y && i->z == z && i->w == w) {
		ht->buckets[bidx] = ht_free_bucket(i);
		ht->count -= 1;
		return;
	}

	// traverse
	while (i) {
		if (i->next == NULL)
			return;
		
		if (i->next->x == x && i->next->y == y && i->next->z == z && i->next->w == w) {
			i->next = ht_free_bucket(i->next);
			ht->count -= 1;
			return;
		}
		i = i->next;
	}
}

/*
 *	ht_free - frees hashtable, including buckets.
 */
void ht_free(hashtable *ht)
{
	int i;
	bucket *b;
	for (i=0; i < ht->size; i++) {
		b = ht->buckets[i];
		while (b)
			b = ht_free_bucket(b);
	}
	free(ht->buckets);
}

/*
 *	ht_free_bucket - frees passed in bucket, adjusts prev/next ptrs.
 */
bucket *ht_free_bucket(bucket *current)
{
	bucket *temp = current->next;
	free(current);
	return temp;
}
