//
// Created by qinzhou on 23-4-20.
//

#ifndef ATOMIC_MAP_H
#define ATOMIC_MAP_H

#include <unordered_map>
#include <iostream>
#include <mutex>
#include <functional>

namespace Canon {
namespace Component {

template<class T, class U>
class AtomicMap
{
public:
	template<class... Args> void Emplace(Args &&... args)
	{
		std::lock_guard<std::mutex> locker(mutex);
		map.template emplace(std::forward<Args>(args)...);
	}
	
	bool Get(const T &key, U &val)
	{
		std::lock_guard<std::mutex> locker(mutex);
		if (map.find(key) != map.end()) {
			val = map[key];
			return true;
		}
		return false;
	}
	
	U &operator[](const T &key)
	{
		std::lock_guard<std::mutex> locker(mutex);
		return map[key];
	}
	
	void Remove(const T &key)
	{
		std::lock_guard<std::mutex> locker(mutex);
		if (map.find(key) != map.end()) {
			map.erase(key);
		}
	}
	
	bool IsExist(const T &key)
	{
		std::lock_guard<std::mutex> locker(mutex);
		return map.find(key) != map.end();
	}
	
	void Loop(std::function<void(const T &, U &)> &&func)
	{
		std::lock_guard<std::mutex> locker(mutex);
		for (auto &[key, value] : map) {
			func(key, value);
		}
	}

private:
	std::unordered_map<T, U> map;
	mutable std::mutex mutex;
	
};

}
}

#endif //ATOMIC_MAP_H
