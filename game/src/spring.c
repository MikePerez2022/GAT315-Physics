#include "spring.h"
#include "Body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k)
{
	ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t));
	assert(spring);

	memset(spring, 0, sizeof(ncSpring_t));
	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;

	AddSpring(spring);

	return spring;
}

void AddSpring(ncSpring_t* spring)
{
	assert(spring);

	spring->prev = NULL;
	spring->next = &ncSprings[0];

	if (ncSprings) ncSprings[0].prev = spring;
	ncSprings = spring;
}

void DestroySpring(ncSpring_t* spring)
{
	assert(spring);

	if (spring->prev) spring->prev->next = spring->next;
	if (spring->next) spring->next->prev = spring->prev;

	if (!spring->prev) ncSprings = ncSprings->next;	
	free(spring);
}

void ApplySpringForce(ncSpring_t* spring)
{
	for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->body1->position, spring->body2->position);
		if (direction.x == 0 && direction.y == 0) continue;

		float length = Vector2Length(direction);
		float x = length - spring->restLength;
		float force = -spring->k * x;

		Vector2 ndirection = Vector2Normalize(direction);

		ApplyForce(spring->body1, Vector2Scale(ndirection,force), FM_FORCE);
		ApplyForce(spring->body2, Vector2Negate(Vector2Scale(ndirection, force)), FM_FORCE);
	}
}