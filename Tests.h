#pragma once
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "GridBins.h"
#include "Bounds.h"

class Tests
{
public:
	static void TestRandomGridBinCoordinates(int count, Vector3 size, int density)
	{
		Bounds bounds = Bounds(Vector3{ 0, 0, 0 }, size);
		GridBins<bool> gridBins = GridBins<bool>(bounds, density);

		int pass = 0, fail = 0;

		Vector3 min = bounds.Min();
		Vector3 max = bounds.Max();
		for (int i = 0; i < count; i++)
		{
			int x = GetRandomValue(min.x, max.x);
			int y = GetRandomValue(min.y, max.y);
			int z = GetRandomValue(min.z, max.z);

			std::cout << "Random Position: " << x << ", " << y << ", " << z << std::endl;

			Vector3 pos = { x + (i * 1.1f), y + (i * 0.9f), z + (i * 0.1f) };

			std::cout << "World Position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;

			int index = gridBins.WorldPosToVectorIndex(pos);

			std::cout << "Bin Index: " << index << std::endl;

			Vector3 binMin = gridBins.GetBinMin(index);
			Vector3 binMax = gridBins.GetBinMax(index);
			bool contained = pos.x >= binMin.x && pos.x <= binMax.x &&
				pos.y >= binMin.y && pos.y <= binMax.y &&
				pos.z >= binMin.z && pos.z <= binMax.z;

			std::cout << "Bin Min: " << binMin.x << ", " << binMin.y << ", " << binMin.z << std::endl;
			std::cout << "Bin Max: " << binMax.x << ", " << binMax.y << ", " << binMax.z << std::endl;

			if (contained)
			{
				pass++;
				std::cout << "PASS" << std::endl;
			}
			else
			{
				fail++;
				std::cout << "FAIL" << std::endl;
			}
		}
	}


};

