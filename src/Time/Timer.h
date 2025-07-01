#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <SDL2/SDL.h>

template <typename T>
bool is_close(T x, T y, T epsilon = 1e-9) {
    return std::fabs(x - y) <= epsilon;
}


struct Time {
    Uint64 gameTime = 0.0;  // �ۼƵġ���Ϸ����ʱ�䡱�����룩
    Uint64 deltaTime = 0.0; // ��һ֡�ġ���Ϸʱ�䡱���������룩
    float timeScale = 1.0f; // ʱ�����ţ�0 �� ��ͣ��1 �� ����
    bool paused = false;    // �Ƿ���ͣ��Ҳ���� timeScale == 0 �жϣ�
    Uint64 lastRealTime;    // �ڲ���ʵʱ���¼�㣨���룩

    // ��ʼ�����ڳ�������������ʱ����
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
    inline static Uint64 targetFrameTime = 1000 / 60; // Ŀ��֡ʱ�������룩

    // ÿ֡����ѭ�������
    static void update() {
        // 1. ȡ��ǰ��ʵʱ��
        auto nowReal = SDL_GetTicks64();
        // 2. ������ʵ delta
        auto realDelta = nowReal - time.lastRealTime;

        // 3. ���С��Ŀ��֡ʱ������ sleep ����
        if (realDelta < targetFrameTime) {
            auto toWait = targetFrameTime - realDelta;
            SDL_Delay(toWait);
            // ��������²���
            nowReal = SDL_GetTicks64();
            realDelta = nowReal - time.lastRealTime;
        }

        // 4. ��¼Ϊ��һ֡��׼
        time.lastRealTime = nowReal;

        // 5. ��ͣ���
        if (time.paused || time.timeScale == 0.0f) {
            time.deltaTime = 0;
            return;
        }

        // 6. clamp ��������ֹ���Զϵ㣯���ٵ��¼��� delta
        const Uint64 maxDeltaClamp = 100;
        realDelta = realDelta > maxDeltaClamp ? maxDeltaClamp : realDelta;

        // 7. ������Ϸ���������ۼӵ� gameTime
        time.deltaTime = static_cast<Uint64>(realDelta * time.timeScale);
        time.gameTime += time.deltaTime;
    }

    // ������ͣ
    static void pause() {
        time.paused = true;
        time.timeScale = 0.0f;
    }

    // ���ƻָ�
    static void resume() {
        time.paused = false;
        time.timeScale = 1.0f;
        time.lastRealTime = SDL_GetTicks64();
    }
};

#endif // !TIMER_H
