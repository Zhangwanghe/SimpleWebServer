#include "WorkerDynamic.h"
#include "ThreadPoolDynamic.h"
#include "../Runnable.h"
using namespace std;

void WorkerDynamic::run(std::shared_ptr<ThreadPoolDynamic> pool) {
    while (true) {
        auto optTask = pool->getTask();
        if (!optTask) {
            break;
        }

        optTask.value()->run();
    }

    pool->removeWorker(this_thread::get_id());
}