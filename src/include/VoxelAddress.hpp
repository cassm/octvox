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

        VoxelAddress(addr_t _x, addr_t _y, addr_t _z) :
                x(_x), y(_y), z(_z) {
        }

        inline size_t getLinearIndex() const;
        inline subtree_index_t getSubtreeIndex(uint_fast8_t hight) const;

    private:
    };

    inline size_t VoxelAddress::getLinearIndex() const {
        return (x & OctLeaf::lengthMask) * OctLeaf::edgeLength * OctLeaf::edgeLength
                + (y & OctLeaf::lengthMask) * OctLeaf::edgeLength
                + (z & OctLeaf::lengthMask);
    }

    inline VoxelAddress::subtree_index_t VoxelAddress::getSubtreeIndex(uint_fast8_t hight) const {
        // speed optimise this bittwiddling later if profiling indicates that it matters.
        return ((x & (1 << hight)) ? (1 << 2) : 0 |
                ((y & (1 << hight)) ? (1 << 1) : 0) |
                ((z & (1 << hight)) ? (1 << 0) : 0));
    }

}
#endif