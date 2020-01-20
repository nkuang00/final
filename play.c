#include "play.h"

struct card * play(struct card * top, struct hand * h1){
  //h1 is player's hand
  int turn_end_shm;
  int * turn_end;
  turn_end_shm = shmget(TURN_END_KEY, TURN_END_SEG_SIZE, IPC_CREAT | 0644);
  turn_end = shmat(turn_end_shm, 0, 0);
  *turn_end = 0;
  while(! * turn_end){
    char input[50];

    //print current game info
    printf("top card: ");
    print_card(top);
    printf("\n");
    print_hand(h1);

    //prompts user for a move and stores the shit in input
    printf("your move: ");
    fflush(stdout);
    fgets(input, 50, stdin);
    * strchr(input, '\n') = 0;

    //if quit: free all malloc shits and exits while loop
    if (strcmp(input, "quit") == 0){
      h1 = free_hand(h1);
      top = free_card(top);
      printf("ok bye loser\n");
      break;
    }

    //if draw: draws cards appropriately:
    //1 if shm for draw is 0; draws shit stored in shm otherwise
    if (strcmp(input, "draw") == 0){
      int draw_shm;
      int * draw_val;
      draw_shm = shmget(DRAW_KEY, DRAW_SEG_SIZE, IPC_CREAT | 0644);
      draw_val = shmat(draw_shm, 0, 0);
      if (*draw_val == 0){
        draw_n(1, h1);
      }
      else{
        draw_n(*draw_val, h1);
        *draw_val = 0;
      }
      shmdt(draw_val);
      * turn_end = 1;
    }

    //if entry is not draw/quit
    else{

      //if help: print help info
      if (strcmp(input, "help") == 0){
        print_help();
      }

      //interprets input as dumb hand to check for valid input
      else{
        top = play_cards(input, top, h1);
      }
    }

    printf("\n");

    //if win condition (hand empty):
    //frees shit and exits while loop
    if (h1->size == 0){
      h1 = free_hand(h1);
      top = free_card(top);
      printf("no more cards. you win! :0\n");
      return NULL;
    }
  }
  shmdt(turn_end);
  shmctl(turn_end_shm, IPC_RMID, 0);
  return top;
}

//playing cards, accepts string input, top card, and h
struct card * play_cards(char * input, struct card * top, struct hand * h){

  //gets shm for dumb draw shit
  int draw_shm;
  int * draw_val;
  int turn_end_shm;
  int * turn_end;
  draw_shm = shmget(DRAW_KEY, DRAW_SEG_SIZE, IPC_CREAT | 0644);
  draw_val = shmat(draw_shm, 0, 0);

  turn_end_shm = shmget(TURN_END_KEY, TURN_END_SEG_SIZE, IPC_CREAT | 0644);
  turn_end = shmat(turn_end_shm, 0, 0);
  //if player needs to draw cards/stack plus or whatever shit:
  if (*draw_val != 0){
    shmdt(draw_val);
    return play_cards_plus(input, top, h);
  }

  //if player does not need to draw cards
  else{

    //takes input and dumps input cards into a "playing" hand
    struct hand * playing;
    playing = create_hand(0);
    if (!add_str(input, playing)){
      printf("error: invalid input. check format of cards\n");
      playing = free_hand(playing);
      return top;
    }
    if (!valid_play(playing, h, top)){
      printf("error: invalid cards. check sequence of cards\n");
      playing = free_hand(playing);
      return top;
    }
    if (playing->size > 0){
      * draw_val += count_draws(playing);
      printf("draw count: %d\n", *draw_val);
      free_card(top);
      top = remove_handh(playing, h);
      remove_card(top, playing);
      *turn_end = 1;
    }
    playing = free_hand(playing);
    shmdt(draw_val);
  }
  shmdt(turn_end);
  return top;
}

struct card * play_cards_plus(char * input, struct card * top, struct hand * h){
  int draw_shm;
  int * draw_val;
  draw_shm = shmget(DRAW_KEY, DRAW_SEG_SIZE, IPC_CREAT | 0644);
  draw_val = shmat(draw_shm, 0, 0);
  struct hand * playing;
  playing = create_hand(0);
  if (!add_str(input, playing)){
    printf("error: invalid input. check format of cards\n");
    playing = free_hand(playing);
    return top;
  }
  if (!valid_play_plus(playing, h, top)){
    printf("error: invalid cards. check sequence of cards\n");
    playing = free_hand(playing);
    return top;
  }
  if (playing->size > 0){
    * draw_val += count_draws(playing);
    printf("draw count: %d\n", *draw_val);
    free_card(top);
    top = remove_handh(playing, h);
    remove_card(top, playing);
  }
  playing = free_hand(playing);
  return top;
}

int count_draws(struct hand *p){
  int count;
  int i;
  count = 0;
  for (i = 0; i < p->size; i++){
    count += is_plus(p->cards[i]) * 2;
  }
  return count;
}

int valid_play_plus(struct hand *p, struct hand * h, struct card * top){
  if (contains_repeats(p, h)){
    return 0;
  }
  int i;
  for (i = 0; i < p->size; i++){
    if (!is_plus(p->cards[i])){
      return 0;
    }
  }
  return 1;
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

//also check for all pluses, or all normals
int colors_match(struct card * c, struct hand * p){
  int i;
  char col;
  col = c->color;
  for (i = 0; i < p->size; i++){
    if (col != p->cards[i]->color){
      return 0;
    }
  }
  return gen_types_match(c, p);
}

int gen_types_match(struct card * c, struct hand * p){
  int i;
  int t0;
  t0 = is_plus(p->cards[0]);
  for (i = 1; i < p->size; i++){
    if (t0 != is_plus(p->cards[i])){
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


struct hand * create_hand(int n){
  struct hand * h = malloc(sizeof(struct hand));
  h->size = 0;
  draw_n(n, h);
  return h;
}
