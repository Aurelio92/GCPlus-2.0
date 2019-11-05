#pragma once

#include <vector>

using namespace std;

template <class T>
class Animation {
    private:
        class Step {
            public:
                u64 t;
                T val;
        };

        u64 timer; //milliseconds
        u64 duration;
        vector<Step> steps;
        bool playing;

    public:
        Animation();
        ~Animation();

        void addStep(u64 t, T val);
};