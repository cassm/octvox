#include "gtest/gtest.h"
#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"

namespace {

    class OctLeafTest : public ::testing::Test {
    protected:
        OctLeaf *emptyLeaf;
        OctLeaf *fullLeaf;
        OctLeaf *partialLeaf;
        VoxelAddress full, empty, origin;


        OctLeafTest() : full(1, 2, 3), empty(3, 2, 1), origin(0, 0, 0) {
            // You can do set-up work for each test here.
        }

        virtual ~OctLeafTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        virtual void SetUp() {
            std::bitset<OctLeaf::volume> voxels;
            emptyLeaf = new OctLeaf(voxels);
            voxels.set(full.getLinearIndex());
            partialLeaf = new OctLeaf(voxels);
            voxels.set();
            fullLeaf = new OctLeaf(voxels);
        }

        virtual void TearDown() {
            delete emptyLeaf;
            delete fullLeaf;
        }
    };

    TEST_F(OctLeafTest, Construction) {}

    TEST_F(OctLeafTest, getVoxelReturnsFalseWhenGettingAnEmptyVoxel) {
        ASSERT_FALSE(emptyLeaf->getVoxel(origin));
    }

    TEST_F(OctLeafTest, getVoxelReturnsTrueWhenGettingAFullVoxel) {
        ASSERT_TRUE(fullLeaf->getVoxel(origin));
    }

    TEST_F(OctLeafTest, getVoxelReturnsCorrectlyAtNonZeroAddress) {
        ASSERT_TRUE(partialLeaf->getVoxel(full));
        ASSERT_FALSE(partialLeaf->getVoxel(empty));
    }

}  // namespace
