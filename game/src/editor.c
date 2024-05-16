#include "editor.h"
#include "Body.h"
#include "render.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;

Rectangle editorRect;

Texture2D cursorTexture;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

    Image image = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    HideCursor();

    ncEditorData.anchor01 = (Vector2){ 967, 74 };
    ncEditorData.EditorBoxActive = true;

    //Body
    ncEditorData.BodyTypeEditMode = false;
    ncEditorData.BodyTypeActive = 0.0f;
    ncEditorData.MassValue = 2.0f;
    ncEditorData.DampingValue = 0.0f;
    ncEditorData.GravityScaleValue = 1.0f;
    ncEditorData.StiffnessValue = 10.0f;

    //World
    ncEditorData.GravitationValue = 0.0f;
    ncEditorData.GravityValue = 0.0f;

    editorRect = (Rectangle){ ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 304, 616 };
}

void UpdateEditor(Vector2 position)
{
    if (IsKeyPressed(KEY_TAB)) ncEditorData.EditorBoxActive = !ncEditorData.EditorBoxActive;

    ncEditorIntersect = ncEditorData.EditorBoxActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor(Vector2 position)
{
    if(ncEditorData.BodyTypeEditMode) GuiLock();

    if (ncEditorData.EditorBoxActive)
    {
        ncEditorData.EditorBoxActive = !GuiWindowBox(editorRect, "Editor");
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 20, ncEditorData.anchor01.y + 50, 264, 200 }, "Body");
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 20, ncEditorData.anchor01.y + 280, 264, 200 }, "World");
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 112, ncEditorData.anchor01.y + 110, 130, 16 }, "Mass", TextFormat("%0.2f", ncEditorData.MassValue), & ncEditorData.MassValue, 0, 5);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 112, ncEditorData.anchor01.y + 135, 130, 16 }, "Damping:", TextFormat("%0.2f", ncEditorData.DampingValue), & ncEditorData.DampingValue , 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 112,ncEditorData.anchor01.y + 160, 130, 16 }, "Gravity Scale:", TextFormat("%0.2f", ncEditorData.GravityScaleValue), & ncEditorData.GravityScaleValue, 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 112,ncEditorData.anchor01.y + 185, 130, 16 }, "Stiffness:", TextFormat("%0.2f", ncEditorData.StiffnessValue), & ncEditorData.StiffnessValue, 0, 50);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 112, ncEditorData.anchor01.y + 300, 130, 16 }, "Gravity", TextFormat("%0.2f", ncEditorData.GravityValue), & ncEditorData.GravityValue, 0, 50);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 112, ncEditorData.anchor01.y + 320, 130, 16 }, "Gravitation", TextFormat("%0.2f", ncEditorData.GravitationValue), & ncEditorData.GravitationValue, 0, 50);
        if (GuiDropdownBox((Rectangle){ ncEditorData.anchor01.x + 35, ncEditorData.anchor01.y + 70, 240, 20 }, "DYNAMIC;STATIC;KINEMATIC", &ncEditorData.BodyTypeActive, ncEditorData.BodyTypeEditMode)) ncEditorData.BodyTypeEditMode = !ncEditorData.BodyTypeEditMode;
    }

    GuiUnlock();

    if (!ncEditorIntersect)
    {
        if (IsCursorOnScreen()) HideCursor();
        DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, (Color) { 255, 255, 255, 255 });
    }
    else ShowCursor();

}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass * 0.5f)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}