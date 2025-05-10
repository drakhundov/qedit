#include <stdio.h>

#include "buf.h"
#include "logging.h"
#include "routines.h"

char buf[BUFSZ];
int bufptr;

char* fname;
int fd;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buf_not_empty = PTHREAD_COND_INITIALIZER;

char finished;

int main(int argc, char** argv) {
  init_logs(".log", O_WRONLY | O_CREAT);
  bufptr = 0;
  finished = 0;
  fname = argv[1];
  pthread_t _fs, _kw;
  pthread_create(&_fs, NULL, fs_routine, NULL);
  pthread_create(&_kw, NULL, kw_routine, NULL);
  pthread_join(_fs, NULL);
  pthread_join(_kw, NULL);
  end_logs();
  return 0;
}