#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "OctLeaf.hpp"
#include "VoxelAddress.hpp"

#include <bitset>
#include <memory>
#include <type_traits>
#include <search.h>

namespace octvox {

    class VoxelAddress;

    template<uint_fast8_t height>
    class Octree {
    public:
        static constexpr size_t childrenSize = 8;
        static_assert(height >= 0, "Attempt to instantiate Octree with negative height");
        // childType is Octree<h-1> or OctLeaf
        using childType =
            typename std::conditional<(height > 0), Octree< (height > 0) ? (height - 1) : 0 >, OctLeaf>::type;
        using childrenType = std::array<std::shared_ptr<const childType>, childrenSize>;

        Octree() = default;
        Octree(const Octree &) = default;
        Octree(childrenType _children) : children(_children) {}
        // Sets the OctLeaf that contains addr to leaf.
        Octree(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) {
            children[addr.getSubtreeIndex(height)] = std::make_shared<const Octree<height - 1> >(leaf, addr);
        }
        // For testing.
        Octree(const std::bitset<childrenSize> _full) : full(_full) {}

        ~Octree() = default;

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

        std::shared_ptr<const Octree> setLeaf(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) const {
            return setLeafImpl<height>(leaf, addr, childrenType(children));
        }

        std::shared_ptr<const Octree> intersectionWith(std::shared_ptr<const Octree> other) const;
        std::shared_ptr<const Octree> unionWith(std::shared_ptr<const Octree> other) const;

        bool operator==(const Octree &other) const {

        }

    private:
        childrenType children;
        std::bitset<childrenSize> full;

        template <decltype(height) h>
        inline std::shared_ptr<const Octree> setLeafImpl(
                const std::shared_ptr<const OctLeaf> leaf,
                const VoxelAddress addr,
                childrenType&& newChildren) const{
            auto& child = newChildren[addr.getSubtreeIndex(height)];
            if(child) {
                child = child->setLeaf(leaf, addr);
            } else {
                child = std::make_shared<childType>(leaf, addr);
            }
            return std::make_shared<const Octree>(newChildren);
        }

    };

    template <> template<>
    inline std::shared_ptr<const Octree<0u> > Octree<0u>::setLeafImpl<0u>(
            const std::shared_ptr<const OctLeaf> leaf,
            const VoxelAddress addr,
            childrenType&& newChildren) const {
        static const auto height = 0u;
        newChildren[addr.getSubtreeIndex(height)] = leaf;
        return std::make_shared<const Octree<height> >(newChildren);
    }

    template<>
    Octree<0u>::Octree(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) {
        children[addr.getSubtreeIndex(0u)] = leaf;
    }

}

#endif