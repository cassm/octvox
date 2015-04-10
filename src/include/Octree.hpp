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

        // TODO move this out of Octree
        class Child {
        public:
            Fullness fullness;
            std::shared_ptr<const childType> child;
            Child() : fullness(Fullness::empty) {}
            inline void fill() noexcept;
            inline void empty() noexcept;
            inline void set(std::shared_ptr<const childType> child);
            inline bool get(const VoxelAddress addr) const noexcept;
            inline bool operator==(const Octree<height>::Child& other) const noexcept;
            inline bool operator!=(const Octree<height>::Child& other) const noexcept;
        };

        Octree() = default;
        // ToDo: convert parameter type to rvalue ref and move
        Octree(std::array<Child, childrenSize> _children) noexcept : children(_children) {}
        // For Testing. Sets the OctLeaf that contains addr to leaf.
        Octree(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) noexcept;

        inline bool operator==(const Octree<height>& other) const noexcept;
        inline bool operator!=(const Octree<height>& other) const noexcept;

        inline std::shared_ptr<const Octree> intersectionWith(std::shared_ptr<const Octree> other) const;
        inline std::shared_ptr<const Octree> unionWith(std::shared_ptr<const Octree> other) const;

        // For testing.
        bool getVoxel(const VoxelAddress addr) const;

    private:
        std::array<Child, childrenSize> children;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Octree Public Methods

    template <heightType height>
    Octree<height>::Octree(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) noexcept {
        children[addr.getSubtreeIndex(height)].set(std::make_shared<const Octree<height - 1> >(leaf, addr));
    }

    template<>
    Octree<0>::Octree(const std::shared_ptr<const OctLeaf> leaf, const VoxelAddress addr) noexcept {
        children[addr.getSubtreeIndex(0)].set(leaf);
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
        std::array<Child, childrenSize> newChildren;
        for(int i = 0; i < childrenSize; ++i) {
            if (children[i].fullness == Fullness::empty || other->children[i].fullness == Fullness::empty) {
                // newly constructed child defaults to empty
                //newChildren[i].fullness = Fullness::empty;
            }
            else if (children[i].fullness == Fullness::full && other->children[i].fullness == Fullness::full) {
                newChildren[i].fullness = Fullness::full;
            }
            else {
                newChildren[i].fullness = Fullness::partial;
                newChildren[i].child = children[i].child->intersectionWith(other->children[i].child);
            }
        }
        return std::make_shared<const Octree<height> >(newChildren);
    }

    template<heightType height>
    bool Octree<height>::getVoxel(const VoxelAddress addr) const {
        return children[addr.getSubtreeIndex(height)].get(addr);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Octree::Child Public Methods

    template <heightType height>
    inline void Octree<height>::Child::fill() noexcept {
        fullness = Fullness::full;
        child.reset();
    }

    template <heightType height>
    inline void Octree<height>::Child::empty() noexcept {
        fullness = Fullness::empty;
        child.reset();
    }

    template <heightType height>
    inline void Octree<height>::Child::set(std::shared_ptr<const childType> _child) {
        fullness = Fullness::partial;
        child = _child;
    }

    template <heightType height>
    inline bool Octree<height>::Child::get(VoxelAddress addr) const noexcept {
        if (fullness == Fullness::empty) {
            return false;
        } else if (fullness == Fullness::full){
            return true;
        } else {
            return child->getVoxel(addr);
        }
    }

    template <heightType height>
    inline bool Octree<height>::Child::operator==(const Octree<height>::Child& other) const noexcept {
        if(fullness != other.fullness) {
            return false;
        } else {
            for(int i = 0; i < childrenSize; ++i) {
                if (fullness == Fullness::partial && (*child != *(other.child))) {
                    return false;
                }
            }
            return true;
        }
    }

    template <heightType height>
    inline bool Octree<height>::Child::operator!=(const Octree<height>::Child& other) const noexcept {
        return !operator==(other);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Private Methods

}

#endif