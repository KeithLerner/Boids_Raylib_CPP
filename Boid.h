#pragma once
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "Bounds.h"

struct Boid
{	
public:
    int id;
	Vector3 position;
	Vector3 velocity;

	static float maxSpeed;
	static float alignmentWeight;
	static float cohesionWeight;
	static float avoidEdgesWeight;
	static float separationWeight;
	static float separationDistance;
	static float senseDistance;

	void Movement(std::vector<Boid> neighbors, Bounds bounds)
	{
        Vector3 alignment =  { 0.0f, 0.0f, 0.0f };
        Vector3 cohesion =   { 0.0f, 0.0f, 0.0f };
        Vector3 separation = { 0.0f, 0.0f, 0.0f };
        Vector3 seekCenter = { 0.0f, 0.0f, 0.0f };

        int count = 0;
        for (size_t i = 0; i < neighbors.size(); i++)
        {
			Boid b = neighbors[i];
			if (b.id == id) continue;

			Vector3 bPos = b.position;
			Vector3 toBoid = bPos - position;

			alignment += b.velocity;
            cohesion  += bPos;
			if (Vector3Length(toBoid) < separationDistance)
				separation -= Vector3Normalize(toBoid);

			count++;
        }

        float dist =
            Vector3Distance(position, bounds.Center());
        if (dist > Vector3Length(bounds.Extents()) / 2)
            seekCenter += 
            Vector3Normalize(bounds.Center() - position) * maxSpeed;

        if (count > 0)
        {
            alignment = Vector3Normalize(alignment / count) * maxSpeed;
            cohesion = Vector3Normalize(cohesion / count) * maxSpeed;
            separation = Vector3Normalize(separation) * maxSpeed;
        }

        float deltaTime = GetFrameTime();

        velocity += (alignment * alignmentWeight +
            cohesion * cohesionWeight +
            separation * separationWeight +
            seekCenter * avoidEdgesWeight) * deltaTime;
        velocity = Vector3Normalize(velocity) * maxSpeed;

        position += velocity * deltaTime;
	}

    void FixToBounds(Bounds bounds)
    {
        if (bounds.Contains(position)) return;

		Vector3 min = bounds.Min();
		Vector3 max = bounds.Max();

        Vector3 newPos = position;
        if (position.x >= max.x)
            newPos.x = min.x + 1;
        if (position.y >= max.y)
            newPos.y = min.y + 1;
        if (position.z >= max.z)
            newPos.z = min.z + 1;

        if (position.x <= min.x)
            newPos.x = max.x - 1;
        if (position.y <= min.y)
            newPos.y = max.y - 1;
        if (position.z <= min.z)
            newPos.z = max.z - 1;

        position = newPos;
    }
};

