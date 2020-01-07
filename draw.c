#include "draw.h"

struct card * draw(){
  char colors [4] = {'R', 'G', 'B', 'Y'};
  char types [10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  struct card * c = malloc(sizeof(struct card));
  c->color = colors[rand() % 4];
  c->type = types[rand() % 10];
  return c;
}


int draw_n(int n, struct hand * h){
  int i;
  for (i = 0; i < n; i++){
    *(h->cards + h->size) = draw();
    h->size += 1;
  }
  return n;
}
