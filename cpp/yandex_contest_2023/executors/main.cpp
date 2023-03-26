#include <iostream>
#include <string>
#include <vector>
#include <charconv>
#include <numeric>
#include <array>
#include <iomanip>
#include <queue>
#include <set>

struct Executor {
    size_t num = 0;
    size_t cur_task_start = 0;
    size_t cur_task_duration = 0;
};

struct Task {
    uint32_t num = 0;
    uint32_t add_time = 0;
    uint64_t duration = 0;
    uint32_t queue_num = 0;
    uint32_t executor_num = 0;
    uint64_t start_time = 0;
};

int main() 
{
    uint32_t tasks_count = 0;
    uint32_t executor_count = 0;
    uint32_t queue_count = 0;
    std::cin >> tasks_count;
    std::cin >> executor_count;
    std::cin >> queue_count;

    std::cout << "tasks: " << tasks_count << "; executors:" << executor_count << "; queues: " << queue_count << std::endl;

    std::vector<Task> tasks_list(tasks_count);

    auto task_cmp = [](Task* a, Task* b) {
        if (a->add_time != b->add_time)
            return a->add_time < b->add_time;
        return a->queue_num < b->queue_num; 
    };
    std::set<Task*, decltype(task_cmp)> tasks_in_exec_order;

    for (uint32_t i = 0; i < tasks_count; ++i) {
        auto& task = tasks_list[i];
        task.num = i;
        std::cin >> task.add_time;
        std::cin >> task.queue_num;
        std::cin >> task.duration;
        tasks_in_exec_order.emplace(&task);
        std::cout << "task_num: " << task.num << "; add_time: " << task.add_time << "; queue: " << task.queue_num << "; duration: " << task.duration << std::endl;
    }
    auto exec_cmp = [](const Executor& a, const Executor& b) {
        if (a.cur_task_start + a.cur_task_duration != b.cur_task_duration + b.cur_task_start) 
            return a.cur_task_start + a.cur_task_duration < b.cur_task_duration + b.cur_task_start;
        return a.num < b.num;
    };
    std::set<Executor, decltype(exec_cmp)> executors;
    for (size_t i = 1; i <= executor_count; ++i) {
        executors.emplace(Executor{i, 0, 0});
    }
    for (auto* task: tasks_in_exec_order) {
        Executor exec = *(executors.begin()); // copy
        executors.erase(executors.begin()); // erase old
        task->executor_num = exec.num;
        task->start_time = std::max(exec.cur_task_start + exec.cur_task_duration, size_t(task->add_time));
        exec.cur_task_start = task->start_time;
        exec.cur_task_duration = task->duration;
        executors.insert(exec); // return updated
    }
    for (auto& tsk: tasks_list) {
        std::cout << "task: " << tsk.num << "; start_time: " << tsk.start_time << "; executor_num: " << tsk.executor_num << std::endl;
    }
	return 0;
}