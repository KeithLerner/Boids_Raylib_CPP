#pragma once
#include <vector>
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "Bounds.h"
#include "GridBins.h"

struct Boid
{	
public:
    int id;
	Vector3 position;
	Vector3 velocity;
	int binIndex;

	static float maxSpeed;
	static float alignmentWeight;
	static float cohesionWeight;
	static float avoidEdgesWeight;
	static float separationWeight;
	static float separationDistance;
	static float senseDistance;

	void Movement(std::vector<Boid> neighbors, Bounds bounds)
	{
        // Set base values for boid rules
        Vector3 alignment =  { 0.0f, 0.0f, 0.0f };
        Vector3 cohesion =   { 0.0f, 0.0f, 0.0f };
        Vector3 separation = { 0.0f, 0.0f, 0.0f };
        Vector3 seekCenter = { 0.0f, 0.0f, 0.0f };

		// Loop through neighbors and sum effect of rules
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

        // Apply unique rule for avoiding edges of the simulation
        Vector3 extents = bounds.Extents();
        Vector3 center = bounds.Center();
        Vector3 toCenter = bounds.Center() - position;
        if (position.x >= (center.x + extents.x) * .9f || 
            position.x <= (center.x - extents.x) * .9f ||
            position.y >= (center.y + extents.y) * .9f ||
            position.y <= (center.y - extents.y) * .9f ||
            position.z >= (center.z + extents.z) * .9f ||
            position.z <= (center.z - extents.z) * .9f)
			seekCenter += toCenter;
        seekCenter = Vector3Normalize(seekCenter) * maxSpeed;

        // Average boid rule data and normlize vectors to a set magnitude
        if (count > 0)
        {
            alignment = Vector3Normalize(alignment / count) * maxSpeed;
            cohesion = Vector3Normalize(cohesion / count) * maxSpeed;
            separation = Vector3Normalize(separation) * maxSpeed;
        }

        // Caclulate new velocity
        float deltaTime = GetFrameTime();
        velocity += (
            alignment  * alignmentWeight +
            cohesion   * cohesionWeight +
            separation * separationWeight +
            seekCenter * avoidEdgesWeight) * deltaTime;
        velocity = Vector3Normalize(velocity) * maxSpeed;

		// Apply velocity to position
        position += velocity * deltaTime;
	}

    void FixToBounds(Bounds bounds)
    {
        if (bounds.Contains(position)) return;

		Vector3 min = bounds.Min();
		Vector3 max = bounds.Max();
        Vector3 fix = Vector3Scale(bounds.Size(), 0.1f);

        Vector3 newPos = position;

		if (position.x >= max.x)
            position.x = min.x + fix.x;
        if (position.y >= max.y)
            position.y = min.y + fix.y;
        if (position.z >= max.z)
            position.z = min.z + fix.z;

        if (position.x <= min.x)
            position.x = max.x - fix.x;
        if (position.y <= min.y)
            position.y = max.y - fix.y;
        if (position.z <= min.z)
            position.z = max.z - fix.z;
    }

	void UpdateBinIndex(int calculatedBinIndex)
	{
        if (calculatedBinIndex == binIndex) return;
        binIndex = calculatedBinIndex;
	}
};

