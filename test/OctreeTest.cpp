#include "gtest/gtest.h"
#include "Octree.hpp"
#include "VoxelAddress.hpp"
#include <boost/make_shared.hpp>

namespace {

    using boost::shared_ptr;
    using boost::make_shared;
    using std::bitset;
    using namespace octvox;

    class OctreeTest : public ::testing::Test {
    protected:
        static const auto height = 2;
        shared_ptr<const Octree<height> > emptyTree;
        shared_ptr<const Octree<height> > fullTree;
        shared_ptr<const Octree<height> > partialTree;
        shared_ptr<const Octree<height> > a2;
        shared_ptr<const Octree<height> > b;
        shared_ptr<const Octree<height> > aIntersectB;
        shared_ptr<const Octree<height> > aUnionB;
        shared_ptr<const OctLeaf> partialLeaf;
        const VoxelAddress origin;
        const VoxelAddress full;
        const VoxelAddress empty;

        OctreeTest() :
                origin(0, 0, 0),
                full(1, 2, 3),
                empty(3, 2, 1)

        {
            emptyTree = make_shared<const Octree<height> >();
            fullTree = make_shared<const Octree<height> >(bitset<Octree<height>::childrenSize>().set());
            bitset<OctLeaf::volume> voxels;
            voxels.set(full.getLinearIndex());
            partialLeaf = make_shared<const OctLeaf>(voxels);
            partialTree = emptyTree->setLeaf(partialLeaf, full);
        }

        virtual ~OctreeTest() {}

        virtual void SetUp() {}

        virtual void TearDown() {}
    };

    TEST_F(OctreeTest, Construction) {}

    TEST_F(OctreeTest, ConstructorH0LeafAddress) {
        auto h0FromLeaf = Octree<0>(partialLeaf, full);
        ASSERT_TRUE(h0FromLeaf.getVoxel(full));
        ASSERT_FALSE(h0FromLeaf.getVoxel(empty));
    }

    TEST_F(OctreeTest, ConstructorH2LeafAddress) {
        auto h2FromLeaf = Octree<2>(partialLeaf, full);
        ASSERT_TRUE(h2FromLeaf.getVoxel(full));
        ASSERT_FALSE(h2FromLeaf.getVoxel(empty));
    }

    TEST_F(OctreeTest, getVoxelReturnsFalseWhenGettingAnEmptyVoxel) {
        ASSERT_FALSE(emptyTree->getVoxel(origin));
    }

    TEST_F(OctreeTest, getVoxelReturnsTrueWhenGettingAFullVoxel) {
        ASSERT_TRUE(fullTree->getVoxel(origin));
    }

    TEST_F(OctreeTest, getVoxelReturnsCorrectlyAtNonZeroAddressOnHeightZeroOctree) {
        Octree<0> emptyTree0;
        auto partialTree0 = emptyTree0.setLeaf(partialLeaf, full);
        ASSERT_TRUE(partialTree0->getVoxel(full));
        ASSERT_FALSE(partialTree0->getVoxel(empty));
    }

    TEST_F(OctreeTest, getVoxelReturnsCorrectlyAtNonZeroAddress) {
        ASSERT_TRUE(partialTree->getVoxel(full));
        ASSERT_FALSE(partialTree->getVoxel(empty));
    }

    TEST_F(OctreeTest, getVoxelDoesNotAliasOutOfRangeAddresses) {
        VoxelAddress outOfRangeFull(OctLeaf::edgeLength + full.x, OctLeaf::edgeLength + full.y, OctLeaf::edgeLength + full.z);
        VoxelAddress outOfRangeEmpty(OctLeaf::edgeLength + empty.x, OctLeaf::edgeLength + empty.y, OctLeaf::edgeLength + empty.z);

        ASSERT_FALSE(partialTree->getVoxel(outOfRangeFull));
        ASSERT_FALSE(partialTree->getVoxel(outOfRangeEmpty));
    }

#if 0
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
