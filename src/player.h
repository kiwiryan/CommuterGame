#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum Scenarios{COLLISION, COFFEE, TRAFFIC};
enum Scenarios event;

void setName(char* name);

char* getName();

void setTime(int);
void setHealth(int);

int getTime();
int getHealth();

#endif /*PLAYER_H*/
