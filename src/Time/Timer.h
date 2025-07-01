#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <SDL2/SDL.h>

template <typename T>
bool is_close(T x, T y, T epsilon = 1e-9) {
    return std::fabs(x - y) <= epsilon;
}


struct Time {
    Uint64 gameTime = 0.0;  // 累计的“游戏世界时间”（毫秒）
    Uint64 deltaTime = 0.0; // 上一帧的“游戏时间”增量（毫秒）
    float timeScale = 1.0f; // 时间缩放：0 → 暂停，1 → 正常
    bool paused = false;    // 是否暂停（也可由 timeScale == 0 判断）
    Uint64 lastRealTime;    // 内部真实时间记录点（毫秒）

    // 初始化：在程序启动或重置时调用
    void reset() {
        lastRealTime = SDL_GetTicks64();
        gameTime = 0.0;
        deltaTime = 0.0;
        paused = false;
        timeScale = 1.0f;
    }

    Time()
    {
        reset();
    }
};

struct Timer {
    inline static Time time;
    inline static Uint64 targetFrameTime = 1000 / 60; // 目标帧时长（毫秒）

    // 每帧在主循环里调用
    static void update() {
        // 1. 取当前真实时间
        auto nowReal = SDL_GetTicks64();
        // 2. 计算真实 delta
        auto realDelta = nowReal - time.lastRealTime;

        // 3. 如果小于目标帧时长，就 sleep 补足
        if (realDelta < targetFrameTime) {
            auto toWait = targetFrameTime - realDelta;
            SDL_Delay(toWait);
            // 补足后重新采样
            nowReal = SDL_GetTicks64();
            realDelta = nowReal - time.lastRealTime;
        }

        // 4. 记录为下一帧基准
        time.lastRealTime = nowReal;

        // 5. 暂停检测
        if (time.paused || time.timeScale == 0.0f) {
            time.deltaTime = 0;
            return;
        }

        // 6. clamp 大跳，防止调试断点／卡顿导致极大 delta
        const Uint64 maxDeltaClamp = 100;
        realDelta = realDelta > maxDeltaClamp ? maxDeltaClamp : realDelta;

        // 7. 计算游戏增量，并累加到 gameTime
        time.deltaTime = static_cast<Uint64>(realDelta * time.timeScale);
        time.gameTime += time.deltaTime;
    }

    // 控制暂停
    static void pause() {
        time.paused = true;
        time.timeScale = 0.0f;
    }

    // 控制恢复
    static void resume() {
        time.paused = false;
        time.timeScale = 1.0f;
        time.lastRealTime = SDL_GetTicks64();
    }
};

#endif // !TIMER_H
