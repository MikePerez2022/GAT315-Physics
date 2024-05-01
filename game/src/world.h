#pragma once
#include "Body.h"

extern ncBody* ncBodies;
extern int ncBodyCount;

ncBody* CreateBody();
void DestroyBody(ncBody* body);
