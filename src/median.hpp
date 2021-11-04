#pragma once

#include <queue>

class Median
{
    std::priority_queue<double, std::vector<double>, std::greater<double>> min_heap;
    std::priority_queue<double, std::vector<double>, std::less<double>> max_heap;
public:
    Median();
    virtual ~Median(){};

    void add(double);
    double get_median();
};

