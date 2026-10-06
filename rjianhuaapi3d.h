#pragma once
#include <raylib.h>
#include "raymath.h"   // raylib 3D 数学库（Vector3Distance 等）
#include <map>
#include <string>
#include <cmath>       // sqrtf
#include <unordered_map>
// ============================================================
// Guay3D：3D 简化 API
// 命名约定：
//   Guay = 关于（About），所有类的前缀
//   Sx   = 摄像机
//   Mx   = 模型
//   Jd   = 几何体
//   Dr   = 敌人
// ============================================================

// ============================================================
// Guaywindow：窗口
// ============================================================
class Guaywindow {
public:
    // 创建窗口：宽, 高, 标题
    static void Cjwindow(int w, int h, const char* t) {
        InitWindow(w, h, t);
    }
    // 窗口是否还开着（true = 继续，false = 该退出了）
    static bool Sfkqwindow() {
        return !WindowShouldClose();
    }
    // 关闭窗口
    static void Gbwindow() {
        CloseWindow();
    }
};

// ============================================================
// Guayfps：帧率
// ============================================================
class Guayfps {
public:
    // 设置目标帧率
    static void Setfps(int sud) {
        SetTargetFPS(sud);
    }
};

// ============================================================
// Guayxr：渲染
// ============================================================
class Guayxr {
public:
    // 清屏（渲染窗口背景）
    static void Xrwindow(Color color) {
        ClearBackground(color);
    }
    // 开始绘制一帧
    static void Begin() { BeginDrawing(); }
    // 结束绘制一帧
    static void End() { EndDrawing(); }
};

// ============================================================
// Guaysx3D：3D 摄像机（Sx = 摄像机）
// ============================================================
class Guaysx3D {
public:
    // 创建透视摄像机：位置, 看向, 视野角度
    static Camera3D Cjsx(Vector3 position, Vector3 target, float fovy = 45.0f) {
        Camera3D cam = { 0 };
        cam.position = position;        // 摄像机在哪
        cam.target = target;            // 看向哪
        cam.up = { 0, 1, 0 };           // 上方方向（通常是 Y 轴向上）
        cam.fovy = fovy;                // 视野角度，越大看得越多
        cam.projection = CAMERA_PERSPECTIVE;   // 透视投影
        return cam;
    }

    // 创建正交摄像机：位置, 看向, 视野角度
    // 正交投影：远近一样大，适合 2.5D 或 UI
    static Camera3D CjsxZJ(Vector3 position, Vector3 target, float fovy = 45.0f) {
        Camera3D cam = { 0 };
        cam.position = position;
        cam.target = target;
        cam.up = { 0, 1, 0 };
        cam.fovy = fovy;
        cam.projection = CAMERA_ORTHOGRAPHIC;   // 正交投影
        return cam;
    }

    // 摄像机控制：摄像机, 模式
    // 模式：CAMERA_FREE（自由飞）、CAMERA_ORBITAL（绕目标转）、
    //       CAMERA_FIRST_PERSON（第一人称）、CAMERA_THIRD_PERSON（第三人称）
    static void Kongzhi(Camera3D& camera, int mode = CAMERA_ORBITAL) {
        UpdateCamera(&camera, mode);
    }

    // 开始 3D 绘制（之后的绘制按世界坐标）
    static void Begin(Camera3D& camera) {
        BeginMode3D(camera);
    }

    // 结束 3D 绘制
    static void End() {
        EndMode3D();
    }

    // 摄像机看向目标
    static void Kanxiang(Camera3D& camera, Vector3 target) {
        camera.target = target;
    }

    // 设置摄像机位置
    static void Weizhi(Camera3D& camera, Vector3 position) {
        camera.position = position;
    }
};

// ============================================================
// Guayjd：几何体（Jd = 几何）
// 所有函数都要在 Guaysx3D::Begin / End 之间调用
// ============================================================
class Guayjd {
public:
    // 立方体：中心, 宽, 高, 深, 颜色
    static void Lifang(Vector3 pos, float w, float h, float d, Color color) {
        DrawCube(pos, w, h, d, color);
    }

    // 立方体线框：中心, 宽, 高, 深, 颜色
    static void LifangXian(Vector3 pos, float w, float h, float d, Color color) {
        DrawCubeWires(pos, w, h, d, color);
    }

    // 球：中心, 半径, 颜色
    static void Qiu(Vector3 pos, float r, Color color) {
        DrawSphere(pos, r, color);
    }

    // 球线框：中心, 半径, 颜色
    static void QiuXian(Vector3 pos, float r, Color color) {
        DrawSphereWires(pos, r, 8, 8, color);
    }

    // 圆柱：底心, 顶半径, 底半径, 高, 边数, 颜色
    static void Yuanzhu(Vector3 pos, float topR, float bottomR, float h, int sides, Color color) {
        DrawCylinder(pos, topR, bottomR, h, sides, color);
    }

    // 圆锥：底心, 底半径, 高, 边数, 颜色
    // 内部调用 DrawCylinder，顶半径为 0
    static void Yuanzhui(Vector3 pos, float r, float h, int sides, Color color) {
        DrawCylinder(pos, 0.0f, r, h, sides, color);
    }

    // 网格地面：格子数, 每格间距
    static void Wangge(int slices, float spacing) {
        DrawGrid(slices, spacing);
    }

    // 线：起点, 终点, 颜色
    static void Xian(Vector3 start, Vector3 end, Color color) {
        DrawLine3D(start, end, color);
    }

    // 点：位置, 颜色
    static void Dian(Vector3 pos, Color color) {
        DrawPoint3D(pos, color);
    }
};

// ============================================================
// Guaymx：模型（Mx = 模型）
// 支持 .obj / .gltf / .glb / .iqm / .vox
// 内部带缓存，同一路径只加载一次
// ============================================================
class Guaymx {
private:
    // 缓存：路径 → 模型
    static std::map<std::string, Model>& GetCache() {
        static std::map<std::string, Model> cache;
        return cache;
    }

public:
    // 加载模型：路径
    // 第一次调用会加载，之后从缓存取
    static Model Jz(const char* path) {
        auto& cache = GetCache();
        std::string key(path);

        auto it = cache.find(key);
        if (it == cache.end()) {
            Model m = LoadModel(path);
            if (m.meshCount == 0) {
                // 加载失败，存空模型
                cache[key] = Model{ 0 };
                return cache[key];
            }
            cache[key] = m;
            it = cache.find(key);
        }
        return it->second;
    }

    // 绘制模型：模型, 位置, 缩放, 颜色
    static void Hz(Model model, Vector3 pos, float scale, Color color) {
        if (model.meshCount == 0) return;   // 空模型，跳过
        DrawModel(model, pos, scale, color);
    }

    // 绘制模型（带旋转）：模型, 位置, 旋转轴, 角度, 缩放, 颜色
    static void HzXz(Model model, Vector3 pos, Vector3 axis, float angle,
                     float scale, Color color) {
        if (model.meshCount == 0) return;
        DrawModelEx(model, pos, axis, angle, { scale, scale, scale }, color);
    }

    // 绘制模型线框：模型, 位置, 缩放, 颜色
    static void HzXian(Model model, Vector3 pos, float scale, Color color) {
        if (model.meshCount == 0) return;
        DrawModelWires(model, pos, scale, color);
    }

    // 释放所有缓存的模型
    // 程序结束前调用
    static void SfMx() {
        for (auto& pair : GetCache()) {
            if (pair.second.meshCount > 0) {
                UnloadModel(pair.second);
            }
        }
        GetCache().clear();
    }
};

// ============================================================
// Guay3D：3D 辅助（碰撞、距离）
// ============================================================
class Guay3D {
public:
    // 两点距离
    static float Juli(Vector3 a, Vector3 b) {
        return Vector3Distance(a, b);
    }

    // 球碰撞：中心1, 半径1, 中心2, 半径2
    // 两个球相交返回 true
    static bool QiuPeng(Vector3 c1, float r1, Vector3 c2, float r2) {
        return CheckCollisionSpheres(c1, r1, c2, r2);
    }

    // 包围盒碰撞：中心1, 尺寸1, 中心2, 尺寸2
    // 两个盒子相交返回 true
    static bool HePeng(Vector3 c1, Vector3 s1, Vector3 c2, Vector3 s2) {
        // 把“中心 + 尺寸”转成“最小点 + 最大点”
        BoundingBox b1 = {
            { c1.x - s1.x / 2, c1.y - s1.y / 2, c1.z - s1.z / 2 },
            { c1.x + s1.x / 2, c1.y + s1.y / 2, c1.z + s1.z / 2 }
        };
        BoundingBox b2 = {
            { c2.x - s2.x / 2, c2.y - s2.y / 2, c2.z - s2.z / 2 },
            { c2.x + s2.x / 2, c2.y + s2.y / 2, c2.z + s2.z / 2 }
        };
        return CheckCollisionBoxes(b1, b2);
    }
};

// ============================================================
// Guaydr：敌人/寻敌（Dr = 敌人）
// ============================================================
class Guaydr {
public:
    // 朝目标移动：当前位置（引用）, 目标位置, 速度, dt
    // 会直接修改 pos，让它朝 target 移动
    static void Xiang(Vector3& pos, Vector3 target, float speed, float dt) {
        // 计算方向：目标 - 当前位置
        Vector3 dir = {
            target.x - pos.x,
            target.y - pos.y,
            target.z - pos.z
        };

        // 归一化（让方向长度为 1，这样速度不受距离影响）
        float len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        if (len > 0.001f) {
            dir.x /= len;
            dir.y /= len;
            dir.z /= len;
        }

        // 按速度和帧时间移动
        pos.x += dir.x * speed * dt;
        pos.y += dir.y * speed * dt;
        pos.z += dir.z * speed * dt;
    }
};
// ============================================================
// GuayUI3D：3D 进度条（直接画在世界里）
// Jdt = 进度条
// ============================================================
class GuayUI3D {
private:
    // 每个目标头顶已画的进度条数量
    static std::unordered_map<const void*, int>& GetStack() {
        static std::unordered_map<const void*, int> stack;
        return stack;
    }

    static double& LastTime() {
        static double t = -1.0;
        return t;
    }

public:
    // 进度条：百分比, 宽, 高, 位置, 颜色, 层叠顺序
    //   percent : 0 ~ 100
    //   w, h    : 宽、高（世界单位）
    //   pos     : 3D 位置（通常传角色位置）
    //   color   : 颜色，默认绿色
    //   layer   : 层叠顺序，默认 0（自动叠）
    static void Jdt(float percent, float w, float h,
                    Vector3 pos,
                    Color color = GREEN,
                    int layer = 0) {
        // 自动判断新帧
        double currentTime = GetTime();
        if (currentTime != LastTime()) {
            GetStack().clear();
            LastTime() = currentTime;
        }

        if (percent < 0.0f) percent = 0.0f;
        if (percent > 100.0f) percent = 100.0f;
        float progress = percent / 100.0f;

        // 用坐标算 key
        const void* key = (const void*)(intptr_t)(pos.x * 10000 + pos.z * 100);
        auto& stack = GetStack();
        int& count = stack[key];

        int actualLayer = (layer > 0) ? layer : count;
        float offsetY = (h + 0.1f) * (actualLayer + 1);

        Vector3 barPos = { pos.x, pos.y + offsetY+3, pos.z };

        // 背景
        Guayjd::Lifang(barPos, w, h, 0.1f, LIGHTGRAY);

        // 前景，宽度按进度
        Vector3 fillPos = {
            barPos.x - w / 2 + w * progress / 2,
            barPos.y,
            barPos.z
        };
        Guayjd::Lifang(fillPos, w * progress, h, 0.1f, color);

        count++;
    }
};
class GuayUI2d {
private:
    static std::unordered_map<const void*, int>& GetStack() {
        static std::unordered_map<const void*, int> stack;
        return stack;
    }

    static double& LastTime() {
        static double t = -1.0;
        return t;
    }

    static void DrawBar(float progress, float w, float h, float x, float y, Color color) {
        DrawRectangle((int)x, (int)y, (int)w, (int)h, LIGHTGRAY);
        DrawRectangle((int)x, (int)y, (int)(w * progress), (int)h, color);
        DrawRectangleLines((int)x, (int)y, (int)w, (int)h, DARKGRAY);
    }

public:
    // 进度条：百分比, 宽, 高, x, y, 是否用于角色, 层叠顺序, 文字, 颜色
    static void Jdt2d(float percent, float w, float h,
                    float x, float y,
                    bool forCharacter = false,
                    int layer = 0,
                    const char* text = "",
                    Color color = GREEN) {
        double currentTime = GetTime();
        if (currentTime != LastTime()) {
            GetStack().clear();
            LastTime() = currentTime;
        }

        if (percent < 0.0f) percent = 0.0f;
        if (percent > 100.0f) percent = 100.0f;
        float progress = percent / 100.0f;

        float actualX, actualY;
        if (forCharacter) {
            const void* key = (const void*)(intptr_t)(x * 10000 + y);
            auto& stack = GetStack();
            int& count = stack[key];

            int actualLayer = (layer > 0) ? layer : count;
            float offsetY = -(h + 2) * (actualLayer + 1);

            actualX = x + 50 - w / 2;
            actualY = y + offsetY;

            count++;
        } else {
            actualX = x;
            actualY = y;
        }

        // 画进度条
        DrawBar(progress, w, h, actualX, actualY, color);

        // 如果有文字，居中画
        if (text[0] != '\0') {
            int fontSize = (int)h - 2;
            if (fontSize < 8) fontSize = 8;
            int textW = MeasureText(text, fontSize);
            DrawText(text,
                     (int)(actualX + (w - textW) / 2),
                     (int)(actualY + (h - fontSize) / 2),
                     fontSize, BLACK);
        }
    }
};
class GuayMouse {
public:
    // 鼠标是否碰到角色（世界坐标）
    // 参数：角色x, y, 宽, 高, 摄像机
    static bool OnChar(float x, float y, float w, float h, Camera2D& camera) {
        Vector2 world = GetScreenToWorld2D(GetMousePosition(), camera);
        return CheckCollisionPointRec(world, { x, y, w, h });
    }

    // 鼠标是否点击了角色
    static bool ClickChar(float x, float y, float w, float h, Camera2D& camera) {
        return OnChar(x, y, w, h, camera)
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }
};