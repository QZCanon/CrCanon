#pragma once

#include <iostream>


namespace Canon::Ptr {

template <typename T>
class SharedPtr
{
private:
    size_t* m_count_;
    T* m_ptr_;

public:
    SharedPtr() : m_ptr_(nullptr), m_count_(new size_t) {}

    explicit SharedPtr(T* ptr) : m_ptr_(ptr), m_count_(new size_t) { *m_count_ = 1; }

    ~SharedPtr()
    {
        --(*m_count_);
        if (*m_count_ == 0) {
            delete m_ptr_;
            delete m_count_;
            m_ptr_ = nullptr;
            m_count_ = nullptr;
        }
    }

    SharedPtr(const SharedPtr& ptr)
    {
        m_count_ = ptr.m_count_;
        m_ptr_ = ptr.m_ptr_;
        ++(*m_count_);
    }
    void operator=(const SharedPtr& ptr) { SharedPtr(std::move(ptr)); }

    SharedPtr(SharedPtr&& ptr) : m_ptr_(ptr.m_ptr_), m_count_(ptr.m_count_) { ++(*m_count_); }

    void operator=(SharedPtr&& ptr) { SharedPtr(std::move(ptr)); }

    T& operator*() { return *m_ptr_; }

    T* operator->() { return m_ptr_; }

    operator bool() { return m_ptr_ == nullptr; }

    T* get() { return m_ptr_; }

    size_t use_count() { return *m_count_; }

    bool unique() { return *m_count_ == 1; }

    void swap(SharedPtr& ptr) { std::swap(*this, ptr); }
};

template <typename T, class... Args>
SharedPtr<T> MakeShared(Args&&... args)
{
    return SharedPtr(new T(std::forward<Args>(args)...));
}

} // namespace Canon::Ptr
