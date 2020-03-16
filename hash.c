#define KEY_STR_MAX (128)
#define BUCKET_SIZE	(127)

#include <stdlib.h>
#include <string.h>

typedef struct _ENTRY {
	char key[KEY_STR_MAX];
    int val;
    struct _ENTRY *next;
} T_ENTRY;

T_ENTRY *buckets[BUCKET_SIZE];

int hash(const char *key) {
	int val = 0;
	while(*key) {
		val += *key * *key;
		key++;
	}
	return val % BUCKET_SIZE;
}

int insert(const char *key, int val) {
    int idx = hash(key);
    T_ENTRY *entry;
    entry = buckets[idx];

    while(entry) {
        if (strcmp(entry->key, key) == 0) {
            // duplicate key
            return -1;
        }
        entry = entry->next;
    }
    entry = malloc(sizeof(T_ENTRY));
    strcpy(entry->key, key);
    entry->val = val;
    entry->next = buckets[idx];
    buckets[idx] = entry;

    return 0;
}

int find(char *key, int *pVal) {
    int idx = hash(key);
    T_ENTRY *entry = buckets[idx];
    while(entry) {
        if (strcmp(entry->key, key) == 0) {
            *pVal = entry->val;
            return 0;
        }
        entry = entry->next;
    }

    return -1;
}

int delete(char *key) {
    int idx = hash(key);
    T_ENTRY *prev;
    T_ENTRY *entry;
    prev = buckets[idx];
    entry = buckets[idx];
    while(entry) {
        if (strcmp(entry->key, key) == 0) {
            prev = entry->next;
            free(entry);
            return 0;
        }
        prev = entry;
        entry = entry->next;
    }

    return 0;
}
