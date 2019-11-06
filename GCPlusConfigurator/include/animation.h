#pragma once

#include <vector>
#include <gccore.h>

using namespace std;

template <class T>
class IntInterpolator {
    public:
        T interpolate(T start, T end, float t) {
            float temp = start + t * (end - start);
            return (T)temp;
        }
};

template <class T = int, class interpolator = IntInterpolator<T> >
class Animation {
    private:
        class Step {
            public:
                u64 time;
                u64 duration;
                T start;
                T end;
        };

        u64 startTime; //ticks
        u64 pauseTime;
        u64 totDuration;
        vector<Step> steps;
        bool playing;
        T* output;

        int findStep(u64 time);
    public:
        Animation() {
            startTime = 0;
            totDuration = 0;
            playing = false;
            output = NULL;
        }

        ~Animation() {
            steps.clear();
        }

        void addStep(u64 duration, const T& start, const T& end);

        void setOutput(T* _output) {
            output = _output;
        }

        bool animate();

        void pause() {
            pauseTime = gettime() - startTime;
            playing = false;
        }

        void resume() {
            startTime = gettime() - (pauseTime - startTime);
            playing = true;
        }

        void stop() {
            startTime = 0;
            playing = false;
        }
};
