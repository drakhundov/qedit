#include <pthread.h>

extern char* fname;

extern pthread_mutex_t lock;

extern char finished;
extern pthread_cond_t buf_not_empty;

// Thread Routines.
void* fs_routine(void* arg);
void* kw_routine(void* arg);