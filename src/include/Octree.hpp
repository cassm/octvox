#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "OctLeaf.hpp"

#include <boost/shared_ptr.hpp>
#include <bitset>

namespace octvox {

    class VoxelAddress;

    class Octree {
    public:
        Octree(const std::bitset<OctLeaf::volume> &_voxels, const VoxelAddress);

        ~Octree();

        Octree(const Octree &) = default;

        bool getVoxel(const VoxelAddress) const;

        boost::shared_ptr<const Octree> intersectionWith(boost::shared_ptr<const Octree> other) const;

        boost::shared_ptr<const Octree> unionWith(boost::shared_ptr<const Octree> other) const;

        bool operator==(const Octree &other) const;

    private:
    };

}

#endif