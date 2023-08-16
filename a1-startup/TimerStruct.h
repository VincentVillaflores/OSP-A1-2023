#ifndef TIMERSTRUCT_H
#define TIMERSTRUCT_H

#include <vector>
#include <ctime>

struct TimerStruct {
    std::vector<clock_t> readLineStart;
    std::vector<clock_t> readLineEnd;
    std::vector<clock_t> readLineDuration;
    std::vector<clock_t> writeLineStart;
    std::vector<clock_t> writeLineEnd;
    std::vector<clock_t> writeLineDuration;
};

#endif // TIMERSTRUCT_H
