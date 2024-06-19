#pragma once
#include <GLFW/glfw3.h>

// Base class
class Object {
public:
    float x, y, width, height;
    float r, g, b;

    Object(float x, float y, float width, float height, float r, float g, float b);

    virtual void Draw() const;
    virtual void OnCollisionEnter(Object& other);

    void Move(float dx, float dy);
    void Rotate(float angle_degree);
    void Scale(float sx, float sy);

    bool IsCollidingWith(const Object& other) const;
};

// Derived classes
class Player : public Object {
public:
    Player(float x, float y);
    void Draw(float rotationAngle) const; // 회전 각도 인자 추가
    void OnCollisionEnter(Object& other) override;
    void SetHeight(float newHeight); // 추가된 메서드
};

class EnemyBlock : public Object {
public:
    EnemyBlock(); // 기본 생성자 추가
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

bool PhysicsAABB(const Object& A, const Object& B);
