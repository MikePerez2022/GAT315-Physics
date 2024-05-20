#include "world.h"
#include "mathf.h"
#include "Integrator.h"
#include "Force.h"
#include "spring.h"
#include "raylib.h"
#include "render.h"
#include "editor.h"
#include "collision.h"
#include "contact.h"

#include <stdlib.h>

#define MAX_BODIES 100
#define MAX_TRAIL_LENGTH 100

int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;
	ncContact_t* contacts = NULL;

	float fixedTimestep = 1.0f / 60;
	float timeAccumulator = 0.02f;

	InitWindow(1280, 720, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	//game loop
	while (!WindowShouldClose())
	{
		//update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();
		ncGravity = (Vector2){ 0, -ncEditorData.GravityValue };

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);

		timeAccumulator += dt;

		UpdateEditor(position);

		selectedBody = GetBodyIntersect(ncBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines((int)screen.x, (int)screen.y, ConvertWorldToPixel(selectedBody->mass) + 5, YELLOW);
		}
		
		if (!ncEditorIntersect)
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)))
			{
				float angle = GetRandomFloatValue(0, 360);
				for (int i = 0; i < 1; i++)
				{
					ncBody* body = CreateBody(ConvertScreenToWorld(position), ncEditorData.MassValue, ncEditorData.BodyTypeActive);

					body->damping = ncEditorData.DampingValue;
					body->gravityScale = ncEditorData.GravityScaleValue;
					body->color = WHITE;//ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);
					body->restitution = 0.8f;
				}
			}
		}

		

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			if (selectedBody && selectedBody != connectBody)
			{
				ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), ncEditorData.StiffnessValue);
			}
		}

		while (timeAccumulator >= fixedTimestep)
		{
			timeAccumulator -= fixedTimestep;
			ApplyGravitation(ncBodies, ncEditorData.GravitationValue * ncEditorData.GravityScaleValue);
			ApplySpringForce(ncSprings);

			//Update bodies
			for (ncBody* body = ncBodies; body; body = body->next)
			{
				Step(body, fixedTimestep);
			}

			//collision
			DestroyAllContacts(&contacts);
			CreateContacts(ncBodies, &contacts);
			SeparateContacts(contacts);
			ResolveContacts(contacts);
		}

		//render
		BeginDrawing();
		ClearBackground(BLACK);

		//Stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		//DrawCircle((int)position.x, (int)position.y, 15, YELLOW);

		//Draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}

		//Draw bodies
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
			//DrawLineEx(body->pastPosition, body->position, body->mass, body->color);
		}

		//Draw contacts
		for (ncContact_t* contact = contacts; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
		}

		DrawEditor(position);

		EndDrawing();
	}
	CloseWindow();

	free(ncBodies);

	return 0;
}