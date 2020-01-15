#include "headers.h"
//struct for cards
//colors: R, G, Y, B (wilds and +4 will be w)
//types: 0-9 [v (reverse), s (skip), t (+2), f(+4)]
struct card {
  char color;
  char type;
};

struct hand {
    struct card * cards[109];
    int size;
};

void add_card(struct card * c, struct hand * h);
int add_str(char * str, struct hand * h);
struct card * str_to_card(char * str);
struct card * free_card(struct card * c);
struct hand * free_hand(struct hand * h);
int count_cards(struct card * c, struct hand * h);
int card_cmp(struct card * c0, struct card * c1);
struct card * remove_hand(struct hand * h0, struct hand * h1);
struct card * remove_card(struct card * c, struct hand * h);
struct card * remove_handh(struct hand * h0, struct hand * h1);
struct card * remove_cardh(struct card * c, struct hand * h);
