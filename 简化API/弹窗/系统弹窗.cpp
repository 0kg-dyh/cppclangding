#include "系统弹窗.h"
#include <windows.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER


// ========== 全局状态 ==========
static HWND g_hEdit = nullptr;
static HWND g_hLabel = nullptr;
static HWND g_hOkBtn = nullptr;
static HWND g_hCancelBtn = nullptr;
static std::string g_result;
static bool g_ok = false;

// ========== 宽字符转换 ==========
static std::wstring ToWide(const std::string& s) {
    if (s.empty()) return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    std::wstring ws(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &ws[0], len);
    if (!ws.empty() && ws.back() == L'\0') ws.pop_back();
    return ws;
}

// ========== 窗口过程 ==========
LRESULT CALLBACK InputBoxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        // 标签
        g_hLabel = CreateWindowW(L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            15, 15, 360, 20, hwnd, nullptr, nullptr, nullptr);

        // 输入框
        g_hEdit = CreateWindowW(L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            15, 45, 360, 25, hwnd, nullptr, nullptr, nullptr);

        // 确定按钮
        g_hOkBtn = CreateWindowW(L"BUTTON", L"确定",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, 85, 75, 28, hwnd, (HMENU)IDOK, nullptr, nullptr);

        // 取消按钮
        g_hCancelBtn = CreateWindowW(L"BUTTON", L"取消",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            300, 85, 75, 28, hwnd, (HMENU)IDCANCEL, nullptr, nullptr);

        return 0;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == IDOK) {
            wchar_t buf[1024] = {};
            GetWindowTextW(g_hEdit, buf, 1024);

            int len = WideCharToMultiByte(CP_UTF8, 0, buf, -1, nullptr, 0, nullptr, nullptr);
            std::string s(len, '\0');
            WideCharToMultiByte(CP_UTF8, 0, buf, -1, &s[0], len, nullptr, nullptr);
            if (!s.empty() && s.back() == '\0') s.pop_back();

            g_result = s;
            g_ok = true;
            DestroyWindow(hwnd);
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            g_ok = false;
            DestroyWindow(hwnd);
        }
        return 0;
    }
    case WM_CLOSE:
        g_ok = false;
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// ========== 实现 ==========
std::string GuayInputBox::Show(const std::string& title,
                               const std::string& prompt,
                               const std::string& defaultValue) {
    const wchar_t CLASS_NAME[] = L"GuayInputBoxClass";

    // 注册窗口类
    WNDCLASSW wc = {};
    wc.lpfnWndProc = InputBoxProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassW(&wc);

    // 窗口居中
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int winW = 400, winH = 160;
    int x = (screenW - winW) / 2;
    int y = (screenH - winH) / 2;

    // 创建窗口
    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, ToWide(title).c_str(),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        x, y, winW, winH,
        nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

    if (!hwnd) return "";

    // 设置提示和默认值
    SetWindowTextW(g_hLabel, ToWide(prompt).c_str());
    SetWindowTextW(g_hEdit, ToWide(defaultValue).c_str());
    SetFocus(g_hEdit);

    // 消息循环
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        if (!IsDialogMessageW(hwnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return g_ok ? g_result : "";
}

#else
// 非 Windows 平台：用控制台
#include <iostream>

std::string GuayInputBox::Show(const std::string& title,
                               const std::string& prompt,
                               const std::string& defaultValue) {
    std::cout << "[" << title << "] " << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}
#endif