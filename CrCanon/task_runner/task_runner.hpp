//
// Created by qinzhou on 23-5-8.
//

#ifndef TASK_RUNNER_HPP
#define TASK_RUNNER_HPP

#include <vector>
#include "component/sync_queue.h"
#include "thread.hpp"
#include "task.hpp"

const int THREAD_POOL_MAX_SIZE = 100;

class TaskRunner {
public:
	explicit TaskRunner() 
	{
		runners.reserve(THREAD_POOL_MAX_SIZE);
		for (int i = 0; i < THREAD_POOL_MAX_SIZE; i++) {
			runners.push_back(new BaseRunner(this));
		}
	}

	~TaskRunner()
	{
		for (int i = 0; i < THREAD_POOL_MAX_SIZE; i++) {
			runners[i]->stop();
			runners[i]->join();
			delete runners[i];
			runners[i] = nullptr;
		}
	}

	void AddTask(const Task& task)
	{
		if (taskQueue.Size() >= THREAD_POOL_MAX_SIZE) {
			// LOGE("task queue's size = %zu", taskQueue.Size());
		}
		taskQueue.Put(task);
	}

	void Start()
	{
		for (int i = 0; i < THREAD_POOL_MAX_SIZE; i++) {
			runners[i]->start(); // use base class
		}
	}


private:
	class BaseRunner : public Thread
	{
	public:
		explicit BaseRunner(TaskRunner* run): runner(run) {}
	protected:
		void run() override {
			Task t;
			runner->taskQueue.Take(t); // get a task from queue
			while(loop()) {
				t();
			}
		}
	private:
		TaskRunner* runner {nullptr};
	};

private:
	SyncQueue<Task, THREAD_POOL_MAX_SIZE> taskQueue;
	std::vector<BaseRunner*> runners;
};

#endif //TASK_RUNNER_HPP
