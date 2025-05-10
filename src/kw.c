#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "buf.h"
#include "logging.h"
#include "routines.h"

void enable_raw_mode() {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);           // Get current terminal attributes
  term.c_lflag &= ~(ICANON | ECHO);         // Disable canonical mode and echo
  tcsetattr(STDIN_FILENO, TCSANOW, &term);  // Apply changes immediately
}

void disable_raw_mode() {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag |= (ICANON | ECHO);  // Re-enable canonical mode and echo
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void* kw_routine(void* arg) {
  enable_raw_mode();
  printf("%s\\\\\n", fname);
  char c;
  while (!finished) {
    c = getchar();
    pthread_mutex_lock(&lock);
    if ((int)c == 27) {  // Esc
      logMsg(INFO_LOG, "The user escaped.");
      finished = 1;
    } else {
      buf[bufptr++] = c;
      putchar(c);
      fflush(stdout);
    }
	pthread_cond_signal(&buf_not_empty);
    pthread_mutex_unlock(&lock);
  }
  disable_raw_mode();
  logMsg(INFO_LOG, "Escaped kw.");
  return NULL;
}