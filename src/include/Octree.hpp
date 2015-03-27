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

        // childType is Octree<h-1> or OctLeaf. Trigraph is to prevent static_assert failure for height = 0.
        using childType =
            typename std::conditional<(height > 0), Octree< (height > 0) ? (height - 1) : 0 >, OctLeaf>::type;

        enum class Fullness {empty, partial, full};

        //using childrenType = std::array<childHolder, childrenSize>;
        class Children {
        public:
            Children() noexcept {fullness.fill(Fullness::empty);}
            inline void fill(subtreeIndexType) noexcept;
            inline void empty(subtreeIndexType) noexcept;
            inline void set(subtreeIndexType, std::shared_ptr<const childType> child);
            inline bool get(const VoxelAddress addr) const noexcept;
            inline bool operator==(const Octree<height>::Children& other) const noexcept;
            inline bool operator!=(const Octree<height>::Children& other) const noexcept;
        private:
            std::array<Fullness, childrenSize> fullness;
            std::array<std::shared_ptr<const childType>, childrenSize> children;
        };

        Octree() noexcept = default;
        // ToDo: convert parameter type to rvalue ref and move
        Octree(Children _children) noexcept : children(_children) {}
        // For Testing. Sets the OctLeaf that contains addr to leaf.
        Octree(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) noexcept;

        inline bool operator==(const Octree<height>& other) const noexcept;
        inline bool operator!=(const Octree<height>& other) const noexcept;

        inline std::shared_ptr<const Octree> intersectionWith(std::shared_ptr<const Octree> other) const;
        inline std::shared_ptr<const Octree> unionWith(std::shared_ptr<const Octree> other) const;

        // For testing.
        bool getVoxel(const VoxelAddress addr) const;

    private:
        Children children;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Octree Public Methods

    template <heightType height>
    Octree<height>::Octree(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) noexcept {
        children.set(addr.getSubtreeIndex(height), std::make_shared<const Octree<height - 1> >(leaf, addr));
    }

    template<>
    Octree<0>::Octree(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) noexcept {
        children.set(addr.getSubtreeIndex(0), leaf);
    }

    template <heightType height>
    inline bool Octree<height>::operator==(const Octree<height>& other) const noexcept {
        return children == other.children;
    }

    template <heightType height>
    inline bool Octree<height>::operator!=(const Octree<height>& other) const noexcept {
        return !operator==(other);
    }

    template<heightType height>
    inline std::shared_ptr<const Octree<height> >Octree<height>::intersectionWith(
            std::shared_ptr<const octvox::Octree<height> > other) const {
        Children newChildren;
        for(int i = 0; i < childrenSize; ++i) {
            //newChildren[i] = children[i]->intersectionWith(other->children[i]);
        }
        return std::make_shared<const Octree<height> >(newChildren);
    }

    template<heightType height>
    bool Octree<height>::getVoxel(const VoxelAddress addr) const {
        return children.get(addr);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Octree::Children Public Methods

    template <heightType height>
    inline void Octree<height>::Children::fill(subtreeIndexType i) noexcept {
        fullness[i] = Fullness::full;
        children[i].reset();
    }

    template <heightType height>
    inline void Octree<height>::Children::empty(subtreeIndexType i) noexcept {
        fullness[i] = Fullness::empty;
        children[i].reset();
    }

    template <heightType height>
    inline void Octree<height>::Children::set(subtreeIndexType i, std::shared_ptr<const childType> child) {
        fullness[i] = Fullness::partial;
        children[i] = child;
    }

    template <heightType height>
    inline bool Octree<height>::Children::get(VoxelAddress addr) const noexcept {
        auto childIndex = addr.getSubtreeIndex(height);
        if (fullness[childIndex] == Fullness::empty) {
            return false;
        } else if (fullness[childIndex] == Fullness::full){
            return true;
        } else {
            return children[childIndex]->getVoxel(addr);
        }
    }

    template <heightType height>
    inline bool Octree<height>::Children::operator==(const Octree<height>::Children& other) const noexcept {
        if(fullness != other.fullness) {
            return false;
        } else {
            for(int i = 0; i < childrenSize; ++i) {
                if (fullness[i] == Fullness::partial && (*children[i] != *(other.children[i]))) {
                    return false;
                }
            }
            return true;
        }
    }

    template <heightType height>
    inline bool Octree<height>::Children::operator!=(const Octree<height>::Children& other) const noexcept {
        return !operator==(other);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Private Methods

}

#endif