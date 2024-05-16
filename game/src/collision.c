#include "collision.h"
#include "contact.h"
#include "Body.h"
#include "mathf.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

bool Intersects(ncBody* body1, ncBody* body2)
{
	float distance = Vector2Distance(body1->position, body2->position);
	float radius = body1->mass + body2->mass;

	return (distance <= radius);
}

void CreateContacts(ncBody* bodies, ncContact_t** contacts)
{
	for (ncBody* body1 = bodies; body1; body1 = body1->next)
	{
		for (ncBody* body2 = body1->next; body2; body2 = body2->next)
		{
			if (body1 == body2) continue;
			if (body1->type != BT_DYNAMIC && body2->type != BT_DYNAMIC) continue;

			if (Intersects(body1, body2))
			{
				ncContact_t* contact = GenerateContact(body1, body2);
				AddContact(contact, contacts);
			}
		}
	}
}

ncContact_t* GenerateContact(ncBody* body1, ncBody* body2)
{
	ncContact_t* contact = (ncContact_t*)malloc(sizeof(ncContact_t));
	assert(contact);

	memset(contact, 0, sizeof(ncContact_t));

	contact->body1 = body1;
	contact->body2 = body2;

	Vector2 direction = Vector2Subtract(body1->position, body2->position);
	float distance = Vector2Length(direction);
	if (distance == 0)
	{
		direction = (Vector2){ GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f) };
	}

	float radius = (body1->mass + body2->mass);

	contact->depth = radius - distance;
	contact->normal = Vector2Normalize(direction);
	contact->restitution = (body1->restitution + body2->restitution) * 0.5f;

	return contact;
}

void SeparateContacts(ncContact_t* contacts)
{
	// Iterate through each contact in the linked list
	for (ncContact_t* contact = contacts; contact; contact = contact->next)
	{
		// Calculate the total inverse mass of the two bodies involved in the contact
		float totalInverseMass = contact->body1->inverseMass + contact->body2->inverseMass;
		// Calculate the separation vector by scaling the contact normal by the depth divided by the total inverse mass
		Vector2 seperation = Vector2Scale(contact->normal, contact->depth / totalInverseMass);
		// Adjust the position of body1 by adding its portion of the separation vector
		contact->body1->position = Vector2Add(contact->body1->position, Vector2Scale(seperation, contact->body1->inverseMass));
		// Adjust the position of body2 by subtracting its portion of the separation vector
		contact->body2->position = Vector2Add(contact->body2->position, Vector2Scale(seperation, -contact->body2->inverseMass));

	}
}

void ResolveContacts(ncContact_t* contacts)
{
	// Iterate through each contact in the linked list
	for (ncContact_t* contact = contacts; contact; contact = contact->next)
	{
		// Calculate the relative velocity between body1 and body2
		Vector2 rv = Vector2Subtract(contact->body1->velocity, contact->body2->velocity);
		// Compute the normal component of the relative velocity
		float nv = Vector2DotProduct(rv, contact->normal);
		// If the normal component is positive, the bodies are separating, so continue to the next contact
		if (nv > 0) continue;
		// Calculate the total inverse mass of the two bodies involved in the contact
		float totalInverseMass = contact->body1->inverseMass + contact->body2->inverseMass;
		// Calculate the magnitude of the impulse to be applied
		// // The impulse magnitude is scaled by the restitution (bounciness) factor
		float impulseMagnitude = -(1 + contact->restitution) * nv / totalInverseMass;
		// Compute the impulse vector by scaling the contact normal by the impulse magnitude
		Vector2 impulse = Vector2Scale(contact->normal, impulseMagnitude);
		// Apply the impulse to body1 in the direction of the contact normal
		ApplyForce(contact->body1, impulse, FM_IMPULSE);
		// Apply the negative impulse to body2 to ensure equal and opposite reaction
		ApplyForce(contact->body2, Vector2Negate(impulse), FM_IMPULSE);
	}
}