#ifndef TASK_H
#define TASK_H

#include <iostream>
#include "functional"

using TaskFunc = std::function<void()>;

class Task {
public:
	void SetTaskFunc(TaskFunc func) {
		mTaskCallback = std::move(func);
	}

    void operator()() { // 通过重载（）执行回调函数
		if (mTaskCallback) {
			mTaskCallback();
		}
	}

private:
	TaskFunc mTaskCallback;
};


#endif