#ifndef _MEMCHK_
#define _MEMCHK_

#define malloc(size) (my_malloc(__FUNCTION__, size))
#define free(ptr) (my_free(__FUNCTION__, ptr))

int init_leakfinder(int len, char **keys);
void show_result(void);

void *my_malloc(const char *func, size_t size);
void my_free(const char *func, void *ptr);

#endif // _MEMCHK_
