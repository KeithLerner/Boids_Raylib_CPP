#pragma once
#include "raylib.h"

struct Boid
{	
public:
	Vector3 position;
	Vector3 velocity;

	static float maxSpeed;
	static float alignmentWeight;
	static float cohesionWeight;
	static float separationWeight;
};

