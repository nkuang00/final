#include "play.h"

int main(){
  srand(time(0));
  struct card * top;
  print_intro();
  //make_deck
  //shuffle_deck
  top = draw();

  play(top);
  return 0;
}
