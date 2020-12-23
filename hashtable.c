#include "hashtable.h"

/*
 *	ht_hash - Knuth's integer hash function.
 */
int ht_hash(int key, int ht_size)
{
	return (key * 2654435761) % ht_size;
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
void ht_put(hashtable *ht, int key, int value)
{
	// hash key
	int bidx = ht_hash(key, ht->size);

	// traverse bucket chain
	bucket *i = ht->buckets[bidx];
	while (i) {
		if (i->key == key) {
			i->value = value;
			return;
		}
		if (i->next == NULL)
			break;
		i = i->next;
	}

	// key not in hashtable, create new bucket
	bucket *b = malloc(sizeof(bucket));
	b->key    = key;
	b->value  = value;
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
int ht_get(hashtable *ht, int key)
{
	// ht_hash key
	unsigned long bidx = ht_hash(key, ht->size);

	// search for key
	bucket *i = ht->buckets[bidx];
	while (i) {
		if (i->key == key)
			return i->value;
		i = i->next;
	}
	return -1;
}

/*
 *	ht_del - deletes entry for 'key' if it exists, else no panic.
 */
void ht_del(hashtable *ht, int key)
{
	// hash key
	int bidx = ht_hash(key, ht->size);

	// index to correct bucket
	bucket *i = ht->buckets[bidx];

	// check if head is target
	if (i->key == key) {
		ht->buckets[bidx] = ht_free_bucket(i);
		ht->count -= 1;
		return;
	}

	// traverse
	while (i) {
		if (i->next == NULL)
			return;
		
		if (i->next->key == key) {
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
