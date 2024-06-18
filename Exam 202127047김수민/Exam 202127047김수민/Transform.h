#pragma once

namespace Transform {
    struct Vertex {
        float x;
        float y;
    };

    Vertex RotateVertex(const Vertex& point, float angle_degree, float aspect_ratio);
}
