#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"

OctLeaf::~OctLeaf() {}

OctLeaf::OctLeaf(const OctLeaf&) {
    // FIXME
}

bool OctLeaf::getVoxel(const VoxelAddress a) const {
    return voxels[a.getLinearIndex()];
}

bool OctLeaf::operator==(const OctLeaf &other) const {
    return voxels == other.voxels;
}
