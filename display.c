#include "display.h"

//print start game text
void print_intro(){
  printf("welcome to fake tres!\n");
  printf("\ninstructions:\n");
  printf("to play one or more cards, enter the cards that you would like to play,\n");
  printf("separated by a space, in the order that you would like to play them in\n");
  printf("note: you may play multiple cards iff they are of the same color or of\n");
  printf("      the same type as the top card\n");
  printf("to draw, enter \"draw\" when prompted\n");
  printf("to quit, enter \"quit\"\n");
  printf("for further instructions on valid cards, enter \"help\"\n");
  printf("enjoy!\n\n");
}

//print each card (ex: R7)
void print_card(struct card * c){
  printf("%c%c", c->color, c-> type);
}

//print each hand, followed by newline (ex:hand: R1 B2 G3 Y+\n)
void print_hand(struct hand * h){
  int i;
  printf("hand: ");
  for (i = 0; i < h->size; i++){
    print_card(*(h->cards + i));
    printf(" ");
  }
  printf("\n");
}

//print help info
void print_help(){
  printf("\nhelp stuff\n");
}
