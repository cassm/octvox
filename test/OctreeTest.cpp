#include "gtest/gtest.h"
#include "Octree.hpp"

namespace {

    using std::shared_ptr;
    using std::make_shared;
    using std::bitset;
    using namespace octvox;

    class OctreeTest : public ::testing::Test {
    protected:
        static const auto height = 2;
        shared_ptr<const Octree<height> > emptyTree;
        shared_ptr<const Octree<height> > fullTree;
        shared_ptr<const Octree<height> > partialTree;
        shared_ptr<const Octree<height> > a;
        shared_ptr<const Octree<height> > a2;
        shared_ptr<const Octree<height> > b;
        shared_ptr<const Octree<height> > aIntersectB;
        shared_ptr<const Octree<height> > aUnionB;
        shared_ptr<const OctLeaf> partialLeaf;
        const VoxelAddress origin;
        const VoxelAddress full;
        const VoxelAddress empty;
        const VoxelAddress onlyInA;
        const VoxelAddress onlyInB;
        const VoxelAddress inBoth;

        OctreeTest() :
                origin(0, 0, 0),
                full(1, 2, 3),
                empty(3, 2, 1),
                onlyInA(6, 7, 8),
                onlyInB(7, 8, 9),
                inBoth(8, 9, 10)

        {
            emptyTree = make_shared<const Octree<height> >();

            std::array<Octree<height>::Child, Octree<height>::childrenSize> allFull;
            for(subtreeIndexType i = 0; i < Octree<height>::childrenSize; ++i) {
                allFull[i].fill();
            }
            fullTree = make_shared<const Octree<height> >(allFull);

            bitset<OctLeaf::volume> voxels;
            voxels.set(full.getLinearIndex());
            partialLeaf = make_shared<const OctLeaf>(voxels);
            partialTree = make_shared<const Octree<height> >(partialLeaf, full);

            voxels.reset();
            voxels.set(onlyInA.getLinearIndex());
            voxels.set(inBoth.getLinearIndex());
            a = make_shared<const Octree<height> >(make_shared<const OctLeaf>(voxels), inBoth);
            a2 = make_shared<const Octree<height> >(make_shared<const OctLeaf>(voxels), inBoth);

            voxels.reset();
            voxels.set(onlyInA.getLinearIndex());
            voxels.set(inBoth.getLinearIndex());
            voxels.set(onlyInB.getLinearIndex());
            aUnionB = make_shared<const Octree<height> >(make_shared<const OctLeaf> (voxels), inBoth);

            voxels.reset();
            voxels.set(inBoth.getLinearIndex());
            aIntersectB = make_shared<const Octree<height> >(make_shared<const OctLeaf> (voxels), inBoth);

            voxels.reset();
            voxels.set(onlyInB.getLinearIndex());
            b = make_shared<const Octree<height> >(make_shared<const OctLeaf> (voxels), inBoth);
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
        Octree<0> partialTree0(partialLeaf, full);
        ASSERT_TRUE(partialTree0.getVoxel(full));
        ASSERT_FALSE(partialTree0.getVoxel(empty));
    }

    TEST_F(OctreeTest, getVoxelReturnsCorrectlyAtNonZeroAddress) {
        ASSERT_TRUE(partialTree->getVoxel(full));
        ASSERT_FALSE(partialTree->getVoxel(empty));
    }

    TEST_F(OctreeTest, getVoxelDoesNotAliasOutOfRangeAddresses) {
        VoxelAddress outOfRangeFull(VoxelAddress::leafLength + full.x, VoxelAddress::leafLength + full.y, VoxelAddress::leafLength + full.z);
        VoxelAddress outOfRangeEmpty(VoxelAddress::leafLength + empty.x, VoxelAddress::leafLength + empty.y, VoxelAddress::leafLength + empty.z);

        ASSERT_FALSE(partialTree->getVoxel(outOfRangeFull));
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

    TEST_F(OctreeTest, thatIntersectionOfTwoEmptyOctreesIsEmpty) {
        shared_ptr<const Octree<height> > alsoEmpty = make_shared<const Octree<height> >();
        shared_ptr<const Octree<height> > i = emptyTree->intersectWith(alsoEmpty);
        ASSERT_TRUE(*i == Octree<height>());
    }

    TEST_F(OctreeTest, thatIntersectionOfAPartialWithAnEmptyIsEmpty) {
        shared_ptr<const Octree<height> > i = a->intersectWith(emptyTree);
        ASSERT_TRUE(*i == Octree<height>());
    }

    TEST_F(OctreeTest, thatIntersectionOfAnEmptyWithAPartialIsEmpty) {
        shared_ptr<const Octree<height> > i = emptyTree->intersectWith(a);
        ASSERT_TRUE(*i == Octree<height>());
    }

    TEST_F(OctreeTest, thatIntersectionOfAPartialWithItselfIsEqualToThatPartial) {
        shared_ptr<const Octree<height> > i = a2->intersectWith(a);
        ASSERT_TRUE(*i == *a);
    }

#if 0
    TEST_F(OctreeTest, unionWorks) {
        shared_ptr<const Octree> i = a->unionWith(b);
        ASSERT_TRUE(*i == *aUnionB);
    }

    #endif

}  // namespace
