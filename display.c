#include "display.h"

void print_intro(){
  printf("welcome to fake tres!\n");
  printf("\ninstructions:\n");
  printf("to draw, enter \"draw\" when prompted\n");
  printf("to play a card, enter the position of the card that you would like to play\n");
  printf("note: indexing begins at 1\n");
  printf("to quit, enter \"quit\"\n");
  printf("enjoy!\n\n");
}

void print_card(struct card * c){
  printf("%c%c", c->color, c-> type);
}

void print_hand(struct hand * h){
  int i;
  printf("hand: ");
  for (i = 0; i < h->size; i++){
    print_card(*(h->cards + i));
    printf(" ");
  }
  printf("\n");
}
