#pragma once
#include <GLFW/glfw3.h>
#include "Transform.h"

// Base class
class Object {
public:
    float x, y, width, height;
    float r, g, b;

    Object(float x, float y, float width, float height, float r, float g, float b);

    virtual void Draw() const;
    virtual void OnCollisionEnter(Object& other);

    void Move(Transform::Vector meter);
    void Rotate(float angle_degree);
    void Scale(Transform::Vector meter);

    bool IsCollidingWith(const Object& other) const;
};

// Derived classes
class Player : public Object {
public:
    Player(float x, float y);
    void Draw() const override;
    void OnCollisionEnter(Object& other) override;
};

class EnemyBlock : public Object {
public:
    EnemyBlock(float x, float y, float height);
    void OnCollisionEnter(Object& other) override;
};

class Ground : public Object {
public:
    Ground(float y);
    void OnCollisionEnter(Object& other) override;
};

class Star : public Object {
public:
    float angle; // 각도 상태 추가
    Star(float x, float y, float size);
    void Draw() const override;
    void OnCollisionEnter(Object& other) override;
    void UpdateRotation(float deltaAngle); // 각도 업데이트 메서드 추가
};

int PhysicsAABB(const Object& A, const Object& B);
