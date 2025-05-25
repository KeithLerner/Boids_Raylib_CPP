#pragma once
#include <iostream>
#include <array>
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

		std::cout << "Bins per axis: " << density << std::endl;
		std::cout << "Bin size: " << gridBins.BinSize().x << ", " << gridBins.BinSize().y << ", " << gridBins.BinSize().z << std::endl;
		std::cout << "Bounds Min: " << min.x << ", " << min.y << ", " << min.z << std::endl;
		std::cout << "Bounds Max: " << max.x << ", " << max.y << ", " << max.z << std::endl;
		std::cout << "----------------------------------------" << std::endl;

		for (int i = 0; i < count; i++)
		{
			int x = GetRandomValue(min.x, max.x);
			int y = GetRandomValue(min.y, max.y);
			int z = GetRandomValue(min.z, max.z);

			Vector3 offset = { 
				GetRandomValue(-1.0f, 1.0f) * .1f, 
				GetRandomValue(-1.0f, 1.0f) * .1f, 
				GetRandomValue(-1.0f, 1.0f) * .1f };

			Vector3 pos = { x + offset.x, y + offset.y, z + offset.z };

			int index = gridBins.WorldPosToVectorIndex(pos);

			Vector3 binMin = gridBins.GetBinMin(index);
			Vector3 binMax = gridBins.GetBinMax(index);

			bool contained = pos.x >= binMin.x && pos.x <= binMax.x &&
				pos.y >= binMin.y && pos.y <= binMax.y &&
				pos.z >= binMin.z && pos.z <= binMax.z;

			if (!contained)
			{
				fail++;
				std::cout << "Random Position: " << x << ", " << y << ", " << z << std::endl;
				std::cout << "Offset: " << offset.x << ", " << offset.y << ", " << offset.z << std::endl;
				std::cout << "World Position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
				std::cout << "Bin Index: " << index << std::endl;
				std::cout << "Bin Min: " << binMin.x << ", " << binMin.y << ", " << binMin.z << std::endl;
				std::cout << "Bin Max: " << binMax.x << ", " << binMax.y << ", " << binMax.z << std::endl;
				std::cout << "FAIL" << std::endl;
			}
			else
			{
				pass++;
				std::cout << "PASS (" << i << ")" << std::endl;
			}

			std::cout << "----------------------------------------" << std::endl;
		}

		std::cout << "Passed: " << pass << std::endl;
		std::cout << "Failed: " << fail << std::endl;
	}

	static void TestSetGridBinCoordinates()
	{
		int sizeMult = 3, density = 2;
		Bounds bounds = Bounds(Vector3{ 0, 0, 0 }, Vector3One() * sizeMult);
		GridBins<bool> gridBins = GridBins<bool>(bounds, density);

		Vector3 min = bounds.Min();
		Vector3 max = bounds.Max();

		std::cout << "Bins per axis: " << density << std::endl;
		std::cout << "Bin size: " << gridBins.BinSize().x << ", " << gridBins.BinSize().y << ", " << gridBins.BinSize().z << std::endl;
		std::cout << "Bounds Min: " << min.x << ", " << min.y << ", " << min.z << std::endl;
		std::cout << "Bounds Max: " << max.x << ", " << max.y << ", " << max.z << std::endl;
		std::cout << "----------------------------------------" << std::endl;

		std::array<Vector3, 8> positions = {
			bounds.Min(),
			{ bounds.Min().x, bounds.Min().y, bounds.Max().z },
			{ bounds.Min().x, bounds.Max().y, bounds.Min().z },
			{ bounds.Min().x, bounds.Max().y, bounds.Max().z },
			{ bounds.Max().x, bounds.Min().y, bounds.Min().z },
			{ bounds.Max().x, bounds.Min().y, bounds.Max().z },
			{ bounds.Max().x, bounds.Max().y, bounds.Min().z },
			bounds.Max()
		};

		int pass = 0, fail = 0;

		for (int i = 0; i < positions.size(); i++)
		{
			Vector3 pos = positions[i];

			int index = gridBins.WorldPosToVectorIndex(pos);

			Vector3 binMin = gridBins.GetBinMin(index);
			Vector3 binMax = gridBins.GetBinMax(index);

			bool contained = pos.x >= binMin.x && pos.x <= binMax.x &&
				pos.y >= binMin.y && pos.y <= binMax.y &&
				pos.z >= binMin.z && pos.z <= binMax.z;

			std::cout << "World Position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
			std::cout << "Bin Index: " << index << std::endl;
			std::cout << "Bin Min: " << binMin.x << ", " << binMin.y << ", " << binMin.z << std::endl;
			std::cout << "Bin Max: " << binMax.x << ", " << binMax.y << ", " << binMax.z << std::endl;

			if (contained)
			{
				pass++;
				std::cout << "PASS (" << i << ")" << std::endl;
			}
			else
			{
				fail++;
				std::cout << "FAIL (" << i << ")" << std::endl;
			}

			std::cout << "----------------------------------------" << std::endl;
		}

		std::cout << "Passed: " << pass << std::endl;
		std::cout << "Failed: " << fail << std::endl;
	}
};

