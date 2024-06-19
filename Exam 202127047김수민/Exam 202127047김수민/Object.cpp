#include "Object.h"
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
    // 회전 변환은 필요하지 않음
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
    : Object(x, y, 50.0f / 800.0f * 2, (50.0f / 800.0f * 2)* (800.0f / 600.0f), 1.0f, 0.0f, 0.0f) {}

void Player::Draw(float rotationAngle) const {
    glPushMatrix(); // 현재 행렬 저장

    // 중심점을 기준으로 회전하기 위해 이동
    glTranslatef(x + width / 2, y + height / 2, 0.0f);
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-(x + width / 2), -(y + height / 2), 0.0f);

    // 플레이어 그리기
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // 테두리 그리기
    glColor3f(1.0f, 1.0f, 1.0f);  // 흰색 테두리
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    glPopMatrix(); // 이전 행렬 복원
}

void Player::OnCollisionEnter(Object& other) {}

void Player::SetHeight(float newHeight) {
    this->height = newHeight;
}

EnemyBlock::EnemyBlock()
    : Object(0.0f, 0.0f, 50.0f / 800.0f * 2, 100.0f / 600.0f * 2, 0.0f, 1.0f, 0.0f) {} // 기본 생성자 정의

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
    glPushMatrix(); // 현재 행렬 저장

    // 중심점을 기준으로 회전하기 위해 이동
    glTranslatef(x + width / 2, y + height / 2, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-(x + width / 2), -(y + height / 2), 0.0f);

    // 별 그리기
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    float angle_offset = M_PI / 2; // Rotate star to point upwards
    for (int i = 0; i <= 5; ++i) {
        float angle_deg_outer = 72.0f * i - 90.0f; // 각도 계산
        float angle_rad_outer = angle_deg_outer * M_PI / 180.0f;
        float x_outer = (x + width / 2) + (width / 2) * cos(angle_rad_outer);
        float y_outer = (y + height / 2) + (height / 2) * sin(angle_rad_outer);
        glVertex2f(x_outer, y_outer);

        if (i < 5) {
            float angle_deg_inner = angle_deg_outer + 36.0f; // 각도 계산
            float angle_rad_inner = angle_deg_inner * M_PI / 180.0f;
            float x_inner = (x + width / 2) + (width / 4) * cos(angle_rad_inner);
            float y_inner = (y + height / 2) + (height / 4) * sin(angle_rad_inner);
            glVertex2f(x_inner, y_inner);
        }
    }
    glEnd();

    glPopMatrix(); // 이전 행렬 복원
}

bool PhysicsAABB(const Object& A, const Object& B) {
    return A.IsCollidingWith(B);
}
