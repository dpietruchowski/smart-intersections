#ifndef EXECUTIONTIME_H
#define EXECUTIONTIME_H

#include <functional>

long countExecutionTime(const std::function<void()>& function);

#endif // EXECUTIONTIME_H
