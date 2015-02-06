#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"

OctLeaf::~OctLeaf() {}

OctLeaf::OctLeaf(const OctLeaf&) {
    // FIXME
}

bool OctLeaf::getVoxel(VoxelAddress a) {
    return voxels[a.getLinearIndex()];
}
