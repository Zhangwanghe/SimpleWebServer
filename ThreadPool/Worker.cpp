#include "Worker.h"
#include "ThreadPool.h"
using namespace std;

void Worker::run(ThreadPool* pool) {
    while (true) {
        auto optTask = pool->getTask();
        if (!optTask) {
            break;
        }

        optTask.value()();
    }

    pool->removeWorker(this_thread::get_id());
}