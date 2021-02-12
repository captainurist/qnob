#pragma once

#include <vector>

class PodPool {
public:
    PodPool(size_t capacity = 1024);

    PodPool(const PodPool&) = delete;

    PodPool(PodPool&& other) noexcept;

    ~PodPool();

    void* insert(const void* src, size_t size, size_t alignment);

private:
    void allocate(size_t capacity);

private:
    std::vector<void*> m_chunks;
    char* m_pos = nullptr;
    char* m_end = nullptr;
    std::size_t m_capacity = 0;
};
