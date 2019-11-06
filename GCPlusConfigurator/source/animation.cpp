#include <ogc/lwp_watchdog.h>
#include "animation.h"

template <class T, class interpolator>
void Animation<T, interpolator>::addStep(u64 duration, const T& start, const T& end) {
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
    s.start = start;
    s.end = end;
    steps.push_back(s);

    //Increment total duration
    totDuration += duration;
}

template <class T, class interpolator>
int Animation<T, interpolator>::findStep(u64 time) {
    if (time >= this->totDuration || steps.empty())
        return -1; //Not found

    //Binary search
    int left = 0;
    int right = steps.size() - 1;
    int center;

    while(left <= right) {
        center = (left + right - 1) / 2;
        Step s = steps[center];
        u64 tEnd = s.time + s.duration;

        if (time >= s.time && time <= tEnd) {
            return center;
        }

        //Keep only right side if our target time is greater than center's tEnd
        if (time > tEnd)
            left = center + 1;
        else //Keep only left side otherwise
            right = center - 1;
    }

    return -1; //Should never happen
}

template <class T, class interpolator>
bool Animation<T, interpolator>::animate() {
    u64 time;
    if (playing)
        time = gettime() - startTime;
    else
        time = pauseTime;

    int idx = findStep(time);
    if (idx < 0)
        return false;

    if (output != NULL)
        *output = interpolator::interpolate(steps[idx].start, steps[idx].end, time - steps[idx].time);

    return true;
}
