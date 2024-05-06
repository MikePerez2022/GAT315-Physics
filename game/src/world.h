#pragma once
#include "raylib.h"

typedef struct ncBody ncBody;

extern ncBody* ncBodies;
extern int ncBodyCount;
extern Vector2 ncGravity;

ncBody* CreateBody();
void DestroyBody(ncBody* body);