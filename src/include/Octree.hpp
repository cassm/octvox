#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <bitset>
#include <type_traits>
#include <search.h>

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
        static_assert(height >= 0, "Attempt to instantiate Octree with negative height");
        using childType = typename std::conditional<(height > 0), Octree<height - 1>, OctLeaf>::type;
        using childrenType = std::array<boost::shared_ptr<const childType>, childrenSize>;

        Octree() = default;
        Octree(const Octree &) = default;
        Octree(childrenType _children) : children(_children) {}
        Octree(const boost::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) {
            children[addr.getSubtreeIndex(height)] = boost::make_shared<const Octree<height - 1> >(leaf, addr);
        }
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

        boost::shared_ptr<const Octree> setLeaf(const boost::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) const {
            return setLeafImpl<height>(leaf, addr, childrenType(children));
        }

        boost::shared_ptr<const Octree> intersectionWith(boost::shared_ptr<const Octree> other) const;
        boost::shared_ptr<const Octree> unionWith(boost::shared_ptr<const Octree> other) const;

        bool operator==(const Octree &other) const;

    private:
        childrenType children;

        template <decltype(height) h>
        inline boost::shared_ptr<const Octree> setLeafImpl(
                const boost::shared_ptr<const OctLeaf> leaf,
                const VoxelAddress addr,
                childrenType&& newChildren) const{
            auto& child = newChildren[addr.getSubtreeIndex(height)];
//            if(child) {
                child = child->setLeaf(leaf, addr);
//            } else {
//                child = boost::make_shared<childType>(leaf, addr);
//            }
            return boost::make_shared<const Octree>(newChildren);
        }

    };

    template <> template<>
    inline boost::shared_ptr<const Octree<0u> > Octree<0u>::setLeafImpl<0u>(
            const boost::shared_ptr<const OctLeaf> leaf,
            const VoxelAddress addr,
            childrenType&& newChildren) const {
        static const auto height = 0u;
        newChildren[addr.getSubtreeIndex(height)] = leaf;
        return boost::make_shared<const Octree<height> >(newChildren);
    }

    template<>
    Octree<0u>::Octree(const boost::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) {
        children[addr.getSubtreeIndex(0u)] = leaf;
    }


#if 0
    template<>
    class Octree<0> : public OctreeHoist {
    public:
        using childrenType = std::array<boost::shared_ptr<const OctLeaf>, childrenSize>;

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

#endif

}

#endif