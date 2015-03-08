#include "gtest/gtest.h"
#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"
#include <boost/make_shared.hpp>

namespace {

    using boost::shared_ptr;
    using boost::make_shared;
    using namespace octvox;

    class VoxelAddressTest : public ::testing::Test {
    protected:
        VoxelAddressTest() {}

        virtual ~VoxelAddressTest() {}

        virtual void SetUp() {}

        virtual void TearDown() {}
    };

    TEST_F(VoxelAddressTest, Construction) {
        VoxelAddress(1, 2, 3);
    }

    // This test demonstrates why we don't make address bit masks by right shifting.
    TEST_F(VoxelAddressTest, bitwiseRightShiftFlowsZerosFromTop) {
        VoxelAddress::addr_t topSet = 0x8000000000000000;
        VoxelAddress::addr_t topFourSet = 0xf000000000000000;
        ASSERT_NE(topFourSet, (topSet >> 3));
    }

}  // namespace
