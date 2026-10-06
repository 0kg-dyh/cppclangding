#include <raylib.h>
#include "头文件/简化API/raylib/rjianhuaapi3d.h"
#include "raymath.h"
Vector3 diren1 = { 10,0.5f,10 };
int main() {
    // 玩家位置
    Vector3 wanjia = { 0, 0.5f, 0 };
    // 摄像机位置
    Vector3 shexiang = { 0, 5, 5 };

    // 创建摄像机
    Camera3D camera = Guaysx3D::Cjsx(shexiang, wanjia, 45.5f);

    Guaywindow::Cjwindow(1000, 1000, "3d");
    int speed = 6;

    while (Guaywindow::Sfkqwindow()) {
        float dt = GetFrameTime();

        // 玩家移动（WASD / 方向键）
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    wanjia.z -= speed * dt;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  wanjia.z += speed * dt;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  wanjia.x -= speed * dt;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) wanjia.x += speed * dt;

        // 摄像机 y 单独控制（空格上，Shift 下）
        if (IsKeyDown(KEY_SPACE))        shexiang.y += speed * dt;
        if (IsKeyDown(KEY_LEFT_SHIFT))   shexiang.y -= speed * dt;

        // 摄像机 x、z 跟着玩家
        shexiang.x = wanjia.x;
        shexiang.z = wanjia.z + 5;   // 在玩家后面 5 单位
        Guaydr::Xiang(diren1, wanjia, speed, dt);
        // 写回摄像机位置
        Guaysx3D::Weizhi(camera, shexiang);

        // 摄像机看向玩家
        camera.target = wanjia;

        // 绘制
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        Guayjd::Lifang(diren1, 1, 1, 1, RED);
        Guayjd::Qiu(wanjia, 0.5f, BLUE);
        Guayjd::Wangge(100, 2.0f);
        GuayUI2d::Jdt2d(100, 1000, 30, 500, 985, 0, 0, "HP", GREEN);



        EndMode3D();
        EndDrawing();
    }
    Guaywindow::Gbwindow();
    return 0;
}