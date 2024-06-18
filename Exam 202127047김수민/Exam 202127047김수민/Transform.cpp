#include "Transform.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Rotate a vertex around the origin considering aspect ratio
Transform::Vertex Transform::RotateVertex(const Transform::Vertex& point, float angle_degree, float aspect_ratio) {
    float angle_rad = angle_degree * M_PI / 180.0f;
    float cos_a = cos(angle_rad);
    float sin_a = sin(angle_rad);
    return { point.x * cos_a - point.y * sin_a * aspect_ratio, point.x * sin_a / aspect_ratio + point.y * cos_a };
}
