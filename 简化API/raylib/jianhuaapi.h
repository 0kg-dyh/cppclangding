#pragma once
#include <raylib.h>
#include<map>
#include<string>
#define KEY_SHANG KEY_UP
#define KEY_XIA KEY_DOWN
#define KEY_ZUO KEY_LEFT
#define KEY_YOU KEY_RIGHT
class Guaywindow {
public:
    static void Cjwindow(int w, int h, const char* t) {
        InitWindow(w, h, t);
    }
    static bool Sfkqwindow() {
        return !WindowShouldClose();
    }
    static void Gbwindow() {
        CloseWindow();
    }
};

class Guayxr {
public:
    static void Xrwindow(Color color) {
        ClearBackground(color);
    }
};

class Guayfps {
public:
    static void Setfps(int sud) {
        SetTargetFPS(sud);
    }
};
namespace Guaytp {

    // 内部缓存
    inline std::map<std::string, Texture2D>& GetCache() {
        static std::map<std::string, Texture2D> cache;
        return cache;
    }

    // 显示图片：路径, x, y, 宽, 高, 是否翻转, 透明度
    inline void Xstp(const char* path, float x, float y, float w, float h,
                     bool flip = false, float alpha = 1.0f) {
        auto& cache = GetCache();
        std::string key(path);

        auto it = cache.find(key);
        if (it == cache.end()) {
            Texture2D tex = LoadTexture(path);
            if (tex.id == 0) {
                cache[key] = Texture2D{ 0, 0, 0, 0, 0 };
                return;
            }
            cache[key] = tex;
            it = cache.find(key);
        }

        Texture2D tex = it->second;
        if (tex.id == 0) return;

        // 透明度（0.0 ~ 1.0 → 0 ~ 255）
        unsigned char a = (unsigned char)(alpha * 255);
        Color tint = { 255, 255, 255, a };

        Rectangle src, dst;

        if (flip) {
            src = { (float)tex.width, 0, -(float)tex.width, (float)tex.height };
            dst = { x + w, y, w, h };
        } else {
            src = { 0, 0, (float)tex.width, (float)tex.height };
            dst = { x, y, w, h };
        }

        DrawTexturePro(tex, src, dst, { 0, 0 }, 0.0f, tint);
    }

    // 释放
    inline void SfTp() {
        for (auto& pair : GetCache()) {
            if (pair.second.id != 0) {
                UnloadTexture(pair.second);
            }
        }
        GetCache().clear();
    }
};
// ============================================================
// Guaysx：摄像机（Sx = 摄像机）
// ============================================================
class Guaysx {
public:
    // 创建摄像机：看向目标点，画在屏幕中心
    static Camera2D Cjsx(Vector2 target, float zoom = 1.0f) {
        Camera2D cam = { 0 };
        cam.target = target;
        cam.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        cam.rotation = 0.0f;
        cam.zoom = zoom;
        return cam;
    }

    // 平滑跟随：camera, 目标 x, 目标 y, 平滑系数
    static void Gensui(Camera2D& camera, float x, float y, float lerp = 0.1f) {
        camera.target.x += (x - camera.target.x) * lerp;
        camera.target.y += (y - camera.target.y) * lerp;
    }

    // 直接跟随（无平滑）
    static void GensuiZhi(Camera2D& camera, float x, float y) {
        camera.target = { x, y };
    }

    // 开始用摄像机画（世界坐标）
    static void Begin(Camera2D& camera) {
        BeginMode2D(camera);
    }

    // 结束摄像机绘制
    static void End() {
        EndMode2D();
    }

    // 设置缩放
    static void SetZoom(Camera2D& camera, float zoom) {
        camera.zoom = zoom;
    }
};
class GuayKey {
public:
    // 按键是否按住（持续检测）
    static bool Down(int key) {
        return IsKeyDown(key);
    }

    // 按键是否刚按下（只触发一次）
    static bool Pressed(int key) {
        return IsKeyPressed(key);
    }

    // 按键是否刚松开（只触发一次）
    static bool Up(int key) {
        return IsKeyReleased(key);
    }

    // 按键是否没被按
    static bool NotDown(int key) {
        return IsKeyUp(key);
    }

    // 是否有任意键按下
    static bool Any() {
        return GetKeyPressed() != 0;
    }

    // 获取刚按下的键（没有返回 0）
    static int GetPressed() {
        return GetKeyPressed();
    }

    // 获取刚输入的字符（用于文本输入）
    static int GetChar() {
        return GetCharPressed();
    }
};
class Guaybg {
private:
    std::map<std::string, Texture2D> cache;

    Guaybg() = default;
    Guaybg(const Guaybg&) = delete;
    Guaybg& operator=(const Guaybg&) = delete;

    static Guaybg& Get() {
        static Guaybg instance;
        return instance;
    }

public:
    ~Guaybg() {
        for (auto& pair : cache) {
            if (pair.second.id != 0) {
                UnloadTexture(pair.second);
            }
        }
        cache.clear();
    }

    // 贴瓷砖背景：图片路径, 摄像机
    static void Tcbj(const char* path, Camera2D& camera) {
        auto& self = Get();
        std::string key(path);

        // 查缓存
        auto it = self.cache.find(key);
        if (it == self.cache.end()) {
            Texture2D tex = LoadTexture(path);
            if (tex.id == 0) {
                self.cache[key] = Texture2D{ 0, 0, 0, 0, 0 };
                return;
            }
            self.cache[key] = tex;
            it = self.cache.find(key);
        }

        Texture2D tex = it->second;
        if (tex.id == 0) return;

        // 自动获取瓦片大小 = 图片宽度
        int tileSize = tex.width;

        // 自动获取窗口大小
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        float zoom = camera.zoom;
        float camLeft   = camera.target.x - screenW / (2.0f * zoom);
        float camTop    = camera.target.y - screenH / (2.0f * zoom);
        float camRight  = camera.target.x + screenW / (2.0f * zoom);
        float camBottom = camera.target.y + screenH / (2.0f * zoom);

        int startX = (int)(camLeft / tileSize) - 1;
        int startY = (int)(camTop  / tileSize) - 1;
        int endX   = (int)(camRight  / tileSize) + 1;
        int endY   = (int)(camBottom / tileSize) + 1;

        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                DrawTexture(tex, x * tileSize, y * tileSize, WHITE);
            }
        }
    }
};