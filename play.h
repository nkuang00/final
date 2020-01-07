#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "draw.h"

void play(struct card * top);
struct card * play_cards(char * input, struct card * top, struct hand * h);
struct hand * create_hand(int n);
int valid_play(struct hand * p, struct hand * h, struct card * top);
int contains_repeats(struct hand * p, struct hand * h);
int colors_match(struct card * c, struct hand * p);
int types_match(struct card * c, struct hand * p);
