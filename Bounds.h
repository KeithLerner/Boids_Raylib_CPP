#pragma once
#include "raylib.h"

class Bounds
{
	const Vector3 _center;
	const Vector3 _size;

public:
    Bounds(Vector3 center, Vector3 size) : 
		_center(center), _size(size) {}

	Vector3 Center()
	{
		return _center;
	}

	Vector3 Size() 
	{
		return _size;
	}

	Vector3 Extents() 
	{
		return Vector3{ _size.x / 2, _size.y / 2, _size.z / 2 };
	}

	Vector3 Min() 
	{
		Vector3 extents = Extents();
		return Vector3{ _center.x - extents.x, 
			_center.y - extents.y, _center.z - extents.z };
	}

	Vector3 Max() 
	{
		Vector3 extents = Extents();
		return Vector3{ _center.x + extents.x, 
			_center.y + extents.y, _center.z + extents.z };
	}

	bool Contains(Vector3 point, bool inclusive = true)
	{
		Vector3 min = Min();
		Vector3 max = Max();

		if (inclusive)
		{
			return (point.x >= min.x && point.x <= max.x &&
				point.y >= min.y && point.y <= max.y &&
				point.z >= min.z && point.z <= max.z);
		}
		else
		{
			return (point.x > min.x && point.x < max.x &&
				point.y > min.y && point.y < max.y &&
				point.z > min.z && point.z < max.z);
		}
	}
};

