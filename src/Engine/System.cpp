#include "Engine/System.hpp"

#include <thread>

ThreadPool SimpleParallelSystem::pool(std::thread::hardware_concurrency());
const size_t SimpleParallelSystem::nthreads = std::thread::hardware_concurrency();
