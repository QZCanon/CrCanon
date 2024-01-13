    

#include <iostream>
#include "task_runner.hpp"

using namespace std;

void Func1()
{
    std::cout << "Func1\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void Func2()
{
    std::cout << "FuncCallback2\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(800));
}

int main()
{
    Task task1, task2;
    task1.SetTaskFunc(Func1);
    task2.SetTaskFunc(Func2);

    TaskRunner runner;
    runner.AddTask(task1);
    runner.AddTask(task2);

    runner.Start();

    while(1);

    return 0;
}