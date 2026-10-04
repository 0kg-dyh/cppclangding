#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

// ============================================================
// GuayTime：简化计时（Time = 时间）
// 用法：
//   GuayTime::Start();           // 开始计时
//   GuayTime::ElapsedMs();       // 已过多少毫秒
//   GuayTime::ElapsedSec();      // 已过多少秒
// ============================================================
class GuayTime {
private:
    // 用 steady_clock（单调递增，适合测时间间隔）
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    // 全局开始时间点
    static TimePoint& StartPoint() {
        static TimePoint start = Clock::now();
        return start;
    }

public:
    // 开始计时（重置起点）
    static void Start() {
        StartPoint() = Clock::now();
    }

    // 获取当前时间点
    static TimePoint Now() {
        return Clock::now();
    }

    // 从开始到现在，过了多少毫秒
    static double ElapsedMs() {
        auto now = Clock::now();
        auto diff = now - StartPoint();
        return std::chrono::duration<double, std::milli>(diff).count();
    }

    // 从开始到现在，过了多少秒
    static double ElapsedSec() {
        auto now = Clock::now();
        auto diff = now - StartPoint();
        return std::chrono::duration<double>(diff).count();
    }

    // 从开始到现在，过了多少微秒
    static double ElapsedUs() {
        auto now = Clock::now();
        auto diff = now - StartPoint();
        return std::chrono::duration<double, std::micro>(diff).count();
    }

    // 从开始到现在，过了多少纳秒
    static double ElapsedNs() {
        auto now = Clock::now();
        auto diff = now - StartPoint();
        return std::chrono::duration<double, std::nano>(diff).count();
    }

    // 获取当前日期时间字符串
    // 格式：2026-10-03 15:30:45
    static std::string NowString() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};