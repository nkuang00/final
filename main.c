#include "main.h"

int main(){

  srand(time(0));     //needed to draw random cards
  struct card * top;
  print_intro();
  //make_deck
  //shuffle_deck
  int draw_shm = make_drawshm();
  top = draw_top();     //draws random normal card as a top card

  play(top);      //infinte one player shit
  rem_drawshm(draw_shm);

  return 0;
}

//shm for draw to keep track of plus count (stackable)
int make_drawshm(){
  return shmget(DRAW_KEY, DRAW_SEG_SIZE, IPC_CREAT | 0644);
}

//removing shm for draw
void rem_drawshm(int shmd){
  shmctl(shmd, IPC_RMID, 0);
}
