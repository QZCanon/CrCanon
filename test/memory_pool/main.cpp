//
// Created by qinzhou on 23-4-21.
//

// #include <iostream>
// #include <gtest/gtest.h>
#include "memory_pool/memory_pool_manager.h"
//#include "log/Log.h"

using namespace std;
// using namespace testing;
using namespace Canon::MemoryPool;

class A {
public:
	A() {
		a = 10;
		arr[2] = 1;
		arr[3] = 12;
	}
	~A() {
		cout << "对象析构\n";
	}
	int a = 0;
	int arr[20] = {0};
};

// TEST(TestMemoryPool, pool)
void test()
{
	{
		auto pool = MemoryManager::GetInstance()->GetMemoryPool<A>();
		auto obj = pool->CreateObjectSPtr();	
		auto obj1 = pool->CreateObjectSPtr();
		auto obj2 = pool->CreateObjectSPtr();
		auto obj3 = pool->CreateObjectSPtr();
		auto obj4 = pool->CreateObjectSPtr();
		auto obj5 = pool->CreateObjectSPtr();
		std::cout << obj->arr[2] << std::endl;
		std::cout << obj->arr[3] << std::endl;
		// EXPECT_NE(obj, nullptr);
		// EXPECT_EQ(obj->arr[2], 1);
		// EXPECT_EQ(obj->arr[3], 12);
	}

	{
		auto pool = MemoryManager::GetInstance()->GetMemoryPool<A[5]>();
		 auto obj = pool->CreateObjectSPtr();
		 auto obj1 = pool->CreateObjectSPtr();
		 auto obj2 = pool->CreateObjectSPtr();
		 auto obj3 = pool->CreateObjectSPtr();
		 auto obj4 = pool->CreateObjectSPtr();
		 auto obj5 = pool->CreateObjectSPtr();
		std::cout << obj[0].arr[3] << std::endl;
		std::cout << obj[0].arr[2] << std::endl;
		std::cout << obj[0].a << std::endl;
	}
}

int main(int argc, char** argv){
	test();
	return 0;
	// testing::InitGoogleTest(&argc, argv);
	// return RUN_ALL_TESTS();
}

