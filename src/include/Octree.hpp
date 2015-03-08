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
        static const size_t childrenSize = 8;

        OctreeHoist() = default;
        OctreeHoist(const OctreeHoist &) = default;
        OctreeHoist(const std::bitset<childrenSize> _full) : full(_full) {}

    protected:
        std::bitset<childrenSize> full;
    };

    template<uint_fast8_t height>
    class Octree : public OctreeHoist {
    public:
        Octree() = default;
        Octree(const Octree &) = default;
        // For testing.
        Octree(const std::bitset<childrenSize> _full) : OctreeHoist(_full) {}
        ~Octree() {}

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

        boost::shared_ptr<const Octree> setLeaf(boost::shared_ptr<const OctLeaf> leaf, const VoxelAddress) const {
            return boost::make_shared<const Octree>();
        }

        boost::shared_ptr<const Octree> intersectionWith(boost::shared_ptr<const Octree> other) const;
        boost::shared_ptr<const Octree> unionWith(boost::shared_ptr<const Octree> other) const;

        bool operator==(const Octree &other) const;

    private:
        typedef const std::array<boost::shared_ptr<const Octree>, childrenSize> childrenType;

        Octree(childrenType _children) : children(_children) {}
        childrenType children;
    };

    template<>
    class Octree<0> : public OctreeHoist {
    public:
        typedef std::array<boost::shared_ptr<const OctLeaf>, childrenSize> childrenType;

        Octree() = default;
        Octree(const Octree &) = default;
        // For testing.
        Octree(const std::bitset<childrenSize> _full) : OctreeHoist(_full) {}
        Octree(const childrenType _children) : children(_children) {}
        ~Octree() {}

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

        boost::shared_ptr<const Octree> setLeaf(boost::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) const {
            childrenType newChildren(children);
            newChildren[addr.getSubtreeIndex(height)] = leaf;
            return boost::make_shared<const Octree>(newChildren);

        }

        boost::shared_ptr<const Octree> intersectionWith(boost::shared_ptr<const Octree> other) const;
        boost::shared_ptr<const Octree> unionWith(boost::shared_ptr<const Octree> other) const;

        bool operator==(const Octree &other) const;

    private:
        static const uint_fast8_t height = 0;
        const childrenType children;
    };



}

#endif