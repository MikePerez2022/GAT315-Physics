#include "world.h"

#include <stdlib.h>
#include <assert.h>

Body* bodies = NULL;
int bodyCount = 0;

Body* CreateBody()
{
	//Allocate memory for a new Body using malloc.
	Body* newBody = (Body*)malloc(sizeof(Body));

	//Assert that the allocation was successful.
	assert(newBody);

	//Set the new body’s prev pointer to NULL and its next pointer to the current head of the list (bodies).
	newBody->prev = NULL;
	newBody->next = &bodies[0];

	//If the list is not empty, update the prev pointer of the current head.
	if (bodies) bodies[0].prev = newBody;

	//Set the bodies to the new Body
	bodies = newBody;

	//Increment the bodyCount.
	bodyCount++;

	//Return the new Body.
	return newBody;
}

void DestroyBody(Body* body)
{
	//Assert that the given Body pointer is not NULL.
	assert(body);
	
	//Update the next pointer of the prev body if it exists.
	if(body->prev) body->prev->next = body->next;
	
	//Update the prev pointer of the next body if it exists.
	if (body->next) body->next->prev = body->prev;

	//If the body to be removed is the head of the list, update the head.
	if (!body->prev)
	{
		bodies = bodies->next;
	}
	
	//Decrement the bodyCount.
	bodyCount--;
	//Free the memory allocated to the Body.
	free(body);
}