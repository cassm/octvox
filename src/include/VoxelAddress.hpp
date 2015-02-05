#ifndef VOXEL_ADDRESS_HPP
#define VOXEL_ADDRESS_HPP

#include <elf.h>

class VoxelAddress {
public:
    const uint_fast64_t x;
    const uint_fast64_t y;
    const uint_fast64_t z;
    VoxelAddress(uint_fast64_t _x, uint_fast64_t _y, uint_fast64_t _z) :
            x(_x), y(_y), z(_z) {}
};

#endif