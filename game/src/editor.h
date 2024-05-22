#pragma once
#include "raylib.h"

typedef struct ncEditorData
{
    Vector2 anchor01;

    bool EditorBoxActive;
	float MassValue;
	float GravitationValue;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    float DampingValue;
    float GravityScaleValue;
	float GravityValue;
	float StiffnessValue;
    float TimestepValue;
    bool ResetBtnPressed;
    bool SimulateBtnPressed;
}ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 position);
void DrawEditor(Vector2 position);

struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position); 
void DrawLineBodyToPosition(struct ncBody* body, Vector2 position);