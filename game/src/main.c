#include "Body.h"
#include "world.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 100

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	//game loop
	while (!WindowShouldClose())
	{
		//update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		
		if (IsMouseButtonPressed(0))
		{
			CreateBody();
			
			bodies->position = position;
			bodies->velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
		}

		//Destroys bodies 
		if (IsMouseButtonPressed(1) && bodies) DestroyBody(bodies);

		//render
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 15, YELLOW);
		
		// update / draw bodies
		Body* body = bodies;
		while (body)
		{
			// update body position
			body->position = Vector2Add(body->position, body->velocity);
			
			// draw body
			DrawCircle(body->position.x, body->position.y, 10, RED);
			
			// get next body
			body = body->next;
		}

		EndDrawing();
	}
	CloseWindow();

	free(bodies);

	return 0;
}