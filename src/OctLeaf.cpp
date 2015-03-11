#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"
#include <memory>

namespace octvox {

    using std::shared_ptr;
    using std::make_shared;

    OctLeaf::~OctLeaf() {
    }

    bool OctLeaf::getVoxel(const VoxelAddress a) const {
        return voxels[a.getLinearIndex()];
    }

    shared_ptr<const OctLeaf> OctLeaf::intersectionWith(shared_ptr<const OctLeaf> other) const {
        return make_shared<const OctLeaf>(voxels & other->voxels);
    }

    shared_ptr<const OctLeaf> OctLeaf::unionWith(shared_ptr<const OctLeaf> other) const {
        return make_shared<const OctLeaf>(voxels | other->voxels);
    }

}