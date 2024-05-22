#include "world.h"
#include "Body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity;

ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType)
{
	ncBody* newBody = (ncBody*)malloc(sizeof(ncBody));
	assert(newBody);

	memset(newBody, 0, sizeof(ncBody));
	newBody->position = position;
	newBody->mass = mass;
	newBody->inverseMass = (bodyType == BT_DYNAMIC) ? 1 / mass : 0;
	newBody->type = bodyType;

	AddBody(newBody);

	return newBody;
}

void AddBody(ncBody* body)
{
	assert(body);

	body->prev = NULL;
	body->next = &ncBodies[0];

	if (ncBodies) ncBodies[0].prev = body;
	ncBodies = body;
	ncBodyCount++;
}

void DestroyBody(ncBody* body)
{
	assert(body);	
	if(body->prev) body->prev->next = body->next;	
	if (body->next) body->next->prev = body->prev;

	if (!body->prev) ncBodies = ncBodies->next;
	ncBodyCount--;
	free(body);
}

void DestroyAllBodies()
{
	if (!ncBodies) return;

	ncBody* body = ncBodies;
	while (body)
	{
		ncBody* next = body->next;
		DestroyBody(body);
		body = next;
	}

	/*for (ncBody* body = ncBodies; body; body = body->next)
	{
		if (!body->next)
		{
			DestroyBody(body);
			break;
		}
		if (!body->prev) body = body->next;
		DestroyBody(body->prev);
	}

	DestroyBody(ncBodies);*/
}