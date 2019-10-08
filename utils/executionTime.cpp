#include "executionTime.h"

#include <algorithm>
#include <chrono>
#include <iostream>

long countExecutionTime(const std::function<void ()>& function)
{
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    function();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count();
}
