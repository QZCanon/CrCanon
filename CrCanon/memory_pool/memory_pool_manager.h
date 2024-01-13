//
// Created by qinzhou on 23-4-20.
//

#ifndef MEMORY_POOL_MANAGER_H
#define MEMORY_POOL_MANAGER_H

#include <string>

#include "component/refile.h"
#include "../component/atomic_map.h"
#include "component/any.h"
#include "memory_pool.h"

namespace Canon::MemoryPool {

class MemoryManager
{
public:
	static MemoryManager *GetInstance()
	{
		static MemoryManager instance;
		return &instance;
	}

	template<class T>
	std::shared_ptr<memory_pool<T>> GetMemoryPool()
	{
		std::string key = TypeName<T>();
		Component::Any pool;
		if (m_typeArray.Get(key, pool)) {
			std::cout << "find pool\n";
			return pool.AnyCast<std::shared_ptr<memory_pool<T>>>();
		}
		pool = std::make_shared<memory_pool<T>>();
		m_typeArray[key] = pool;
		std::cout << "key : " << key << std::endl;
		return pool.AnyCast<std::shared_ptr<memory_pool<T>>>();
	}

private:
	Component::AtomicMap<std::string, Component::Any> m_typeArray;
};
}

#endif //MEMORY_POOL_MANAGER_H
