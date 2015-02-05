#include "gtest/gtest.h"
#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"

namespace {

    class OctLeafTest : public ::testing::Test {
    protected:
        OctLeaf *leaf;
        OctLeaf *fullLeaf;

        OctLeafTest() {
            // You can do set-up work for each test here.
        }

        virtual ~OctLeafTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        virtual void SetUp() {
            std::bitset<OctLeaf::volume> voxels;
            leaf = new OctLeaf(voxels);
            voxels.set();
            fullLeaf = new OctLeaf(voxels);
        }

        virtual void TearDown() {
            delete leaf;
            delete fullLeaf;
        }
    };

    TEST_F(OctLeafTest, Construction) {}

    TEST_F(OctLeafTest, getVoxelReturnsFalseWhenGettingAnEmptyVoxel) {
        ASSERT_FALSE(leaf->getVoxel(VoxelAddress(0, 0, 0)));
    }

    TEST_F(OctLeafTest, getVoxelReturnsTrueWhenGettingAFullVoxel) {
        ASSERT_TRUE(fullLeaf->getVoxel(VoxelAddress(0, 0, 0)));
    }

}  // namespace
