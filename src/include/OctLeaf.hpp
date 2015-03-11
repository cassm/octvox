#ifndef OCT_LEAF_HPP
#define OCT_LEAF_HPP

#include "VoxelAddress.hpp"

#include <bitset>
#include <memory>

namespace octvox {

    // An OctLeaf stores a cubic array of voxels: NxNxN, where N = 2^VoxelAddress::leafBits.
    class OctLeaf {
    public:
        // The number of voxels stored in an OctLeaf
        static constexpr size_t volume = VoxelAddress::leafLength * VoxelAddress::leafLength * VoxelAddress::leafLength;

        constexpr OctLeaf(const std::bitset<volume> &_voxels) noexcept : voxels(_voxels) {}

        ~OctLeaf() = default;

        OctLeaf(const OctLeaf &) = default;

        inline bool getVoxel(const VoxelAddress) const noexcept;

        inline std::shared_ptr<const OctLeaf> intersectionWith(std::shared_ptr<const OctLeaf> other) const noexcept;

        inline std::shared_ptr<const OctLeaf> unionWith(std::shared_ptr<const OctLeaf> other) const noexcept;

        inline bool operator==(const OctLeaf& other) const noexcept {
            return voxels == other.voxels;
        }

        inline bool operator!=(const OctLeaf& other) const noexcept {
            return !operator==(other);
        }

    private:
        // There may be more optimal representations, but this one will do for now -
        // it should be fairly easy to swap out
        // Especially attractive would be a constexpr compatible datastructure.
        const std::bitset<volume> voxels;
    };

    inline std::shared_ptr<const OctLeaf> OctLeaf::intersectionWith(std::shared_ptr<const OctLeaf> other) const noexcept {
        return std::make_shared<const OctLeaf>(voxels & other->voxels);
    }

    inline std::shared_ptr<const OctLeaf> OctLeaf::unionWith(std::shared_ptr<const OctLeaf> other) const noexcept {
        return std::make_shared<const OctLeaf>(voxels | other->voxels);
    }

    inline bool OctLeaf::getVoxel(const VoxelAddress a) const noexcept {
        return voxels[a.getLinearIndex()];
    }
}
#endif