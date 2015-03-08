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
        inline subtree_index_t getSubtreeIndex() const;

    private:
    };

    inline size_t VoxelAddress::getLinearIndex() const {
        return (x & OctLeaf::lengthMask) * OctLeaf::edgeLength * OctLeaf::edgeLength
                + (y & OctLeaf::lengthMask) * OctLeaf::edgeLength
                + (z & OctLeaf::lengthMask);
    }

    inline VoxelAddress::subtree_index_t VoxelAddress::getSubtreeIndex() const {
        return 0;
    }

}
#endif