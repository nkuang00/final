#include "main.h"


void clear(){
  if (!fork()){
    char * clear_array[2];
    clear_array[0] = "clear";
    clear_array[1] = NULL;
  	int error = execvp("clear", clear_array);
  	if (error == -1){
  		printf("error clear %d: %s\n", errno, strerror(errno));
        exit(1);
    }
  	exit(0);
  }
}

int main(){

  srand(time(0));     //needed to draw random cards
  struct card * top;
  struct hand * h1;
  char * topc;
  char * topt;

  //make_deck
  //shuffle_deck


  int nop_key, wpa_key, tc_key, dir_key, draw_key, topc_key, topt_key;

  //moved to headers.h
  //int player_number;

  int * direction;

  //moved to headers.h
  //int * nop;

  int wpa[10];
  int * tc;
  char buffer[256];
  int nop_end, wpa_end, tc_end, dir_end, draw_end, topc_end, topt_end;
  int nop_term, wpa_term, tc_term, dir_term, draw_term, topc_term, topt_term;
  int i;

  nop_key = shmget(NUMBER_OF_PLAYERS_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0666);

  // not first player
  if (nop_key == -1){

    top = malloc(sizeof(struct card));

     //get number of players
     nop_key = shmget(NUMBER_OF_PLAYERS_KEY, sizeof(int), 0666);
     if (nop_key == -1){
       printf("error nop_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     nop = shmat(nop_key, 0, 0);
     if (nop == NULL){
       printf("error nop_shmat %d: %s\n", errno, strerror(errno));
       exit(1);
     }

     //get player number
     (*nop)++;
     player_number = *nop;

     //wait for game to start
     int spoon = fork();
     if (spoon == 0){
       while(1){
         printf("Welcome to the card game Tres!\n");
         printf("Waiting for first player to start the game!\n");
         sleep(100);
       }
     }

     //get draw
     draw_key = shmget(DRAW_KEY, sizeof(int), 0666);
     if (draw_key == -1){
       printf("error draw_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }

     //get top
     topc_key = shmget(TOPC_KEY, TOP_SEG_SIZE, 0666);
     if (topc_key == -1){
       printf("error topc_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     topt_key = shmget(TOPT_KEY, TOP_SEG_SIZE, 0666);
     if (topt_key == -1){
       printf("error topt_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     topc = shmat(topc_key, 0, 0);
     topt = shmat(topt_key, 0, 0);

     //get turn count
     tc_key = shmget(TURN_COUNTER_KEY, sizeof(int), 0666);
     if (tc_key == -1){
       printf("error tc_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     tc = shmat(tc_key, 0, 0);

     //get direction
     dir_key = shmget(DIRECTION_KEY, sizeof(int), 0666);
     if (dir_key == -1){
       printf("error dir_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     direction = shmat(dir_key, 0, 0);

     //get waiting players array
     wpa_key = shmget(WAITING_PLAYERS_ARRAY_KEY, sizeof(wpa), 0666);
     if (wpa_key == -1){
       printf("error wpa_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     int * wpa = (int *) shmat(wpa_key, 0, 0);
     if (wpa == NULL){
       printf("error wpa_shmat %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     wpa[player_number] = spoon;

     wpa_term = shmdt(wpa);
     if (wpa_end == -1){
       printf("error wpa_end %d: %s\n", errno, strerror(errno));
       exit(1);
     }

     //wait for child to die (for game to start)
     wait(NULL);
  }

  //first player
  else{

    //create number of players
    nop = shmat(nop_key, 0, 0);
    if (nop == NULL){
      printf("error nop_shmat %d: %s\n", errno, strerror(errno));
      exit(1);
    }

    //get player number
    *nop = 1;
    player_number = 1;

    //create direction
    dir_key = shmget(DIRECTION_KEY, sizeof(int), IPC_CREAT | 0666);
    if (dir_key == -1){
      printf("error dir_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    direction = shmat(dir_key, 0, 0);
    *direction = 1;

    //create top
     topc_key = shmget(TOPC_KEY, TOP_SEG_SIZE, IPC_CREAT | 0666);
     if (topc_key == -1){
       printf("error topc_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     topt_key = shmget(TOPT_KEY, TOP_SEG_SIZE, IPC_CREAT | 0666);
     if (topt_key == -1){
       printf("error topt_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }

     //create draw
     draw_key = shmget(DRAW_KEY, DRAW_SEG_SIZE, IPC_CREAT | 0666);
     if (draw_key == -1){
       printf("error draw_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     int * draw_val = shmat(draw_key, 0, 0);
     *draw_val = 0;

     //create turn end
     // turn_end_key = shmget(TURN_END_KEY, TURN_END_SEG_SIZE, IPC_CREAT | 0666);
     // if (turn_end_key == -1){
     //   printf("error turn_end_key %d: %s\n", errno, strerror(errno));
     //   exit(1);
     // }

    //create waiting players array
    wpa_key = shmget(WAITING_PLAYERS_ARRAY_KEY, sizeof(wpa), IPC_CREAT | 0666);
    if (wpa_key == -1){
      printf("error wpa_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    int * wpa = (int *) shmat(wpa_key, 0, 0);

    //create turn count
    tc_key = shmget(TURN_COUNTER_KEY, sizeof(int), IPC_CREAT | 0666);
    if (tc_key == -1){
      printf("error tc_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    tc = shmat(tc_key, 0, 0);
    *tc = 1;

    //set top
    topc = shmat(topc_key, 0, 0);
    topt = shmat(topt_key, 0, 0);
    top = draw_top();
    * topc = top->color;
    * topt = top->type;


    //input to start the game
    printf("Welcome to the card game Tres!\n");
    printf("Enter \"start\" into the terminal at any time to start the game!\n");
    fgets(buffer, 256, stdin);
    while (strcmp(buffer, "start\n") != 0){
      printf("Tip: If you wanted to start the game, enter \"start\"!\n");
      fgets(buffer, 256, stdin);
    }


    //tell other players that game has started
    for (i = 2; i <= *nop; i++){
      kill(wpa[i], SIGKILL);
      wpa[i] = 0;
    }

    wpa_term = shmdt(wpa);
    if (wpa_end == -1){
      printf("error wpa_end %d: %s\n", errno, strerror(errno));
      exit(1);
    }
  }

  //show game has started, and player number
  printf("Fantastic! Now the game has begun!\n");
  printf("Your are player %d\n", player_number);
  print_intro();

  h1 = create_hand(5);


/////////////// After Initialization (Game Started) /////////////////////////

  while(1) {

    //check if someone has won yet
    if (*topc == 'W') {
      printf("The game is over.\n");
      break;
    }

    //check if someone has quit
    if (*topc == 'Q') {
      printf("Someone quit.\n The game is over.\n");
      break;
    }



    wpa_key = shmget(WAITING_PLAYERS_ARRAY_KEY, sizeof(wpa), 0666);
    if (wpa_key == -1){
      printf("error wpa_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    int * wpa = (int *) shmat(wpa_key, 0, 0);
    if (wpa == NULL){
      printf("error wpa_shmat %d: %s\n", errno, strerror(errno));
      exit(1);
    }

    if ((*tc % *nop) == (player_number % *nop)) {
      printf("It's your turn\n");

      int * draw_val = shmat(draw_key, 0, 0);
      if (*draw_val == 0) {
        printf("Draw count: 1\n");
      }
      else {
        printf("Draw count: %d\n", *draw_val);
      }

      //get top card
      top->color = *topc;
      top->type = *topt;

      //play cards
      top = play(top, h1);
      *topc = top->color;
      *topt = top->type;

      //change turn (taking into account negative turn)
      *tc += *direction;

      while (*tc < 0) {
        *tc += *nop;
      }

      //if win condition is met
      if (h1->size == 0) {
        printf("Congratulations. You win!\n");
        int topc_key = shmget(TOPC_KEY, TOP_SEG_SIZE, 0666);
        char * topc = shmat(topc_key, 0, 0);
        *topc = 'W';
        // int topt_key = shmget(TOPT_KEY, TOP_SEG_SIZE, 0666);
        // char * topt = shmat(topt_key, 0, 0);
        // *topt = player_number + '0';

        //insert removal code here
      }
      //kill children except own
      for (i = 1; i <= *nop; i++) {
        if (i != player_number) {
          kill(wpa[i], SIGKILL);
          wpa[i] = 0;
        }
      }

    }

    else {
      //printf("It's not your turn yet\n");



      int spoon = fork();
      if (spoon == 0){
        while(1){
          clear();
          sleep(1);
          printf("Waiting for your turn...\n");
          //say whose turn it is now
          printf("It is player %d's turn\n", *tc % *nop);
          //print current game info
          printf("top card: ");
          print_card(top);
          printf("\n");
          print_hand(h1);
          sleep(100);
        }
      }

      wpa[player_number] = spoon;

      wait(NULL);
    }

    wpa_end = shmdt(wpa);
    if (wpa_end == -1){
      printf("error wpa_end %d: %s\n", errno, strerror(errno));
      exit(1);
    }

  }

  top = free_card(top);
  h1 = free_hand(h1);

  nop_end = shmdt(nop);
  if (nop_end == -1){
    printf("error nop_end %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  tc_end = shmdt(tc);
  if (tc_end == -1){
    printf("error tc_end %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  dir_end = shmdt(direction);
  if (dir_end == -1){
    printf("error dir_end %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  topc_end = shmdt(topc);
  if (topc_end == -1){
    printf("error topc_end %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  topt_end = shmdt(topt);
  if (topt_end == -1){
    printf("error topt_end %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  if (player_number == 1){
    nop_term = shmctl(nop_key, IPC_RMID, 0);
    if (nop_term == -1){
      printf("error nop_end %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    tc_term = shmctl(tc_key, IPC_RMID, 0);
    if (tc_term == -1){
      printf("error tc_term %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    dir_term = shmctl(dir_key, IPC_RMID, 0);
    if (dir_term == -1){
      printf("error term_end %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    draw_term = shmctl(draw_key, IPC_RMID, 0);
    if (draw_term == -1){
      printf("error draw_term %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    topc_term = shmctl(topc_key, IPC_RMID, 0);
    if (topc_term == -1){
      printf("error topc_term %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    topt_term = shmctl(topt_key, IPC_RMID, 0);
    if (topt_term == -1){
      printf("error topt_term %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    wpa_term = shmctl(wpa_key, IPC_RMID, 0);
    if (wpa_term == -1){
      printf("error wpa_term %d: %s\n", errno, strerror(errno));
      exit(1);
    }
  }







  //===========================OLD STUFF================================


  // int draw_key = make_drawshm();
  // int top_shm = shmget(TOP_KEY, TOP_SEG_SIZE, IPC_CREAT | 0666);
  // top = shmat(top_shm, 0, 0);
  // top = draw_top();     //draws random normal card as a top card
  //
  // //when child is killed
  // print_intro();
  //
  // play(top);      //infinte one player shit

  return 0;
}

//removing shm for draw
void rem_drawshm(int shmd){
  shmctl(shmd, IPC_RMID, 0);
}
