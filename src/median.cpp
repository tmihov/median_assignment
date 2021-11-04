#include "median.hpp"

#include <spdlog/spdlog.h>

Median::Median()
{
    spdlog::info("Median()");
}

void Median::add(double n)
{
    spdlog::info("add({})", n);
    
    if(max_heap.size() > 0 &&
       n < max_heap.top())
    {
        max_heap.push(n);
    }
    else
    {
        min_heap.push(n);
    }

    size_t max_s = max_heap.size();
    size_t min_s = min_heap.size();

    if(min_s > max_s && min_s - max_s > 1)
    {
        max_heap.push(min_heap.top());
        min_heap.pop();
    }
    else if(max_s > min_s && max_s - min_s > 1)
    {
        min_heap.push(max_heap.top());
        max_heap.pop();
    }
}

double Median::get_median()
{
    spdlog::info("get_median()");

    if (min_heap.size() == 0 && max_heap.size() == 0)
    {
        return 0;
    }

    if (min_heap.size() == max_heap.size())
    {
        return (min_heap.top() + max_heap.top()) / 2;
    }
    else if (min_heap.size() > max_heap.size())
    {
        return min_heap.top();
    }
    else
    {
        return max_heap.top();
    }
}
