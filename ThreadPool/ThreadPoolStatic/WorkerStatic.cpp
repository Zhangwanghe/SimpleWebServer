#include "WorkerStatic.h"
#include "ThreadPoolStatic.h"
#include "../Runnable.h"
using namespace std;

void WorkerStatic::run(std::shared_ptr<ThreadPoolStatic> pool) {
    while (true) {
        auto optTask = pool->getTask();
        if (!optTask) {
            continue;
        }

        optTask.value()->run();
    }
}