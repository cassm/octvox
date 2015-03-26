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

    using heightType = int_fast8_t;

    template<heightType height>
    class Octree {
    public:
        static constexpr size_t childrenSize = 8;
        static_assert(height >= 0, "Attempt to instantiate Octree with negative height");
        // childType is Octree<h-1> or OctLeaf
        using childType =
            typename std::conditional<(height > 0), Octree< (height > 0) ? (height - 1) : 0 >, OctLeaf>::type;
        using childrenType = std::array<std::shared_ptr<const childType>, childrenSize>;

        Octree() noexcept = default;
        Octree(childrenType _children) noexcept : children(_children) {}
        // Sets the OctLeaf that contains addr to leaf.
        Octree(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) noexcept;
        // For testing.
        Octree(const std::bitset<childrenSize> _full) : full(_full) {}

        inline std::shared_ptr<const Octree> setLeaf(
                const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) const noexcept;
        inline bool operator==(const Octree<height>& other) const noexcept;
        inline bool operator!=(const Octree<height>& other) const noexcept {return !operator==(other);}

        std::shared_ptr<const Octree> intersectionWith(std::shared_ptr<const Octree> other) const;
        std::shared_ptr<const Octree> unionWith(std::shared_ptr<const Octree> other) const;

        // For testing.
        bool getVoxel(const VoxelAddress addr) const;

    private:
        childrenType children;
        std::bitset<childrenSize> full;

        template <heightType h>
        inline std::shared_ptr<const Octree> setLeafImpl(
                const std::shared_ptr<const OctLeaf> leaf,
                const VoxelAddress addr,
                childrenType&& newChildren) const noexcept;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Public Methods

    template <heightType height>
    Octree<height>::Octree(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) noexcept {
        children[addr.getSubtreeIndex(height)] = std::make_shared<const Octree<height - 1> >(leaf, addr);
    }

    template <heightType height>
    inline std::shared_ptr<const Octree<height> > Octree<height>::setLeaf(
            const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) const noexcept {
        return setLeafImpl<height>(leaf, addr, childrenType(children));
    }

    template <heightType height>
    inline bool Octree<height>::operator==(const Octree<height>& other) const noexcept {
        if(full != other.full) {
            return false;
        } else {
            for(unsigned i = 0u; i < childrenSize; ++i) {
                if (!full[i]) {
                    if (children[i]) {
                        if (!other.children[i] || (*children[i] != *(other.children[i]))) {
                            return false;
                        }
                    } else if (other.children[i]) {
                        return false;
                    }
                }
            }
            return true;
        }
    }

    template<heightType height>
    inline std::shared_ptr<const Octree<height> >Octree<height>::intersectionWith(
            std::shared_ptr<const octvox::Octree<height> > other) const {
        return std::make_shared<const Octree<height> >();
    }

    template<heightType height>
    bool Octree<height>::getVoxel(const VoxelAddress addr) const {
        auto childIndex = addr.getSubtreeIndex(height);
        if(full[childIndex]) {
            return true;
        } else if (children[childIndex].use_count() == 0) {
            return false;
        } else {
            return children[childIndex]->getVoxel(addr);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Private Methods

    // setLeafImpl is a template method so that it can be specialised independently of Octree.
    template <heightType height>
    template <heightType h>
    inline std::shared_ptr<const Octree<height> > Octree<height>::setLeafImpl(
            const std::shared_ptr<const OctLeaf> leaf,
            const VoxelAddress addr,
            childrenType&& newChildren) const noexcept {
        auto& child = newChildren[addr.getSubtreeIndex(height)];
        if(child) {
            child = child->setLeaf(leaf, addr);
        } else {
            child = std::make_shared<childType>(leaf, addr);
        }
        return std::make_shared<const Octree>(newChildren);
    }

    template <> template<>
    inline std::shared_ptr<const Octree<0u> > Octree<0u>::setLeafImpl<0u>(
            const std::shared_ptr<const OctLeaf> leaf,
            const VoxelAddress addr,
            childrenType&& newChildren) const noexcept {
        static const auto height = 0u;
        newChildren[addr.getSubtreeIndex(height)] = leaf;
        return std::make_shared<const Octree<height> >(newChildren);
    }

    template<>
    Octree<0u>::Octree(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) noexcept {
        children[addr.getSubtreeIndex(0u)] = leaf;
    }

}

#endif