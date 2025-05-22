#pragma once
#include <vector>
#include "Boid.h"
#include "Bounds.h"

class GridBins
{
	std::vector<std::vector<Boid>> bins;

private:
	const Bounds bounds;
	const Vector3 binSize;
	const int binDensity = 100;


public:
	GridBins(Bounds bounds, int density)
		: bounds(bounds),
		binSize{ bounds.Size().x / density, bounds.Size().y / density, bounds.Size().z / density},
		binDensity(density)
	{
		bins.reserve(density * density * density);

		for (int i = 0; i < density; ++i)
		{
			for (int j = 0; j < density; ++j)
			{
				for (int k = 0; k < density; ++k)
				{
					bins.push_back(std::vector<Boid>());
				}
			}
		}
	}
};

