#pragma once
#include <vector>
#include "Boid.h"
#include <math.h>
#include "Bounds.h"

template <typename T> class GridBins
{

protected:
	std::vector<std::vector<T>> bins;
	Bounds bounds;
	const Vector3 binSize;
	const int binDensity = 10;

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
					bins.push_back(std::vector<T>());
				}
			}
		}
	}

	std::vector<std::vector<T>> Bins()
	{
		return bins;
	}

	int Density()
	{
		return binDensity;
	}

	Vector3 BinSize() 
	{
		return binSize;
	}

	void AddToBin(int binIndex, T item)
	{
		if (binIndex < 0 || binIndex >= bins.size()) return;

		bins[binIndex].push_back(item);
	}

	void RemoveFromBin(int binIndex, T item)
	{
		if (binIndex < 0 || binIndex >= bins.size()) return;

		std::vector<T>& bin = bins[binIndex];
		T foundItem = std::remove(bin.begin(), bin.end(), item);
		if (foundItem != bin.end())
		{
			bin.erase(foundItem, bin.end());
		}
	}

	Vector3 GetBinMin(int binIndex)
	{
		if (binIndex < 0 || binIndex >= bins.size()) return Vector3{ -1, -1, -1 };

		int x = binIndex % binDensity;
		int y = binIndex / binDensity % binDensity;
		int z = binIndex / (binDensity * binDensity);

		return Vector3
		{
			bounds.Min().x + x * binSize.x,
			bounds.Min().y + y * binSize.y,
			bounds.Min().z + z * binSize.z
		};
	}

	Vector3 GetBinMax(int binIndex)
	{
		if (binIndex < 0 || binIndex >= bins.size()) return Vector3{ -1, -1, -1 };

		int x = binIndex % binDensity;
		int y = binIndex / binDensity % binDensity;
		int z = binIndex / (binDensity * binDensity);

		return Vector3
		{
			bounds.Min().x + (x + 1) * binSize.x,
			bounds.Min().y + (y + 1) * binSize.y,
			bounds.Min().z + (z + 1) * binSize.z
		};
	}

	/// <summary>
	/// Convert a world position into a grid bin vector index coordinate.
	/// </summary>
	/// <param name="worldPosition"> The query position. </param>
	/// <returns> -1 if input world position is outside the bounds of the bins, 
	/// -2 if calculated index is outside array range, 
	/// otherwise returns a valid index to a bin in the grid. </returns>
	int WorldPosToVectorIndex(Vector3 worldPosition)
	{
		if (!bounds.Contains(worldPosition)) return -1;

		float halfBinDensity = .5f * binDensity;
		int i = (int)floorf(halfBinDensity + worldPosition.x / binSize.x) +
			    (int)floorf(halfBinDensity + worldPosition.y / binSize.y) * binDensity +
				(int)floorf(halfBinDensity + worldPosition.z / binSize.z) * binDensity * binDensity;

		if (i < 0 || i >= bins.size()) return -2; 

		return i;
	}

	/* 
	// ===== BIN INDICES ARE NOT USED, ONLY WORLD POSITION AND ARRAY INDEX =====
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
	
	int BinIndexToArrayIndex(Vector3 binIndex)
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
	*/

	/// <summary>
	/// Get the indices of bins directly surrounding the given bin index.
	/// </summary>
	/// <param name="index"> The index of the bin to query neighbors of. </param>
	/// <returns> A vector containing </returns>
	std::vector<int> GetNeighborBinIndices(int index, bool includeIndexedBin)
	{
		std::vector<int> results = std::vector<int>();

		// TODO: Check conversion works as intended
		int ix = index % binDensity;
		int iy = index / binDensity % binDensity;
		int iz = index / (binDensity * binDensity);

		for (int x = ix - 1; x <= ix + 1; x++)
		{
			for (int y = iy - 1; y <= iy + 1; y++)
			{
				for (int z = iz - 1; z <= iz + 1; z++)
				{
					// Skip the indexed bin if not included
					if (!includeIndexedBin && x == ix && y == iy && z == iz) 
						continue;

					// Check that modified index exists within grid space before
					// adding to results
					if (x < binDensity - 2 && x > 0 &&
						y < binDensity - 2 && y > 0 &&
						z < binDensity - 2 && z > 0)
						results.push_back(x + 
							y * binDensity + 
							z * binDensity * binDensity);
				}
			}
		}

		return results;
	}
};

