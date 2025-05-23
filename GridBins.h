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

	int BinIndexToVectorIndex(Vector3 binIndex)
	{
		return binIndex.x +
			binIndex.y * binDensity +
			binIndex.z * binDensity * binDensity;
	}

	int BinIndexToArrayIndex(int x, int y, int z)
	{
		return x +
			y * binDensity +
			z * binDensity * binDensity;
	}

	Vector3 ArrayIndexToBinIndex(int index)
	{
		float x = index % binDensity;
		float y = index / binDensity % binDensity;
		float z = index / (binDensity * binDensity);

		return Vector3{ x, y, z };
	}

	std::vector<Vector3> GetNeighborBinIndices(Vector3 index)
	{
		std::vector<Vector3> results = std::vector<Vector3>();

		for (int x = index.x - 1; x <= index.x + 1; x++)
		{
			for (int y = index.y - 1; y <= index.y + 1; y++)
			{
				for (int z = index.z - 1; z <= index.z + 1; z++)
				{
					// Check that modified index exists within grid space before
					// adding to results
					if (index.x < binDensity - 2 && index.x > 0 &&
						index.y < binDensity - 2 && index.y > 0 &&
						index.z < binDensity - 2 && index.z > 0)
						results.push_back(Vector3{ (float)x, (float)y, (float)z });
				}
			}
		}

		return results;
	}
};

