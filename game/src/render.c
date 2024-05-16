#include "render.h"
#include "raymath.h"

// Define screen size and zoom level
Vector2 ncScreenSize = { 1280, 720 };
float ncScreenZoom = 1.0f;
float ncViewSize = 25.0f;

// Function to convert screen coordinates to world coordinates
Vector2 ConvertScreenToWorld(Vector2 screen)
{
    // Calculate the aspect ratio of the screen
    float ratio = ncScreenSize.x / ncScreenSize.y;
    // Calculate the extents of the view
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Calculate the lower and upper bounds of the view
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Normalize screen coordinates
    float nx = screen.x / ncScreenSize.x;
    float ny = (ncScreenSize.y - screen.y) / ncScreenSize.y;

    // Map normalized screen coordinates to world coordinates
    Vector2 world;
    world.x = ((1 - nx) * lower.x) + (nx * upper.x);
    world.y = ((1 - ny) * lower.y) + (ny * upper.y);

    return world;
}

// Function to convert world coordinates to screen coordinates
Vector2 ConvertWorldToScreen(Vector2 world)
{
    // Calculate the aspect ratio of the screen
    float ratio = ncScreenSize.x / ncScreenSize.y;
    // Calculate the extents of the view
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Calculate the lower and upper bounds of the view
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Normalize world coordinates
    float nx = (world.x - lower.x) / (upper.x - lower.x);
    float ny = (world.y - lower.y) / (upper.y - lower.y);

    // Map normalized world coordinates to screen coordinates
    Vector2 screen;
    screen.x = nx * ncScreenSize.x;
    screen.y = (1.0f - ny) * ncScreenSize.y;

    return screen;
}

// Function to convert world coordinates to pixel coordinates
float ConvertWorldToPixel(float world)
{
    // Calculate the aspect ratio of the screen
    float ratio = ncScreenSize.x / ncScreenSize.y;
    // Calculate the extents of the view
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Normalize world coordinate
    float nx = world / extents.x;
    // Convert normalized coordinate to pixel coordinate
    float pixel = nx * ncScreenSize.x;

    return pixel;
}
