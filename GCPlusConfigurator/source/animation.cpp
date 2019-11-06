#include "animation.h"

Animation::Animation() {
    timer = 0;
    totDuration = 0;
    playing = false;
}

Animation::~Animation() {
    steps.clear();
}

void Animation::addStep(u64 duration, T val) {
    Step s;
    if (steps.empty()) {
        //steps vector is empty. Start from zero
        s.time = 0;
    } else {
        //steps vector is not empty. Compute start time for this step
        Step back = steps.back();
        s.time = back.time + back.duration;
    }
    s.duration = duration;
    s.val = val;
    steps.push_back(s);

    //Increment total duration
    totDuration += duration;
}

size_t Animation::findStep(u64 time) {
    if (time >= duration)
        return 0;

    size_t left = 0;
    size_t right = steps.size() - 1;
    size_t center;

    while(left < right) {
        center = (right - left) / 2;
        s = steps[center];

    }
}
