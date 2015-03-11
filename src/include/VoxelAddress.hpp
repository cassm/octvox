#ifndef VOXEL_ADDRESS_HPP
#define VOXEL_ADDRESS_HPP

#include <elf.h>
#include "OctLeaf.hpp"

namespace octvox {

    class VoxelAddress {
    public:
        typedef uint_fast64_t addr_t;
        const addr_t x;
        const addr_t y;
        const addr_t z;

        typedef uint_fast8_t subtree_index_t;

        constexpr VoxelAddress(addr_t _x, addr_t _y, addr_t _z) noexcept :
                x(_x), y(_y), z(_z) {}

        constexpr inline size_t getLinearIndex() noexcept;
        constexpr inline subtree_index_t getSubtreeIndex(uint_fast8_t height) noexcept;

    };

    constexpr inline size_t VoxelAddress::getLinearIndex() noexcept {
        // There is a trivial transformation from multiplies to shifts,
        // but this expression is slightly clearer and should be just as fast.s
        return (x & OctLeaf::lengthMask) * OctLeaf::edgeLength * OctLeaf::edgeLength
                + (y & OctLeaf::lengthMask) * OctLeaf::edgeLength
                + (z & OctLeaf::lengthMask);
    }

    constexpr inline VoxelAddress::subtree_index_t VoxelAddress::getSubtreeIndex(uint_fast8_t height) noexcept {
        // speed optimise this bittwiddling later if profiling indicates that it matters.
        return ((x & (1 << (height + OctLeaf::edgeBits))) ? (1 << 2) : 0) |
                ((y & (1 << (height + OctLeaf::edgeBits))) ? (1 << 1) : 0) |
                ((z & (1 << (height + OctLeaf::edgeBits))) ? (1 << 0) : 0);
    }

}
#endif