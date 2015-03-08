#include "Octree.hpp"
#include <boost/make_shared.hpp>

namespace octvox {
    using boost::shared_ptr;

    shared_ptr<const Octree> Octree::setLeaf(const OctLeaf &&leaf, VoxelAddress const aConst) {
        return boost::shared_ptr<Octree const>();
    }

    // for testing
    bool Octree::getVoxel(VoxelAddress const aConst) const {
        return false;
    }

}