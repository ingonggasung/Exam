#include "Transform.h"
#include <cmath>
float M_PI = 3.1415f;
// Transform namespace functions
Transform::Matrix Transform::IdentityMatrix() {
    Transform::Matrix mat = { {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    } };
    return mat;
}

Transform::Matrix Transform::MultiplyMatrix(const Transform::Matrix& a, const Transform::Matrix& b) {
    Transform::Matrix result = { 0 };
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j];
        }
    }
    return result;
}

Transform::Vertex Transform::ApplyMatrix(const Transform::Matrix& mat, const Transform::Vertex& v) {
    Transform::Vertex result;
    result.x = mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2];
    result.y = mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2];
    return result;
}

Transform::Matrix Transform::TranslationMatrix(float tx, float ty) {
    Transform::Matrix mat = IdentityMatrix();
    mat.m[0][2] = tx;
    mat.m[1][2] = ty;
    return mat;
}

Transform::Matrix Transform::RotationMatrix(float angle_degree) {
    Transform::Matrix mat = IdentityMatrix();
    float angle_rad = angle_degree * M_PI / 180.0f;
    float cos_a = cos(angle_rad);
    float sin_a = sin(angle_rad);
    mat.m[0][0] = cos_a;
    mat.m[0][1] = -sin_a;
    mat.m[1][0] = sin_a;
    mat.m[1][1] = cos_a;
    return mat;
}

Transform::Vertex Transform::RotateVertex(const Transform::Vertex& point, float angle_degree) {
    float angle_rad = angle_degree * M_PI / 180.0f;
    float cos_a = cos(angle_rad);
    float sin_a = sin(angle_rad);
    return { point.x * cos_a - point.y * sin_a, point.x * sin_a + point.y * cos_a };
}

// TransformStack class functions
TransformStack::TransformStack() {
    LoadIdentity();
}

void TransformStack::Push() {
    stack.push(stack.top());
}

void TransformStack::Pop() {
    if (!stack.empty()) {
        stack.pop();
    }
}

void TransformStack::LoadIdentity() {
    stack.push(Transform::IdentityMatrix());
}

void TransformStack::MultMatrix(const Transform::Matrix& mat) {
    if (!stack.empty()) {
        stack.top() = Transform::MultiplyMatrix(stack.top(), mat);
    }
}

Transform::Vertex TransformStack::TransformVertex(const Transform::Vertex& v) {
    if (!stack.empty()) {
        return Transform::ApplyMatrix(stack.top(), v);
    }
    return v;
}
