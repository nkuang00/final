#include "main.h"

int main(){
  srand(time(0));
  struct card * top;
  print_intro();
  //make_deck
  //shuffle_deck
  int draw_shm = make_drawshm();
  top = draw_top();

  play(top);
  rem_drawshm(draw_shm);

  return 0;
}

int make_drawshm(){
  return shmget(DRAW_KEY, DRAW_SEG_SIZE, IPC_CREAT | 0644);
}

void rem_drawshm(int shmd){
  shmctl(shmd, IPC_RMID, 0);
}
