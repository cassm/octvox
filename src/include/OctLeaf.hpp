#ifndef OCT_LEAF_HPP
#define OCT_LEAF_HPP

#include <bitset>
#include <boost/shared_ptr.hpp>

namespace octvox {

    class VoxelAddress;

    class OctLeaf {
    public:
        static const size_t edgeLength = 4;
        static const uint_fast64_t lengthMask = edgeLength - 1;
        static const size_t volume = edgeLength * edgeLength * edgeLength;

        OctLeaf(const std::bitset<volume> &_voxels) : voxels(_voxels) {}

        ~OctLeaf();

        OctLeaf(const OctLeaf &) = default;

        bool getVoxel(const VoxelAddress) const;

        boost::shared_ptr<const OctLeaf> intersectionWith(boost::shared_ptr<const OctLeaf> other) const;

        boost::shared_ptr<const OctLeaf> unionWith(boost::shared_ptr<const OctLeaf> other) const;

        bool operator==(const OctLeaf &other) const;

    private:
        const std::bitset<volume> voxels;
    };

}
#endif