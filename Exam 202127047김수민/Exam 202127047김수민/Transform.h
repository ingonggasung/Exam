#pragma once
#include <cmath>
#include <stack>

namespace Transform {
    struct Vertex {
        float x;
        float y;
    };

    struct Matrix {
        float m[3][3];
    };

    Matrix IdentityMatrix();
    Matrix MultiplyMatrix(const Matrix& a, const Matrix& b);
    Vertex ApplyMatrix(const Matrix& mat, const Vertex& v);
    Matrix TranslationMatrix(float tx, float ty);
    Matrix RotationMatrix(float angle_degree);
}

class TransformStack {
public:
    void Push();
    void Pop();
    void LoadIdentity();
    void MultMatrix(const Transform::Matrix& mat);
    Transform::Vertex TransformVertex(const Transform::Vertex& v);

private:
    std::stack<Transform::Matrix> stack;
};

