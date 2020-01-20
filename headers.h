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
#include <sys/sem.h>
#include <string.h>
#include <sys/wait.h>

#define DRAW_KEY 87654321
#define DRAW_SEG_SIZE sizeof(int)
#define DIRECTION 69420
#define TOPC_KEY 122333
#define TOPT_KEY 112233
#define TOP_SEG_SIZE sizeof(char)
#define TURN_END_KEY 1020304
#define TURN_END_SEG_SIZE sizeof(int)

#define NUMBER_OF_PLAYERS_KEY 2222222
#define WAITING_PLAYERS_ARRAY_KEY 2222
#define TURN_COUNTER_KEY 22222
#define DIRECTION_KEY 222222

//struct for cards
//colors: R, G, Y, B (wilds and +4 will be w)
//types: 0-9 [v (reverse), s (skip), t (+2), f(+4)]
struct card {
  char color;
  char type;
};

struct hand {
    struct card * cards[109];
    int size;
};
