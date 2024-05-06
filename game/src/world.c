#include "world.h"
#include "Body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity;

ncBody* CreateBody()
{
	ncBody* newBody = (ncBody*)malloc(sizeof(ncBody));
	assert(newBody);

	memset(newBody, 0, sizeof(ncBody));

	newBody->prev = NULL;
	newBody->next = &ncBodies[0];

	if (ncBodies) ncBodies[0].prev = newBody;
	ncBodies = newBody;
	ncBodyCount++;

	return newBody;
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