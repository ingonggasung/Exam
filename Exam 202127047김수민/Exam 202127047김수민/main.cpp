#pragma comment(lib, "Opengl32.lib")

#define _CRTDBG_ALLOC_MEM_DF
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DBG_NEW new
#endif

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cmath>
#include <random>
#include <chrono>
#include "Object.h"
#include "Transform.h"

// 글로벌 변수 선언
Player player(-0.75f, -0.75f); // 플레이어 위치를 지면 위로 설정 (-0.75f)
Ground ground(-1.0f);  // 지면 위치 설정 (-1.0f부터 시작)
EnemyBlock obstacles[4] = {
    EnemyBlock(0.0f, 0.0f, 0.0f),
    EnemyBlock(0.0f, 0.0f, 0.0f),
    EnemyBlock(0.0f, 0.0f, 0.0f),
    EnemyBlock(0.0f, 0.0f, 0.0f)
}; // 기본 생성자 매개변수를 전달하여 배열을 초기화
Star stars[5] = {
    Star(0.2f, 0.8f, 0.05f * 800.0f),
    Star(0.5f, 0.5f, 0.05f * 800.0f),
    Star(0.6f, 0.8f, 0.05f * 800.0f),
    Star(0.8f, 0.7f, 0.05f * 800.0f),
    Star(1.0f, 0.85f, 0.05f * 800.0f)
};

float gravity = -9.8f;
float maxJumpForce = 7.0f; // 최대 점프력
float minJumpForce = 5.0f;  // 최소 점프력 설정
float jumpForce = minJumpForce; // 현재 점프력
bool isJumping = false;
bool isKeyHeld = false;
float verticalVelocity = 0.0f;
float groundHeight = -0.75f; // 지면 높이를 -0.75로 설정
float jumpDuration = 0.0f; // 점프 시간 기록
float rotationAngle = 0.0f; // 플레이어 회전 각도

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, 1);

void errorCallback(int error, const char* description) {
    printf("GLFW Error: %s", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE) {
        if (action == GLFW_PRESS && !isJumping && player.y <= groundHeight) {
            isKeyHeld = true;
            jumpDuration = 0.0f;
        }
        else if (action == GLFW_RELEASE && isKeyHeld) {
            isKeyHeld = false;
            isJumping = true;
            jumpForce = minJumpForce + (maxJumpForce - minJumpForce) * (jumpDuration / 1.0f); // 점프력 계산 (최대 1초)
            if (jumpForce > maxJumpForce) {
                jumpForce = maxJumpForce;
            }
            verticalVelocity = jumpForce;
        }
    }
}

void InitializeObstacles() {
    float xStart = 1.0f;
    for (int i = 0; i < 4; ++i) {
        // 장애물 높이 랜덤 설정 (100픽셀 또는 300픽셀)
        float height = (distribution(generator) == 0) ? 100.0f / 600.0f * 2 : 300.0f / 600.0f * 2;
        // 장애물 y 좌표 설정 (ground에 닿도록)
        float y = -0.75f;
        obstacles[i] = EnemyBlock(xStart, y, height);
        xStart += 1.6f;  // 간격 조정 (더 큰 간격으로 설정하여 속도를 줄임)
    }
}

int Physics() {
    // 점프력 계산
    if (isKeyHeld) {
        jumpDuration += 0.016f; // 16ms마다 증가
    }

    // 중력 적용
    verticalVelocity += gravity * 0.016f;  // 16ms마다 중력 적용
    player.y += verticalVelocity * 0.016f;

    // 플레이어 위치 업데이트
    if (isJumping) {
        player.SetHeight(50.0f / 800.0f * 2); // 점프 중일 때 높이 설정
        rotationAngle += 360.0f * (0.016f / (2.0f * jumpForce / -gravity)); // 점프 동안 360도 회전
    }

    if (player.y <= groundHeight) {
        player.y = groundHeight;
        player.SetHeight((50.0f / 800.0f * 2) * (800.0f / 600.0f)); // 착지 시 높이 재설정
        isJumping = false;
        verticalVelocity = 0.0f;
        rotationAngle = 0.0f; // 착지 시 회전 각도 초기화
    }

    // 장애물 이동 및 충돌 처리
    for (int i = 0; i < 4; ++i) {
        obstacles[i].Move(-0.005f, 0.0f);  // 장애물 왼쪽으로 이동 속도 감소
        if (obstacles[i].x + obstacles[i].width < -1.0f) {
            // 장애물이 화면을 벗어나면 오른쪽 끝으로 이동
            obstacles[i].x = 1.0f;
            // 장애물 높이 랜덤 설정 (100픽셀 또는 300픽셀)
            float height = (distribution(generator) == 0) ? 100.0f / 600.0f * 2 : 300.0f / 600.0f * 2;
            // 장애물 y 좌표 설정 (ground에 닿도록)
            obstacles[i].y = -0.75f;
            obstacles[i].height = height;
        }
        if (PhysicsAABB(player, obstacles[i])) {
            // 충돌 시 게임 종료 처리
            printf("Game Over!\n");
            glfwSetWindowShouldClose(glfwGetCurrentContext(), GL_TRUE);
        }
    }

    return 0;
}

int Initialize() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
    InitializeObstacles();
    return 0;
}

int Update() {
    for (int i = 0; i < 5; ++i) {
        stars[i].Move(-0.005f / 3, 0.0f);  // 별 왼쪽으로 이동 속도 감소 (장애물 속도의 1/3)
        stars[i].UpdateRotation(0.5f);  // 각도 업데이트 속도 감소
        if (stars[i].x < -1.0f) {
            stars[i].x = 1.0f;  // 별이 화면을 벗어나면 오른쪽 끝으로 이동
        }
    }

    return 0;
}

int Render() {
    // 지면 그리기
    ground.Draw();

    // 플레이어 그리기
    player.Draw(rotationAngle); // 회전 각도 포함하여 그리기

    // 장애물 그리기
    for (int i = 0; i < 4; ++i) {
        obstacles[i].Draw();
    }

    // 별 그리기
    for (int i = 0; i < 5; ++i) {
        stars[i].Draw();
    }

    return 0;
}

int main(void) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // glfw라이브러리 초기화
    if (!glfwInit())
        return -1;

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "Google Dino Run Copy Game", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(errorCallback);
    glfwSetKeyCallback(window, keyCallback);

    Initialize();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.0f, 0.1176f, 0.3922f, 1.0f);  // 하늘색 설정 (R:0, G:30, B:100)
        glClear(GL_COLOR_BUFFER_BIT);
        Physics();
        Update();
        Render();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
