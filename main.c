#include "main.h"

int main(){

  srand(time(0));     //needed to draw random cards
  struct card * top;
  char * topc;
  char * topt;

  //make_deck
  //shuffle_deck


  int nop_key, wpa_key, tc_key, dir_key, draw_shm, topc_shm, topt_shm, turn_end_shm;
  
  //moved to headers.h
  //int player_number;

  int * direction;

  //moved to headers.h
  //int * nop;

  int wpa[10];
  int * tc;
  char buffer[256];
  int nop_end, wpa_end, tc_end;
  int nop_term, wpa_term, tc_term;
  int i;

  nop_key = shmget(NUMBER_OF_PLAYERS_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);

  // not first player
  if (nop_key == -1){

    top = malloc(sizeof(struct card));

     //get number of players
     nop_key = shmget(NUMBER_OF_PLAYERS_KEY, sizeof(int), 0644);
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
     draw_shm = shmget(DRAW_KEY, sizeof(int), 0644);
     if (draw_shm == -1){
       printf("error draw_shm %d: %s\n", errno, strerror(errno));
       exit(1);
     }

     //get top
     topc_shm = shmget(TOPC_KEY, TOP_SEG_SIZE, 0644);
     if (topc_shm == -1){
       printf("error topc_shm %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     topt_shm = shmget(TOPT_KEY, TOP_SEG_SIZE, 0644);
     if (topt_shm == -1){
       printf("error topt_shm %d: %s\n", errno, strerror(errno));
       exit(1);
     }

     //get turn count
     tc_key = shmget(TURN_COUNTER_KEY, sizeof(int), 0644);
     if (tc_key == -1){
       printf("error tc_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     tc = shmat(tc_key, 0, 0);

     //get direction
     dir_key = shmget(DIRECTION_KEY, sizeof(int), 0644);
     if (dir_key == -1){
       printf("error dir_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     direction = shmat(dir_key, 0, 0);

     //get waiting players array
     wpa_key = shmget(WAITING_PLAYERS_ARRAY_KEY, sizeof(wpa), 0644);
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
    dir_key = shmget(DIRECTION_KEY, sizeof(int), IPC_CREAT | 0644);
    if (dir_key == -1){
      printf("error dir_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    direction = shmat(dir_key, 0, 0);
    *direction = 1;

    //create top
     topc_shm = shmget(TOPC_KEY, TOP_SEG_SIZE, IPC_CREAT | 0644);
     if (topc_shm == -1){
       printf("error topc_shm %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     topt_shm = shmget(TOPT_KEY, TOP_SEG_SIZE, IPC_CREAT | 0644);
     if (topt_shm == -1){
       printf("error topt_shm %d: %s\n", errno, strerror(errno));
       exit(1);
     }

     //create draw
     draw_shm = shmget(DRAW_KEY, DRAW_SEG_SIZE, IPC_CREAT | 0644);
     if (draw_shm == -1){
       printf("error draw_shm %d: %s\n", errno, strerror(errno));
       exit(1);
     }

     //create turn end
     // turn_end_shm = shmget(TURN_END_KEY, TURN_END_SEG_SIZE, IPC_CREAT | 0644);
     // if (turn_end_shm == -1){
     //   printf("error turn_end_shm %d: %s\n", errno, strerror(errno));
     //   exit(1);
     // }

    //create waiting players array
    wpa_key = shmget(WAITING_PLAYERS_ARRAY_KEY, sizeof(wpa), IPC_CREAT | 0644);
    if (wpa_key == -1){
      printf("error wpa_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    int * wpa = (int *) shmat(wpa_key, 0, 0);

    //create turn count
    tc_key = shmget(TURN_COUNTER_KEY, sizeof(int), IPC_CREAT | 0644);
    if (tc_key == -1){
      printf("error tc_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    tc = shmat(tc_key, 0, 0);
    *tc = 1;

    //set top
    topc = shmat(topc_shm, 0, 0);
    topt = shmat(topt_shm, 0, 0);
    top = draw_top();
    printf("\n\n");
    print_card(top);
    printf("\n\n");
    * topc = top->color;
    * topt = top->type;
    shmdt(topc);
    shmdt(topt);


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

  struct hand * h1;
  h1 = create_hand(5);


/////////////// After Initialization (Game Started) /////////////////////////

  while(1) {

    //check if someone has won yet
    topc = shmat(topc_shm, 0, 0);
    if (*topc == 'W') {
      printf("The game is over.\n");
      exit(0);
    }

    wpa_key = shmget(WAITING_PLAYERS_ARRAY_KEY, sizeof(wpa), 0644);
    if (wpa_key == -1){
      printf("error wpa_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    int * wpa = (int *) shmat(wpa_key, 0, 0);
    if (wpa == NULL){
      printf("error wpa_shmat %d: %s\n", errno, strerror(errno));
      exit(1);
    }

    //display current game state

    if ((*tc % *nop) == (player_number % *nop)) {
      printf("It's your turn\n");

      //get top card
      topc = shmat(topc_shm, 0, 0);
      topt = shmat(topt_shm, 0, 0);
      top->color = *topc;
      top->type = *topt;

      //play cards
      top = play(top, h1);
      *topc = top->color;
      *topt = top->type;

      //change turn
      *tc += *direction;

      //if win condition is met
      if (h1->size == 0) {
        printf("Congratulations. You win!\n");
        int topc_shm = shmget(TOPC_KEY, TOP_SEG_SIZE, 0644);
        char * topc = shmat(topc_shm, 0, 0);
        *topc = 'W';
        // int topt_shm = shmget(TOPT_KEY, TOP_SEG_SIZE, 0644);
        // char * topt = shmat(topt_shm, 0, 0);
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

      

      shmdt(topc);
      shmdt(topt);
    }

    else {
      printf("It's not your turn yet\n");

      //say whose turn it is now
      printf("It is player %d's turn\n", *tc % *nop);

      int spoon = fork();
      if (spoon == 0){
        while(1){
          printf("Waiting for your turn...\n");
          sleep(100);
        }
      }

      wpa[player_number] = spoon;

      wait(NULL);
    }

    wpa_term = shmdt(wpa);
    if (wpa_end == -1){
      printf("error wpa_end %d: %s\n", errno, strerror(errno));
      exit(1);
    }
  }







  //===========================OLD STUFF================================


  // int draw_shm = make_drawshm();
  // int top_shm = shmget(TOP_KEY, TOP_SEG_SIZE, IPC_CREAT | 0644);
  // top = shmat(top_shm, 0, 0);
  // top = draw_top();     //draws random normal card as a top card
  //
  // //when child is killed
  // print_intro();
  //
  // play(top);      //infinte one player shit


  rem_drawshm(draw_shm);
  shmdt(top);
  shmctl(topc_shm, IPC_RMID, 0);
  shmctl(topt_shm, IPC_RMID, 0);

  return 0;
}

//removing shm for draw
void rem_drawshm(int shmd){
  shmctl(shmd, IPC_RMID, 0);
}
