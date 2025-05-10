#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "buf.h"
#include "logging.h"
#include "routines.h"

// TODO: Add functionality for printing the content of the file if not empty.

void* fs_routine(void* arg) {
  int fd = open(fname, O_WRONLY | O_CREAT, 0644);
  if (fd < 0) {
    logMsg(ERROR_LOG, "Failed to open file %s.", fname);
    char errmsg[128];
    sprintf(errmsg, "Failed to open file %s.", fname);
    perror(errmsg);
    exit(1);
  }
  logMsg(INFO_LOG, "File %s has been opened/created.", fname);
  while (!finished) {
    pthread_mutex_lock(&lock);
    while (bufptr == 0 && !finished) {
      pthread_cond_wait(&buf_not_empty, &lock);
    }
    if (bufptr > 0) {
      write(fd, buf, bufptr);
      fsync(fd);
      bufptr = 0;
    }
    pthread_mutex_unlock(&lock);
  }
  close(fd);
  logMsg(INFO_LOG, "Escaped fs.");
  return NULL;
}