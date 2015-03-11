#ifndef OCT_LEAF_HPP
#define OCT_LEAF_HPP

#include "VoxelAddress.hpp"

#include <bitset>
#include <memory>

namespace octvox {

    class VoxelAddress;

    // An OctLeaf stores a cubic array of voxels: NxNxN, where N = 2^edgeBits.
    class OctLeaf {
    public:
        // The number of voxels stored in an OctLeaf
        static constexpr size_t volume = VoxelAddress::leafLength * VoxelAddress::leafLength * VoxelAddress::leafLength;

        constexpr OctLeaf(const std::bitset<volume> &_voxels) noexcept : voxels(_voxels) {}

        ~OctLeaf() = default;

        OctLeaf(const OctLeaf &) = default;

        bool getVoxel(const VoxelAddress) const;

        std::shared_ptr<const OctLeaf> intersectionWith(std::shared_ptr<const OctLeaf> other) const;

        std::shared_ptr<const OctLeaf> unionWith(std::shared_ptr<const OctLeaf> other) const;

        inline bool operator==(const OctLeaf& other) const noexcept {
            return voxels == other.voxels;
        }

        inline bool operator!=(const OctLeaf& other) const noexcept {
            return !operator==(other);
        }

    private:
        // There may be more optimal representations, but this one will do for now -
        // it should be fairly easy to swap out
        const std::bitset<volume> voxels;
    };

}
#endif