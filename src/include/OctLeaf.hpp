#ifndef OCT_LEAF_HPP
#define OCT_LEAF_HPP

#include <bitset>
#include <memory>

namespace octvox {

    class VoxelAddress;

    // An OctLeaf stores a cubic array of voxels: NxNxN, where N = 2^edgeBits.
    class OctLeaf {
    public:
        // The number of bits required for one axis of the voxel address
        static constexpr uint_fast8_t edgeBits = 2;
        // The number of voxels along one edge of the cube
        static constexpr uint_fast16_t edgeLength = 1 << edgeBits;
        // A mask for the bits of a complete address that specify the voxel location within an OctLeaf
        static constexpr uint_fast64_t lengthMask = edgeLength - 1;
        // The number of voxels stored in an OctLeaf
        static constexpr size_t volume = edgeLength * edgeLength * edgeLength;

        OctLeaf(const std::bitset<volume> &_voxels) : voxels(_voxels) {}

        ~OctLeaf();

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
        const std::bitset<volume> voxels;
    };

}
#endif