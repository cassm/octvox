#include "gtest/gtest.h"
#include "Octree.hpp"
#include "VoxelAddress.hpp"
#include <boost/make_shared.hpp>

namespace {

    using boost::shared_ptr;
    using boost::make_shared;
    using namespace octvox;

    class OctreeTest : public ::testing::Test {
    protected:
        shared_ptr<const Octree> emptyTree;
        shared_ptr<const Octree> fullTree;
        shared_ptr<const Octree> partialTree;
        shared_ptr<const Octree> a;
        shared_ptr<const Octree> a2;
        shared_ptr<const Octree> b;
        shared_ptr<const Octree> aIntersectB;
        shared_ptr<const Octree> aUnionB;
        const VoxelAddress origin;

        OctreeTest() :
                origin(0, 0, 0)
        {
            emptyTree = make_shared<const Octree>();

        }

        virtual ~OctreeTest() {}

        virtual void SetUp() {}

        virtual void TearDown() {}
    };

    TEST_F(OctreeTest, Construction) {}

    TEST_F(OctreeTest, getVoxelReturnsFalseWhenGettingAnEmptyVoxel) {
        ASSERT_FALSE(emptyTree->getVoxel(origin));
    }
#if 0

    TEST_F(OctreeTest, getVoxelReturnsTrueWhenGettingAFullVoxel) {
        ASSERT_TRUE(fullTree->getVoxel(origin));
    }

    TEST_F(OctreeTest, getVoxelReturnsCorrectlyAtNonZeroAddress) {
        ASSERT_TRUE(partialTree->getVoxel(full));
        ASSERT_FALSE(partialTree->getVoxel(empty));
    }

    TEST_F(OctreeTest, getVoxelAliasesOutOfRangeAddresses) {
        VoxelAddress outOfRangeFull(Octree::edgeLength + full.x, Octree::edgeLength + full.y, Octree::edgeLength + full.z);
        VoxelAddress outOfRangeEmpty(Octree::edgeLength + empty.x, Octree::edgeLength + empty.y, Octree::edgeLength + empty.z);

        ASSERT_TRUE(partialTree->getVoxel(outOfRangeFull));
        ASSERT_FALSE(partialTree->getVoxel(outOfRangeEmpty));
    }

    TEST_F(OctreeTest, equalReturnsTrueOnIdenticalOctrees) {
        ASSERT_TRUE(*a == *a);
    }

    TEST_F(OctreeTest, equalReturnsFalseOnUnequalOctrees) {
        ASSERT_FALSE(*a == *b);
    }

    TEST_F(OctreeTest, equalReturnsTrueOnEqualButNotIdenticalOctrees) {
        ASSERT_TRUE(*a == *a2);
    }

    TEST_F(OctreeTest, intersectionWorks) {
        shared_ptr<const Octree> i = a->intersectionWith(b);
        ASSERT_TRUE(*i == *aIntersectB);
    }

    TEST_F(OctreeTest, unionWorks) {
        shared_ptr<const Octree> i = a->unionWith(b);
        ASSERT_TRUE(*i == *aUnionB);
    }

    #endif

}  // namespace
