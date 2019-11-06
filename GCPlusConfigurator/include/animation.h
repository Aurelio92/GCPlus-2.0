#pragma once

#include <vector>

using namespace std;

template <class T>
class Animation {
    private:
        class Step {
            public:
                u64 time;
                u64 duration;
                T val;
        };

        u64 timer; //milliseconds
        u64 totDuration;
        vector<Step> steps;
        bool playing;

        void findStep(u64 time);
    public:
        Animation();
        ~Animation();

        void addStep(u64 duration, T val);
};
