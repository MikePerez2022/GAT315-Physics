#include "world.h"
#include "mathf.h"
#include "Integrator.h"
#include "raylib.h"

#include <stdlib.h>

#define MAX_BODIES 100

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	//initalize world
	ncGravity = (Vector2){0, 30};

	//game loop
	while (!WindowShouldClose())
	{
		//update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		
		if (IsMouseButtonPressed(0)) 
		{
			for (int i = 0; i < 15; i++)
			{
				ncBody* body = CreateBody();

				//hue is the color | saturation is alpha | value is brightness
				body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);

				body->position = position;
				body->mass = GetRandomFloatValue(5, 10);
				body->inverseMass = 1 / body->mass;
				body->type = BT_DYNAMIC;
				body->damping = 0.5f;
				body->gravityScale = 20;
				ApplyForce(body, (Vector2) { GetRandomFloatValue(-100, 100), GetRandomFloatValue(-100, 100) }, FM_VELOCITY);// might need to add second get rand value
			}
		}

		if (IsMouseButtonDown(1) && ncBodies) DestroyBody(ncBodies);


		for (ncBody* body = ncBodies; body; body = body->next)
		{
			//ApplyForce(body, CreateVector2(0, -50), FM_FORCE);
		}

		//Update bodies
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			Step(body, dt);
		}

		//render
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 15, YELLOW);

		//Draw bodies
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			DrawCircle(body->position.x, body->position.y, body->mass, body->color);
		}

		EndDrawing();
	}
	CloseWindow();

	free(ncBodies);

	return 0;
}