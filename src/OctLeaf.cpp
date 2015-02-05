#include "OctLeaf.hpp"

OctLeaf::~OctLeaf() {}

OctLeaf::OctLeaf(const OctLeaf&) {
    // FIXME
}

bool OctLeaf::getVoxel(VoxelAddress a) {
    return voxels[(a.x & lengthMask) * edgeLength * edgeLength + (a.y & lengthMask) * edgeLength + (a.z & lengthMask)];
}
