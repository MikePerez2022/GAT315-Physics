#pragma once
#include "raylib.h"

typedef struct ncBody
{
	// force -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 force;

	struct ncBody* next;
	struct ncBody* prev;
} ncBody;