#pragma once

#include <bitset>
#include <type_traits>

/**
 * Pretty much like `QFlags`, but for normal enums with consecutive values.
 *
 * The underlying implementation relies on `std::bitset`, and thus this class cannot be made constexpr right now.
 *
 * \tparam Enum                         Enumeration type to use.
 * \tparam valueCount                   Total count of different enum values for the provided enum. Effectively this
 *                                      is the number of bits that will be used for storage.
 */
template<class Enum, Enum valueCount>
class BitsetFlags {
    static_assert(std::is_enum_v<Enum>, "BitsetFlags only work for enum values.");
    static_assert(valueCount > 0, "BitsetFlags must have non-zero size.");

public:
    using enum_type = Enum;

    BitsetFlags() noexcept {}

    BitsetFlags(Enum flag) noexcept {
        m_storage[flag] = true;
    }

    /* Compiler-generated copy is OK. */

    BitsetFlags& operator&=(Enum other) noexcept {
        bool value = m_storage[other];
        m_storage.reset();
        m_storage[other] = value;
        return *this;
    }

    BitsetFlags& operator&=(const BitsetFlags& other) noexcept {
        m_storage &= other.m_storage;
        return *this;
    }

    BitsetFlags& operator|=(Enum other) noexcept {
        m_storage[other] = true;
        return *this;
    }

    BitsetFlags& operator|=(const BitsetFlags& other) noexcept {
        m_storage |= other.m_storage;
        return *this;
    }

    BitsetFlags& operator^=(Enum other) noexcept {
        m_storage[other] = !m_storage[other];
        return *this;
    }

    BitsetFlags& operator^=(const BitsetFlags& other) noexcept {
        m_storage ^= other.m_storage;
        return *this;
    }

    friend BitsetFlags operator|(const BitsetFlags& l, const BitsetFlags& r) noexcept {
        return BitsetFlags(l.m_storage | r.m_storage);
    }

    friend BitsetFlags operator|(const BitsetFlags& l, Enum r) noexcept {
        return BitsetFlags(l) |= r;
    }

    friend BitsetFlags operator|(Enum l, const BitsetFlags& r) noexcept {
        return BitsetFlags(r) |= l;
    }

    friend BitsetFlags operator&(const BitsetFlags& l, const BitsetFlags& r) noexcept {
        return BitsetFlags(l.m_storage & r.m_storage);
    }

    friend BitsetFlags operator&(Enum l, const BitsetFlags& r) noexcept {
        return BitsetFlags(r) &= l;
    }

    friend BitsetFlags operator&(const BitsetFlags& l, Enum r) noexcept {
        return BitsetFlags(l) &= r;
    }

    friend BitsetFlags operator^(const BitsetFlags& l, const BitsetFlags& r) noexcept {
        return BitsetFlags(l.m_storage ^ r.m_storage);
    }

    friend BitsetFlags operator^(Enum l, const BitsetFlags& r) noexcept {
        return BitsetFlags(r) ^= l;
    }

    friend BitsetFlags operator^(const BitsetFlags& l, Enum r) noexcept {
        return BitsetFlags(l) ^= r;
    }

    BitsetFlags operator~() const noexcept {
        return BitsetFlags(~m_storage);
    }

    operator bool() const noexcept {
        return m_storage.any();
    }

    bool operator!() const noexcept {
        return m_storage.none();
    }

private:
    BitsetFlags(const std::bitset<valueCount>& storage) : m_storage(storage) {}

private:
    std::bitset<valueCount> m_storage;
};

#define X_DECLARE_BITSET_FLAGS(FLAGS, ENUM, COUNT)                                                                     \
using FLAGS = BitsetFlags<ENUM, COUNT>;

#define X_DELARE_OPERATORS_FOR_BITSET_FLAGS(FLAGS)                                                                     \
inline FLAGS operator|(FLAGS::enum_type l, FLAGS::enum_type r) noexcept {                                               \
    return FLAGS(l) |= r;                                                                                               \
}
