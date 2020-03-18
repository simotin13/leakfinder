#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define KEY_STR_MAX (128)
#define BUCKET_SIZE	(127)

typedef struct _ENTRY {
	int used;
	char key[KEY_STR_MAX];
	int count;
} T_ENTRY;

T_ENTRY buckets[BUCKET_SIZE];

static int hash(const char *key) {
	int val = 0;
    int offset = 1;
    char *pKey = (char *)key;
	while(*pKey) {
		val ^= (*pKey * *pKey);
        val <<= offset;
        offset++;
		pKey++;
	}
#if DEBUG
    fprintf(stdout, "[DEBUG] key:[%s] val:[%d]\n", key, val);
#endif
	return val % BUCKET_SIZE;
}

int check_collision(int len, char **keys) {
	int i = 0;
	int idx;

	for(i = 0; i < len; i++) {
		idx = hash(keys[i]);
		fprintf(stdout, "func:[%s] idx:[%d]\n", keys[i], idx);
		if (buckets[idx].used == 0) {
			buckets[idx].used = 1;
		} else {
			fprintf(stderr, "collision detected\n");
			return -1;
		}
	}

	return 0;
}
int init_leakfinder(void) {
	memset(buckets, 0, sizeof(T_ENTRY) * BUCKET_SIZE);
	return 0;
}

void show_result(void) {
	int i;
	fprintf(stderr, "------------------- allocated results ------------------------\n");
	for (i = 0; i < BUCKET_SIZE; i++) {
		if (buckets[i].used) {
			fprintf(stderr, "function:[%s] unreleased:[%d]\n", buckets[i].key, buckets[i].count);
		}
	}
	fprintf(stderr, "------------------- allocated results ------------------------\n");
	fprintf(stderr, "\n");
}

void *my_malloc(char *func, size_t size) {
	void *p;
	int idx = hash(func);
	if (buckets[idx].used == 0) {
		// called first time
		buckets[idx].used = 1;
		strcpy(buckets[idx].key, func);
		buckets[idx].count = 1;
	} else {
		buckets[idx].count++;
	}

	p = malloc(size);
	return p;
}

void my_free(char *func, void *p) {
	int idx = hash(func);
	if (buckets[idx].used == 0) {
		fprintf(stderr, "free called for not allocated in:%s, addr:[%p]\n", func, p);
		assert(0);
	} else {
		if (0 < buckets[idx].count) {
			buckets[idx].count--;
		} else {
			fprintf(stderr, "double free in:%s, addr:[%p]\n", func, p);
			assert(0);
		}
	}

	free(p);
}
