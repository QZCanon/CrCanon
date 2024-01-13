

#include <iostream>
#include "timer.hpp"

using namespace std;
using namespace Canon::Timer;

int main()
{

	Timer t;

	for (int i = 0; i < 1000; i++) ;

	int64_t time = t.elapsed_nano();

	std::cout << time << std::endl;

	t.reset();

	std::cout << t.elapsed_nano() << std::endl;

	return 0;
}