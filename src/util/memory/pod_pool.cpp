#include "pod_pool.h"

#include <cassert>
#include <algorithm> /* For std::max. */

#include <util/bitops.h>

static char* alignPtrUp(char* ptr, size_t alignment) {
    return reinterpret_cast<char*>(alignUp(reinterpret_cast<std::intptr_t>(ptr), alignment));
}

PodPool::PodPool(size_t capacity) {
    allocate(capacity);
}

PodPool::PodPool(PodPool&& other) noexcept {
    std::swap(m_chunks, other.m_chunks);
    std::swap(m_pos, other.m_pos);
    std::swap(m_end, other.m_end);
    std::swap(m_capacity, other.m_capacity);
}

PodPool::~PodPool() {
    for (void* chunk : m_chunks)
        delete chunk;
}

void* PodPool::insert(const void* src, size_t size, size_t alignment) {
    assert(isPower2(alignment));

    char* alignedPtr = alignPtrUp(m_pos, alignment);

    if (alignedPtr + size > m_end) {
        allocate(std::max(m_capacity * 2, size + alignment));
        return insert(src, size, alignment);
    }

    memcpy(alignedPtr, src, size);
    m_pos = alignedPtr + size;
    return alignedPtr;
}

void PodPool::allocate(size_t capacity) {
    m_pos = new char[capacity];
    m_end = m_pos + capacity;
    m_capacity = capacity;

    m_chunks.push_back(m_pos);
}
