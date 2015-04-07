#include "gtest/gtest.h"
#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"

namespace {

    using std::shared_ptr;
    using std::make_shared;
    using namespace octvox;

    class OctLeafTest : public ::testing::Test {
    protected:
        shared_ptr<const OctLeaf> emptyLeaf;
        shared_ptr<const OctLeaf> fullLeaf;
        shared_ptr<const OctLeaf> partialLeaf;
        shared_ptr<const OctLeaf> a;
        shared_ptr<const OctLeaf> a2;
        shared_ptr<const OctLeaf> b;
        shared_ptr<const OctLeaf> aIntersectB;
        shared_ptr<const OctLeaf> aUnionB;
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

            voxels.reset();
            voxels.set(onlyInB.getLinearIndex());
            voxels.set(inBoth.getLinearIndex());
            b = make_shared<const OctLeaf> (voxels);

            voxels.reset();
            voxels.set(onlyInA.getLinearIndex());
            voxels.set(inBoth.getLinearIndex());
            voxels.set(onlyInB.getLinearIndex());
            aUnionB = make_shared<const OctLeaf> (voxels);

            voxels.reset();
            voxels.set(inBoth.getLinearIndex());
            aIntersectB = make_shared<const OctLeaf> (voxels);
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
        VoxelAddress outOfRangeFull(VoxelAddress::leafLength + full.x, VoxelAddress::leafLength + full.y, VoxelAddress::leafLength + full.z);
        VoxelAddress outOfRangeEmpty(VoxelAddress::leafLength + empty.x, VoxelAddress::leafLength + empty.y, VoxelAddress::leafLength + empty.z);

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
        shared_ptr<const OctLeaf> i = a->intersectWith(b);
        ASSERT_TRUE(i->getVoxel(inBoth));
        ASSERT_FALSE(i->getVoxel(onlyInA));
        ASSERT_FALSE(i->getVoxel(onlyInB));
        ASSERT_TRUE(*i == *aIntersectB);
    }

    TEST_F(OctLeafTest, unionWorks) {
        shared_ptr<const OctLeaf> i = a->unionWith(b);
        ASSERT_TRUE(*i == *aUnionB);
    }

}  // namespace
