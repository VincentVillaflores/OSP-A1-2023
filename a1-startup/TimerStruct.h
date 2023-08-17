#ifndef TIMERSTRUCT_H
#define TIMERSTRUCT_H

#include <vector>
#include <ctime>
#include <time.h>

struct TimerStruct {
    std::vector<clock_t> readLineDuration;
    std::vector<clock_t> writeLineDuration;
};

#endif // TIMERSTRUCT_H
