#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>

#define DRAW_KEY 22222222
#define DRAW_SEG_SIZE sizeof(int)
//#define DIRECTION 69420
