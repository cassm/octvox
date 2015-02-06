#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"
#include <boost/make_shared.hpp>

using boost::shared_ptr;
using boost::make_shared;

OctLeaf::~OctLeaf() {}

bool OctLeaf::getVoxel(const VoxelAddress a) const {
    return voxels[a.getLinearIndex()];
}

shared_ptr<const OctLeaf> OctLeaf::intersectionWith(shared_ptr<const OctLeaf> other) const {
    return make_shared<const OctLeaf>(voxels | other->voxels);
}

shared_ptr<const OctLeaf> OctLeaf::unionWith(shared_ptr<const OctLeaf> other) const {
    return make_shared<const OctLeaf>(voxels & other->voxels);
}

bool OctLeaf::operator==(const OctLeaf &other) const {
    return voxels == other.voxels;
}
