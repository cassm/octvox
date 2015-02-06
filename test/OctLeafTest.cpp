#include "gtest/gtest.h"
#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"
#include <boost/make_shared.hpp>

namespace {

    using boost::shared_ptr;
    using boost::make_shared;

    class OctLeafTest : public ::testing::Test {
    protected:
        shared_ptr<const OctLeaf> emptyLeaf;
        shared_ptr<const OctLeaf> fullLeaf;
        shared_ptr<const OctLeaf> partialLeaf;
        shared_ptr<const OctLeaf> a;
        shared_ptr<const OctLeaf> a2;
        shared_ptr<const OctLeaf> b;
        shared_ptr<const OctLeaf> aIntersectB;
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
        {
            std::bitset<OctLeaf::volume> voxels;

            emptyLeaf = make_shared<const OctLeaf> (voxels);

            voxels.set(full.getLinearIndex());
            partialLeaf = make_shared<const OctLeaf> (voxels);

            voxels.set();
            fullLeaf = make_shared<const OctLeaf> (voxels);

            voxels.reset();
            voxels.set(onlyInA.getLinearIndex());
            voxels.set(inBoth.getLinearIndex());
            a = make_shared<const OctLeaf> (voxels);
            a2 = make_shared<const OctLeaf> (voxels);

            voxels.set(onlyInB.getLinearIndex());
            aIntersectB = make_shared<const OctLeaf> (voxels);

            voxels.reset();
            voxels.set(onlyInB.getLinearIndex());
            voxels.set(inBoth.getLinearIndex());
            b = make_shared<const OctLeaf> (voxels);
        }

        virtual ~OctLeafTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        virtual void SetUp() {}

        virtual void TearDown() {}
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

    TEST_F(OctLeafTest, equalReturnsTrueOnIdenticalOctLeafs) {
        ASSERT_TRUE(*a == *a);
    }

    TEST_F(OctLeafTest, equalReturnsFalseOnUnequalOctLeafs) {
        ASSERT_FALSE(*a == *b);
    }

    TEST_F(OctLeafTest, equalReturnsTrueOnEqualButNotIdenticalOctLeafs) {
        ASSERT_TRUE(*a == *a2);
    }

    TEST_F(OctLeafTest, intersectionWorks) {
        shared_ptr<const OctLeaf> i = a->intersection(b);
        ASSERT_TRUE(*i == *aIntersectB);
    }

}  // namespace
