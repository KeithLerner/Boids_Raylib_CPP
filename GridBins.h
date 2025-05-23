#pragma once
#include <vector>
#include "Boid.h"
#include <math.h>
#include "Bounds.h"

class GridBins
{
	std::vector<std::vector<Boid>> bins;

private:
	Bounds bounds;
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

	int Density()
	{
		return binDensity;
	}

	Vector3 BinSize() 
	{
		return binSize;
	}

	/// <summary>
	/// Convert a world position into a grid bin index coordinate.
	/// </summary>
	/// <param name="worldPosition"> The query position. </param>
	/// <returns> -Vector3Int.one if invalid input given, otherwise returns a
	/// valid index to a bin in the grid. </returns>
	Vector3 WorldPosToBinIndex(Vector3 worldPosition)
	{
		if (!bounds.Contains(worldPosition)) return Vector3{ -1, -1, -1 };

		return Vector3
		{
			floorf(.5f * binDensity + worldPosition.x / binSize.x),
			floorf(.5f * binDensity + worldPosition.y / binSize.y),
			floorf(.5f * binDensity + worldPosition.z / binSize.z)
		};
	}
};

