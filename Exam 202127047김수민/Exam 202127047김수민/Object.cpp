#include "Object.h"
#include "Transform.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Base class
Object::Object(float x, float y, float width, float height, float r, float g, float b)
    : x(x), y(y), width(width), height(height), r(r), g(g), b(b) {}

void Object::Draw() const {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void Object::OnCollisionEnter(Object& other) {}

void Object::Move(float dx, float dy) {
    x += dx;
    y += dy;
}

void Object::Rotate(float angle_degree) {
    Transform::Vertex center = { x + width / 2, y + height / 2 };
    Transform::Vertex vertices[4] = {
        { x, y },
        { x + width, y },
        { x + width, y + height },
        { x, y + height }
    };

    for (int i = 0; i < 4; i++) {
        vertices[i].x -= center.x;
        vertices[i].y -= center.y;
        vertices[i] = Transform::RotateVertex(vertices[i], angle_degree);
        vertices[i].x += center.x;
        vertices[i].y += center.y;
    }
}

void Object::Scale(float sx, float sy) {
    width *= sx;
    height *= sy;
}

bool Object::IsCollidingWith(const Object& other) const {
    return x < other.x + other.width &&
        x + width > other.x &&
        y < other.y + other.height &&
        y + height > other.y;
}

// Derived classes
Player::Player(float x, float y)
    : Object(x, y, 50.0f / 800.0f * 2, 50.0f / 600.0f * 2, 1.0f, 0.0f, 0.0f) {}

void Player::Draw() const {
    Object::Draw();
    // 테두리 그리기
    glColor3f(1.0f, 1.0f, 1.0f);  // 흰색 테두리
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void Player::OnCollisionEnter(Object& other) {}

EnemyBlock::EnemyBlock(float x, float y, float height)
    : Object(x, y, 50.0f / 800.0f * 2, height, 0.0f, 1.0f, 0.0f) {}

void EnemyBlock::OnCollisionEnter(Object& other) {}

Ground::Ground(float y)
    : Object(-1.0f, y, 2.0f, 0.25f, 1.0f, 0.784f, 0.058f) {}

void Ground::OnCollisionEnter(Object& other) {}

Star::Star(float x, float y, float size)
    : Object(x, y, size / 800.0f * 2, size / 600.0f * 2, 1.0f, 1.0f, 1.0f), angle(0.0f) {}

void Star::OnCollisionEnter(Object& other) {}

void Star::UpdateRotation(float deltaAngle) {
    angle += deltaAngle;
}

void Star::Draw() const {
    TransformStack transformStack;
    transformStack.LoadIdentity();
    transformStack.Push();

    // 중심점으로 이동
    transformStack.MultMatrix(Transform::TranslationMatrix(x + width / 2, y + height / 2));
    // 회전
    transformStack.MultMatrix(Transform::RotationMatrix(angle));
    // 원래 위치로 이동
    transformStack.MultMatrix(Transform::TranslationMatrix(-(x + width / 2), -(y + height / 2)));

    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    float angle_offset = M_PI / 2; // Rotate star to point upwards
    for (int i = 0; i < 5; ++i) {
        float angle_deg = 72.0f * i + 18.0f; // 72 degrees between points, start at 18 degrees
        float angle_rad = angle_deg * M_PI / 180.0f;
        Transform::Vertex outer = { x + width / 2 * cos(angle_rad + angle_offset), y + height / 2 * sin(angle_rad + angle_offset) };
        outer = transformStack.TransformVertex(outer);
        glVertex2f(outer.x, outer.y);

        angle_deg += 36.0f; // 36 degrees between outer and inner points
        angle_rad = angle_deg * M_PI / 180.0f;
        Transform::Vertex inner = { x + width / 4 * cos(angle_rad + angle_offset), y + height / 4 * sin(angle_rad + angle_offset) };
        inner = transformStack.TransformVertex(inner);
        glVertex2f(inner.x, inner.y);
    }
    glEnd();
    transformStack.Pop();
}

bool PhysicsAABB(const Object& A, const Object& B) {
    return A.IsCollidingWith(B);
}
