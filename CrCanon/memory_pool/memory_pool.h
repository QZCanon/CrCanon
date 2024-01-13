#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <climits>
#include <cstddef>
#include <type_traits>
#include <cstdint>
#include <utility>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

namespace Canon::MemoryPool {

using MemMutex = std::unique_lock<std::mutex>;

template <class T>
struct BlockSlot
{
    union Slot
    {
        Slot* next;
        T element;
    };

    using ObjectType = T;
    using Pointer = T*;
    using SlotType = Slot;
    using SharedPointer = std::shared_ptr<T>;
    constexpr static size_t ARR_SIZE = 0;
};

template <class T, size_t N>
struct BlockSlot<T[N]>
{
    union Slot
    {
        Slot* next;
        T element[N];
    };

    using ObjectType = T;
    using Pointer = T*;
    using SlotType = Slot;
    using SharedPointer = std::shared_ptr<T[N]>;
    constexpr static size_t ARR_SIZE = N;
};

template <class T>
class memory_pool
{
public:
    using SharedPointer = typename BlockSlot<T>::SharedPointer;
    using SlotType = typename BlockSlot<T>::SlotType;
    using SlotPoint = SlotType*;
    using Pointer = typename BlockSlot<T>::Pointer;

    memory_pool();
    ~memory_pool();

    template <class... Args>
    SharedPointer CreateObjectSPtr(Args&&... args)
    {
        auto result = Allocate();
        return std::shared_ptr<T>(new (result) T(std::forward<Args>(args)...),
                                  [this](Pointer obj) { DestoryObject(obj); });
    }

    template <typename Deleter, typename... Args>
    SharedPointer CreateObjectSPtrWithDeleter(Deleter deleteFunc, Args&&... args)
    {
        auto result = Allocate();
        return std::shared_ptr<T>(new (result) T(std::forward<Args>(args)...), deleteFunc);
    }

    void DestoryObject(Pointer p)
    {
        if (p != nullptr) {
            //            p->~T();
            Deallocate(p);
        } else {
            std::cout << "p = null\n";
        }
    }

private:
    SlotPoint m_currentSlot; // The header of the currently available slot
    SlotPoint m_lastSlot;    // Last slot
    SlotPoint m_freeSlots;   // Free linked list

    std::mutex mutex;
    size_t m_blockCount;
    size_t m_memoryPoolBytes;

    const int m_objectNumber = 5;

    std::vector<void*> m_blockArray;

private:
    SlotPoint Allocate();
    void Deallocate(Pointer p);
    void AllocateBlock();

    size_t MemoryPoolBytes() { return (m_objectNumber) * (sizeof(SlotType)); }
};

template <class T>
memory_pool<T>::memory_pool()
{
    m_currentSlot = nullptr;
    m_lastSlot = nullptr;
    m_freeSlots = nullptr;
    m_blockCount = 0;
    m_memoryPoolBytes = MemoryPoolBytes();
    std::cout << "m_memoryPoolBytes : " << m_memoryPoolBytes << std::endl;
}

template <class T>
memory_pool<T>::~memory_pool()
{
    std::cout << "\n\nThe number of block = " << m_blockCount << std::endl;
    auto cnt = m_blockCount;
    for (auto& block : m_blockArray) {
        std::cout << "delete block " << cnt - (m_blockCount--) + 1 << "\n";
        operator delete(block);
    }
}

template <class T>
void memory_pool<T>::AllocateBlock()
{
    auto newBlock = reinterpret_cast<char*>(operator new(m_memoryPoolBytes));
    m_blockArray.push_back(newBlock);

    m_currentSlot = reinterpret_cast<SlotPoint>(newBlock); // Skip the extra bytes
    m_lastSlot = m_currentSlot + (m_objectNumber - 1);
}

template <class T>
inline typename memory_pool<T>::SlotPoint memory_pool<T>::Allocate()
{
    SlotPoint result{nullptr};
    if (m_freeSlots != nullptr) {
        std::cout << "freeSlot 有空闲区域，直接获取\n";
        {
            MemMutex loc(mutex);
            result = reinterpret_cast<SlotPoint>(m_freeSlots);
            m_freeSlots = m_freeSlots->next;
        }
    } else {
        std::cout << "freeSlot 无空闲区域\n";
        if ((m_currentSlot > m_lastSlot && m_currentSlot != nullptr && m_lastSlot != nullptr) ||
            (m_currentSlot == nullptr && m_lastSlot == nullptr)) {
            printf("  +++++++Block没有可用的Slot, 重新申请Block\n");
            m_blockCount++;
            {
                MemMutex loc(mutex);
                AllocateBlock();
            }
        } else {
            printf("  >>>>>>>当前Block还有未使用的slot\n");
        }
        {
            MemMutex loc(mutex);
            result = reinterpret_cast<SlotPoint>(m_currentSlot++);
        }
    }
    return result;
}

template <class T>
inline void memory_pool<T>::Deallocate(Pointer p)
{
    if (p != nullptr) {
        std::cout << "Deallocate  p = " << p << ", 放入空闲区域\n";
        {
            MemMutex loc(mutex);
            reinterpret_cast<SlotPoint>(p)->next = m_freeSlots;
            m_freeSlots = reinterpret_cast<SlotPoint>(p);
        }
        std::cout << "-----------------------\n";
    }
}

} // namespace Canon::MemoryPool

#endif // MEMORY_POOL_H
