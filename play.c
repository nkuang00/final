#include "play.h"

void play(struct card * top){
  struct hand * h1;
  h1 = create_hand(5);
  while(1){
    char input[50];

    printf("top card: ");
    print_card(top);
    printf("\n");
    print_hand(h1);
    printf("your move: ");
    fflush(stdout);
    fgets(input, 50, stdin);
    * strchr(input, '\n') = 0;
    if (strcmp(input, "quit") == 0){
      h1 = free_hand(h1);
      top = free_card(top);
      break;
    }
    if (strcmp(input, "draw") == 0){
      draw_n(1, h1);
    }
    else{
      top = play_cards(input, top, h1);
    }
    printf("\n");
  }
}

struct card * play_cards(char * input, struct card * top, struct hand * h){
  struct hand * playing;
  playing = create_hand(0);
  if (!add_str(input, playing)){
    printf("error: invalid input. check format of cards\n");
    return top;
  }
  if (!valid_play(playing, h, top)){
    printf("error: invalid cards. check sequence of cards\n");
    return top;
  }
  top = remove_hand(playing, h);
  remove_card(top, playing);
  playing = free_hand(playing);
  return top;
}

int valid_play(struct hand * p, struct hand * h, struct card * top){
  //check for repeats
  //we may be able to improve runtime but whatever :')'
  if (contains_repeats(p, h)){
    return 0;
  }

  //check for colors/numbers, make sure sum isnt 0
  int c;
  int t;
  c = colors_match(top, p);
  t = types_match(top, p);
  if (c + t == 0){
    return 0;
  }

  return 1;
}

int colors_match(struct card * c, struct hand * p){
  int i;
  char col;
  col = c->color;
  for (i = 0; i < p->size; i++){
    if (col != p->cards[i]->color){
      return 0;
    }
  }
  return 1;
}

int types_match(struct card * c, struct hand * p){
  int i;
  char t;
  t = c->type;
  for (i = 0; i < p->size; i++){
    if (t != p->cards[i]->type){
      return 0;
    }
  }
  return 1;
}

int contains_repeats(struct hand * p, struct hand * h){
  int i;
  for (i = 0; i < p->size; i++){
    if (count_cards(p->cards[i], p) > count_cards(p->cards[i], h)){
      return 1;
    }
  }
  return 0;
}

//default: 5 cards
struct hand * create_hand(int n){
  struct hand * h = malloc(sizeof(struct hand));
  h->size = 0;
  draw_n(n, h);
  return h;
}
