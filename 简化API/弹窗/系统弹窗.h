#pragma once
#include <string>

namespace GuayInputBox {
    // 弹出输入框，返回用户输入的内容
    // 用户取消时返回空字符串
    std::string Show(const std::string& title = "输入",
                     const std::string& prompt = "请输入：",
                     const std::string& defaultValue = "");
}