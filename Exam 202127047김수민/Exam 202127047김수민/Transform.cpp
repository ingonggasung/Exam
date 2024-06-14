#include "Transform.h"
#include <cmath>
float M_PI = 3.1415f;
namespace Transform
{
	Vertex MoveVertex(Vertex point, Vector meter)
	{
		point.x += meter.x_meter;
		point.y += meter.y_meter;
		return point;
	}

	Vertex RotateVertex(Vertex point, float angle_degree)
	{
		float angle_rad = angle_degree * M_PI / 180.0f;
		float cos_angle = cos(angle_rad);
		float sin_angle = sin(angle_rad);
		Vertex rotated;
		rotated.x = point.x * cos_angle - point.y * sin_angle;
		rotated.y = point.x * sin_angle + point.y * cos_angle;
		return rotated;
	}

	Vertex ScaleVertex(Vertex point, Vector meter)
	{
		point.x *= meter.x_meter;
		point.y *= meter.y_meter;
		return point;
	}
};
