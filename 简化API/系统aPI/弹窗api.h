#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// ============================================================
// GuayMsgBox 宏：简化 MessageBox 图标常量
// ============================================================

// 图标
#define GB_XX   MB_ICONINFORMATION   // 信息
#define GB_JG   MB_ICONWARNING       // 警告
#define GB_CW   MB_ICONERROR         // 错误
#define GB_WH   MB_ICONQUESTION      // 问号

// 按钮
#define GB_QD   MB_OK                // 确定
#define GB_QD_QX MB_OKCANCEL         // 确定/取消
#define GB_SF   MB_YESNO             // 是/否
#define GB_SF_QX MB_YESNOCANCEL      // 是/否/取消
#define GB_CZ_QX MB_RETRYCANCEL
#endif