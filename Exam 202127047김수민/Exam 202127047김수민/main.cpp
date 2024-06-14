#pragma comment(lib, "Opengl32.lib")

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DBG_NEW new
#endif

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Object.h"
#include "Transform.h"

// 글로벌 변수 선언
Player player(-0.75f, -0.7f); // 플레이어 위치를 지면 위로 설정 (-0.7f)
Ground ground(-1.0f);  // 지면 위치 설정 (-1.0f부터 시작)
std::vector<EnemyBlock> obstacles;

std::vector<Star> stars = {
    Star(0.2f, 0.8f, 0.05f * 800.0f),
    Star(0.4f, 0.6f, 0.05f * 800.0f),
    Star(0.6f, 0.9f, 0.05f * 800.0f),
    Star(0.8f, 0.7f, 0.05f * 800.0f),
    Star(1.0f, 0.85f, 0.05f * 800.0f)
};

float gravity = -9.8f;
float jumpForce = 5.0f;
bool isJumping = false;
float verticalVelocity = 0.0f;
float groundHeight = -0.75f; // 지면 높이를 -0.75로 설정

void errorCallback(int error, const char* description) {
    printf("GLFW Error: %s", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !isJumping) {
        verticalVelocity = jumpForce;
        isJumping = true;
    }
}

void InitializeObstacles() {
    obstacles.clear();
    float xStart = 1.0f;
    for (int i = 0; i < 4; ++i) {
        // 장애물 높이 랜덤 설정 (100픽셀 또는 300픽셀)
        float height = (rand() % 2 == 0) ? 100.0f / 600.0f * 2 : 300.0f / 600.0f * 2;
        // 장애물 y 좌표 설정 (ground에 닿도록)
        float y = -0.75f;
        obstacles.push_back(EnemyBlock(xStart, y, height));
        xStart += 0.4f;  // 간격 조정
    }
}

int Physics() {
    // 플레이어 위치 업데이트
    if (isJumping) {
        verticalVelocity += gravity * 0.016f;  // 16ms마다 중력 적용
        player.y += verticalVelocity * 0.016f;

        if (player.y <= groundHeight) {
            player.y = groundHeight;
            isJumping = false;
            verticalVelocity = 0.0f;
        }
    }

    // 장애물 이동 및 충돌 처리
    for (auto& obstacle : obstacles) {
        obstacle.Move(-0.01f, 0.0f);  // 장애물 왼쪽으로 이동
        if (obstacle.x + obstacle.width < -1.0f) {
            // 장애물이 화면을 벗어나면 오른쪽 끝으로 이동
            obstacle.x = 1.0f;
            // 장애물 높이 랜덤 설정 (100픽셀 또는 300픽셀)
            float height = (rand() % 2 == 0) ? 100.0f / 600.0f * 2 : 300.0f / 600.0f * 2;
            // 장애물 y 좌표 설정 (ground에 닿도록)
            obstacle.y = -0.75f;
            obstacle.height = height;
        }
        if (PhysicsAABB(player, obstacle)) {
            // 충돌 시 게임 종료 처리
            printf("Game Over!\n");
            glfwSetWindowShouldClose(glfwGetCurrentContext(), GL_TRUE);
        }
    }

    return 0;
}

int Initialize() {
    srand(static_cast<unsigned>(time(0)));
    InitializeObstacles();
    return 0;
}

int Update() {
    for (auto& star : stars) {
        star.Move(-0.01f / 3, 0.0f);  // 별 왼쪽으로 이동 (장애물 속도의 1/3)
        star.UpdateRotation(1.0f);  // 각도 업데이트
        if (star.x < -1.0f) {
            star.x = 1.0f;  // 별이 화면을 벗어나면 오른쪽 끝으로 이동
        }
    }

    return 0;
}

int Render() {
    // 지면 그리기
    ground.Draw();

    // 플레이어 그리기
    player.Draw();

    // 장애물 그리기
    for (const auto& obstacle : obstacles) {
        obstacle.Draw();
    }

    // 별 그리기
    for (const auto& star : stars) {
        star.Draw();
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
