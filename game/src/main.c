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
	//Initalize
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;
	ncContact_t* contacts = NULL;

	InitWindow(1280, 720, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	float fixedTimestep = 1.0f / ncEditorData.TimestepValue;
	float timeAccumulator = 0.02f;

	//game loop
	while (!WindowShouldClose())
	{
		//update
		timeAccumulator += GetFrameTime();//add dt
		//allows contol of timestep in gui
		fixedTimestep = 1.0f / ncEditorData.TimestepValue;
		ncGravity = (Vector2){ 0, -ncEditorData.GravityValue };

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);



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

		//Connect spring
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			if (selectedBody && selectedBody != connectBody)
			{
				ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), ncEditorData.StiffnessValue);
			}
		}

		//Unselect Body
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
		{
			selectedBody = NULL;
			connectBody = NULL;
		}

		//Drag Body
		if (IsKeyDown(KEY_LEFT_ALT))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
			if (connectBody)
			{
				Vector2 world = ConvertScreenToWorld(position);
				ApplySpringForcePosition(world, connectBody, 0, 20, 5);
			}
		}

		//Reset
		if (ncEditorData.ResetBtnPressed)
		{
			DestroyAllSprings();
			DestroyAllContacts(&contacts);
			DestroyAllBodies();
		}

		while (timeAccumulator >= fixedTimestep)
		{
			if (!ncEditorData.SimulateBtnPressed) break;
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
		DrawText(TextFormat("FPS: %.2f (%.2fms)", ncEditorData.TimestepValue, 1000 / ncEditorData.TimestepValue), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", timeAccumulator), 10, 30, 20, LIME);

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