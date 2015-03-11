#ifndef VOXEL_ADDRESS_HPP
#define VOXEL_ADDRESS_HPP

#include <cstddef>
#include <cstdint>

namespace octvox {

    class Octleaf;

    class VoxelAddress {
    public:
        // The number of bits required for one leaf part of axis of the voxel address
        static constexpr uint_fast8_t leafBits = 2;
        // The number of voxels along one edge of the leaf voxel cube
        static constexpr uint_fast16_t leafLength = 1 << leafBits;
        // A mask for the bits of one axis of a complete address that specify the voxel location within an OctLeaf
        static constexpr uint_fast64_t leafBitsMask = leafLength - 1;

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
        return (x & leafBitsMask) * leafLength * leafLength
                + (y & leafBitsMask) * leafLength
                + (z & leafBitsMask);
    }

    constexpr inline VoxelAddress::subtree_index_t VoxelAddress::getSubtreeIndex(uint_fast8_t height) noexcept {
        // speed optimise this bittwiddling later if profiling indicates that it matters.
        return static_cast<VoxelAddress::subtree_index_t>(((x & (1 << (height + leafBits))) ? (1 << 2) : 0) |
                ((y & (1 << (height + leafBits))) ? (1 << 1) : 0) |
                ((z & (1 << (height + leafBits))) ? (1 << 0) : 0));
    }

}
#endif