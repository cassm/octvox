#include "gtest/gtest.h"
#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"

namespace {

    class OctLeafTest : public ::testing::Test {
    protected:
        OctLeaf *emptyLeaf;
        OctLeaf *fullLeaf;
        OctLeaf *partialLeaf;
        OctLeaf *a;
        OctLeaf *b;
        VoxelAddress full;
        VoxelAddress empty;
        VoxelAddress origin;
        VoxelAddress onlyInA;
        VoxelAddress onlyInB;
        VoxelAddress inBoth;

        OctLeafTest() :
                full(1, 2, 3),
                empty(3, 2, 1),
                origin(0, 0, 0),
                onlyInA(6, 7, 8),
                onlyInB(7, 8, 9),
                inBoth(8, 9, 10)
        {}

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

            voxels.reset();
            voxels.set(onlyInA.getLinearIndex());
            voxels.set(inBoth.getLinearIndex());
            a = new OctLeaf(voxels);

            voxels.reset();
            voxels.set(onlyInB.getLinearIndex());
            voxels.set(inBoth.getLinearIndex());
            b = new OctLeaf(voxels);
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

    TEST_F(OctLeafTest, getVoxelAliasesOutOfRangeAddresses) {
        VoxelAddress outOfRangeFull(OctLeaf::edgeLength + full.x, OctLeaf::edgeLength + full.y, OctLeaf::edgeLength + full.z);
        VoxelAddress outOfRangeEmpty(OctLeaf::edgeLength + empty.x, OctLeaf::edgeLength + empty.y, OctLeaf::edgeLength + empty.z);

        ASSERT_TRUE(partialLeaf->getVoxel(outOfRangeFull));
        ASSERT_FALSE(partialLeaf->getVoxel(outOfRangeEmpty));
    }

    TEST_F(OctLeafTest, intersectionWorks) {
//        OctLeaf &i = a.intersection(b);
    }

}  // namespace
