#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"

#include <boost/shared_ptr.hpp>
#include <bitset>

namespace octvox {

    class VoxelAddress;

    template<uint_fast8_t height>
    class Octree {
    public:
        static const size_t childrenSize = 8;

        Octree() = default;
        // For testing.
        Octree(std::bitset<childrenSize> _full) : full(_full) {}

        ~Octree() {}

        Octree(const Octree &) = default;

        // For testing.
        bool getVoxel(const VoxelAddress addr) const {
            auto childIndex = addr.getSubtreeIndex(height);
            return false;
        }

        boost::shared_ptr<const Octree> setLeaf(const OctLeaf&& leaf, const VoxelAddress) {
            return boost::shared_ptr<Octree const>();
        }

        boost::shared_ptr<const Octree> intersectionWith(boost::shared_ptr<const Octree> other) const;
        boost::shared_ptr<const Octree> unionWith(boost::shared_ptr<const Octree> other) const;

        bool operator==(const Octree &other) const;

    private:
        std::bitset<childrenSize> populated;
        std::bitset<childrenSize> full;
        std::array<boost::shared_ptr<const Octree>, childrenSize> children;
    };



}

#endif