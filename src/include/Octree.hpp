#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <bitset>

namespace octvox {

    class VoxelAddress;

    class OctreeHoist {
    public:

    };

    template<uint_fast8_t height>
    class Octree : public OctreeHoist {
    public:
        static const size_t childrenSize = 8;

        // For testing.
        Octree(const std::bitset<childrenSize> _full) : full(_full) {}

        // For testing.
        bool getVoxel(const VoxelAddress addr) const {
            auto childIndex = addr.getSubtreeIndex(height);
            if(full[childIndex]) {
                return true;
            } else if (children[childIndex].use_count() == 0) {
                return false;
            } else {
                return children[childIndex]->getVoxel(addr);
            }
        }

        boost::shared_ptr<const Octree> setLeaf(const OctLeaf& leaf, const VoxelAddress) const {
            return boost::make_shared<const Octree>();
        }

        boost::shared_ptr<const Octree> intersectionWith(boost::shared_ptr<const Octree> other) const;
        boost::shared_ptr<const Octree> unionWith(boost::shared_ptr<const Octree> other) const;

        bool operator==(const Octree &other) const;

    private:
        std::bitset<childrenSize> full;
        std::array<boost::shared_ptr<const Octree>, childrenSize> children;
    };

    template<>
    class Octree<0> {
    public:
        static const size_t childrenSize = 8;

        Octree() = default;
        // For testing.
        Octree(const std::bitset<childrenSize> _full) : full(_full) {}

        ~Octree() {}

        Octree(const Octree &) = default;

        // For testing.
        bool getVoxel(const VoxelAddress addr) const {
            auto childIndex = addr.getSubtreeIndex(height);
            if(full[childIndex]) {
                return true;
            } else if (children[childIndex].use_count() == 0) {
                return false;
            } else {
                return children[childIndex]->getVoxel(addr);
            }
        }

        boost::shared_ptr<const Octree> setLeaf(const OctLeaf& leaf, const VoxelAddress) const {
            return boost::make_shared<const Octree>();
        }

        boost::shared_ptr<const Octree> intersectionWith(boost::shared_ptr<const Octree> other) const;
        boost::shared_ptr<const Octree> unionWith(boost::shared_ptr<const Octree> other) const;

        bool operator==(const Octree &other) const;

    private:
        static const uint_fast8_t height = 0;
        std::bitset<childrenSize> full;
        std::array<boost::shared_ptr<const OctLeaf>, childrenSize> children;
    };



}

#endif