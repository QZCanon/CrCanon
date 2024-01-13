//
// Created by qinzhou on 23-4-18.
//

#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include <thread>
#include <condition_variable>
#include <list>
#include <mutex>
#include <iostream>

template <class T, int maxSize = 100>
class SyncQueue {
public:
    explicit SyncQueue() : m_maxSize(maxSize), m_needStop(false) {}
    void Put(const T& x)
    {
		Add((T &&)x);
    }
    
    void Put(T&& x) {
		Add(std::forward<T>(x));
    }
    
    void Take(std::list<T>& list) {
		std::unique_lock<std::mutex> locker(m_mutex);
		m_notEmpty.wait(locker, [this]{ return m_needStop || NotEmpty(); });
		if (m_needStop) {
			return;
		}
		list = std::move(m_queue);
		m_notFull.notify_one();
    }
    
    void Take(T& t) {
		std::unique_lock<std::mutex> locker(m_mutex);
		m_notEmpty.wait(locker, [this]{ return m_needStop || NotEmpty(); });
		if (m_needStop) {
			return;
		}
		t = m_queue.front();
		m_queue.pop_front();
		m_notFull.notify_one();
    }
    
    void Stop() {
		{
			std::lock_guard<std::mutex> locker(m_mutex);
			m_needStop = true;
		}
		m_notFull.notify_all();
		m_notEmpty.notify_all();
    }
    
    bool Empty() {
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.size() == m_maxSize;
    }
    
    size_t Size()
    {
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.size();
    }
    
    int Count() {
		return m_queue.size();
    }
    
private:
    bool NotFull() const {
		bool full = m_queue.size() >= m_maxSize;
		if (full) {
//			std::cout << "缓冲区满了，需要等待\n";
		}
		return !full;
    }
    
    bool NotEmpty() const  {
		bool empty = m_queue.empty();
		// if (empty) {
		// 	std::cout << "缓冲区空了。需要等待，异步层的线程ID ：" << std::this_thread::get_id() << std::endl;
		// }
		return !empty;
    }
    
    void Add(T&& x) {
		std::unique_lock<std::mutex> locker(m_mutex);
		m_notFull.wait(locker, [this]{ return m_needStop || NotFull(); });
		if (m_needStop) {
			return;
		}
		m_queue.push_back(std::forward<T>(x));
		m_notEmpty.notify_one();
    }
    
private:
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_notEmpty; // 不为空的条件变量
    std::condition_variable m_notFull;	// 没有满的条件变量
    int m_maxSize;
    bool m_needStop;
};

#endif //SYNCQUEUE_H
